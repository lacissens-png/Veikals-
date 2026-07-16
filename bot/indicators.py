"""Tehniskie indikatori: EMA un RSI."""
import pandas as pd
from ta.trend import EMAIndicator
from ta.momentum import RSIIndicator


def add_indicators(
    df: pd.DataFrame, ema_fast: int, ema_slow: int, rsi_period: int
) -> pd.DataFrame:
    """Pievieno EMA un RSI kolonnas datu rāmim."""
    df = df.copy()
    df["ema_fast"] = EMAIndicator(df["close"], window=ema_fast).ema_indicator()
    df["ema_slow"] = EMAIndicator(df["close"], window=ema_slow).ema_indicator()
    df["rsi"] = RSIIndicator(df["close"], window=rsi_period).rsi()
    return df
