"""Galvenā bota loģika — signāla ģenerēšana un orderu izpilde."""
import logging
import time

from config import Config
from exchange import Exchange
from indicators import add_indicators
from strategy import Strategy, Signal
from risk import PositionBook, Position, calc_sl_tp, should_exit

log = logging.getLogger(__name__)


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
                order = self.exchange.market_buy(symbol, self.cfg.trade_amount_usdt)
                entry = float(order["price"])
                sl, tp = calc_sl_tp(
                    entry, self.cfg.stop_loss_percent, self.cfg.take_profit_percent
                )
                self.book.open(
                    Position(
                        symbol=symbol,
                        entry_price=entry,
                        amount=float(order["amount"]),
                        stop_loss=sl,
                        take_profit=tp,
                    )
                )
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
