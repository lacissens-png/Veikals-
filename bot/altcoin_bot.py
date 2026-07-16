"""
Altcoin tirdzniecības bots — visa loģika vienā failā.

Palaišana:
    python altcoin_bot.py                              # live bots (dry-run pēc .env)
    python altcoin_bot.py backtest                     # backtests visiem .env pāriem
    python altcoin_bot.py backtest SOL/USDT AVAX/USDT  # konkrēti pāri
    python altcoin_bot.py backtest --candles 5000 SOL/USDT
    python altcoin_bot.py backtest --timeframe 1h --capital 5000 SOL/USDT

Konfigurācija: kopē .env.example uz .env un aizpildi savas Binance atslēgas.
"""
from __future__ import annotations

import argparse
import json
import logging
import os
import sys
import time
from dataclasses import dataclass, field, asdict
from enum import Enum
from pathlib import Path
from typing import Iterable

import ccxt
import pandas as pd
from dotenv import load_dotenv

load_dotenv()

log = logging.getLogger("altcoin_bot")

STATE_FILE = Path(__file__).parent / "logs" / "positions.json"


# ============================================================================
# KONFIGURĀCIJA
# ============================================================================

def _bool(name: str, default: str = "false") -> bool:
    return os.getenv(name, default).strip().lower() in ("1", "true", "yes", "y")


def _list(name: str, default: str = "") -> list[str]:
    return [x.strip() for x in os.getenv(name, default).split(",") if x.strip()]


@dataclass
class Config:
    api_key: str = field(default_factory=lambda: os.getenv("BINANCE_API_KEY", ""))
    api_secret: str = field(default_factory=lambda: os.getenv("BINANCE_API_SECRET", ""))

    dry_run: bool = field(default_factory=lambda: _bool("DRY_RUN", "true"))
    use_testnet: bool = field(default_factory=lambda: _bool("USE_TESTNET", "true"))

    trading_pairs: list[str] = field(
        default_factory=lambda: _list("TRADING_PAIRS", "SOL/USDT,AVAX/USDT")
    )
    timeframe: str = field(default_factory=lambda: os.getenv("TIMEFRAME", "15m"))
    trade_amount_usdt: float = field(
        default_factory=lambda: float(os.getenv("TRADE_AMOUNT_USDT", "50"))
    )
    max_open_positions: int = field(
        default_factory=lambda: int(os.getenv("MAX_OPEN_POSITIONS", "3"))
    )

    stop_loss_percent: float = field(
        default_factory=lambda: float(os.getenv("STOP_LOSS_PERCENT", "2.0"))
    )
    take_profit_percent: float = field(
        default_factory=lambda: float(os.getenv("TAKE_PROFIT_PERCENT", "5.0"))
    )
    min_take_profit_percent: float = field(
        default_factory=lambda: float(os.getenv("MIN_TAKE_PROFIT_PERCENT", "5.0"))
    )
    lookback_candles: int = field(
        default_factory=lambda: int(os.getenv("LOOKBACK_CANDLES", "50"))
    )

    ema_fast: int = field(default_factory=lambda: int(os.getenv("EMA_FAST", "5")))
    ema_slow: int = field(default_factory=lambda: int(os.getenv("EMA_SLOW", "13")))
    rsi_period: int = field(default_factory=lambda: int(os.getenv("RSI_PERIOD", "14")))
    rsi_oversold: float = field(
        default_factory=lambda: float(os.getenv("RSI_OVERSOLD", "35"))
    )
    rsi_overbought: float = field(
        default_factory=lambda: float(os.getenv("RSI_OVERBOUGHT", "70"))
    )

    loop_interval_sec: int = field(
        default_factory=lambda: int(os.getenv("LOOP_INTERVAL_SEC", "60"))
    )

    def validate(self) -> None:
        if not self.dry_run and (not self.api_key or not self.api_secret):
            raise ValueError(
                "Reālai tirdzniecībai nepieciešamas BINANCE_API_KEY un BINANCE_API_SECRET"
            )
        if self.ema_fast >= self.ema_slow:
            raise ValueError("EMA_FAST jābūt mazākam par EMA_SLOW")
        if not self.trading_pairs:
            raise ValueError("Jānorāda vismaz viens tirdzniecības pāris")
        if self.take_profit_percent < self.min_take_profit_percent:
            raise ValueError(
                f"TAKE_PROFIT_PERCENT ({self.take_profit_percent}) nevar būt "
                f"mazāks par MIN_TAKE_PROFIT_PERCENT ({self.min_take_profit_percent})"
            )


# ============================================================================
# INDIKATORI (EMA, RSI)
# ============================================================================

def ema(series: pd.Series, period: int) -> pd.Series:
    return series.ewm(span=period, adjust=False).mean()


def rsi(series: pd.Series, period: int = 14) -> pd.Series:
    delta = series.diff()
    gain = delta.clip(lower=0)
    loss = -delta.clip(upper=0)
    avg_gain = gain.ewm(alpha=1 / period, adjust=False).mean()
    avg_loss = loss.ewm(alpha=1 / period, adjust=False).mean()
    rs = avg_gain / avg_loss.replace(0, 1e-10)
    return 100 - (100 / (1 + rs))


def add_indicators(
    df: pd.DataFrame, ema_fast: int, ema_slow: int, rsi_period: int
) -> pd.DataFrame:
    df = df.copy()
    df["ema_fast"] = ema(df["close"], ema_fast)
    df["ema_slow"] = ema(df["close"], ema_slow)
    df["rsi"] = rsi(df["close"], rsi_period)
    return df


# ============================================================================
# STRATĒĢIJA
# ============================================================================

class Signal(Enum):
    BUY = "BUY"
    SELL = "SELL"
    HOLD = "HOLD"


class Strategy:
    def __init__(self, cfg: Config):
        self.cfg = cfg

    def generate_signal(self, df: pd.DataFrame) -> Signal:
        if len(df) < max(self.cfg.ema_slow, self.cfg.rsi_period) + 2:
            return Signal.HOLD

        last = df.iloc[-1]
        prev = df.iloc[-2]

        cross_up = (
            prev["ema_fast"] <= prev["ema_slow"]
            and last["ema_fast"] > last["ema_slow"]
        )
        cross_down = (
            prev["ema_fast"] >= prev["ema_slow"]
            and last["ema_fast"] < last["ema_slow"]
        )

        rsi_val = last["rsi"]
        prev_rsi = prev["rsi"]

        # RSI atlēkšana no oversold + augšupejošs trends
        rsi_bounce = (
            prev_rsi < self.cfg.rsi_oversold
            and rsi_val >= self.cfg.rsi_oversold
            and last["ema_fast"] > last["ema_slow"]
        )

        if (cross_up or rsi_bounce) and rsi_val < self.cfg.rsi_overbought:
            return Signal.BUY
        if cross_down or rsi_val > self.cfg.rsi_overbought:
            return Signal.SELL
        return Signal.HOLD

    def tp_upside_percent(self, df: pd.DataFrame, current_price: float) -> float:
        """Cik % virs pašreizējās cenas ir pēdējo lookback sveču maksimums."""
        window = df.tail(self.cfg.lookback_candles)
        recent_high = float(window["high"].max())
        return (recent_high - current_price) / current_price * 100


# ============================================================================
# POZĪCIJAS UN RISKU PĀRVALDĪBA
# ============================================================================

@dataclass
class Position:
    symbol: str
    entry_price: float
    amount: float
    stop_loss: float
    take_profit: float


class PositionBook:
    def __init__(self):
        self.positions: dict[str, Position] = {}
        self._load()

    def _load(self) -> None:
        if not STATE_FILE.exists():
            return
        try:
            data = json.loads(STATE_FILE.read_text())
            for sym, p in data.items():
                self.positions[sym] = Position(**p)
            log.info(f"Ielādētas {len(self.positions)} pozīcijas")
        except Exception as e:
            log.warning(f"Neizdevās ielādēt pozīcijas: {e}")

    def _save(self) -> None:
        STATE_FILE.parent.mkdir(parents=True, exist_ok=True)
        data = {s: asdict(p) for s, p in self.positions.items()}
        STATE_FILE.write_text(json.dumps(data, indent=2))

    def open(self, pos: Position) -> None:
        self.positions[pos.symbol] = pos
        self._save()
        log.info(
            f"ATVĒRTA {pos.symbol}: cena={pos.entry_price}, "
            f"apjoms={pos.amount}, SL={pos.stop_loss}, TP={pos.take_profit}"
        )

    def close(self, symbol: str) -> Position | None:
        pos = self.positions.pop(symbol, None)
        if pos:
            self._save()
            log.info(f"SLĒGTA {symbol}")
        return pos

    def has(self, symbol: str) -> bool:
        return symbol in self.positions

    def count(self) -> int:
        return len(self.positions)


def calc_sl_tp(entry_price: float, sl_pct: float, tp_pct: float) -> tuple[float, float]:
    return entry_price * (1 - sl_pct / 100), entry_price * (1 + tp_pct / 100)


def should_exit(pos: Position, current_price: float) -> str | None:
    if current_price <= pos.stop_loss:
        return "SL"
    if current_price >= pos.take_profit:
        return "TP"
    return None


# ============================================================================
# BIRŽAS SAVIENOTĀJS
# ============================================================================

class Exchange:
    def __init__(self, cfg: Config):
        self.cfg = cfg
        self.client = ccxt.binance({
            "apiKey": cfg.api_key,
            "secret": cfg.api_secret,
            "enableRateLimit": True,
            "options": {"defaultType": "spot"},
        })
        if cfg.use_testnet:
            self.client.set_sandbox_mode(True)
            log.info("Izmantojam Binance TESTNET režīmu")

    def fetch_ohlcv(self, symbol: str, limit: int = 200) -> pd.DataFrame:
        raw = self.client.fetch_ohlcv(symbol, timeframe=self.cfg.timeframe, limit=limit)
        df = pd.DataFrame(
            raw, columns=["timestamp", "open", "high", "low", "close", "volume"]
        )
        df["timestamp"] = pd.to_datetime(df["timestamp"], unit="ms")
        return df

    def get_price(self, symbol: str) -> float:
        return float(self.client.fetch_ticker(symbol)["last"])

    def market_buy(self, symbol: str, usdt_amount: float) -> dict:
        price = self.get_price(symbol)
        qty = float(self.client.amount_to_precision(symbol, usdt_amount / price))
        if self.cfg.dry_run:
            log.info(f"[DRY_RUN] PIRKT {qty} {symbol} @ {price}")
            return {"id": "dry-buy", "symbol": symbol, "price": price, "amount": qty}
        order = self.client.create_market_buy_order(symbol, qty)
        log.info(f"PIRKTS: {order}")
        return order

    def market_sell(self, symbol: str, qty: float) -> dict:
        qty = float(self.client.amount_to_precision(symbol, qty))
        price = self.get_price(symbol)
        if self.cfg.dry_run:
            log.info(f"[DRY_RUN] PĀRDOT {qty} {symbol} @ {price}")
            return {"id": "dry-sell", "symbol": symbol, "price": price, "amount": qty}
        order = self.client.create_market_sell_order(symbol, qty)
        log.info(f"PĀRDOTS: {order}")
        return order


# ============================================================================
# BOTA GALVENĀ LOĢIKA
# ============================================================================

class TradingBot:
    def __init__(self, cfg: Config):
        self.cfg = cfg
        self.exchange = Exchange(cfg)
        self.strategy = Strategy(cfg)
        self.book = PositionBook()

    def process_symbol(self, symbol: str) -> None:
        try:
            df = self.exchange.fetch_ohlcv(symbol, limit=200)
            df = add_indicators(
                df, self.cfg.ema_fast, self.cfg.ema_slow, self.cfg.rsi_period
            )
            current_price = float(df.iloc[-1]["close"])

            # 1. Ja pozīcija atvērta — pārbaudām SL/TP
            if self.book.has(symbol):
                pos = self.book.positions[symbol]
                reason = should_exit(pos, current_price)
                if reason:
                    log.info(f"{symbol}: {reason} sasniegts pie {current_price}")
                    self.exchange.market_sell(symbol, pos.amount)
                    self.book.close(symbol)
                    return

            # 2. Ģenerējam signālu
            signal = self.strategy.generate_signal(df)
            log.info(
                f"{symbol}: cena={current_price:.4f} RSI={df.iloc[-1]['rsi']:.1f} "
                f"signāls={signal.value}"
            )

            # 3. Rīkojamies pēc signāla
            if signal == Signal.BUY and not self.book.has(symbol):
                if self.book.count() >= self.cfg.max_open_positions:
                    log.info(
                        f"Sasniegts MAX_OPEN_POSITIONS ({self.cfg.max_open_positions}), "
                        f"izlaižam {symbol}"
                    )
                    return
                upside = self.strategy.tp_upside_percent(df, current_price)
                if upside < self.cfg.min_take_profit_percent:
                    log.info(
                        f"{symbol}: izlaižam — potenciāls augšup {upside:.2f}% "
                        f"< min TP {self.cfg.min_take_profit_percent}%"
                    )
                    return
                order = self.exchange.market_buy(symbol, self.cfg.trade_amount_usdt)
                entry = float(order["price"])
                sl, tp = calc_sl_tp(
                    entry, self.cfg.stop_loss_percent, self.cfg.take_profit_percent
                )
                self.book.open(Position(
                    symbol=symbol, entry_price=entry,
                    amount=float(order["amount"]),
                    stop_loss=sl, take_profit=tp,
                ))
            elif signal == Signal.SELL and self.book.has(symbol):
                pos = self.book.positions[symbol]
                self.exchange.market_sell(symbol, pos.amount)
                self.book.close(symbol)
        except Exception as e:
            log.exception(f"Kļūda apstrādājot {symbol}: {e}")

    def run(self) -> None:
        mode = "DRY-RUN" if self.cfg.dry_run else "REĀLA TIRDZNIECĪBA"
        log.info(
            f"Bots startēts. Režīms: {mode}. Pāri: {', '.join(self.cfg.trading_pairs)}"
        )
        while True:
            for symbol in self.cfg.trading_pairs:
                self.process_symbol(symbol)
            log.info(f"Gaidām {self.cfg.loop_interval_sec}s līdz nākamajam ciklam...")
            time.sleep(self.cfg.loop_interval_sec)


# ============================================================================
# BACKTESTS
# ============================================================================

@dataclass
class Trade:
    symbol: str
    entry_time: pd.Timestamp
    entry_price: float
    exit_time: pd.Timestamp
    exit_price: float
    reason: str  # "TP", "SL", "SIGNAL"
    pnl_pct: float


def fetch_history(symbol: str, timeframe: str, candles: int) -> pd.DataFrame:
    """Publiskais produkcijas endpoint — testnetā vēsture nav pieejama."""
    client = ccxt.binance({"enableRateLimit": True})
    tf_ms = client.parse_timeframe(timeframe) * 1000
    since = client.milliseconds() - candles * tf_ms

    all_rows: list[list] = []
    while True:
        batch = client.fetch_ohlcv(symbol, timeframe=timeframe, since=since, limit=1000)
        if not batch:
            break
        all_rows.extend(batch)
        since = batch[-1][0] + tf_ms
        if len(batch) < 1000 or since >= client.milliseconds():
            break

    df = pd.DataFrame(
        all_rows, columns=["timestamp", "open", "high", "low", "close", "volume"]
    )
    df["timestamp"] = pd.to_datetime(df["timestamp"], unit="ms")
    return df.drop_duplicates(subset="timestamp").reset_index(drop=True)


def simulate(cfg: Config, symbol: str, df_full: pd.DataFrame) -> list[Trade]:
    """Signāls no aizvērtas sveces; ieeja nākamās sveces atvēršanā."""
    df = add_indicators(
        df_full, cfg.ema_fast, cfg.ema_slow, cfg.rsi_period
    ).dropna().reset_index(drop=True)
    strat = Strategy(cfg)
    trades: list[Trade] = []

    in_pos = False
    entry_price = 0.0
    entry_time: pd.Timestamp | None = None
    sl = tp = 0.0
    min_start = max(cfg.ema_slow, cfg.rsi_period) + 2

    for i in range(min_start, len(df) - 1):
        window = df.iloc[: i + 1]
        current = df.iloc[i]
        next_bar = df.iloc[i + 1]

        if in_pos:
            hit_sl = next_bar["low"] <= sl
            hit_tp = next_bar["high"] >= tp
            reason = None
            exit_price = None
            # Pesimistiski: ja abi trāpīti vienā svecē — SL vispirms
            if hit_sl:
                reason, exit_price = "SL", sl
            elif hit_tp:
                reason, exit_price = "TP", tp
            elif strat.generate_signal(window) == Signal.SELL:
                reason, exit_price = "SIGNAL", float(next_bar["open"])
            if reason:
                trades.append(Trade(
                    symbol=symbol, entry_time=entry_time,
                    entry_price=entry_price,
                    exit_time=next_bar["timestamp"],
                    exit_price=exit_price, reason=reason,
                    pnl_pct=(exit_price - entry_price) / entry_price * 100,
                ))
                in_pos = False
            continue

        if strat.generate_signal(window) != Signal.BUY:
            continue
        current_price = float(current["close"])
        upside = strat.tp_upside_percent(window, current_price)
        if upside < cfg.min_take_profit_percent:
            continue
        entry_price = float(next_bar["open"])
        entry_time = next_bar["timestamp"]
        sl, tp = calc_sl_tp(entry_price, cfg.stop_loss_percent, cfg.take_profit_percent)
        in_pos = True

    return trades


def report(all_trades: list[Trade], start_capital: float) -> None:
    if not all_trades:
        print("\nNav neviena darījuma. Mēģini garāku periodu vai citu pāri.")
        return

    df = pd.DataFrame([t.__dict__ for t in all_trades])
    wins = df[df["pnl_pct"] > 0]
    losses = df[df["pnl_pct"] <= 0]

    equity = start_capital
    curve = [equity]
    for pnl in df.sort_values("entry_time")["pnl_pct"]:
        equity *= 1 + pnl / 100
        curve.append(equity)
    peak = pd.Series(curve).cummax()
    dd = (pd.Series(curve) - peak) / peak * 100
    max_dd = dd.min()

    print("\n" + "=" * 60)
    print("BACKTEST REZULTĀTI")
    print("=" * 60)
    print(f"Darījumu skaits:       {len(df)}")
    print(f"  Uzvarēti:            {len(wins)} ({len(wins)/len(df)*100:.1f}%)")
    print(f"  Zaudēti:             {len(losses)}")
    print(f"Vidējais PnL:          {df['pnl_pct'].mean():+.2f}%")
    print(f"Labākais darījums:     {df['pnl_pct'].max():+.2f}%")
    print(f"Sliktākais darījums:   {df['pnl_pct'].min():+.2f}%")
    print(f"Kopējais atgriezums:   {(equity/start_capital - 1)*100:+.2f}%")
    print(f"Sākuma kapitāls:       {start_capital:.2f} USDT")
    print(f"Beigu kapitāls:        {equity:.2f} USDT")
    print(f"Max drawdown:          {max_dd:.2f}%")
    print(f"Izejas iemesli:        "
          f"TP={len(df[df['reason']=='TP'])}, "
          f"SL={len(df[df['reason']=='SL'])}, "
          f"SIGNAL={len(df[df['reason']=='SIGNAL'])}")
    print("=" * 60)

    print("\nPēdējie 10 darījumi:")
    for _, t in df.sort_values("entry_time").tail(10).iterrows():
        print(
            f"  {t['symbol']:12} "
            f"{t['entry_time'].strftime('%Y-%m-%d %H:%M')} "
            f"@ {t['entry_price']:.4f} → {t['exit_price']:.4f} "
            f"[{t['reason']}] {t['pnl_pct']:+.2f}%"
        )


def run_backtest(argv: list[str]) -> None:
    p = argparse.ArgumentParser(prog="altcoin_bot.py backtest",
                                description="Altcoin stratēģijas backtests")
    p.add_argument("symbols", nargs="*", help="Pāri (piem., SOL/USDT)")
    p.add_argument("--candles", type=int, default=1000)
    p.add_argument("--timeframe")
    p.add_argument("--capital", type=float, default=1000.0)
    args = p.parse_args(argv)

    cfg = Config()
    if args.timeframe:
        cfg.timeframe = args.timeframe
    symbols: Iterable[str] = args.symbols or cfg.trading_pairs

    print(
        f"Testējam: {', '.join(symbols)} | intervāls={cfg.timeframe} | "
        f"sveces={args.candles} | TP={cfg.take_profit_percent}% "
        f"SL={cfg.stop_loss_percent}% min_TP={cfg.min_take_profit_percent}%"
    )

    all_trades: list[Trade] = []
    for sym in symbols:
        try:
            print(f"\n→ Lejupielādē {sym}...")
            df = fetch_history(sym, cfg.timeframe, args.candles)
            print(f"  Ieguvām {len(df)} sveces "
                  f"({df['timestamp'].iloc[0]} → {df['timestamp'].iloc[-1]})")
            trades = simulate(cfg, sym, df)
            print(f"  {len(trades)} darījumi")
            all_trades.extend(trades)
        except Exception as e:
            log.warning(f"{sym}: {e}")

    report(all_trades, args.capital)


# ============================================================================
# IEEJAS PUNKTS
# ============================================================================

def setup_logging() -> None:
    logs_dir = Path(__file__).parent / "logs"
    logs_dir.mkdir(parents=True, exist_ok=True)
    logging.basicConfig(
        level=logging.INFO,
        format="%(asctime)s [%(levelname)s] %(name)s: %(message)s",
        handlers=[
            logging.StreamHandler(sys.stdout),
            logging.FileHandler(logs_dir / "bot.log", encoding="utf-8"),
        ],
    )


def main() -> None:
    setup_logging()

    # Sub-komanda: backtest
    if len(sys.argv) > 1 and sys.argv[1] == "backtest":
        run_backtest(sys.argv[2:])
        return

    # Citādi — live bots
    cfg = Config()
    cfg.validate()
    bot = TradingBot(cfg)
    try:
        bot.run()
    except KeyboardInterrupt:
        log.info("Apturēts ar Ctrl+C")


if __name__ == "__main__":
    main()
