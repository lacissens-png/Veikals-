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
Bieži ir efektīvs multiplier (piem., Vecā Burvja *Realitātes Karaļvalsts* Reality Rift pastāvīgs = triple dmg buff pastāvīgi).

### 3.8. Uber Mistiskie items
Katrs Uber ir savs multiplier vai maiņa. Melnais Kronis dod +2 līmeņus visām prasmēm = **~×1.2 pastāvīgs multiplier**.

### 3.9. Uber Mistiskie Gems (Ēnu slot)
Nolādētais Asinsakmens: **×2 dubultkritim**. Melnā Zvaigzne: one-shot mechanic.

### 3.10. Paragon Leģendārie Mezgli
Katrs Paragon Dēļa leģendārais mezgls ir spēles-maiņas multiplier. **5 pilni dēļi = 5 leģendāri mezgli**, katrs ×1.3 līdz ×2.0.

### 3.11. Rūnu procs (Runes)
Level 15+ Rūnas dod significant multiplicative bonusus, ja stat sliekšņi izpildīti. Piem., *Nolādētais Sirdsdedzis* Lv 15 = **rādiuss bonusi ×2**.

### 3.12. Combat Windows (aktīvās prasmes)
- **Ult windows** (Kunga Zvans, Impērija Vaidēs, Reality Rift): katrs dod ×1.5 līdz ×3 damage 8-25s logā.
- Rīki, kā *"ja Dominance 100"*, *"peldošā formā"* vai *"Reality Rift laikā"* — situatīvie multipliers, kas var būt izlīdzināti.

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

**Fortify** ir stackable buff, kas aug ar saņemtu damage — Pirmais Kungs (Sardzes Zvērests) var uzkrāt **500K+ Fortify** ilgā cīņā, jo minjoni dalīti tai saņem daļu bojājumus.

Ja Pirmais Kungs Overpower ar 500K Fortify + 100K HP + 50K Barrier = **650 000 bāzes damage** vietā 2 400 = **×271 multiplier**.

Pēc tam visi pārējie multipliers → **trillion range**.

---

## 5. Combat Windows — kad viss ir izlīdzināts

Broken build nav pastāvīgs — tas dzīvo **combat windows**, kad daudz mehāniku vienlaicīgi aktīvas:

**Perfekts combat window (~2-5 sekundes):**
- Boss ir **Vulnerable** (uzlikts ar Zvana Metiens vai citu)
- Boss ir **Frozen/Stunned** (uzlikts ar Time Wave vai Kunga Zvans)
- Tavs **Ult ir aktīvs** (Overpower window vai buff)
- Setu proc ir gatavs
- Aspekta proc ir gatavs
- Uber Gem proc ir gatavs

Šie logi ir **reti** un **grūti** — bet kad tie aligned, damage ir triljonos.

---

## 6. Broken Build #1 — Vecais Burvis "Realitātes Bezgalība"

**Koncepts:** Reality Rift pastāvīgs (keystone) + Torņa Ass double cast + Nolādētais Asinsakmens dubultkritis + Zilspāri ×5 spell power + eksponenciāls piercing chain caur mob grupu.

**Setup:**
- Vecais Burvis, keystone *Realitātes Karaļvalsts* (Reality Rift pastāvīgs).
- Setu *Realitātes Karaļvalsts* pilnās 6/6 daļas.
- Uber Mistiskais: **Nolādētais Asinsakmens** (Ēnu slots, dubultkritis) + **Melnais Kronis** (skills +2).
- Aspekti: 5 imprint (Realitātes Karogs, Torņa Sirds, Vientuļā Sirds, Ledus Šauta Ceļš, Divu Cast Kronis).
- Paragon 350 — leg mezgls "Realitātes Karaļvalsts Kronis" aktīvs.

**Combat window setup (~30s uzkrāj):**
1. Ienāc bosa cīņā ar 10 Zilspāri (max ar setu bonuss).
2. Uzkrāj Ark. Enerģija līdz 100.
3. Aktivē Reality Rift → pastāvīgi 80% time slow.
4. Boss ir Vulnerable no Time Wave.
5. Setu 6-piece proc gatavs (crits triple dmg Reality Rift laikā).

**Trigger:** viens Arkānā Lauska LMB pret grupu.

**Aprēķins:**

Base = 2400 × 1.40 × 2.00 (Reality Rift buff) × 3.00 (setu 6-piece triple crit) = **20 160** per hit.

Multipliers stacked (crit vs Vulnerable, minjoni tuvāk, ...):
- Prāta mult ×2.49
- Additive bucket (Spell 90% + Elite 30% + Frost 30%) = ×2.50
- Crit ×2.5 × 1.6 (GA + Nolādētais Asinsakmens dubultā) = ×8.00
- Vulnerable ×1.75 × 1.25 = ×2.19
- Spell Reizinātājs ×1.45
- Aspect Realitātes Karogs ×1.50
- Aspect Torņa Sirds ×2.00 (Zilspāri ×5)
- Aspect Vientuļā Sirds ×1.35 (kad esi 15m+)
- Aspect Ledus Šauta Ceļš ×1.50 (piercing exp)
- Aspect Divu Cast Kronis ×2.00 (Torņa Ass sinerģija)
- Melnais Kronis skill +2 ×1.20
- Paragon leg Realitātes Karaļvalsts Kronis ×2.00
- Rūna Realitātes Zīme Lv 21 ×1.60

Total multiplier:
```
2.49 × 2.50 × 8.00 × 2.19 × 1.45 × 1.50 × 2.00 × 1.35 × 1.50 × 2.00 × 1.20 × 2.00 × 1.60

= 2.49 × 2.50  = 6.23
× 8.00        = 49.80
× 2.19        = 109.06
× 1.45        = 158.14
× 1.50        = 237.20
× 2.00        = 474.40
× 1.35        = 640.44
× 1.50        = 960.66
× 2.00        = 1 921.32
× 1.20        = 2 305.58
× 2.00        = 4 611.16
× 1.60        = 7 377.86
```

Total mult = **× 7 378**.

Damage per hit (single mērķis, base 20 160):
```
20 160 × 7 378 = 148 741 480
              ≈ 148.7 miljoni per hit
```

**Bet Ledus Šauta Ceļš piercing** — hit chains cauri visai mob grupai ar +50% dmg katrā mērķī.
- Hit 1: 148.7M
- Hit 2: 223.1M (×1.5)
- Hit 3: 334.6M
- Hit 4: 501.9M
- Hit 5: 752.9M

Kopējais chain damage 5 mērķi: **1 961 M = 1.96 miljardi per LMB cast**.

**Torņa Ass double cast bonuss:** katrs LMB = 2 casts.
- **~3.92 miljardi per LMB combo**.

**Ar 1.5 hits/sec ar Ark. Enerģija 0 patēriņš (setu proc kombinēts ar Nolādētais Asinsakmens):**
= **~5.88 miljardi DPS burst window** (ilgums ~15s pirms Ark. Enerģija plok).

**Uber Boss 500 miljardu HP:** **~85 sekundes kill** ideālā combo window.

**Ar Reality Rift ult sinerģiju + Vecākais Māc proc window:** peak spike sasniedz **~50 miljardus 3-sekunžu combo**.

---

## 7. Broken Build #2 — Otrs Kungs "Impēriskā Zibens Impērija"

**Koncepts:** Peldošā formā triple dmg + Melnās Zibens Kronis bezgalīgi lēcieni + setu 6-piece chain explosion = ķēdes reakcija cauri visai mob grupai.

**Setup:**
- Otrs Kungs, keystone *Impēriskā Karaļvalsts* (Impērija Vaidēs pastāvīgs).
- Setu *Impēriskā Karaļvalsts* pilnās 6/6 daļas.
- Uber: **Melnais Kronis** + **Nolādētais Asinsakmens** (Uber Gem).
- Aspekti: Melnās Zibens Kronis, Impēriskā Sirds, Zibens Kalts, Divu Rokas Kronis, Impērisks Vairogs.
- Paragon 350 — leg "Impēriskā Karaļvalsts Kronis".

**Combat window (~15 sekundes):**
1. Uzkrāj Imp. Dusma līdz 100 pret gaišā ordeņa adds.
2. Impērija Vaidēs → peldošā formā aktivē.
3. Boss ir Vulnerable no Impēriskais Sitiens combo.
4. 15+ mob grupā ietvertai.
5. Setu 6-piece proc window (chain lightning eksplozija).

**Trigger:** viens Melnās Zibenis Q pret grupu.

**Aprēķins:**

Melnās Zibenis chains ar keystone: bezgalīgi lēcieni (līdz nav vairāk mērķu).
- Base per hit = 2400 × 1.30 × 3.00 (peldošā formā triple) = 9 360.
- Ar visiem mult stacked (~×1 200 combined):
- Per hit ~11.2M damage.
- **20 lēcienu chain (mob grupā): 20 × 11.2M × 0.5 (each lec drop-off ar Zibens Kalts kompensē) = 224 miljoni.**
- **Setu 6-piece explosion pēc pēdējā lēciena: 500% weap × visi mults = ~55M papildu AoE.**

**Kopējais Melnās Zibenis burst: ~280 miljoni.**

**Cooldown -30% no Trono Māksla + Impērija Vaidēs sinerģija:** Melnās Zibenis cooldown ~3s.

**DPS: ~93 miljoni sustained grupā.**

**Peak spike:** ja pieskaitis Impēriskais Sitiens combo (Divu Rokas Kronis +100% dmg 3s), sasniedz **~2 miljardus 3-sekunžu combo**.

---

## 8. Broken Build #3 — Pirmais Kungs "Meistara Impērija"

**Koncepts:** 38 minjoni max (setu + Barveža Karogs + keystone) + Ceturto Kroni pastāvīgs +30% + Universālais Kalts ×15% papildu + Kunga Zvans ult ar Melnā Sirds (M) stacking.

**Setup:**
- Pirmais Kungs, keystone *Kunga Impērija* (max 35 minjoni + setu +3 = 38).
- Setu *Kunga Impērija* pilnās 6/6 daļas.
- Uber: **Melnais Kronis** (visas prasmes +2 = vēl vairāk minjonu bonuss).
- Uber Gem: **Nolādētais Asinsakmens**.
- Aspekti: Ceturto Kroni, Universālais Kalts, Meistara Karogs, Kunga Zvana Māc, Melnā Sirds (M) ×2 stackē.
- Paragon 350 — leg "Meistars pāri Visiem Kronis".

**Combat window (~20s uzkrāj):**
1. Grupā visi 4 minjonu tipi × ~10 katrs = **38 minjoni**.
2. Ceturto Kroni pastāvīgs +30% dmg.
3. Universālais Kalts pastāvīgs +15%.
4. Aura Māc + Meistara Karogs procs sinerģē.
5. Uzkrāj Dominance līdz 100.

**Trigger:** Kunga Zvans ult (visi minjoni max buff 8s + tu iegūsti barjeru).

**Aprēķins:**

Katrs minjons Kunga Zvana laikā:
- Base = 350 (avg)
- Ar visiem stack (aurā ×1.35 × ×1.30 × ×1.30 × ×2.00 (Kunga Zvans ×2 dmg proc) × ×1.20 (Melnais Kronis skills +2 aura efekts) × ×1.50 (Ceturto Kroni + Universālais Kalts kombinēts))
- Katrs minjons: 350 × 1.35 × 1.30 × 1.30 × 2.00 × 1.20 × 1.50 = **2 878 per swing**

**38 minjoni × 2 878 × 1/sec = 109 364 DPS sustained** (baseline)

**Ar Melnā Sirds (M) 2× stackē:** katrs minjons dod tev +2% dmg = **+76% tavas prasmes** (broken potenciāls).

**Kunga Zvana burst window (8s):**
- 38 minjoni × 2 878 × 2 × 8 = **1 749 824 = ~1.75 miljardi per 8s window**.
- Katras 20s cikls (ar Dominance uzkrāj): **~87.5 miljoni DPS burst avg**.

**Peak spike:** ja Kunga Zvans + Domination Wave + Master's Call sync pilnā perfekcijā, ar visiem minjoniem gauged uz vienu bosu, damage sasniedz **~5 miljardus 8s window**.

Uz Uber Boss 500 miljardu HP: **~100 minūtes kill** (šis nav DPS build, bet drošs sustain).

---

## 9. Kopsavilkums — trillion-range builds

| Build                                          | Peak Burst  | Sustained DPS | Cīņas ilgums | Prasa |
|------------------------------------------------|-------------|---------------|--------------|-------|
| Vecais Burvis "Realitātes Bezgalība"          | 50 miljardi | 5.9 miljardi  | 15s combo     | Zilspāri max + Ark. Enerģija full |
| Otrs Kungs "Impēriskā Zibens Impērija"        | 2 miljardi  | 280 miljoni   | 3s combo      | Mob density + Impērija Vaidēs |
| Pirmais Kungs "Meistara Impērija"             | 5 miljardi  | 1.75 miljardi | 8s window     | 38 minjoni + Kunga Zvans sync |

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
