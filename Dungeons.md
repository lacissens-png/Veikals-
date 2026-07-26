# Dungeoni — 50 Vietas

*Pilna specifikācija par visiem 50 dungeoniem: layout, bosi, Kodekss unlock, Murgu variants.*

Pamata mehānika — skat. `GameDesignDocument.md` §8.13 un `Aspects.md` §4.

---

## 1. Ievads

**50 dungeoni** izklāsts pa 7 reģioniem:

| Reģions            | Dungeoni | Aptuvenais level range |
|--------------------|----------|-----------------------|
| Debesu Virsotne    | 9        | 1–25                  |
| Asins Arēna        | 8        | 15–40                 |
| Zelta Raktuves     | 9        | 25–50                 |
| Puvušais Mežs      | 8        | 35–60                 |
| Tuksneša Bāzars    | 7        | 45–70                 |
| Ledus Kroņi        | 7        | 55–80                 |
| Melnais Kalns      | 2        | 75–80 + endgame       |
| **Kopā**           | **50**   |                       |

### 1.1. Dungeon tipi

| Tips           | Nozīme                                                              |
|----------------|---------------------------------------------------------------------|
| **Story**      | Galvenā kampaņas dungeon; obligāts sižetiskai virzībai              |
| **Mini**       | Blakus dungeon; ~15-25 min clear                                    |
| **Elite**      | Sarežģītāks blakus dungeon ar Elite Chest guaranteed                |
| **Klases**     | Klases-specifisks dungeon ar tavu Zvērestu-boss cīņu                |
| **Reģiona Boss**| Reģiona lielākais boss (Ansels, Talos, Goldo, Ninhera, Vezirs, Turaņa) |
| **Endgame**    | Melnā Kalna specifiski, atslēdzas pēc kampaņas                      |

### 1.2. Kopīgā dungeon struktūra

Katrs dungeon:
- **3-7 telpas** / stāvi
- **1-4 Elite mini-bosi** (katrs ar savu Elite Chest)
- **1 fināla boss** (ar **Kunga Lāde** garantēts drop)
- **Kodekss Aspekts unlock** pirmajā pabeigšanā (ne visiem — daži tikai mob density)
- **Murgu variants** (T1–T100) atslēdzas endgame

### 1.3. Murgu Dungeon modifikatori

Katrs dungeon Murgu tierī iegūst **2 random affixi** no pool:

| Affix                  | Efekts                                                         |
|------------------------|----------------------------------------------------------------|
| **Ledus Vēji**         | Katras 15s uzbrūkums pastāvīga sasals ienaidnieku līmenī       |
| **Uguns Pēdas**        | Ceļš pēc ienaidniekiem paliek 3s DoT                           |
| **Melnais Zvans**      | Katras 30s viss ekrāns īsu īpaši lielu dmg spike (izvairies!)  |
| **Sardzes Māte**       | Elite bosi ģenerē adds katrs 20s                               |
| **Vēja Solis**         | Ienaidnieku MS +40%                                             |
| **Bruņas Kaisms**      | Ienaidnieku bruņas +30%                                         |
| **Puve Ilgstoša**      | Ienaidnieki dropo DoT lauku uz nāves                           |
| **Zvana Skaņa**        | Ienaidnieku crit chance +25%                                    |
| **Nepieklauses Fāze**  | Boss iesāk fāzē 2 uzreiz pie 90% HP                            |
| **Melnās Uguns**       | Ceļa laukumi dedz — uzmanies uz kā stāvi                        |

---

# 2. Debesu Virsotne (9 dungeoni)

## #1. **Kritušā Ordeņa Kripta**
*Story · Level 1-5 · Rūnas ligzdas: 0*

**Layout:** 5 telpas linear. **Mini-bosi:** 1 (Novices). **Fināla boss:** Krisis Bīskaps (~8K HP).
**Kodekss:** *Krusta Karogs* (#33) — Bruņinieks spec.
**Piedziņa:** kampaņas pirmais dungeon. Iepazīsti spēles pamati.
**Murgu variants:** T20+, jaunas Novices grupas ar Cast prasmēm.

---

## #2. **Zvana Sirds Kripta**
*Mini · Level 5-10 · Rūnas ligzdas: 1*

**Layout:** 4 telpas ar 2 boss cīņas fāzēm. **Mini-bosi:** 2 (Novices grupas). **Fināla boss:** Zvana Sirds Novices Karagalvenis (~15K HP).
**Kodekss:** *Zvana Barjēra* (#11).
**Piedziņa:** Barrier-fokusa cīņa — boss met Barjeras uz savām grupām.
**Murgu variants:** T25+, Barjeras stack agresīvāk.

---

## #3. **Bīskapa Roku Kripta**
*Mini · Level 6-12 · Rūnas ligzdas: 1*

**Layout:** 5 telpas ar 1 slēptu istaba. **Mini-bosi:** 1. **Fināla boss:** Bīskapa Roka (kolosāla akmens roka, 25K HP).
**Kodekss:** *Grēksūdzes Piespriedu* (#37) — Bruņinieks.
**Piedziņa:** slēpta istaba dropo klases-agnostiskā Uber Mistiskais chase materiāls.
**Murgu variants:** T30+, Roka pieaug par 3× HP pie 25%.

---

## #4. **Katedrāles Kripta**
*Elite · Level 10-15 · Rūnas ligzdas: 2*

**Layout:** 6 telpas, 2 stāvi. **Mini-bosi:** 3. **Fināla boss:** Katedrāles Mūks (30K HP + 3 kohortas).
**Kodekss:** *Katedrāles Vēls* (#39) — Bruņinieks.
**Piedziņa:** pirmais elite dungeon ar 3 Elite Chest garantētiem.
**Murgu variants:** T35+, Mūks izraisa massive AoE ērģeļu skaņu (izvairies).

---

## #5. **Pazudušā Bīskapa Kripta**
*Story · Level 12-18 · Rūnas ligzdas: 2*

**Layout:** 7 telpas ar sarežģītu labirintu. **Mini-bosi:** 3. **Fināla boss:** Pazudušais Bīskaps (50K HP, 2 fāzes).
**Kodekss:** *Aizmirsts Vaids* (#5) — universāls.
**Piedziņa:** kampaņas otrs galvenais dungeon; unlock jaunu prasmes slotu.
**Murgu variants:** T40+, Bīskaps sauc palīgus no līķu grupām apkārt.

---

## #6. **Krusta Sardzes Kripta**
*Klases (Bruņinieks) · Level 15-20 · Rūnas ligzdas: 2*

**Layout:** 5 telpas. **Mini-bosi:** 2. **Fināla boss:** *Kritušā Sardzes* (tavs bijušais Zvērests, 60K HP, 3 fāzes).
**Kodekss:** *Kritušā Zvana* (#12) — universāls DR.
**Piedziņa:** Bruņinieka klases stāsta arka; klases-specifiska Rūna Sardzes Zīme drop.
**Murgu variants:** T45+, Sardzes cīņas fāzes 2 ir tavs paša "false" build cīņā pret tevi.

---

## #7. **Nemirstīgā Bīskapa Kripta**
*Elite · Level 18-25 · Rūnas ligzdas: 2*

**Layout:** 6 telpas, 3 slēpti sarkas. **Mini-bosi:** 3. **Fināla boss:** Nemirstīgais Bīskaps (80K HP, refills 30% HP pie 20%).
**Kodekss:** *Krusta Barjēra* (#14) — universāls Barrier proc.
**Piedziņa:** sustain-based cīņa; ideāls Heal-Tank test.
**Murgu variants:** T50+, refill count +2 (kopā 3× refills).

---

## #8. **Krusta Solis Kripta**
*Mini · Level 20-25 · Rūnas ligzdas: 1*

**Layout:** 4 telpas platform-hopping. **Mini-bosi:** 2. **Fināla boss:** Krusta Skrējēju Kapteinis (60K HP, ātrs).
**Kodekss:** *Krusta Sirds* (#22) — Dodge fokuss.
**Piedziņa:** kustības tests; Dodge builds noderīgi.
**Murgu variants:** T35+, platform sabrūk aiz tevis (jāsteidz).

---

## #9. **Uguns Zvana Kripta** — *Bīskaps Ansels boss*
*Reģiona Boss · Level 25 · Rūnas ligzdas: 3*

**Layout:** Katedrāles augstākais tornis, 3 stāvi + fināla plaza. **Mini-bosi:** 4 (visu Debesu Virsotnes ordeņa liekšanas). **Fināla boss:** **Bīskaps Ansels** (150K HP, 3 fāzes, sešroku vampīrs pie ērģelēm).
**Kodekss:** *Zvana Sprādzien* (#40) — Bruņinieks hibrīds.
**Piedziņa:** Reģiona fināls, pirmais milzīgais boss. Guarantee 1 Reliktu drop.
**Murgu variants:** T60+, 4. fāze ar visu triju pārvēršanu, drop chance uz Uber Mistiskais.

---

# 3. Asins Arēna (8 dungeoni)

## #10. **Vēju Māja**
*Mini · Level 15-20 · Rūnas ligzdas: 2*

**Layout:** 5 arēnas, vējš aizrauj tevi ceļā. **Mini-bosi:** 2. **Fināla boss:** Vēju Kapteinis (60K HP).
**Kodekss:** *Vēja Ass* (#2) VAI *Vēja Ceļš* (#41) — nejauši viens.
**Piedziņa:** ātrs, kustības-based dungeon.
**Murgu variants:** T35+, vēji arī sit dmg.

---

## #11. **Ķēdes Māja**
*Mini · Level 18-24 · Rūnas ligzdas: 1*

**Layout:** 4 arēnas ar ķēdes puzles. **Mini-bosi:** 2. **Fināla boss:** Verdzenieku Pārraugs (70K HP).
**Kodekss:** *Sirds Solis (M)* (#30).
**Piedziņa:** verdzeņu atbrīvošana blakusuzdevums, dropo cosmetic.
**Murgu variants:** T35+, verdzeņi kļūst par apsēstiem sabiedrotiem.

---

## #12. **Asins Kripta**
*Story · Level 22-28 · Rūnas ligzdas: 2*

**Layout:** 6 arēnas. **Mini-bosi:** 3. **Fināla boss:** Asins Karagalvenis (90K HP, 2 fāzes).
**Kodekss:** *Asins Ātruma* (#43) — Berserker.
**Piedziņa:** kampaņas story dungeon.
**Murgu variants:** T45+, boss uzsāk Asins Trakumā.

---

## #13. **Trakuma Sirds Kripta**
*Klases (Berserker) · Level 25-30 · Rūnas ligzdas: 2*

**Layout:** 5 arēnas, katras pieaugs karstas smilšas. **Mini-bosi:** 3. **Fināla boss:** *Trakuma Sirds* (tavs bijušais Berserker Zvērests, 100K HP, 3 fāzes).
**Kodekss:** *Neapstādināmais* (#44).
**Piedziņa:** Berserker klases stāsta arka; Trakuma Zīme Rūna drop.
**Murgu variants:** T50+, boss ir pastāvīgi Asins Trakumā (glass cannon vs glass cannon).

---

## #14. **Karoga Kripta**
*Elite · Level 25-32 · Rūnas ligzdas: 2*

**Layout:** 6 arēnas, karogs jānoved kartei. **Mini-bosi:** 3 (katrs sarga karogu). **Fināla boss:** Karogu Karagalvenis (110K HP).
**Kodekss:** *Karoga Sirds* (#45) — Berserker.
**Piedziņa:** grupas kontenta ideāls, karogs mehānika.
**Murgu variants:** T50+, karogu jānodrošina 3 vietās vienlaikus.

---

## #15. **Kliedziena Kripta**
*Mini · Level 28-34 · Rūnas ligzdas: 1*

**Layout:** 4 arēnas ar reverberācijas mehāniku. **Mini-bosi:** 2. **Fināla boss:** Kliedzēja (60K HP + izraisa echo hits).
**Kodekss:** *Kliedziena Vējš* (#47) — Berserker.
**Piedziņa:** unique audio dungeon — vizuāli klusi, viss caur skaņu.
**Murgu variants:** T40+, echo hit +50% dmg.

---

## #16. **Karaļa Māja**
*Story · Level 30-38 · Rūnas ligzdas: 2*

**Layout:** 7 arēnas, katras kompana no bijušajiem karaļiem. **Mini-bosi:** 4. **Fināla boss:** Karaļa Rēguls (130K HP, 3 fāzes).
**Kodekss:** *Karaļa Zvans* (#48) — Berserker.
**Piedziņa:** kampaņas dungeon ar lore reveal.
**Murgu variants:** T55+, katrā fāzē karalis sauc citas frakcijas kā adds.

---

## #17. **Nemirstīgā Trono Kripta** — *Karagalvenis Talos boss*
*Reģiona Boss · Level 38 · Rūnas ligzdas: 3*

**Layout:** Milzīga arēna + fināla plaza. **Mini-bosi:** 3 klanu karavīri. **Fināla boss:** **Karagalvenis Talos** (250K HP, 3 fāzes, trīsroku).
**Kodekss:** *Nemirstīgs Karogs* (#7) — universāls crit chance.
**Piedziņa:** Berserker klases boss (arī citām klasēm), pirmais tiešs Zvērests-vs-Zvērests.
**Murgu variants:** T60+, 4. fāze ar Talos "berserker" mode + drop chance uz Uber.

---

# 4. Zelta Raktuves (9 dungeoni)

## #18. **Uguns Kripta**
*Mini · Level 25-30 · Rūnas ligzdas: 2*

**Layout:** 5 telpas ar magmas plūsmām. **Mini-bosi:** 2. **Fināla boss:** Uguns Elementāls (80K HP).
**Kodekss:** *Krusta Uguns* (#3) — universāls elemental.
**Piedziņa:** Fire dmg fokuss, Ugunslīnija Rūnu drops.
**Murgu variants:** T40+, magma plūsmas ceļ ceļus.

---

## #19. **Bruņu Kalts**
*Elite · Level 28-34 · Rūnas ligzdas: 2*

**Layout:** 6 telpas ar craftinga tēmām. **Mini-bosi:** 3 Kalēju Māstri. **Fināla boss:** Vecais Kalējs Būris (120K HP + ekstrēmi bruņu).
**Kodekss:** *Bruņu Vaids* (#9).
**Piedziņa:** high-armor cīņa — parpower nevar caururbt bez Physical Reizinātājs.
**Murgu variants:** T50+, kalējs kalta jaunas bruņas pastāvīgi.

---

## #20. **Zvana Kripta**
*Mini · Level 30-36 · Rūnas ligzdas: 1*

**Layout:** 4 telpas ar pundurķēniņu koncertzāli. **Mini-bosi:** 2. **Fināla boss:** Pundurķēniņu Zvana Māstris (100K HP).
**Kodekss:** *Alkas Aura* (#50) — Alkas Kungs.
**Piedziņa:** audio-based dungeon, zvana skaņa dod buff.
**Murgu variants:** T40+, zvana skaņa dubultā ienaidnieku dmg (jāapstādina zvani).

---

## #21. **Monētu Kripta**
*Mini · Level 32-38 · Rūnas ligzdas: 1*

**Layout:** 5 telpas ar monētu upēm. **Mini-bosi:** 2. **Fināla boss:** Monētu Sirds (90K HP).
**Kodekss:** *Monētu Sirds* (#51) — Alkas Kungs.
**Piedziņa:** zelta drop 3× (loot spēle).
**Murgu variants:** T45+, ienaidnieku Zelts izzūd 2× ātrāk (aug spiediens Bagātā Sirds sinerģijai).

---

## #22. **Bezcelis Kripta**
*Challenge · Level 35-42 · Rūnas ligzdas: 2*

**Layout:** 6 telpas, katras zelta bilance sabrūk uz 0. **Mini-bosi:** 3. **Fināla boss:** Bezcelis Kunga Sekotāji (140K HP kopā).
**Kodekss:** *Zelta Sirdsdedzis* (#53) — Alkas Kungs.
**Piedziņa:** tavs zelts nulle visā dungeonā — jātiek galā bez resursa.
**Murgu variants:** T55+, tu arī pastāvīgi zaudē 100 HP/sec.

---

## #23. **Alkas Kripta**
*Klases (Alkas Kungs) · Level 40-45 · Rūnas ligzdas: 2*

**Layout:** 5 telpas. **Mini-bosi:** 3. **Fināla boss:** *Alkas Ēna* (tavs bijušais Alkas Zvērests, 180K HP, 3 fāzes).
**Kodekss:** *Alkas Zvana Kronis* (#54).
**Piedziņa:** Alkas Kunga klases stāsta arka; Alkas Vaids Rūna drop.
**Murgu variants:** T55+, boss dubultā resursus par nāvēm.

---

## #24. **Konstrukta Kripta**
*Mini · Level 42-48 · Rūnas ligzdas: 2*

**Layout:** 5 telpas ar mehāniskiem konstruktiem. **Mini-bosi:** 3 (Golemi). **Fināla boss:** Māstris Konstruktors (160K HP).
**Kodekss:** *Divi Golemi* (#55) — Alkas Kungs (KODEKSA + Reliktu abas versijas).
**Piedziņa:** golemu-heavy cīņa, Konstruktu build test.
**Murgu variants:** T55+, konstrukti autoreplaced pēc katra 30s.

---

## #25. **Kāzu Kripta**
*Mini · Level 42-48 · Rūnas ligzdas: 1*

**Layout:** 4 telpas, kāzu tēma pundurķēniņu stilā. **Mini-bosi:** 2 (Līgava un Līgavainis kā elite). **Fināla boss:** Kāzu Ceremonijas Meistars (140K HP, 2 fāzes — svētki tad kaušana).
**Kodekss:** *Metāla Kāzas* (#56) — Alkas Kungs.
**Piedziņa:** unikāls tone shift dungeon — jautrs pirms tumšs.
**Murgu variants:** T50+, kāzu viesi kļūst par pastāvīgu adds waves.

---

## #26. **Vecais Goldo Trono** — *Vecais Goldo boss*
*Reģiona Boss · Level 48 · Rūnas ligzdas: 3*

**Layout:** Milzīga zelta zāle, magma tumsā. **Mini-bosi:** 3 (Goldo apsargi). **Fināla boss:** **Vecais Goldo** (350K HP, 3 fāzes, tavas paša sen-vec-vec-tēvs Zvērests).
**Kodekss:** *Karaļa Sirds* (#25) — universāls minjonu HP.
**Piedziņa:** Alkas Kunga klases boss; visiem cita klasei arī spēcīgs Zvērests-vs-Zvērests.
**Murgu variants:** T65+, Goldo pilnīgi apzeltīts pie 25% HP + drop chance uz Uber Mistiskais.

---

# 5. Puvušais Mežs (8 dungeoni)

## #27. **Puves Migla Kripta**
*Mini · Level 35-40 · Rūnas ligzdas: 2*

**Layout:** 5 telpas nomērķoju ar Puvušo Miglu vidū. **Mini-bosi:** 2. **Fināla boss:** Puvušā Miglas Māte (100K HP).
**Kodekss:** *Puves Migla Aspekts* (#57) — Mežonis.
**Piedziņa:** DoT-tēma; ideāls Mežonis DoT Spam.
**Murgu variants:** T50+, migla pastāvīgi tuvinās kartē (jāizbēg).

---

## #28. **Karogs Kripta**
*Mini · Level 38-44 · Rūnas ligzdas: 1*

**Layout:** 4 telpas ar puvušu koku karogiem. **Mini-bosi:** 2. **Fināla boss:** Puvušu Koku Karagalvenis (120K HP).
**Kodekss:** *Puves Karogs* (#58) — Mežonis + Reliktu-only alt.
**Piedziņa:** DoT stack cap upgrade.
**Murgu variants:** T50+, stack cap upgrade neaktivējas — spiediens uz raw DPS.

---

## #29. **Ērceles Kripta**
*Mini · Level 40-46 · Rūnas ligzdas: 1*

**Layout:** 4 telpas ar ērceles krūmiem, kas šauj šautriņus. **Mini-bosi:** 2. **Fināla boss:** Ērceles Karaliene (110K HP).
**Kodekss:** *Ērceles Ceļš* (#59) — Mežonis.
**Piedziņa:** platform-based avoidance dungeon.
**Murgu variants:** T50+, šautriņi caururbj bruņas.

---

## #30. **Vecā Kaudze**
*Elite · Level 42-48 · Rūnas ligzdas: 2*

**Layout:** 6 telpas ar līķu kaudēm. **Mini-bosi:** 3 (Ghouli). **Fināla boss:** Vecais Ghoulis Karalis (160K HP).
**Kodekss:** *Vecā Vaids* (#16) — universāls Overpower reduction.
**Piedziņa:** ghouli aug ar katru līķu kaudi (density density).
**Murgu variants:** T55+, katra ienaidnieka nāve dropo līķi, kas pēc 5s ceļas kā ghoulis.

---

## #31. **Zvēra Kripta**
*Klases (Mežonis) · Level 45-52 · Rūnas ligzdas: 2*

**Layout:** 5 telpas. **Mini-bosi:** 3. **Fināla boss:** *Zvēra Zvērests* (tavs bijušais Mežonis Zvērests, 200K HP, 3 fāzes, transformējas pats).
**Kodekss:** *Zvēra Ceļš* (#60) — Mežonis Beast Melee.
**Piedziņa:** Mežoņa klases stāsta arka; Zvēras Zīme Rūna drop.
**Murgu variants:** T60+, boss transformējas 4× (visos 4 zvēros).

---

## #32. **Sakņu Kripta**
*Elite · Level 48-55 · Rūnas ligzdas: 2*

**Layout:** 6 telpas ar sakņu labirintu. **Mini-bosi:** 3. **Fināla boss:** Sakņu Kunga (180K HP).
**Kodekss:** *Sakņu Karogs* (#63) — Mežonis Zone Control.
**Piedziņa:** roots mehānika visur — jāplan movement.
**Murgu variants:** T55+, roots ilgāki + boss transforms visu telpu par saknēm 15s intervālos.

---

## #33. **Aizmirsta Kripta**
*Mini · Level 50-56 · Rūnas ligzdas: 1*

**Layout:** 4 telpas — visas šķiet vienādas, jāsaproti kāpēc. **Mini-bosi:** 2. **Fināla boss:** Aizmirsts Vecais (140K HP).
**Kodekss:** *Aizmirsta Zeme* (#64) — Mežonis.
**Piedziņa:** puzzle dungeon — pareizā ceļa atrašana.
**Murgu variants:** T55+, katra pareiza izvēle uzliek DoT (patiesais ceļš ir grūts).

---

## #34. **Elfu Karalienes Vārti** — *Ninhera boss*
*Reģiona Boss · Level 58 · Rūnas ligzdas: 3*

**Layout:** Elfu drupu templis, 4 fāzes vietas. **Mini-bosi:** 3 (Elfu māsēs). **Fināla boss:** **Elfu Karaliene Ninhera** (500K HP, 3 fāzes, puse sieva puse koks).
**Kodekss:** *Puves Zvana* (Rūna, nevis Aspekts) + *Zvēras Ass* (#26) — universāls minjonu.
**Piedziņa:** Mežoņa klases boss; ideāls DoT build test.
**Murgu variants:** T65+, Ninhera "root form" cīņas fāze 4 + drop chance uz Uber.

---

# 6. Tuksneša Bāzars (7 dungeoni)

## #35. **Vēju Solis Kripta**
*Mini · Level 45-50 · Rūnas ligzdas: 1*

**Layout:** 4 telpas platform-hopping virs smilšu jūras. **Mini-bosi:** 2. **Fināla boss:** Smilšu Ceļotājs (150K HP).
**Kodekss:** *Ēnu Solis Ass* (#67) — Zaglis.
**Piedziņa:** movement-focused dungeon, teleport chains.
**Murgu variants:** T55+, smiltis "sabrūk" — jāstāv kustībā.

---

## #36. **Neredzamā Kripta**
*Elite · Level 50-58 · Rūnas ligzdas: 2*

**Layout:** 6 telpas, gaisma ir low. **Mini-bosi:** 3. **Fināla boss:** Neredzams Vezirs (200K HP + pastāvīga invis).
**Kodekss:** *Neredzamā Krunkas* (#69) — Zaglis Permanent Invis.
**Piedziņa:** invisibility mehānika visur — tu redzi tikai signāles.
**Murgu variants:** T60+, boss teleports every 5s + drops decoys.

---

## #37. **Ēna Sirds Kripta**
*Klases (Zaglis) · Level 55-62 · Rūnas ligzdas: 2*

**Layout:** 5 telpas. **Mini-bosi:** 3. **Fināla boss:** *Ēnu Ēna* (tavs bijušais Zaglis Zvērests, 250K HP, 3 fāzes, klonējas).
**Kodekss:** *Ēnas Sirds Karogs* (#70) — Zaglis.
**Piedziņa:** Zagļa klases stāsta arka; Naktsāda Zīme Rūna drop.
**Murgu variants:** T65+, boss klonējas 5× vienlaikus (jātiek gaŗam ar pareizu real target).

---

## #38. **Zaļču Karogs Kripta**
*Mini · Level 55-60 · Rūnas ligzdas: 1*

**Layout:** 4 telpas ar Zaļču koloniju. **Mini-bosi:** 2. **Fināla boss:** Zaļču Māte (170K HP + auto-sauc adds).
**Kodekss:** *Zaļču Karogs* (#71) — Zaglis Zaļču Armija.
**Piedziņa:** minjonu bosu density, Zaļču sinerģija test.
**Murgu variants:** T55+, Zaļču Māte spawn 10 Zaļču katras 15s.

---

## #39. **Sinerģija Kripta**
*Mini · Level 58-64 · Rūnas ligzdas: 1*

**Layout:** 5 telpas, katrā jāsinerģē ar sabiedroto. **Mini-bosi:** 3. **Fināla boss:** Sinerģija Māstris (180K HP).
**Kodekss:** *Ēnas Sinerģija* (#72) — Zaglis.
**Piedziņa:** koop-tēma; solo grūtāks, bet grupā vienkāršs.
**Murgu variants:** T60+, boss dubultā HP, ja solo.

---

## #40. **Bezēnu Karaļvalsts**
*Story · Level 60-68 · Rūnas ligzdas: 2*

**Layout:** 7 telpas — Bezēnu Karaļvalsts galvenā pils. **Mini-bosi:** 4. **Fināla boss:** Bezēnu Karalis (220K HP, 3 fāzes).
**Kodekss:** *Melnais Solis* (#23) — universāls stealth MS.
**Piedziņa:** kampaņas story dungeon, lore reveal par Zaglis Zvērests.
**Murgu variants:** T60+, boss pastāvīgi invisible pēc fāze 1.

---

## #41. **Vezirs Trono** — *Vezirs boss*
*Reģiona Boss · Level 68 · Rūnas ligzdas: 3*

**Layout:** Mūžsens karaliskais slepkavas mājas trono zāle. **Mini-bosi:** 3 (Vezirs sekotāji). **Fināla boss:** **Melnās Kūkas Vezirs** (600K HP, 3 fāzes, 12 dunči, 3 ēnu-dvīņu klonis).
**Kodekss:** *Zibens Solis* (Rūna) + *Ēnu Zīme* (#9) universāls Shadow.
**Piedziņa:** Zagļa klases boss; grūts fāzēs.
**Murgu variants:** T70+, 4. fāze ar visu 3 klonu vienlaikus.

---

# 7. Ledus Kroņi (7 dungeoni)

## #42. **Ledus Katedrāle**
*Mini · Level 55-62 · Rūnas ligzdas: 2*

**Layout:** 5 telpas gotiska ledus templī. **Mini-bosi:** 2. **Fināla boss:** Ledus Priestere (180K HP).
**Kodekss:** *Sasala Zvana* (#6) — universāls Vulnerable.
**Piedziņa:** Vulnerable-heavy cīņa, Sasala Zīme Rūna drops.
**Murgu variants:** T55+, priesteres vairāk Vulnerable uzliek.

---

## #43. **Sasalis Kripta**
*Elite · Level 58-65 · Rūnas ligzdas: 2*

**Layout:** 6 telpas, sasala aug ar cīņas ilgumu. **Mini-bosi:** 3. **Fināla boss:** Sasala Māstris (220K HP).
**Kodekss:** *Sasalis Karogs* (#73) — Sniegurgs Freeze Nuke.
**Piedziņa:** freeze-tēma; boss pat tu sasalu.
**Murgu variants:** T60+, boss iesalda tevi 5s intervālos.

---

## #44. **Sirds Kripta**
*Mini · Level 60-66 · Rūnas ligzdas: 1*

**Layout:** 4 telpas ar sirds tēmām. **Mini-bosi:** 2. **Fināla boss:** Sirds Vārti (190K HP + HP-buff sekotāji).
**Kodekss:** *Sirds Solis* (#10) — universāls Max HP.
**Piedziņa:** HP-fokusa cīņa.
**Murgu variants:** T55+, sekotāji dubultā HP.

---

## #45. **Sasala Trono**
*Mini · Level 62-68 · Rūnas ligzdas: 1*

**Layout:** 5 telpas, ledus tornis. **Mini-bosi:** 2. **Fināla boss:** Sasala Karagalvenis (210K HP).
**Kodekss:** *Ledus Sirds* (#13) — universāls Elemental Resistance.
**Piedziņa:** ledus torņa arhitektūras estētika.
**Murgu variants:** T55+, elemental attacks +50%.

---

## #46. **Kroņa Sirds Kripta**
*Klases (Sniegurgs) · Level 65-72 · Rūnas ligzdas: 2*

**Layout:** 5 telpas. **Mini-bosi:** 3. **Fināla boss:** *Ledus Vientulība* (tavs bijušais Sniegurgs Zvērests, 300K HP, 3 fāzes).
**Kodekss:** *Kroņa Sirds* (#75) — Sniegurgs.
**Piedziņa:** Sniegurga klases stāsta arka; Torņa Zīme Rūna drop.
**Murgu variants:** T65+, boss pastāvīgi Torņa Redzējuma stāvoklī.

---

## #47. **Skeletu Sasala Kripta**
*Mini · Level 68-74 · Rūnas ligzdas: 1*

**Layout:** 5 telpas ar skeletu armiju. **Mini-bosi:** 3 (elite skeleti). **Fināla boss:** Skeletu Karalis (240K HP + sauc adds).
**Kodekss:** *Sasala Karogs (Sk)* (#77) — Sniegurgs Skeletu.
**Piedziņa:** minjonu-armiju density; Skeleta Vaids Rūna drops.
**Murgu variants:** T65+, skeleti +50% dmg.

---

## #48. **Ievas Turaņa Trono** — *Ievas Turaņa boss*
*Reģiona Boss · Level 78 · Rūnas ligzdas: 3*

**Layout:** Kalnu virsotne, peldoši ledus salu fragmenti. **Mini-bosi:** 3 (Ievas Turaņa māsēs). **Fināla boss:** **Ievas Turaņa** (700K HP, 3 fāzes, savs veidols laikā, kad viņa bija vēl dzīva burve).
**Kodekss:** *Sasala Zīme* (Rūna) + *Vientuļā Sirds* (#79) — Sniegurgs.
**Piedziņa:** Sniegurga klases boss; ideāls Torņa Redzējums test.
**Murgu variants:** T80+, 4. fāze ar Turaņa "pusskelet pusledus" transformāciju + drop chance uz Uber.

---

# 8. Melnais Kalns (2 dungeoni)

## #49. **Melnā Kalna Kalts**
*Endgame Utility · Level 75+ · Rūnas ligzdas: 3*

**Layout:** 8 telpas, deepest crafting halls. **Mini-bosi:** 4 (Kalēju Vecajs). **Fināla boss:** Melnais Kalējs (400K HP).
**Kodekss:** *Bezdibeņa Zvana* (#24) — universāls kaltēšanas ātrums.
**Piedziņa:** endgame utility — pabeidzot dropo **Zvaigznes Skabarga** garantēts un unlockē Ancient Weapon crafting (Season 3+).
**Murgu variants:** T80+, drops Zvaigznes Skabarga ar 25% šansei per run.

---

## #50. **Nāves Kripta**
*Endgame · Level 78+ · Rūnas ligzdas: 3*

**Layout:** 10 telpas, dziļākais dungeon pirms katakombas. **Mini-bosi:** 5. **Fināla boss:** **Nāves Vēstnesis** (500K HP, 3 fāzes).
**Kodekss:** *Kritušā Solis* (#20) — universāls nāves atgriezšana.
**Piedziņa:** endgame chase dungeon, guarantee Uber Mistiskais chance per run.
**Murgu variants:** T90+, 4. fāze ar Nāves Vēstnesis "true form" — vienas no grūtākajām cīņām spēles.

---

# 9. Kopsavilkums

## 9.1. Sadalījums pa tipiem

| Tips           | Skaits |
|----------------|--------|
| Story          | 6      |
| Mini           | 27     |
| Elite          | 7      |
| Klases         | 6 (1 per klase) |
| Reģiona Boss   | 6      |
| Endgame        | 2      |
| **Kopā**       | **54** ← neliela pārklāšanās (klases un boss dungeoni skaitās vairākos kategorijās) |

## 9.2. Kodekss unlock summary

- **Aspekti Kodeksā:** 43 (skat. individuālie ieraksti)
- **Aspekti no bosiem:** 18
- **Reliktu-only Aspekti:** 19 (nekad neatslēdzas Kodeksā)

Kopā: **80 Aspekti** (kā Aspects.md).

## 9.3. Vidējais dungeon clear laiks

| Tips      | Level      | Solo clear laiks |
|-----------|------------|------------------|
| Mini      | early      | 8-15 min         |
| Story     | mid        | 20-35 min        |
| Elite     | mid-late   | 25-45 min        |
| Klases    | any        | 30-60 min (savs cīņas)|
| Reģiona Boss | endgame  | 45-90 min        |
| Endgame   | endgame    | 60-120 min       |

## 9.4. Ieteikums progresai

1. **Level 1-25:** izlaid Debesu Virsotnes 9 dungeonus + reģiona boss.
2. **Level 25-40:** izlaid Asins Arēnas 8.
3. **Level 40-50:** izlaid Zelta Raktuvju 9.
4. **Level 50-60:** izlaid Puvušā Meža 8.
5. **Level 60-70:** izlaid Tuksneša Bāzara 7.
6. **Level 70-80:** izlaid Ledus Kroņu 7.
7. **Level 80+:** endgame Melnais Kalns 2 + Murgu grinds.

**Full campaign clear:** ~40-60 stundas casual, ~25-35h speedrun.
**All Kodekss unlocks:** ~80-120 stundas.

---

*Skaitļi un layouts ir sākuma dizaina piedāvājums. Tuning notiek pēc playtesting.*
