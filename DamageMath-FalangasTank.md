# Damage Math — Falangas Tank T80

*Konkrēti skaitļi Melnā Bruņinieka Falangas Tank buildam pret bosiem — no vienas LMB līdz Uber bosa kill time.*

Šis dokuments parāda **pilnu damage pipeline** ar reāliem skaitļiem — cik Falangas Tank sit T80 line-up ar Reliktu ekipējumu, Paragon 200, un kā šie skaitļi maināt jaunā mainīgā ietekmē kopējo DPS.

---

## 1. Setup — Test dummy Bruņinieks

**Level 80, Paragon 200, Falangas Tank build**

### 1.1. Ekipējums

| Slot         | Item                                                  | Retums      | iP  |
|--------------|-------------------------------------------------------|-------------|-----|
| Ierocis      | 2h Vēža Zvans, ar *Krusta Karogs* Aspekts             | Sena, 4 GA  | 925 |
| Vairogs      | Kritušā Kroņa Vairogs, ar *Falangas Ābece* Aspekts    | Reliktu     | 925 |
| Ķivere       | Melnais Kronis                                        | Uber Mistiskais | 925 |
| Rumpis       | Kritušā Krusta Rīsli (setu 1/6)                       | Reliktu     | 925 |
| Rokas        | Setu 2/6                                              | Reliktu     | 925 |
| Kājas        | Setu 3/6                                              | Reliktu     | 925 |
| Josta        | Sena ar Grēksūdzes GA                                 | Sena, 3 GA  | 925 |
| Zābaki       | Setu 4/6                                              | Reliktu     | 925 |
| Amulets      | Sena ar Crit Dmg Reizinātājs GA                       | Sena, 4 GA  | 925 |
| Gredzens 1   | Reliktu ar *Kritušā Ordeņa Karogs* Aspekts            | Reliktu     | 925 |
| Gredzens 2   | Setu 5/6                                              | Reliktu     | 925 |
| Ēnu slots    | (tukšs)                                               | —           | —   |

Setu 5/6 daļas — 4 daļas bonuss aktīvs. **Nav pilnas 6/6** (tas ir vēl endgame chase).

### 1.2. Gemi

- Ierocis: **Rubīns** (Royal, tier 6) — +180 Physical Damage
- Bruņas (3 slots): **Rubīns** (Royal) × 3 — +1500 max HP each
- Rotaslietas (3 slots): **Smaragds** (Royal) × 2 (crit dmg) + **Dimants** (Royal) × 1 (resists)

### 1.3. Rezultējošie stati (dēkā)

| Kategorija                    | Vērtība    |
|-------------------------------|------------|
| **Spēks**                     | 1200       |
| **Izturība**                  | 900        |
| **Prāts**                     | 350        |
| **Dzīvība**                   | 45 000     |
| **Bruņas**                    | 12 000     |
| **Kopējais resists**          | 65%        |
| **Grēksūdze** (max)           | 100/100    |
| **Kustība**                   | Normāla    |
| **Attack Speed**              | 1.15 hits/s |

**Damage stati:**

| Stats                                    | Vērtība |
|------------------------------------------|---------|
| **Weapon Damage (base per swing)**       | 2 400   |
| **Kritiskā Šanse**                       | 62%     |
| **Kritiskais Damage**                    | 250% (mult ×2.5) |
| **Crit Dmg Reizinātājs** (GA)            | +50% (mult ×1.5) |
| **Physical Damage %** (additive)         | +90%    |
| **Physical Reizinātājs**                 | +30% (mult ×1.30) |
| **Vulnerable Damage %** (additive)       | +40%    |
| **Vulnerable Reizinātājs**               | +25% (mult ×1.25) |
| **Damage vs Elite/Boss %**               | +30%    |
| **Aspect *Kritušā Ordeņa Karogs* mult**  | +25% (mult ×1.25) — kad minjoni tuvāk 8m |
| **Overpower %**                          | +35%    |

**Paragon 200 devums (kopš 80):**
- Zvērests dēlis: +300 Spēks, +5 000 HP, +8% crit chance.
- Barvedis dēlis: +25% Sūkšņu bojājums, +2 max Sūkšņi.
- Tvirtīgums dēlis: +5 000 HP, +25% bruņas.
- **Rūna *Ģerbonis* Level 12** — +30% pret elementāriem tavā rādiusā.
- **Rūna *Asinssaite* Level 8** — +18% Sūkšņu bojājums.

---

## 2. Damage pipeline formula

Damage aprēķina soļi (D4-style — additive bonuses un multiplicative reizinātāji atsevišķi):

```
FINAL_DMG =
    Base_Skill_Damage                          ← WeaponDmg × Skill%
  × (1 + Additive_Bonuses)                     ← Physical% + Elite% + citi additive
  × Attribute_Multiplier                       ← 1 + Str × 0.001 (Bruņiniekam)
  × Crit_Multiplier                            ← ja crit: CritDmg × (1 + CritDmg_R); citādi 1
  × Vulnerable_Multiplier                      ← ja vuln: (1 + Vuln%) × (1 + Vuln_R); citādi 1
  × Physical_Reizinātājs                       ← katrs "Reizinātājs" affix atsevišķi
  × Aspect_Multipliers                         ← katrs Aspekts atsevišķi
  × Overpower_Multiplier                       ← ja Overpower trigers
```

**Svarīgi:**
- **Additive** ("Physical Damage %" u.c. bez "Reizinātājs") — visi sasummē vienā skaitļā, tad reizina vienreiz.
- **Reizinātājs** un **Multiplier** affixi — katrs pieķer savu ×N kā atsevišķu multiplikatori (spēles-maiņas power).
- Katrs jauns Reizinātājs ir eksponenciāls power gain.

---

## 3. LMB — Krusta Sitiens (120% weapon damage) aprēķins

Situācija: **Sitiens pret Vulnerable bosu, minjoni tuvāk 8m, elite mērķis.**

### Solis 1 — Base Skill Damage
```
Base = WeaponDmg × Skill%
     = 2 400 × 1.20
     = 2 880
```

### Solis 2 — Additive Bonuses
Sasummē visi additive bucket bonusi:
```
Additive = 1 + Physical% + Elite%
         = 1 + 0.90 + 0.30
         = 2.20
```

### Solis 3 — Attribute Multiplier (Spēks Bruņiniekam)
Spēks konvertācija: **0.1% dmg per point** (bāzes 10 punkti dod 0).
```
Str_Mult = 1 + (Str − 10) × 0.001
         = 1 + (1200 − 10) × 0.001
         = 1 + 1.19
         = 2.19
```

### Solis 4 — Crit Multiplier (ja crits, 62% chance)
```
Crit_Mult = CritDmg × (1 + CritDmg_R)
          = 2.50 × 1.50
          = 3.75
```
Ja non-crit: `Crit_Mult = 1`.

### Solis 5 — Vulnerable Multiplier
```
Vuln_Mult = (1 + Vuln%) × (1 + Vuln_R)
          = 1.40 × 1.25
          = 1.75
```

### Solis 6 — Reizinātāji un Aspekti (multiplicative)
```
Physical_R      = 1.30
Aspect_Karogs   = 1.25   (Kritušā Ordeņa Karogs, aktīvs kad minjoni tuvāk)
```

### Kopējais aprēķins uz vienu **CRIT** hit

```
DMG_crit = 2 880 × 2.20 × 2.19 × 3.75 × 1.75 × 1.30 × 1.25

Step 1: 2 880 × 2.20        = 6 336
Step 2: 6 336 × 2.19        = 13 876
Step 3: 13 876 × 3.75       = 52 036
Step 4: 52 036 × 1.75       = 91 062
Step 5: 91 062 × 1.30       = 118 381
Step 6: 118 381 × 1.25      = 147 977

DMG_crit ≈ 148 000
```

### Kopējais aprēķins uz vienu **NON-CRIT** hit
Tas pats, tikai bez Crit_Mult (kas ir ×3.75):
```
DMG_noncrit = 148 000 / 3.75 = 39 460
```

### Vidējais LMB hit
```
Avg_LMB = 0.62 × 148 000 + 0.38 × 39 460
        = 91 760 + 14 995
        = 106 755 dmg per hit
```

### LMB DPS
```
LMB_DPS = Avg_LMB × Attack_Speed
        = 106 755 × 1.15
        = 122 768 DPS
```

**≈ 123 K DPS no LMB vien.**

---

## 4. Vairoga Sitiens (Q, aktīvs, 60% weap dmg, stun 1.5s)

Bāzes ir mazāks, bet **stun** ir kritisks utility.

```
Base = 2 400 × 0.60 = 1 440

Ja crits pret vuln boss ar minjoniem:
DMG = 1 440 × 2.20 × 2.19 × 3.75 × 1.75 × 1.30 × 1.25
    = 73 988

Vidējais (62% crit):
Avg = 0.62 × 73 988 + 0.38 × 19 730 = 53 361
```

Vairoga Sitiens cooldown 4s → **~13 340 DPS** (Vairoga viena hit / 4 sec).

Bet svarīgākais: **1.5s stun** ļauj tev turpināt LMB spam pret imobilizētu bosu, kas nozīmē ~2 LMB hit "brīvi" katrā stun logā = papildu ~213K damage per 4s cikls = **~53K DPS bonus no stun** (ne tieši, bet nedēļas rotācijas ietvarā).

---

## 5. Zvana Metiens (E, 100% weap dmg, ranged, MS slow)

```
Base = 2 400 × 1.00 = 2 400

DMG crit pret vuln boss:
2 400 × 2.20 × 2.19 × 3.75 × 1.75 × 1.30 × 1.25 = 123 314
Avg (62% crit) = 76 466 + 12 496 = 88 962

Zvana Metiens cooldown 8s → 11 120 DPS
```

Utility: slow -30% → boss lēnāk, tavi minjoni saglabā uzbrukuma logu.

---

## 6. Melnais Zvans (100 pt ult, 400% weap dmg AoE + 5s stun + minjoni imunī)

```
Base = 2 400 × 4.00 = 9 600

Crit pret vuln boss:
9 600 × 2.20 × 2.19 × 3.75 × 1.75 × 1.30 × 1.25 = 493 258
Avg = 305 820 + 49 990 = 355 810 dmg

Ult cooldown ~30s (100 pt Grēksūdze uzkrāj ~30s cīņā)
→ ~11 860 DPS

Bet ult log ir 5s stun, kur tu un minjoni sit 100% brīvi
= ~130K papildu damage tajā logā
→ efektīvs DPS pieaugums bosa cīņā: ~4 300 DPS
```

---

## 7. Sūkšņu (minjonu) devums

**15 aktīvi Sūkšņi** (max ar Paragon Barvedis boostu, atskaitot dažus, kas mirst).

**Katrs Sūkšņis:**
- Bāzes attack: 300 dmg per swing
- Attack speed: 1/sec

**Kopīgie modifieri Sūkšņiem (Falangas Tank build):**
- Bruņinieka aura +30% dmg = 1.30
- Paragon Barvedis +25% = 1.25
- Setu 4-piece bonuss: **+25% pastāvīgs bojājums** (stack ap 6 līdz 10, vidēji 8 → +8%) = ~1.30 vidējais buff
- Rūna *Asinssaite* Lv 8: +18% dmg = 1.18
- Zvana Atbalss Aspekts nedod dmg Sūkšņiem tieši, bet stun trigereis nozīmē Sūkšņi sit uz free window

```
Sūkņa vid. dmg per swing:
= 300 × 1.30 × 1.25 × 1.30 × 1.18
= 300 × 1.30 = 390
× 1.25 = 488
× 1.30 = 634
× 1.18 = 748 dmg / swing

Kopējais Sūkšņu DPS:
= 15 × 748 × 1 swings/sec
= 11 220 DPS
```

**≈ 11 K DPS no minjoniem.**

---

## 8. Kopējais Falangas Tank DPS (T80, Paragon 200)

| Avots                        | DPS      | % no kopējā |
|------------------------------|----------|-------------|
| LMB (Krusta Sitiens)         | 122 800  | 63%         |
| Q (Vairoga Sitiens) + stun value | 66 700 | 34%         |
| E (Zvana Metiens)            | 11 120   | 6%          |
| Ult (Melnais Zvans)          | 16 160   | 8%          |
| Sūkšņi (minjoni)             | 11 220   | 6%          |
| Kopīgi grupas buffi (aspekti procs) | 5 000 | 3%       |
| ~ *Additive un rotācijas overlap* | −40 000 | −20% |
| **NETO KOPĒJAIS**            | **~193 000 DPS** | **100%** |

(Ne tieši sanāk 200K sakarā ar rotācijas overlap, bet reālistisks vidējais **~190K DPS**.)

---

## 9. Boss kill time (dažādi HP scenāriji)

Pieņemam **konstantu** DPS (praksē tas svārstās ar rotāciju):

| Boss tips              | HP           | Kill time         |
|------------------------|--------------|-------------------|
| Elite dungeon boss     | 10 M         | ~52 sec           |
| Reģiona boss (Bīskaps Ansels) | 50 M  | ~4 min 20 sec     |
| Murgu Dungeon boss T50 | 100 M        | ~8 min 40 sec     |
| World Boss             | 200 M        | ~17 min 20 sec    |
| Uber Boss (endgame)    | 500 M        | ~43 min 15 sec    |

Falangas Tank **nav** DPS build — tas ir surv-tank. Uber Boss 43 min ir garš, bet **droši** (nemirst). Salīdzinājumā, DPS-fokusēts build (Berserker A) sasniegs Uber Bosu ~15-18 min laikā, bet mirst 3-4× vairāk.

---

## 10. Sensitivity analīze — kas notiek, ja...

### 10.1. Zaudē Amuleta Crit Dmg Reizinātājs GA (−50% mult)

```
Ar GA: Crit_Mult = 2.50 × 1.50 = 3.75
Bez GA: Crit_Mult = 2.50 × 1.0  = 2.50
```

LMB crit tagad: 148 000 × (2.50/3.75) = 98 667
Vid. LMB: 0.62 × 98 667 + 0.38 × 39 460 = 61 174 + 14 995 = 76 169
LMB DPS: 87 594

**Kopējais DPS krīt ~30% (no 193K uz 135K).** Vēl spēlējams, bet manāma zaudēšana.

### 10.2. Bosa ir Vulnerable-immūns

Vulnerable pilnīgi nedarbojas — Vuln_Mult = 1.
```
LMB crit vs non-vuln = 148 000 / 1.75 = 84 571
Vid. LMB = 52 434 + 8 568 = 61 002
LMB DPS = 70 152
```

**Kopējais DPS krīt ~40% (no 193K uz 115K).** Ievērojami, jo Vulnerable pievieno gan additive, gan reizinātājs.

### 10.3. Minjoni miruši (Aspects *Kritušā Ordeņa Karogs* aizkavēts)

```
Bez aspect: 148 000 / 1.25 = 118 400 crit
Vid. LMB = 73 408 + 12 000 = 85 408
LMB DPS = 98 219
```

**Kopējais DPS krīt ~22% (no 193K uz 151K), plus minjonu 11K DPS zaudēts.** Vienlaikus jāpaātrina minjonu resurrect.

### 10.4. Crit chance krīt no 62% uz 40%

```
Vid. LMB = 0.40 × 148 000 + 0.60 × 39 460 = 59 200 + 23 676 = 82 876
LMB DPS = 95 307
```

**Kopējais DPS krīt ~22% (uz 150K).**

### 10.5. Pilnas 6/6 setu daļas (nevis 4/6)

6-piece transformācija:
- Falangas Signāla laikā Sūkšņi imunī → viņi sit non-stop ~15s katras ~30s = pieaugums Sūkšņu uptime ~50%.
- Ar 15 Sūkšņu īsts DPS: 11 220 × 1.5 = 16 830

Plus 6-piece perk tavai keystone: **Kritušā Krusta** dod +200% Grēksūdzes uzkrāšana → ult (Melnais Zvans) katras ~20s vietā 30s → ult DPS pieaugums no 16K uz 24K.

**Kopējais DPS pieaugums ~13% (uz 218K).** Setu 6/6 chase ir vērts.

### 10.6. Uber Mistiskais **Melnais Kronis** slots

Melnais Kronis dod visām prasmēm +2 līmeņus un signāles resurss 2× ātrāk:
- Skill damage aug ~10% per +1 lvl = +20% Base Skill Damage.
- Ult (Melnais Zvans) tagad katras ~15s vietā 30s → +60% ult uptime.

Aprēķins:
- LMB base = 2 880 × 1.20 = 3 456
- LMB crit = 148 000 × (3 456/2 880) = 177 600
- Vid. LMB = 109 512 + 17 990 = 127 502
- LMB DPS = 146 627

**Kopējais DPS pieaugums ~15% (uz 222K).**

**Ar 6/6 setu + Melnais Kronis kopā: DPS ~250K.**

---

## 11. Salīdzinājums ar citiem buildiem T80 (aptuveni)

| Build                                    | Aptuvenais DPS | Survivability  | Pielietojums                     |
|------------------------------------------|----------------|----------------|-----------------------------------|
| **Bruņinieks Falangas Tank**             | 190–250 K      | Ļoti augsta    | Balansēts, drošs, komandvieta    |
| Bruņinieks Heal-Tank                     | 110–150 K      | Ekstrēma       | Boss ar pastāvīgu DoT             |
| Bruņinieks Zvana Uguns (Hibrīds)         | 220–280 K      | Vidēja-augsta  | Multi-target, farm                |
| Berserker Dual-Wield DPS                 | 350–450 K      | Zema           | Ātrs mob clear, glass cannon     |
| Berserker Rage-Sustain                   | 280–350 K      | Augsta         | Boss aug ar cīņas ilgumu          |
| Alkas Kungs Projectile Spam              | 250–320 K      | Vidēja         | Mob density                       |
| Alkas Kungs Golemu Kalts                 | 200–260 K      | Vidēja-augsta  | Boss AFK-style                    |
| Mežonis DoT Spam                         | 170–230 K sust.| Vidēja         | Mob clear ar DoT spread           |
| Mežonis Beast Melee                      | 300–400 K burst| Zema-vidēja    | Spike windows                     |
| Bezēnu Zaglis Teleport-Backstab          | 300–400 K      | Zema           | Elite grupas                      |
| Bezēnu Zaglis Permanent Invis            | 400–550 K single | Zema         | Single-target boss / Uber         |
| Vientuļais Sniegurgs Freeze Nuke         | 350–450 K burst| Vidēja         | Freeze-vulnerable                 |
| Vientuļais Sniegurgs Skeletu Karaļvalsts | 220–290 K      | Augsta         | Boss AFK-style                    |

**Falangas Tank ir vidus jauda, bet augstākā survivability** — labs sākuma un galvenais uzticīgais build.

---

## 12. Uber Boss (500 M HP) — pilna cīņa

Ar **6/6 setu + Melnais Kronis** = 250K DPS:

```
Kill time = 500 000 000 / 250 000 = 2 000 seconds = 33 min 20 sec
```

Vidēji reāli - jau ~30 min (dažas fāzes ar downtime uz mehāniku).

**Salīdzinājums:**
- Berserker glass cannon: ~18 min bet 5× mirst
- Bezēnu Zaglis Permanent Invis: ~15 min drošs
- Falangas Tank: **30 min droši**, viens shortcut = tu dzīvs = neapstādināma kill

---

## 13. Rezumējums — Falangas Tank T80 patiesie skaitļi

- **LMB average hit: ~107 K damage** (crits 148 K, non-crit 39 K)
- **Per-second personal DPS: ~123 K**
- **Kopējais rotācijas DPS: ~193 K** (baseline)
- **Ar 6/6 setu + Melnais Kronis: ~250 K DPS**
- **Uber Boss 500M HP kill time: ~30 min**
- **Survivability: 45 K HP + 12 K bruņas + 65% resists + Grēksūdzes heals + Sūkšņu absorb**
- **Weakest point:** Vulnerable-immūni bosi (−40% DPS)
- **Strongest point:** Elite grupas ar minjoniem uzturētiem tuvumā

*Skaitļi ir sākuma balansa piedāvājums. Reāls balansa tuning notiek pēc playtestingu.*
