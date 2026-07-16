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
5. Ja PIRKT signāls un vēl nav sasniegts `MAX_OPEN_POSITIONS`:
   - Pārbauda **iespējamības filtru**: pēdējo `LOOKBACK_CANDLES` sveču
     maksimumam jābūt vismaz par `MIN_TAKE_PROFIT_PERCENT` % virs
     pašreizējās cenas. Ja nav — signāls tiek izlaists.
   - Ja filtrs izturēts — atver pozīciju ar konfigurēto TP/SL.

Tas nozīmē, ka bots **netirgo** pāri, kur tuvāko sveču vēsture neliecina
par vismaz 5% augšupvirziena potenciālu.

## Drošība

- `DRY_RUN=true` — nekādas reālas orderu izpildes, tikai logi.
- `USE_TESTNET=true` — savienojas ar Binance Testnet (bez īstas naudas).
- `.env` fails ir `.gitignore` sarakstā — atslēgas nekad neieceļo git vēsturē.
- Reālai tirdzniecībai konfigurācijas validācijā tiek prasītas API atslēgas.

## Backtests

Pirms live palaišanas pārbaudi stratēģiju uz vēsturiskiem datiem:

```bash
python backtest.py                              # visi pāri no .env
python backtest.py SOL/USDT                     # viens pāris
python backtest.py --candles 2000 SOL/USDT      # cik sveces
python backtest.py --timeframe 1h SOL/USDT      # cits intervāls
python backtest.py --capital 5000 SOL/USDT      # cits sākuma kapitāls
```

Backtestam **nav vajadzīgas API atslēgas** — dati nāk no publiskā Binance
endpoint. Izpildās secīgi pa svecēm, izmantojot to pašu stratēģiju,
5% min-TP filtru un SL/TP loģiku kā live bots.

Izdrukas paraugs:

```
BACKTEST REZULTĀTI
====================================================
Darījumu skaits:       42
  Uzvarēti:            25 (59.5%)
  Zaudēti:             17
Vidējais PnL:          +0.87%
Labākais darījums:     +5.00%
Sliktākais darījums:   -2.00%
Kopējais atgriezums:   +38.42%
Max drawdown:          -9.14%
Izejas iemesli:        TP=22, SL=15, SIGNAL=5
```

## Ieteikumi tālākai attīstībai

- **Vairākas stratēģijas**: MACD, Bollinger Bands, kombinētie signāli.
- **Paziņojumi**: Telegram / e-pasta trauksmes par darījumiem.
- **Dashboard**: web-arī vizualizācija (piem., FastAPI + esošais HTML).
