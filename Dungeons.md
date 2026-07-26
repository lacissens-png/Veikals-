# Dungeoni — 24 Vietas

*Pilna specifikācija par visiem 24 dungeoniem: layout, bosi, Kodekss unlock, Murgu variants.*

Pamata mehānika — skat. `GameDesignDocument.md` §8 un `Aspects.md` §4.

---

## 1. Ievads

**24 dungeoni** izklāsts pa 4 reģioniem:

| Reģions            | Dungeoni | Aptuvenais level range |
|--------------------|----------|-----------------------|
| Melnais Tornis     | 7        | 1–30                  |
| Nordberga          | 7        | 25–55                 |
| Vecais Tornis      | 7        | 50–80                 |
| Melnais Kalns      | 3        | 75–80 + endgame       |
| **Kopā**           | **24**   |                       |

### 1.1. Dungeon tipi

| Tips           | Nozīme                                                              |
|----------------|---------------------------------------------------------------------|
| **Story**      | Galvenā kampaņas dungeon; obligāts sižetiskai virzībai              |
| **Mini**       | Blakus dungeon; ~15-25 min clear                                    |
| **Elite**      | Sarežģītāks blakus dungeon ar Elite Chest guaranteed                |
| **Klases**     | Klases-specifisks dungeon ar tavu Zvērestu-boss cīņu                |
| **Reģiona Boss**| Reģiona lielākais boss (Bornlejs, Solariuss, Selriss)              |
| **Endgame**    | Melnā Kalna specifiski, atslēdzas pēc kampaņas                      |

### 1.2. Murgu Dungeon modifikatori

Katrs dungeon Murgu tierī iegūst **2 random affixi**:

| Affix                  | Efekts                                                         |
|------------------------|----------------------------------------------------------------|
| **Ledus Vēji**         | Katras 15s uzbrūkums pastāvīga sasals ienaidnieku līmenī       |
| **Uguns Pēdas**        | Ceļš pēc ienaidniekiem paliek 3s DoT                           |
| **Melnais Zvans**      | Katras 30s viss ekrāns īsu īpaši lielu dmg spike               |
| **Sardzes Māte**       | Elite bosi ģenerē adds katrs 20s                               |
| **Vēja Solis**         | Ienaidnieku MS +40%                                             |
| **Bruņas Kaisms**      | Ienaidnieku bruņas +30%                                         |
| **Puve Ilgstoša**      | Ienaidnieki dropo DoT lauku uz nāves                           |
| **Zvana Skaņa**        | Ienaidnieku crit chance +25%                                    |
| **Nepieklauses Fāze**  | Boss iesāk fāzē 2 uzreiz pie 90% HP                            |
| **Melnās Uguns**       | Ceļa laukumi dedz — uzmanies uz kā stāvi                        |

---

# 2. Melnais Tornis — Pirmais Kungs kingdom (7 dungeoni)

## #1. **Sardzes Piena Kripta**
*Story · Level 1-5 · Rūnas ligzdas: 0*

**Layout:** 5 telpas linear. **Mini-bosi:** 1 (Novices). **Fināla boss:** Sardzes Novices (~8K HP).
**Kodekss:** *Krusta Uguns* (#3) — universāls elemental.
**Piedziņa:** kampaņas pirmais dungeon. Iepazīsti minjonu komandas pamati.
**Murgu variants:** T20+, novices sauc backup wave.

---

## #2. **Kunga Sirds Kripta**
*Mini · Level 5-10 · Rūnas ligzdas: 1*

**Layout:** 4 telpas ar minion command puzles. **Mini-bosi:** 2. **Fināla boss:** Sardzes Karagalvenis (~15K HP).
**Kodekss:** *Kunga Sirds* (#34) — Pirmais Kungs.
**Piedziņa:** Dominance-fokusa cīņa — boss ģenerē adds tavai Dominance uzkrāj.
**Murgu variants:** T25+, adds pastāvīgi.

---

## #3. **Aura Kripta**
*Mini · Level 8-14 · Rūnas ligzdas: 1*

**Layout:** 5 telpas ar aura zone puzles. **Mini-bosi:** 2. **Fināla boss:** Aura Māstris (25K HP).
**Kodekss:** *Aura Māc* (#36) — Pirmais Kungs.
**Piedziņa:** aura-focused cīņa; buff/debuff sfēras nepieciešami.
**Murgu variants:** T30+, aura sfēras pastāvīgi maināmas.

---

## #4. **Meistara Kripta**
*Klases (Pirmais Kungs) · Level 12-20 · Rūnas ligzdas: 2*

**Layout:** 6 telpas ar visu 4 minjonu tipu izaicinājumiem. **Mini-bosi:** 3. **Fināla boss:** *Meistara Ēna* (bijušais Pirmā Zvērests, 60K HP, 3 fāzes).
**Kodekss:** *Meistara Karogs* (#33) — Pirmais Kungs.
**Piedziņa:** Pirmā Kunga klases stāsta arka; Dominance Zīme Rūna drop.
**Murgu variants:** T40+, boss izmanto katru minjonu tipu.

---

## #5. **Vairoga Kripta**
*Elite · Level 15-22 · Rūnas ligzdas: 2*

**Layout:** 6 telpas ar Barrier-heavy ienaidniekiem. **Mini-bosi:** 3. **Fināla boss:** Vairoga Bīskaps (100K HP, refills Barrier).
**Kodekss:** *Kunga Vairogs* (#37) — Pirmais Kungs.
**Piedziņa:** Barrier-based cīņa; ideāls Sardzes Zvērests build test.
**Murgu variants:** T45+, Barrier +50%.

---

## #6. **Domination Kripta**
*Mini · Level 20-26 · Rūnas ligzdas: 1*

**Layout:** 5 telpas ar mob density challenges. **Mini-bosi:** 3. **Fināla boss:** Domination Karagalvenis (80K HP + auto-summon).
**Kodekss:** *Dominance Karogs* (#38) — Pirmais Kungs.
**Piedziņa:** area denial cīņa; ideāls Kunga Impērija build.
**Murgu variants:** T40+, spawn rate 2×.

---

## #7. **Sardzes Kapteinis Bornlejs Trono** — Reģiona Boss
*Reģiona Boss · Level 30 · Rūnas ligzdas: 3*

**Layout:** Melnā Torņa augstākā zāle. **Mini-bosi:** 3 (Bornleja apsargi). **Fināla boss:** **Sardzes Kapteinis Bornlejs** (150K HP, 3 fāzes, pēdējais gaisais paladins).
**Kodekss:** *Universālais Kalts* (#39) — Pirmais Kungs Reliktu-only, arī no bosā drops.
**Piedziņa:** Pirmā Kunga klases boss + Melnā Torņa kampaņas fināls. Guarantee 1 Reliktu.
**Murgu variants:** T60+, 4. fāze ar visiem 4 minjonu tipiem, drop chance uz Uber.

---

# 3. Nordberga — Otrs Kungs kingdom (7 dungeoni)

## #8. **Impērijas Vārti**
*Story · Level 25-30 · Rūnas ligzdas: 2*

**Layout:** 5 sniegotas arēnas. **Mini-bosi:** 2. **Fināla boss:** Impērijas Vārtu Sargs (~90K HP).
**Kodekss:** *Krusta Karogs* nav — universāls *Vēja Ass* (#2).
**Piedziņa:** Nordbergas ievads, Impēriskā Dusma mehānika.
**Murgu variants:** T35+, sniegoti vēji sit dmg.

---

## #9. **Nordbergas Zīme Kripta**
*Mini · Level 28-34 · Rūnas ligzdas: 1*

**Layout:** 4 telpas ar Sūkšņi + Zilspāri sinerģijas puzles. **Mini-bosi:** 2. **Fināla boss:** Nordbergas Karagalvenis (100K HP).
**Kodekss:** *Nordbergas Karogs* (#41) — Otrs Kungs.
**Piedziņa:** hybrid melee + spell cīņa.
**Murgu variants:** T40+, minjonu tipi rotējas ienaidnieku pusē.

---

## #10. **Impērijas Kripta**
*Mini · Level 32-38 · Rūnas ligzdas: 1*

**Layout:** 5 telpas ar iekarošanas mehāniku. **Mini-bosi:** 3. **Fināla boss:** Impērijas Bīskaps (110K HP).
**Kodekss:** *Impērijas Vaids* (#42) — Otrs Kungs.
**Piedziņa:** "gaišā" ordeņa fokusa cīņa — Imp. Dusma uzkrāj ātri.
**Murgu variants:** T45+, boss dubultā prasme pret Otro Kungu.

---

## #11. **Zibens Kripta**
*Mini · Level 35-42 · Rūnas ligzdas: 2*

**Layout:** 6 telpas ar chain lightning tēmām. **Mini-bosi:** 3. **Fināla boss:** Zibens Māstris (140K HP).
**Kodekss:** *Melnās Zibens Kronis* (#44) — Otrs Kungs.
**Piedziņa:** Chain lightning build test; ideāls Otrs Kungs Build B.
**Murgu variants:** T50+, ienaidnieku chain lightning atsvēr.

---

## #12. **Impērija Vaidēs Kripta**
*Klases (Otrs Kungs) · Level 40-48 · Rūnas ligzdas: 2*

**Layout:** 6 telpas ar peldošu formu izaicinājumiem. **Mini-bosi:** 3. **Fināla boss:** *Impēriskā Ēna* (bijušais Otrā Zvērests, 200K HP, 3 fāzes, peldošā formā).
**Kodekss:** *Peldošā Sirds* (#46) — Otrs Kungs.
**Piedziņa:** Otrā Kunga klases stāsta arka; Impērijas Zīme Rūna drop.
**Murgu variants:** T55+, boss pastāvīgi peldošā formā.

---

## #13. **Vairoga Māca Kripta**
*Elite · Level 45-50 · Rūnas ligzdas: 2*

**Layout:** 6 telpas ar heavy armored ienaidniekiem. **Mini-bosi:** 3. **Fināla boss:** Vairoga Māstris (200K HP + Impēriskais Vairogs).
**Kodekss:** *Impērisks Vairogs* (#45) — Otrs Kungs.
**Piedziņa:** high-armor cīņa; nepieciešams Physical Reizinātājs.
**Murgu variants:** T55+, boss auto-refresh Impērisks Vairogs.

---

## #14. **Emperors Solariuss Trono** — Reģiona Boss
*Reģiona Boss · Level 55 · Rūnas ligzdas: 3*

**Layout:** Glorius Empire tronis, 4 fāzes vietas. **Mini-bosi:** 3 (Empire senatori). **Fināla boss:** **Emperors Solariuss** (400K HP, 3 fāzes, tagad kā līķis-imperators).
**Kodekss:** *Zibens Kalts* (#48) — Otrs Kungs.
**Piedziņa:** Otrā Kunga klases boss + Nordbergas fināls.
**Murgu variants:** T65+, 4. fāze ar Solariuss ceļ visu Empire armiju + drop chance uz Uber.

---

# 4. Vecais Tornis — Vecais Burvis kingdom (7 dungeoni)

## #15. **Realitātes Vārti**
*Story · Level 50-56 · Rūnas ligzdas: 2*

**Layout:** 5 peldošas platformas ar mainīgu gravitāciju. **Mini-bosi:** 2. **Fināla boss:** Realitātes Novices (~160K HP).
**Kodekss:** *Sasala Zvana* (#6) — universāls Vulnerable.
**Piedziņa:** Vecā Torņa ievads; Ark. Enerģija mehānika.
**Murgu variants:** T55+, gravitācija mainās pastāvīgi.

---

## #16. **Torņa Sirds Kripta**
*Mini · Level 55-60 · Rūnas ligzdas: 2*

**Layout:** 5 telpas ar Zilspāri koloniju. **Mini-bosi:** 2 (Zilspāri elite). **Fināla boss:** Torņa Māte (200K HP + auto-sauc Zilspāri).
**Kodekss:** *Torņa Sirds* (#50) — Vecais Burvis Reliktu-only, arī no boss drops.
**Piedziņa:** minjonu spell density; ideāls Zilspāri Storm.
**Murgu variants:** T60+, Zilspāri spell power dubultā.

---

## #17. **Ledus Šauta Ceļš**
*Mini · Level 58-64 · Rūnas ligzdas: 1*

**Layout:** 4 telpas ar piercing projectile tēmām. **Mini-bosi:** 2. **Fināla boss:** Ledus Šauta Māstris (180K HP + līņas platformā).
**Kodekss:** *Ledus Šauta Ceļš* (#52) — Vecais Burvis.
**Piedziņa:** piercing damage cīņa.
**Murgu variants:** T60+, mērķi atsvēr piercing.

---

## #18. **Realitātes Karogs Kripta**
*Elite · Level 62-68 · Rūnas ligzdas: 2*

**Layout:** 6 telpas ar time slow mehāniku. **Mini-bosi:** 3. **Fināla boss:** Realitātes Bīskaps (220K HP + time slow zones).
**Kodekss:** *Realitātes Karogs* (#49) — Vecais Burvis.
**Piedziņa:** Realitātes Karaļvalsts build test.
**Murgu variants:** T65+, time slow arī iet uz tevi 50%.

---

## #19. **Torņa Redzējuma Kripta**
*Klases (Vecais Burvis) · Level 65-72 · Rūnas ligzdas: 2*

**Layout:** 6 telpas — visas šķiet vienādas, jāsaproti caur reality distortion. **Mini-bosi:** 3. **Fināla boss:** *Realitātes Ēna* (bijušais Vecā Zvērests, 300K HP, 3 fāzes, klonējas reality distortion).
**Kodekss:** *Divu Cast Kronis* (#53) — Vecais Burvis Reliktu-only, arī no boss.
**Piedziņa:** Vecā Burvja klases stāsta arka; Realitātes Zīme Rūna drop.
**Murgu variants:** T70+, boss klonējas 5× (jātiek gaŗam ar pareizo real target).

---

## #20. **Sinerģija Kripta**
*Mini · Level 68-74 · Rūnas ligzdas: 1*

**Layout:** 5 telpas ar Zilspāri spell chain puzles. **Mini-bosi:** 3. **Fināla boss:** Sinerģija Māstris (240K HP).
**Kodekss:** *Arkānā Sinerģija* (#55) — Vecais Burvis.
**Piedziņa:** koop-tēma; grupā vienkāršs; solo grūtāks.
**Murgu variants:** T60+, boss dubultā HP, ja solo.

---

## #21. **Vecākais Māceklis Selriss Trono** — Reģiona Boss
*Reģiona Boss · Level 78 · Rūnas ligzdas: 3*

**Layout:** Vecā Torņa virsotne, peldoši reality fragments. **Mini-bosi:** 3 (Zvēresta mācekļi). **Fināla boss:** **Vecākais Māceklis Selriss** (700K HP, 3 fāzes, mēģina turēt Torni pēc Burvja pazušanas).
**Kodekss:** *Vientuļā Sirds* (#51) — Vecais Burvis.
**Piedziņa:** Vecā Burvja klases boss + Vecā Torņa fināls.
**Murgu variants:** T80+, 4. fāze ar reality collapse (jātiek pa fragmentiem) + drop chance uz Uber.

---

# 5. Melnais Kalns — Endgame (3 dungeoni)

## #22. **Melnā Kalna Kalts**
*Endgame Utility · Level 75+ · Rūnas ligzdas: 3*

**Layout:** 8 telpas, deepest crafting halls. **Mini-bosi:** 4 (Kalēju Vecajs). **Fināla boss:** Melnais Kalējs (400K HP).
**Kodekss:** *Bezdibeņa Zvana* (#24) — universāls kaltēšanas ātrums.
**Piedziņa:** endgame utility — pabeidzot dropo **Zvaigznes Skabarga** garantēts un unlockē Ancient Weapon crafting (Season 3+).
**Murgu variants:** T80+, drops Zvaigznes Skabarga ar 25% šansei per run.

---

## #23. **Nāves Kripta**
*Endgame · Level 78+ · Rūnas ligzdas: 3*

**Layout:** 10 telpas, dziļākais dungeon pirms katakombas. **Mini-bosi:** 5. **Fināla boss:** **Nāves Vēstnesis** (500K HP, 3 fāzes).
**Kodekss:** *Kritušā Solis* (#20) — universāls nāves atgriezšana.
**Piedziņa:** endgame chase dungeon, guarantee Uber Mistiskais chance per run.
**Murgu variants:** T90+, 4. fāze ar Nāves Vēstnesis "true form" — grūtāka cīņa spēles.

---

## #24. **Trīs Zvērestu Sinerģija** — Multi-Class Fināls
*Endgame · Level 80+ · Rūnas ligzdas: 4*

**Layout:** 3 fāzes — katra veltīta vienam Zvērestam (Pirmais / Otrs / Vecais). **Mini-bosi:** 3 (viens per Zvērests). **Fināla boss:** **Trīs Zvērestu Sinerģija** (1M HP, kombinē visu 3 klašu prasmes).
**Kodekss:** *Nolādētais Sirdsdedzis* (Rūna, no boss).
**Piedziņa:** endgame — visu 3 klašu meistrības test.
**Murgu variants:** T100, guaranteed Uber Mistiskais + Uber Gem chance.

---

# 6. Kopsavilkums

## 6.1. Sadalījums pa tipiem

| Tips           | Skaits |
|----------------|--------|
| Story          | 3      |
| Mini           | 11     |
| Elite          | 3      |
| Klases         | 3 (1 per klase) |
| Reģiona Boss   | 3      |
| Endgame        | 3      |
| **Kopā**       | **24** |

## 6.2. Kodekss unlock summary

- **Aspekti Kodeksā:** ~20 (skat. individuālie ieraksti; pārējie ~25 pieejami no citiem dungeoniem vai Elite bosiem)
- **Aspekti no bosiem:** ~15
- **Reliktu-only Aspekti:** 11 (skat. Aspects.md §5)

Kopā: **56 Aspekti** (kā Aspects.md).

## 6.3. Vidējais dungeon clear laiks

| Tips      | Level      | Solo clear laiks |
|-----------|------------|------------------|
| Mini      | early      | 8-15 min         |
| Story     | mid        | 20-35 min        |
| Elite     | mid-late   | 25-45 min        |
| Klases    | any        | 30-60 min        |
| Reģiona Boss | endgame  | 45-90 min        |
| Endgame   | endgame    | 60-120 min       |

## 6.4. Ieteikums progresai

1. **Level 1-30:** izlaid Melnā Torņa 7 dungeonus + reģiona boss.
2. **Level 30-55:** izlaid Nordbergas 7.
3. **Level 55-80:** izlaid Vecā Torņa 7.
4. **Level 80+:** endgame Melnais Kalns 3 + Murgu grinds.

**Full campaign clear:** ~30-45 stundas casual, ~20-25h speedrun.
**All Kodekss unlocks:** ~60-90 stundas.

---

*Skaitļi un layouts ir sākuma dizaina piedāvājums. Tuning notiek pēc playtesting.*
