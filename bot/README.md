# Altcoin tirdzniecības bots

Vienkāršs Python bots altcoinu (SOL, AVAX, MATIC u.c.) tirdzniecībai Binance biržā.
Stratēģija: **EMA(9)/EMA(21) krusts + RSI apstiprinājums**.

> ⚠️ **Brīdinājums:** kriptovalūtu tirdzniecība ir riskanta. Šis kods ir
> izglītojošam nolūkam. Pēc noklusējuma bots strādā **DRY-RUN** (simulācijas)
> režīmā un uz **Binance Testnet**. Reālu naudu iesaisti tikai pēc rūpīgas
> pārbaudes.

## Struktūra

```
bot/
├── main.py         # Ieejas punkts
├── bot.py          # Galvenā cilpa
├── config.py       # Konfigurācijas ielāde
├── exchange.py     # Binance savienotājs (ccxt)
├── indicators.py   # EMA, RSI
├── strategy.py     # Signālu ģenerēšana
├── risk.py         # Pozīcijas, SL/TP
├── requirements.txt
└── .env.example    # Kopēt uz .env un aizpildīt
```

## Uzstādīšana

```bash
cd bot
python3 -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt
cp .env.example .env
# Rediģē .env un ievieto savas Binance API atslēgas
```

## Palaišana

```bash
python main.py
```

Logi tiek rakstīti gan konsolē, gan `logs/bot.log`. Atvērtās pozīcijas
saglabājas `logs/positions.json` — bots atsāk darbu pēc restartēšanas.

## Kā tas strādā

Katru ciklu (pēc noklusējuma ik 60 s) bots katram tirdzniecības pārim:

1. Ielādē pēdējās 200 sveces (`TIMEFRAME` intervālā).
2. Aprēķina EMA fast/slow un RSI.
3. Ja pozīcija ir atvērta — pārbauda stop-loss un take-profit līmeņus.
4. Ja pozīcijas nav — meklē signālu:
   - **PIRKT**: fast EMA šķērso slow EMA uz augšu **un** RSI < overbought.
   - **PĀRDOT**: fast EMA šķērso slow EMA uz leju **vai** RSI > overbought.
5. Ja PIRKT signāls un vēl nav sasniegts `MAX_OPEN_POSITIONS` — atver pozīciju.

## Drošība

- `DRY_RUN=true` — nekādas reālas orderu izpildes, tikai logi.
- `USE_TESTNET=true` — savienojas ar Binance Testnet (bez īstas naudas).
- `.env` fails ir `.gitignore` sarakstā — atslēgas nekad neieceļo git vēsturē.
- Reālai tirdzniecībai konfigurācijas validācijā tiek prasītas API atslēgas.

## Ieteikumi tālākai attīstībai

- **Backtesting**: pievienot vēsturisko datu simulāciju pirms live palaišanas.
- **Vairākas stratēģijas**: MACD, Bollinger Bands, kombinētie signāli.
- **Paziņojumi**: Telegram / e-pasta trauksmes par darījumiem.
- **Dashboard**: web-arī vizualizācija (piem., FastAPI + esošais HTML).
