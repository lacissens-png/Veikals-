"""Konfigurācijas ielāde no .env faila."""
import os
from dataclasses import dataclass, field
from dotenv import load_dotenv

load_dotenv()


def _bool(name: str, default: str = "false") -> bool:
    return os.getenv(name, default).strip().lower() in ("1", "true", "yes", "y")


def _list(name: str, default: str = "") -> list[str]:
    raw = os.getenv(name, default)
    return [x.strip() for x in raw.split(",") if x.strip()]


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
