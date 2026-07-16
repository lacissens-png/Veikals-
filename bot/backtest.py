"""
Backtest — stratēģijas pārbaude uz vēsturiskiem datiem.

Palaišana:
    python backtest.py                          # visi pāri no .env
    python backtest.py SOL/USDT AVAX/USDT       # konkrēti pāri
    python backtest.py --candles 2000 SOL/USDT  # cik sveces lejupielādēt
    python backtest.py --timeframe 1h SOL/USDT  # cits intervāls

Datus ņem publiski no Binance (bez API atslēgām).
"""
import argparse
import logging
import sys
from dataclasses import dataclass
from typing import Iterable

import ccxt
import pandas as pd

from config import Config
from indicators import add_indicators
from strategy import Strategy, Signal
from risk import calc_sl_tp

log = logging.getLogger("backtest")


@dataclass
class Trade:
    symbol: str
    entry_time: pd.Timestamp
    entry_price: float
    exit_time: pd.Timestamp
    exit_price: float
    reason: str  # "TP", "SL", "SIGNAL"
    pnl_pct: float


def fetch_history(
    symbol: str, timeframe: str, candles: int
) -> pd.DataFrame:
    """Lejupielādē vēsturiskās sveces vairākās partijās (Binance limits ~1000).
    Vienmēr izmanto publisko produkcijas endpoint — testnetā vēsture nav pieejama."""
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
    df = df.drop_duplicates(subset="timestamp").reset_index(drop=True)
    return df


def simulate(cfg: Config, symbol: str, df_full: pd.DataFrame) -> list[Trade]:
    """
    Iet cauri svecēm secīgi, replikojot live bota lēmumus.
    Signāls tiek ģenerēts no aizvērtas sveces; ieeja notiek nākamās sveces atvēršanā.
    SL/TP tiek pārbaudīts pret nākamās sveces high/low.
    """
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
            # Pesimistiski: ja abi trāpīti vienā svecē, uzskatām, ka SL vispirms
            if hit_sl:
                reason, exit_price = "SL", sl
            elif hit_tp:
                reason, exit_price = "TP", tp
            else:
                if strat.generate_signal(window) == Signal.SELL:
                    reason, exit_price = "SIGNAL", float(next_bar["open"])
            if reason:
                trades.append(
                    Trade(
                        symbol=symbol,
                        entry_time=entry_time,
                        entry_price=entry_price,
                        exit_time=next_bar["timestamp"],
                        exit_price=exit_price,
                        reason=reason,
                        pnl_pct=(exit_price - entry_price) / entry_price * 100,
                    )
                )
                in_pos = False
            continue

        # Nav pozīcijas — vai signāls?
        if strat.generate_signal(window) != Signal.BUY:
            continue
        current_price = float(current["close"])
        upside = strat.tp_upside_percent(window, current_price)
        if upside < cfg.min_take_profit_percent:
            continue
        entry_price = float(next_bar["open"])
        entry_time = next_bar["timestamp"]
        sl, tp = calc_sl_tp(
            entry_price, cfg.stop_loss_percent, cfg.take_profit_percent
        )
        in_pos = True

    return trades


def report(all_trades: list[Trade], start_capital: float) -> None:
    if not all_trades:
        print("\nNav neviena darījuma. Mēģini garāku periodu vai citu pāri.")
        return

    df = pd.DataFrame([t.__dict__ for t in all_trades])
    wins = df[df["pnl_pct"] > 0]
    losses = df[df["pnl_pct"] <= 0]

    # Vienkārša secīga kapitāla simulācija: pilna reinvestīcija katrā darījumā
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
    tail = df.sort_values("entry_time").tail(10)
    for _, t in tail.iterrows():
        print(
            f"  {t['symbol']:12} "
            f"{t['entry_time'].strftime('%Y-%m-%d %H:%M')} "
            f"@ {t['entry_price']:.4f} → "
            f"{t['exit_price']:.4f} "
            f"[{t['reason']}] "
            f"{t['pnl_pct']:+.2f}%"
        )


def parse_args(argv: list[str]) -> tuple[argparse.Namespace, list[str]]:
    p = argparse.ArgumentParser(description="Altcoin stratēģijas backtests")
    p.add_argument("symbols", nargs="*", help="Pāri (piem., SOL/USDT). Ja tukšs, ņem no .env")
    p.add_argument("--candles", type=int, default=1000, help="Cik sveces lejupielādēt")
    p.add_argument("--timeframe", help="Pārraksta TIMEFRAME no .env")
    p.add_argument("--capital", type=float, default=1000.0, help="Sākuma kapitāls USDT")
    args = p.parse_args(argv)
    return args, args.symbols


def main() -> None:
    logging.basicConfig(level=logging.INFO, format="%(levelname)s: %(message)s")
    args, cli_symbols = parse_args(sys.argv[1:])

    cfg = Config()
    if args.timeframe:
        cfg.timeframe = args.timeframe
    symbols: Iterable[str] = cli_symbols or cfg.trading_pairs

    print(
        f"Testējam: {', '.join(symbols)} | intervāls={cfg.timeframe} | "
        f"sveces={args.candles} | TP={cfg.take_profit_percent}% "
        f"SL={cfg.stop_loss_percent}% min_TP_filtrs={cfg.min_take_profit_percent}%"
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


if __name__ == "__main__":
    main()
