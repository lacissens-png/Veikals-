# Repozitorija saturs — kas šeit ir atrodams

Šis fails ir pilns saraksts ar visu, kas atrasts repozitorijā
`lacissens-png/Veikals-` (visi zari, ne tikai `main`).
Sagatavots: 2026-09-09.

## Kā lasīt

Repozitorijā `main` zarā ir tikai neliels HTML veikala uzmetums.
Viss pārējais darbs dzīvo atsevišķos `claude/*` zaros — tie **nav** iemerģēti
`main`, tāpēc GitHub sākumlapā tos neredz. Lai apskatītu kādu no tiem:

```bash
git fetch origin claude/<zara-nosaukums>
git checkout claude/<zara-nosaukums>
```

## 1. `main` — online veikala uzmetums

| Fails | Apraksts |
|---|---|
| `Index.html` | Veikala lapas karkass: galvene, preču režģis, iepirkumu grozs |
| `Style.css` | Noformējums: divu kolonnu preču režģis, lipīga galvene, pogas |

**Zināmās problēmas (lapa pašlaik nedarbojas):**

1. `Index.html` pieprasa `script.js`, bet **tāda faila repozitorijā nav**.
   Tāpēc nav preču saraksta, grozs nestrādā un `checkout()` izmet kļūdu.
2. `Index.html` pieprasa `style.css` (mazie burti), bet fails saucas
   `Style.css` (lielais S). Uz Linux un GitHub Pages tas nozīmē, ka
   noformējums netiek ielādēts. Uz Windows/macOS tas nejauši strādā.
3. `<main id="product-grid">` ir tukšs — preces bija paredzēts ģenerēt ar
   JavaScript, kura nav.

## 2. Pārējie zari

### `claude/supreme-overlord-core-framework-aokxqp` — 693 faili, 72 commiti (2026-07-29)
Unreal Engine C++ projekts `SupremeOverlord` (izometrisks ARPG).
Pilns `Source/SupremeOverlord/` koks: mana, talanti, quest sistēma, mantu
kritieni, vasaļu izsaukšana, statusa efekti, saglabāšana (`SOSaveGame`),
kritiskie sitieni, bosi, viļņu spawneri. Ir arī `.uproject` un `.slnx`.

### `claude/subscription-audit-app-mvp-t29kym` — 105 faili, 18 commiti (2026-09-03)
Abonementu/rēķinu audita lietotne (MVP). Backend + React Native (Expo)
frontend ar 7 ekrāniem, Enable Banking un Claude integrācija, bankas tokenu
šifrēšana, e-pasta skenēšana abonementu un krāpšanas atrašanai, GitHub
Actions CI, `render.yaml` izvietošanai, palaišanas skripti.
Atvērts PR: https://github.com/lacissens-png/Veikals-/pull/3

### `claude/sveiks-6qjfit` — 14 faili, 20 commitu (2026-07-27)
Spēles dizaina dokumentācija latviski (Overlord × Diablo IV):
`GameDesignDocument.md`, `Aspects.md` (80 aspekti), `Runes.md` (30 rūnas),
`Dungeons.md` (50 pazemju), `ParagonBoards.md`, `Seasons.md`,
`BrokenBuilds.md`, `DamageMath.md` un 3 klašu būvju ceļveži.
Atvērts PR: https://github.com/lacissens-png/Veikals-/pull/2

### `claude/altcoin-trading-bot-2rmuar` — 15 faili, 5 commiti (2026-07-16)
Python altcoin tirdzniecības bots mapē `bot/`: stratēģija, indikatori, riska
pārvaldība, biržas savienojums, backtest modulis un vienfaila versija
`altcoin_bot.py`. Ir `README.md` un `.env.example`.
Atvērts PR: https://github.com/lacissens-png/Veikals-/pull/1

### `claude/cik-gudrs-tu-esi-j1joal` — 3 faili, 4 commiti (2026-07-23)
`pukis.html` — "Puķis", MI draudziņš bērniem vienā HTML failā (2328 rindas):
tēlu izvēle (pūķis, kaķēns, robots, panda), 5 mācību spēles (matemātika,
mīklas, anagrammas, atmiņa, secības), skaņas, sasniegumi, vecāku panelis.

### `claude/piesledzies-manam-unreal-p041d4` — 3 faili, 1 commits (2026-07-28)
`game.html` — 3D raycasting spēle pārlūkā ar Unreal iedvesmotu vizuālo stilu
(607 rindas, bez ārējām bibliotēkām).

### `claude/man-tev-ir-uzdevums-k5briz` — 3 faili, 4 commiti (2026-09-08)
`NISA.md` — nišu analīze 2027-2028. gadam. Gala ieteikums: ES muitas
Product Identifier (PID) lietotne.
Atvērts PR: https://github.com/lacissens-png/Veikals-/pull/4

## 3. Kopsavilkums

- Zari kopā: 8 (`main` + 7 darba zari)
- Atvērti pull request: 4 (Nr. 1, 2, 3, 4) — neviens nav iemerģēts
- Lielākais projekts: Unreal `SupremeOverlord` (693 faili)
- Vienīgais, kas ir `main` zarā: veikala HTML/CSS uzmetums (nepabeigts)
