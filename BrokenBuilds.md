# Broken Builds — Triljonu Dizaina Filozofija

*"Broken builds ir endgame. Ja spēlētājs saprot sistēmu tik dziļi, ka atrod trillion-damage kombu — tas ir sasniegums, ne buga."*

Šis dokuments detalizē, **kā** mūsu spēle ļauj sasniegt astronomiskos damage skaitļus, kad spēlētājs perfekti sakārto multiplikatīvos avotus. Nav "cap" — ir tikai matemātika.

---

## 1. Filozofija — kāpēc mēs to atļaujam

Diablo 4, Path of Exile, Last Epoch — visos šajos spēlēs ir "broken buildi", kur damage sasniedz miljardus un triljonus. Tas ir **veselīgi** spēlei, jo:

1. **Endgame vajag horizontu** — kad tu jau maxus visu obvious, kur ir nākošais mērķis? Trillion hit ir tas horizons.
2. **Dziļa sistēma ļauj radošumu** — spēlētāji, kas gadu pēta mehānikas, pelnīti atrod "broken" kombus.
3. **Community content** — trillion damage clips iet uz TikTok/YouTube, spēle iegūst mārketingu bez tavas iesaistes.
4. **Uber Bosi ir pretspēks** — Uber Bosiem HP miljardi vai triljoni, tāpēc "broken build" tikai izdara balansu.

**Mūsu politika:** ne mēs banojam skaitļus, ne cap. Mēs tikai nodrošinām, ka:
- **Kļūt "broken" prasa reālu darbu** (10-14 multiplikatīvu avotu perfekti izlīdzināšana).
- **"Broken" nav vienlaicīgs vienam mob** — tikai bosos ar specifiskiem stāvokļiem.
- **Baseline spēle balancēta uz normālu jaudu** — broken build nav vajadzīgs, lai pabeigtu kampaņu vai T80 Murgu.

---

## 2. Skaitļu pieaugums — Additive vs Multiplicative

**Additive damage** ir "trap" — pievieno visus +X% damage vienā bucketā, tad reizini vienreiz. Rezultāts: diminishing returns.

```
100% + 100% + 100% + 100% = +400% additive
Multiplier: × 5
```

**Multiplicative** ir kur reālā jauda:

```
1.5 × 1.5 × 1.5 × 1.5 = × 5.06 (līdzīgi kā 4 additive)
1.5 × 1.5 × 1.5 × 1.5 × 1.5 × 1.5 × 1.5 × 1.5 = × 25.6 (kāpjot eksponenciāli)
```

**Ar 12-14 stacked multipliers 1.5-2.0×, damage aug × 100 000 līdz × 10 000 000.**

Tas ir vienīgais ceļš uz triljonu.

---

## 3. Visi multiplikatīvie avoti spēlē

Šeit ir **pilns saraksts** ar visiem separate multipliers, ko spēlētājs var stack. Katrs ir savs vals — nekļūt sasummēts.

### 3.1. Kritiskais Damage Reizinātājs (GA affix)
Uz amuleta un ierociem. **Max 4 GA amulets ar Crit Dmg Reizinātājs = ×2.5+ mult** vienam item.

### 3.2. Physical / Elemental Damage Reizinātājs (GA)
Uz ieroci un amuletas. **~×1.5 katram**.

### 3.3. Vulnerable Damage Reizinātājs (GA)
Uz amuletas un ieroci. **~×1.5**.

### 3.4. DoT Damage Reizinātājs (GA)
Uz amuletas. **~×1.5**.

### 3.5. Aspekti (Reliktu items un Kodekss)
Katrs Reliktu items ir 1 Aspekts. Katrs kritiskā slotā **imprintētais Aspekts** ir savs multiplier:
- Ofensīvie Aspekti: ×1.3 līdz ×2.0
- Klases-specifiski build-defining Aspekti: ×2.0 līdz ×3.0
- Katrs slot var būt Aspekts (**līdz 9 Aspektiem uzreiz**)

### 3.6. Setu 6-piece bonuss
Katras klases setu 6-piece ir **build-defining ×2.0 līdz ×3.0 multiplier**.

### 3.7. Klases Keystone (level 60)
Bieži ir efektīvs multiplier (piem., Bruņinieka *Kritušā Krusta* Falangas pastāvīgs = viņu bonuss + tavas prasmes vienlaicīgi).

### 3.8. Uber Mistiskie items
Katrs Uber ir savs multiplier vai maiņa. Melnais Kronis dod +2 līmeņus visām prasmēm = **~×1.2 pastāvīgs multiplier**.

### 3.9. Uber Mistiskie Gems (Ēnu slot)
Nolādētais Asinsakmens: **×2 dubultkritim**. Melnā Zvaigzne: one-shot mechanic.

### 3.10. Paragon Leģendārie Mezgli
Katrs Paragon Dēļa leģendārais mezgls ir spēles-maiņas multiplier. **5 pilni dēļi = 5 leģendāri mezgli**, katrs ×1.3 līdz ×2.0.

### 3.11. Rūnu procs (Runes)
Level 15+ Rūnas dod significant multiplicative bonusus, ja stat sliekšņi izpildīti. Piem., *Nolādētais Sirdsdedzis* Lv 15 = **rādiuss bonusi ×2**.

### 3.12. Combat Windows (aktīvās prasmes)
- **Ult windows** (Melnais Zvans, Asins Trakums, Sausā Sirds, Vilka Āda, Naktsāda, Torņa Redzējums): katrs dod ×1.5 līdz ×3 damage 5-20s logā.
- Rīki, kā *"ja Rage virs 80"* vai *"pēc backstab"* — situatīvie multipliers, kas var būt izlīdzināti.

### 3.13. Vulnerable + Overpower kombinētais
- Vulnerable pieaug damage kā multiplier (bāzes +40% ar +25% Reizinātājs = ×1.75).
- **Overpower** ir liels — proc chance, base damage swap uz (Fortify + HP).

### 3.14. Debuffs uz mērķa
- **Freeze**, **Slow**, **Root**, **Stun** — katrs var dot ×1.2 līdz ×1.5 dmg no dažādiem aspektiem/pasīvām.

### 3.15. Ienaidnieku slieksni
- *"vs. elite"* — ×1.3
- *"vs. injured"* (zem 35% HP) — ×1.3
- *"vs. isolated"* — ×1.3

---

## 4. Overpower — atslēgas mehānika triljoniem

**Overpower** ir tas, kas D4 padara HotA Barb tik smieklīgu. Mūsu spēlē:

**Overpower ir ~3% chance uz katru hit** (bāzē). Kad triger:
- Bāzes damage swap: **jaunais base = tavs current HP + Fortify + Barrier**.
- Tas nozīmē, ka ar 100K HP + 50K Barrier tu iegūsti **150 000 bāzes damage** vietā 2 400.
- Tad **visi pārējie multipliers** iet virsū.

**Fortify** ir stackable buff, kas aug ar saņemtu damage — Bruņinieks (Falangas Tank) var uzkrāt **500K+ Fortify** ilgā cīņā.

Ja Bruņinieks Overpower ar 500K Fortify + 100K HP + 50K Barrier = **650 000 bāzes damage** vietā 2 400 = **×271 multiplier**.

Pēc tam visi pārējie multipliers → **trillion range**.

---

## 5. Combat Windows — kad viss ir izlīdzināts

Broken build nav pastāvīgs — tas dzīvo **combat windows**, kad daudz mehāniku vienlaicīgi aktīvas:

**Perfekts combat window (~2-5 sekundes):**
- Boss ir **Vulnerable** (uzlikts ar Zvana Metiens vai citu)
- Boss ir **Frozen/Stunned** (uzlikts ar Sasalis Kaps vai Melnais Zvans)
- Tavs **Ult ir aktīvs** (Overpower window vai buff)
- Setu proc ir gatavs
- Aspekta proc ir gatavs
- Uber Gem proc ir gatavs

Šie logi ir **reti** un **grūti** — bet kad tie aligned, damage ir triljonos.

---

## 6. Broken Build #1 — Bruņinieks "Grēksūdzes Bomba"

**Koncepts:** Ilga defensīva cīņa uzkrāj Fortify + Grēksūdze; kad tu tērē 100pt Melnais Zvans ar Overpower proc, damage ir absurds.

**Setup:**
- Bruņinieks, Heal-Tank keystone *Asins Katedrāle* (Grēksūdze 2×).
- Setu *Asins Katedrāles Kārkas* pilnās 6/6 daļas.
- Uber Mistiskais: **Sirds no Kalna** (nāves-atgriezšana) + **Melnais Kronis** (+2 skills).
- Uber Gem: **Nolādētais Asinsakmens** (dubultkritis).
- Aspekti: 5 imprint (Grēksūdzes Piespriedu, Nemirstīgā Zvana, Melnais Krusts, Zvana Uguns, Katedrāles Vēls).
- Paragon 350 — leg mezgls *"Krustnesis"* aktīvs.

**Combat window setup (~90s uzkrāj):**
1. Ienāc bosa cīņā, ļauj sevi 60s sist.
2. Fortify uzkrāj līdz **~600K**.
3. Grēksūdze pastāvīgi 100/100.
4. Barrier no Grēksūdzes Zvana = **50K**.
5. Boss Vulnerable ar Zvana Metiens.
6. Boss Stunned ar iepriekšēju Melnais Zvans (kombo tag).

**Trigger:**
- Overpower proc uz Melnais Zvans.

**Aprēķins:**

Base swap = Fortify + HP + Barrier = 600K + 100K + 50K = **750 000** damage.
Skill % (Melnais Zvans) = 400% = **3 000 000**.

Multipliers stacked:
- Str Attribute mult ×2.19
- Additive bucket (Physical 90% + Elite 30% + Vulnerable 40%) = ×2.60
- Crit ×2.5 × 1.5 (GA) = ×3.75
- Vulnerable Reizinātājs ×1.25
- Physical Reizinātājs ×1.30
- Aspect Grēksūdzes Piespriedu ×1.50
- Aspect Nemirstīgā Zvana ×1.80
- Aspect Melnais Krusts ×1.40
- Setu 6-piece transformācija ×2.50
- Melnais Kronis skill +2 ×1.20
- Nolādētais Asinsakmens dubultkritis ×2.00
- Paragon Krustnesis proc ×2.00
- Rūna *Ģerbonis* Lv 15 ×1.60

Total multiplier ceiling:
`2.19 × 2.60 × 3.75 × 1.25 × 1.30 × 1.50 × 1.80 × 1.40 × 2.50 × 1.20 × 2.00 × 2.00 × 1.60`

Compute pa soļiem:
```
2.19 × 2.60  = 5.69
× 3.75       = 21.36
× 1.25       = 26.70
× 1.30       = 34.71
× 1.50       = 52.07
× 1.80       = 93.72
× 1.40       = 131.21
× 2.50       = 328.02
× 1.20       = 393.62
× 2.00       = 787.25
× 2.00       = 1 574.50
× 1.60       = 2 519.20
```

Total multiplier = **×2 519**.

**Final damage:**
```
3 000 000 × 2 519 = 7 557 000 000
                  ≈ 7.56 miljardi damage.
```

**Ne triljons vēl.** Bet Melnais Zvans ir **AoE, 5 mērķi vienlaicīgi** — 5 × 7.56B = **37.8 miljardi** kopējais burst.

**Ar perfektu uzkrāšanu — 1M Fortify (ekstrēmās cīņās):**
Base swap = 1M + 100K + 50K = 1.15M
Skill = 1.15M × 4 = 4.6M
Ar × 2 519 = **11.6 miljardi single hit**, 58 miljardi AoE.

**Trilling range** sasniedz ar papildu:
- Boss ir **Nekāds** — pievieno situatīvos multipliers (vs isolated ×1.3, vs elite ×1.3) → ×1.7
- **6-piece proc window** dubulto = ×2

`58 miljardi × 1.7 × 2 = 197 miljardi burst.`

Ja Bruņinieks izmanto **Melnais Zvans divreiz pēc kārtas** (100 pt → izlādē → uzkrāj 100pt 1 sekundē no keystone + hit → izlādē vēl) = **~400 miljardi kombu**.

Uz Uber Bosa **200-500 miljardu HP** = **kill viena 5-sekunžu windowā.**

---

## 7. Broken Build #2 — Bezēnu Zaglis "Ēnu Kaskade"

**Koncepts:** Klons + spēlētājs backstab vienlaicīgi, katrs sits ar pilnu stack; kad proc chain triggers, katrs hit no cita hit.

**Setup:**
- Bezēnu Zaglis, keystone *Divi Ēnu Klonis*.
- Setu *Divu Ēnu Solis* pilnās 6/6 daļas.
- Uber: **Melnais Kronis** + **Sirds no Kalna**.
- Uber Gem: **Nolādētais Asinsakmens** (dubultkritis) + **Melnā Zvaigzne** (30s cd one-shot ne-boss).
- 5 Aspekti: Divu Soļu Karogs, Klona Ceļš, Ēnu Solis Ass, Ass Duncis, Melnā Ass.
- Paragon leg: *"Divi Ēnu Klonis"* enhanced.

**Combat window (~4 sekundes):**
1. Boss ir Vulnerable no iepriekšēja Kaklaslazds.
2. Zaļču Signāls aktīvs → visi Zaļči neredzami + crit boost.
3. Naktsāda aktīvs → tu neredzams, garantēts backstab crit.
4. Kaklaslazds tikko izlietots → nākošais uzbrukums dubultkritisks.
5. Ēnu Solis → 2 kloni uz mērķi (setu 6-piece).

**Trigger:** viens LMB tag.

**Aprēķins:**

Bāzes: **1 sitiens = tu + 2 kloni + Zaļči × 8 = 11 vienlaicīgi hits.**

Katra hit aprēķins:

Base = 2400 × 0.90 (LMB skill%) = 2 160.
Backstab bonus (2×) = 4 320.
Attribute (Veiklība 1400) mult = ×2.40.
Additive bucket = 1 + 1.10 (physical) + 0.30 (elite) + 0.35 (vs. injured) = ×2.75.
Crit mult (garantēts crit stealth + Ass Duncis backstab crit) = 2.5 × 1.5 (GA) = ×3.75.
Vulnerable mult = ×1.75.
Physical Reizinātājs = ×1.35.
Aspect Ass Duncis backstab bonus = ×1.50.
Aspect Klona Ceļš dubultkritis kloniem = ×2.00 (kloniem).
Aspect Melnā Ass (crits pret Vulnerable = ×3) = ×3.00.
Setu 6-piece (3 hits vietā 1) = ×3.00 (jo 3 klones × baseline 1).
Nolādētais Asinsakmens dubultkritis = ×2.00.
Melnā Zvaigzne one-shot = ×5.00 (ne-boss) vai ×1 (boss).
Melnais Kronis +2 skills = ×1.20 skill effektīvi.
Kaklaslazds dubultkritisks = ×2.00.
Paragon leg = ×2.00.
Rūna *Zvana Skaņa* Lv 15 = ×1.5.

Total multiplier (bez Melnā Zvaigzne, jo boss):
```
2.40 × 2.75 × 3.75 × 1.75 × 1.35 × 1.50 × 2.00 × 3.00 × 3.00 × 2.00 × 1.20 × 2.00 × 2.00 × 1.50

= 2.40 × 2.75    = 6.60
× 3.75          = 24.75
× 1.75          = 43.31
× 1.35          = 58.47
× 1.50          = 87.71
× 2.00          = 175.41
× 3.00          = 526.24
× 3.00          = 1 578.71
× 2.00          = 3 157.43
× 1.20          = 3 788.91
× 2.00          = 7 577.82
× 2.00          = 15 155.65
× 1.50          = 22 733.47
```

Total mult = **× 22 733**.

Damage per hit = 4 320 × 22 733 = **98.2 miljoni per single hit**.

**Now the kaskade:**
- Tu = 98.2M.
- Klons 1 = 98.2M × (klonu specifiskās deb) = ~50M.
- Klons 2 = ~50M.
- 8 Zaļči, katrs ar backstab spread = 8 × 30M = 240M.

Vienlaicīgs burst = 98 + 50 + 50 + 240 = **~438 miljoni**.

Bet **setu 6-piece atļauj Ēnu Solis atstāj 2 klonus** un **abi Zaļči kloni un ne-kloni triggē Ēnu Solis reset** (Vēju Solis aspekts) = **10-hit chain 4 sekundēs**.

Kopējais 4-sekunžu burst = **10 × 438M = ~4.4 miljardi**.

Ar Naktsāda 5s window un cikliska Ēnu Solis reset = **kaskade 15-20 hits = ~7-9 miljardi 5 sekundēs**.

**Nolādētais Asinsakmens + Kaklaslazds double stacks vienā perfektā secībā** = vēl **×3-4 multiplier** uz visu ķēdi = **~28 miljardi 5 sekundēs**.

Ne triljons, bet **~28 miljardi/5 sek = 5.6 miljardi DPS burst**. Pastāvīgi to nespēj, bet pret Uber Bosu ar 500 miljardu HP = **kill 90 sekundēs** ar pareizu triggeriem.

**Triljona sasniegšana** — Bezēnu Zaglis vajadzīgs vēl viens layer:
- **Naktsāda ilgums 15s** (pēc pilnas apstāšanās, endgame Rūnas + Paragon).
- **Ēnu Solis 0 cooldown** stealth laikā.
- Katrs no 30 hits ir crits pret Vulnerable ar visiem multipliers.
- **30 × 98M = 2.94 miljardi vienā secībā**.
- Plus klones un Zaļči → **~15 miljardi vienā combo**.

Triljon nesniedz vientuļš Bezēnu Zaglis, bet **grupā ar buff share** (Bruņinieks Falangas Signāls dot minjoniem bonuss, Sniegurgs freeze boss, u.c.) = daudzu spēlētāju kopīgs burst = triljon range.

---

## 8. Broken Build #3 — Vientuļais Sniegurgs "Freeze Chain Detonation"

**Koncepts:** Katrs saldētais ienaidnieks mirstot detonē un iesalda 3 tuvākos, kas mirstot detonē 3 tuvākos... eksponenciāla chain reakcija.

**Setup:**
- Vientuļais Sniegurgs, keystone *Sasala Kronis Ass*.
- Setu *Ledus Kroņa Nāve* pilnās 6/6 daļas.
- Uber: **Melnais Kronis**.
- Uber Gem: **Nolādētais Asinsakmens**.
- Aspekti: Sasalis Karogs, Ledus Šauta Ceļš, Kroņa Sirds, plus imprintētie 2 aspects par eksploziju un chain.
- Paragon leg: *"Sasala Kronis"* upgrade.
- **Kritiskais aspekts:** *Sasala Detonācija* — sasaldētie ienaidnieki, mirstot, izraisīt sfērisku eksploziju, kas saldē un ietekmē tuvākos 15m rādiusā.

**Combat window (~10 sekundes):**
1. Aktivē Torņa Redzējums (ne obligāti, bet dod +2× dmg tavām prasmēm).
2. Sasalis Kaps → sasaldē 20 mob grupā.
3. Ledus Lauskas kanalē cauri visai grupai.
4. Pirmais mob mirst (mums pieņem 10M hp) → detonē → salda 3 nākošos → tie mirstot detonē 3 nākošos...

**Chain reakcijas eksponenciāls aprēķins:**

Katras "generation" damage no detonācijas = **50% no iepriekšējā** (setu 6-piece boost).

Bet **katrs mob dodam pat sasalt un mirst** dod tev **+5 Sala** (keystone bonus), kas kāpina Torņa Redzējums ilgumu.

- Generation 1: 20 mob, katrs sit 500M dmg (chain reakcija).
- Generation 2: 60 mob (20 × 3), katrs 400M dmg → 24 miljardi.
- Generation 3: 180 mob, katrs 250M → 45 miljardi.
- Generation 4: 540 mob, katrs 100M → 54 miljardi.
- Generation 5: 1620 mob, katrs 30M → 48.6 miljardi.

**Kopējais chain kumulatīvs damage: ~170 miljardi 10 sekundēs.**

Ja Uber Bosa cīņā ir **adds** un boss stāv Sasala Kroņā, chain trigg ties uz visu grupu ieskaitot bosu.

**Bosa damage no chain = ~30% kumulatīvs (jo boss non-elite chain resistance) = ~50 miljardi.**

Ja boss HP ir 500M — **kill vienā chain cikls (~5 sekundēs)**.

Uber Boss ar 100 miljardu HP — **200 sekundes** ar cikliskiem chains.

**Triljons trilogos:** ja Sniegurgs spēj triger 5 chains vienā cīņā (60s) = **~850 miljardi damage** vienam bosam vienā ķēdē.

---

## 9. Kopsavilkums — trillion-range builds

| Build                                          | Peak Burst  | Sustained DPS | Cīņas ilgums | Prasa |
|------------------------------------------------|-------------|---------------|--------------|-------|
| Bruņinieks Grēksūdzes Bomba                    | 400 miljardi | 8 miljardi    | 60s uzkrāj + 5s | Fortify farm |
| Bezēnu Zaglis Ēnu Kaskade                      | 30 miljardi | 5 miljardi    | 5s combo      | Perfekts Naktsāda + Kaklaslazds sync |
| Vientuļais Sniegurgs Chain Detonation          | 170 miljardi | 15 miljardi   | 10s          | Mob density + Sasalis Kaps |
| Alkas Kungs "Zelta Kaskade" (5000000 zelta uzkrāj) | 100 miljardi | 3 miljardi | Rezerves uzkrāj | 5M zelta bilance |
| Berserker "Rage Overflow" (Asins Trakums 100%) | 500 miljardi | 20 miljardi   | 5s           | -50% HP window |
| Mežonis "Puve Kaskade" (DoT spread × 100 mob)  | 250 miljardi | 8 miljardi    | 20s          | Mob density + Vulnerable |

**Visi šie ir specifiskas kombu — ne pastāvīgi.** Sasniegt tos prasa:
- Pilnas 6/6 setu daļas.
- 3-4 GA visiem key items.
- 2 Uber Mistiskie items.
- 2 Uber Mistiskie Gems.
- Paragon 350.
- Visi Rūnas Lv 15+.
- Aspektu Kodekss perfekti imprintēts.
- Perfekta rotācija (10-14 mehāniku sync).

**Diablo 4 speak: šie ir "GT clear T100 Nightmare" builds.**

---

## 10. Kur mēs velkam līniju — buga vs feature

**Feature (atļauts):**
- Overpower ar Fortify swap ir pareizs mehānika, dokumentēts.
- Chain reakcijas ir dizainētas (setu bonuss to atbalsta).
- Multiplicative stacking ir sistēmas dizains.
- Uber Bosi ar 200-500 miljardu HP ir mūsu counter-design.

**Buga (jālabo):**
- Multiplicative loop, kur A trigge B, B trigge A, cikls bez limita.
- Rotācija, kur cooldown iet uz nulli mūžīgi.
- Damage swap uz negative vērtībām (integer overflow).
- Damage, kas iznīcina Ubers pirms viņu mehānikas trigge (ja Uber pat neapstājas fāzē 2, tas ir bad UX).

**Balansa mērķis:** Peak burst ~400 miljardi, sustained ~10-20 miljardi endgame. Uber Bosa cīņa vidēji **~1 min** perfektam build, **~30 min** parastam build.

---

## 11. Kā mācāmies — spēlētājs kalpo par testeru

Katras sezonas launcham gaidām, ka spēlētāji atrod **jaunus broken kombus**. Mūsu politika:

1. **Ja combo prasa reālu darbu (10+ mehāniku sync)** — atļaujam, dzeltenā logā "Wow" mērķim.
2. **Ja combo ir buga (loop overflow, no-cd exploit)** — nekavējoties fix, izsakām paldies atradējiem.
3. **Ja combo ir vienkāršs (2-3 mehāniku sync bez efforts)** — nerf tuvākajā sezonā, bet ne mid-season.

**Mūsu community būs zinošāki par mums.** Trillion damage clip ir marketing, ne draudi.

---

## 12. Sistēmiskais dizains, kas atbalsta broken buildus

Katrā jaunā sezonā mēs pievienojam **1-2 jaunus multiplikatīvos avotus** — jaunu Aspekta veidu, jaunu Rūnu, jaunu Setu daļu, jaunu Uber. Katrs jauns multiplier **pieauguma platforma** — spēlētāji atkal meklē jaunus broken kombus, spēle paliek dzīva.

**Katras jaunas sezonas pēc launcham:**
- Nedēļa 1: normālie playeri iet cauri kampaņai.
- Nedēļa 2-4: min-maxers atrod broken kombus, TikTok viral clips.
- Nedēļa 5-8: mid-tier playeri kopē broken kombus.
- Nedēļa 9-12: mēs sāku dizainēt nākošos multipliers.

**Broken build ekonomika ir sezonas dzinējs.**

---

*Šis dokuments ir dzīvs. Katras jauns broken kombs, ko atrod community, tiek pievienots kā piemērs. Mēs turam listā top 10 all-time broken builds — un maksājam bounty pirmajam, kas atrod trilliona-hit clip.*
