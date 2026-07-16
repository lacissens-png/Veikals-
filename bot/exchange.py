"""Binance biržas savienotājs caur ccxt bibliotēku."""
import logging
import ccxt
import pandas as pd

from config import Config

log = logging.getLogger(__name__)


class Exchange:
    def __init__(self, cfg: Config):
        self.cfg = cfg
        params = {
            "apiKey": cfg.api_key,
            "secret": cfg.api_secret,
            "enableRateLimit": True,
            "options": {"defaultType": "spot"},
        }
        self.client = ccxt.binance(params)
        if cfg.use_testnet:
            self.client.set_sandbox_mode(True)
            log.info("Izmantojam Binance TESTNET režīmu")

    def fetch_ohlcv(self, symbol: str, limit: int = 200) -> pd.DataFrame:
        """Iegūt sveces (OHLCV) datus par simbolu."""
        raw = self.client.fetch_ohlcv(symbol, timeframe=self.cfg.timeframe, limit=limit)
        df = pd.DataFrame(
            raw, columns=["timestamp", "open", "high", "low", "close", "volume"]
        )
        df["timestamp"] = pd.to_datetime(df["timestamp"], unit="ms")
        return df

    def get_price(self, symbol: str) -> float:
        ticker = self.client.fetch_ticker(symbol)
        return float(ticker["last"])

    def get_balance(self, asset: str = "USDT") -> float:
        if self.cfg.dry_run:
            return 10_000.0  # simulēts atlikums
        bal = self.client.fetch_balance()
        return float(bal.get(asset, {}).get("free", 0.0))

    def market_buy(self, symbol: str, usdt_amount: float) -> dict:
        price = self.get_price(symbol)
        qty = usdt_amount / price
        qty = float(self.client.amount_to_precision(symbol, qty))
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
