"""Pozīciju uzskaite un risku pārvaldība (stop-loss / take-profit)."""
import json
import logging
from dataclasses import dataclass, asdict
from pathlib import Path

log = logging.getLogger(__name__)

STATE_FILE = Path(__file__).parent / "logs" / "positions.json"


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


def calc_sl_tp(
    entry_price: float, sl_pct: float, tp_pct: float
) -> tuple[float, float]:
    """Aprēķina stop-loss un take-profit līmeņus long pozīcijai."""
    stop_loss = entry_price * (1 - sl_pct / 100)
    take_profit = entry_price * (1 + tp_pct / 100)
    return stop_loss, take_profit


def should_exit(pos: Position, current_price: float) -> str | None:
    """Atgriež 'SL', 'TP' vai None."""
    if current_price <= pos.stop_loss:
        return "SL"
    if current_price >= pos.take_profit:
        return "TP"
    return None
