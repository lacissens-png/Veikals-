"""Ieejas punkts. Palaist: python main.py"""
import logging
import sys
from pathlib import Path

from config import Config
from bot import TradingBot


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
    cfg = Config()
    cfg.validate()
    bot = TradingBot(cfg)
    try:
        bot.run()
    except KeyboardInterrupt:
        logging.info("Apturēts ar Ctrl+C")


if __name__ == "__main__":
    main()
