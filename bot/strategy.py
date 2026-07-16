"""Tirdzniecības stratēģija: EMA krusts + RSI apstiprinājums."""
from enum import Enum
import pandas as pd

from config import Config


class Signal(Enum):
    BUY = "BUY"
    SELL = "SELL"
    HOLD = "HOLD"


class Strategy:
    def __init__(self, cfg: Config):
        self.cfg = cfg

    def generate_signal(self, df: pd.DataFrame) -> Signal:
        """
        Ģenerē signālu no pēdējām divām svecēm.
        PIRKT: fast EMA šķērso slow EMA uz augšu un RSI nav pārpirkts.
        PĀRDOT: fast EMA šķērso slow EMA uz leju vai RSI ir pārpirkts.
        """
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

        rsi = last["rsi"]

        if cross_up and rsi < self.cfg.rsi_overbought:
            return Signal.BUY
        if cross_down or rsi > self.cfg.rsi_overbought:
            return Signal.SELL
        return Signal.HOLD

    def tp_upside_percent(self, df: pd.DataFrame, current_price: float) -> float:
        """
        Cik % virs pašreizējās cenas ir pēdējo LOOKBACK_CANDLES sveču maksimums.
        Izmantojam kā indikatoru tam, vai TP vispār ir reāli sasniedzams.
        """
        window = df.tail(self.cfg.lookback_candles)
        recent_high = float(window["high"].max())
        return (recent_high - current_price) / current_price * 100
