# Damage Math — Visi 17 buildi T80 Paragon 200

*Kompakts aprēķinu doc pārējiem 17 buildiem (izņemot Bruņinieks Falangas Tank, kas ir savā `DamageMath-FalangasTank.md`).*

Visi aprēķini pieņem **Level 80, Paragon 200, 4/6 setu daļas, 4 imprintēti Aspekti, viens Uber Mistiskais**. Sensitivity analīze parāda, kā aug ar pilnas 6/6 setu un otro Uber.

Pilna damage pipeline formula un metodoloģija — skat. `DamageMath-FalangasTank.md` sadaļu 2.

---

## Kopīgais setup

- **Weapon base damage: 2 400 per swing** (iP 925 Sena weapon).
- **Attribute Multiplier formula:** `1 + (Primary − 10) × 0.001` (Str/Vek/Prāts).
- **Additive bucket:** Physical/Element% + Elite% + Vulnerable% + situatīvie.
- **Multipliers:** katrs Reizinātājs, Aspekts, Setu proc atsevišķi.
- **Attack Speed base:** 1.0/sec (2h), 1.4/sec (dual wield), 1.6/sec (dagger).

---

# BRUŅINIEKS

## 1.1. Build B — *Asins Katedrāles Kārkas* (Heal-Tank)

**Setup:** 6/6 setu (kritiski svarīgi šim), Sirds no Kalna Uber, keystone *Asins Katedrāle*.

**Key stats:**
- Spēks: 1 100 → mult ×2.09
- Weapon 2 400, AS 1.10/sec
- Crit Chance: 45% (fokuss uz HP)
- Crit Damage: 200% (mult ×2.00)
- Crit Dmg Reizinātājs: +30% → ×1.30
- Additive bucket: +170% → ×2.70
- Physical Reizinātājs: ×1.25
- Vulnerable: ×1.50
- Aspects (3 imprint): kombinēts ×2.20
- Setu 6-piece: Grēksūdzes Zvans cooldown 0 → 3× uptime
- Overpower base swap: HP 55K + Fortify 300K + Barrier 50K = **405 000**

**Main hit — Krusta Sitiens LMB:**
```
Base = 2 400 × 1.20 = 2 880
Mult = 2.09 × 2.70 × (0.45 × 2.00 × 1.30) × 1.50 × 1.25 × 2.20
     = 2.09 × 2.70 × 1.17 × 1.50 × 1.25 × 2.20 (crit weighted)
Hit crit = 2 880 × 2.09 × 2.70 × (2.00×1.30) × 1.50 × 1.25 × 2.20
         = 2 880 × 2.09 × 2.70 × 2.60 × 1.50 × 1.25 × 2.20
         = 174 253
Hit noncrit = 174 253 / 2.60 = 67 020
Avg LMB = 0.45 × 174 253 + 0.55 × 67 020 = 78 414 + 36 861 = 115 275

LMB DPS = 115 275 × 1.10 = 126 800
```

**Overpower Melnais Zvans burst (retais):**
```
Base_swap = 405 000 × 4.00 (skill%) = 1 620 000
Full mult stack = ×140 (kompaktā versija)
Peak = 1 620 000 × 140 = 226 800 000 ≈ 227 M per hit
AoE 5 mērķi = 1.13 miljardi burst
```

**DPS breakdown:**
| Avots | DPS |
|---|---|
| LMB | 126 800 |
| Vairoga Sitiens | 32 000 |
| Grēksūdzes Zvans (setu 6-piece uptime) | 45 000 (buff dmg + minjonu bonuss) |
| Melnais Zvans regular | 8 000 |
| Minjoni (12 Sūkšņi ar heal buff) | 15 000 |
| **Kopā** | **~145 000 DPS** |

**Kill times:**
- Elite 10M: 69 sec
- 100M: 11 min 30 sec
- 500M Uber: **57 min** (drošs, nemirsti)

**Peak burst:** ~1.5 miljardi vienā Overpower Zvana logā.

---

## 1.2. Build C — *Zvana Uguns* (Hibrīds)

**Setup:** 4/6 setu, Melnais Kronis Uber, keystone *Zvana Uguns*, Sārtaji + Sūkšņi grupā.

**Key stats:**
- Spēks: 1 000, Prāts 400 → mult ×2.19
- Crit Chance: 55%
- Crit Damage: 230% + 40% GA → mult ×3.22 kopā
- Fizisk + Uguns additive: +150% → ×2.50
- Physical Reizinātājs: ×1.30
- Uguns Reizinātājs: ×1.35
- Vulnerable: ×1.70
- Aspects: ×2.50 (Uguns Krusta, Zvana Sprādzien, Divu Krāsu Kroņis)
- Setu 4-piece: Sārtaji uguns bumbas priekšlaicīgi eksplodē pret Zvana Metiena mērķi

**Main hit LMB (Krusta Sitiens + uguns proc):**
```
Base = 2 400 × 1.20 = 2 880
Hit crit = 2 880 × 2.19 × 2.50 × 3.22 × 1.70 × 1.30 × 1.35 × 2.50
        = 232 700

Avg LMB = 0.55 × 232 700 + 0.45 × 72 267 = 128 000 + 32 520 = 160 520
LMB DPS = 160 520 × 1.15 = 184 600
```

**Sārtaji uguns proc pret Zvana Metiena mērķi:**
- 5 Sārtaji × 1000 dmg × 1.35 (uguns R) × 2.00 (setu proc detonation) = ~13 500 per Zvana Metiens window
- Cikls 8s → 1 687 DPS papildu no priekšlaicīgas eksplozijas
- **Sūkšņi + Sārtaji parallel DPS: ~35 000**

**DPS breakdown:**
| Avots | DPS |
|---|---|
| LMB | 184 600 |
| Vairoga Sitiens | 42 000 |
| Zvana Metiens + Sārtaji proc | 30 000 |
| Melnais Zvans ult | 15 000 |
| Minjoni (Sūkšņi + Sārtaji) | 35 000 |
| Aspekti overlap | −18 000 |
| **Kopā** | **~288 000 DPS** |

**Kill times:**
- Elite 10M: 35 sec
- 100M: 5 min 47 sec
- 500M Uber: 28 min 55 sec

**Peak burst:** ~50 miljardi (Melnais Zvans + Sārtaji uguns kombu).

---

# BERSERKERS

## 2.1. Build A — *Trīs Tūkstoš Deviņi* (Dual-Wield DPS)

**Setup:** 4/6 setu, Melnais Kronis Uber, keystone *Divu Zobenu Kronis*.

**Key stats:**
- Veiklība: 1 300 → mult ×2.30
- Weapon 2×1200 (dual, katrs mazāks) → efektīvs 2 000 per combo cycle
- AS: 1.5/sec (dual wield)
- Crit Chance: 65%
- Crit Damage: 240% + 60% GA → mult ×3.84
- Fizisk additive: +200% → ×3.00
- Physical Reizinātājs: ×1.40
- Vulnerable: ×1.80
- Aspects: ×2.80 (Vēja Ceļš, Divu Krāsu Asmeņi, Asins Ātruma)
- Setu 4-piece: katrs 5. sitiens garantēts crit → efektīvs crit ~72%

**Main hit — 3-hit LMB combo (Divu Zobenu Deja + Divu Zobenu Kronis):**
```
Base per hit = 2 400 × 0.80 (LMB) = 1 920
Combo 3 hits + 30% per hit (keystone) = 1 920 × 3 × 1.30 = 7 488 base per combo

Hit crit (single) = 1 920 × 2.30 × 3.00 × 3.84 × 1.80 × 1.40 × 2.80
                 = 149 500

Avg combo (0.72 crit weighted):
Avg per hit = 0.72 × 149 500 + 0.28 × 38 933 = 107 640 + 10 901 = 118 541
Combo (3 hits) = 355 623

LMB DPS = 355 623 × 0.5 combos/sec (1.5 hits/sec ÷ 3) = 177 811
Ar keystone auto-triple = LMB DPS × 1.30 = 231 155
```

**DPS breakdown:**
| Avots | DPS |
|---|---|
| LMB combo | 231 155 |
| Sāns Sitiens (Q, 180% skill) | 55 000 |
| Uguns Metiens (E, DoT) | 25 000 |
| Kara Kliedziens buff (Asins Trakums)  | 40 000 |
| Vēja Vējš cleave | 20 000 |
| Sārtaji minjoni | 15 000 |
| **Kopā** | **~386 000 DPS** |

**Kill times:**
- Elite 10M: 26 sec
- 100M: 4 min 20 sec
- 500M Uber: 21 min 40 sec

**Peak burst (Asins Trakums 5s + Kara Kliedziens):**
DPS × 1.5 × 1.3 = ~750 000 DPS = 3.75 miljardi 5s. Peak crit hit ~500 M kombu.

**Sensitivity:** 45% def loss (Asins Trakums) — jāizvairās pastāvīgi, bet burst logs kompensē. Ja tas nolaižas, DPS krīt ~30% (270K).

---

## 2.2. Build B — *Kliedziena Karogs* (Rage-Sustain)

**Setup:** 6/6 setu (kritiski svarīgi), Sirds no Kalna Uber, keystone *Nekad Vairs Mierā*.

**Key stats:**
- Veiklība: 1 100, Izturība 900 → mult ×2.09
- Weapon 2×1200
- AS: 1.4/sec
- Crit Chance: 55%
- Crit Damage: 220% + 50% GA → mult ×3.30
- Additive: +180% → ×2.80
- Physical Reizinātājs: ×1.35
- Vulnerable: ×1.60
- Aspects: ×2.50
- Setu 6-piece: Asins Trakums pastāvīgs pie Rage 50+ → ×1.5 pastāvīgs mult
- Setu 4-piece: <30% HP +100% dmg → +1.0× kompozīts, aktīvs ~40% laika → efektīvs ×1.40

**Main hit LMB pastāvīgā Trakumā:**
```
Base = 1 920 (per hit) × 1.30 (keystone) = 2 496
Hit crit = 2 496 × 2.09 × 2.80 × 3.30 × 1.60 × 1.35 × 2.50 × 1.5 (setu 6) × 1.4 (setu 4 avg)
        = 226 300

Avg per hit = 0.55 × 226 300 + 0.45 × 68 576 = 124 465 + 30 859 = 155 324

LMB DPS = 155 324 × 1.4 = 217 454
```

**DPS breakdown:**
| Avots | DPS |
|---|---|
| LMB (pastāvīgā Trakumā) | 217 454 |
| Sāns Sitiens | 55 000 |
| Uguns Metiens DoT | 25 000 |
| Kara Kliedziens grupas buff | 15 000 |
| Sārtaji minjoni | 15 000 |
| **Kopā** | **~327 000 DPS** |

**Kill times:**
- Elite 10M: 31 sec
- 100M: 5 min 6 sec
- 500M Uber: 25 min 30 sec (drošs, sustain)

**Peak burst:** <15% HP window ar 4-piece bonuss = ×2 dmg pastāvīgi 10s = **~650K DPS uzliesmojums**.

---

## 2.3. Build C — *Sarkano Smilšu Kalambs* (Armijas Kliedzējs)

**Setup:** 4/6 setu, Melnais Kronis Uber, keystone *Sarkano Smilšu Kalambs*, visi 4 minjonu tipi grupā (12-15 kopā).

**Key stats:**
- Veiklība: 1 100 → mult ×2.09
- Weapon dual, AS 1.4/sec
- Crit Chance: 50%
- Crit Damage: 220% + 40% GA → mult ×3.08
- Additive: +160% → ×2.60
- Aspects: ×2.30
- Setu 4-piece: Sūkšņi ar uguns dmg (kā Sārtaji)
- Minjonu bonuss (Sarkano Smilšu Kalambs keystone): visi minjoni +30% ātrāki + berserker mode

**Main hit LMB:**
```
Hit crit = 1 920 × 2.09 × 2.60 × 3.08 × 1.60 × 1.35 × 2.30
        = 143 800

Avg per hit = 0.50 × 143 800 + 0.50 × 46 688 = 71 900 + 23 344 = 95 244
LMB DPS = 133 342
```

**Minjonu DPS (kritiskā daļa šim buildam):**
- 15 minjoni (4× Sūkšņi ar uguns + 6× Sārtaji + 3× Zaļči + 2× Zilspāri)
- Bāze: katrs ~350 dmg, AS 1
- Bonusi: +30% aura, +50% keystone berserker, +40% setu buff = ×2.72
- Per minion DPS = 350 × 2.72 = 952 DPS
- Kopā minjonu DPS: 15 × 952 = **14 280**
- Kara Kliedziens (2-piece bonuss +40% AS 8s uz 15s cd) = ×1.20 avg → 17 136 DPS

**DPS breakdown:**
| Avots | DPS |
|---|---|
| LMB | 133 342 |
| Sāns Sitiens | 40 000 |
| Kara Kliedziens burst | 20 000 |
| Minjonu armija | 17 136 |
| Aspekti overlap | −10 000 |
| **Kopā** | **~200 000 DPS** |

**Kill times:**
- Elite 10M: 50 sec
- 100M: 8 min 20 sec
- 500M Uber: 41 min 40 sec

**Peak burst:** Kara Kliedziens ar visiem minjoniem berserker = **~500K DPS** 8s log.

---

# ALKAS KUNGS

## 3.1. Build A — *Zelta Monētu Karaļvalsts* (Projectile Spam)

**Setup:** 4/6 setu, Vecā Goldo Roka Uber, keystone *Zelts Ir Asinis*.

**Key stats:**
- Prāts: 1 000, Ļaunums 1 100 → mult ×2.10
- Weapon 2 200 (staff/scepter), AS 1.2/sec
- Crit Chance: 60% (Zelta Lietus vsy monētas +30% crit — efektīvs 78% uz projectile)
- Crit Damage: 240% + 50% GA → mult ×3.60
- Additive: +180% → ×2.80
- Physical Reizinātājs: ×1.40
- Vulnerable: ×1.70
- Aspects: ×2.60 (Bagātā Sirds, Alkas Aura, Monētu Sirds)
- Setu 4-piece: katra monēta ir garantēts crit → efektīvs 100% crit uz Zelta Lietus

**Main hit — Zelta Lietus (200 zelta, 5m radius, 12 monētas):**
```
Base per monēta = 2 200 × 0.35 (per monēta skill%) = 770
Hit crit = 770 × 2.10 × 2.80 × 3.60 × 1.70 × 1.40 × 2.60
        = 78 500

12 monētas × 78 500 (visas garantētas crit ar 4-piece)
= 942 000 damage per Zelta Lietus (single mērķi)
```

Zelta Lietus cooldown: 4s (setu 2-piece -50% cena, faktiski var kastēt katrs 2s ar CDR + Zelta rezervēm)
**Zelta Lietus DPS: 942 000 / 2 = 471 000 (uz single target!)**

Uz mob grupā tāda pati, jo katra monēta hit atsevišķu mērķi vai piercing → efektīvs multiplier ×5 grupās.

**LMB Monētu Sitiens (100% weap, dropo monētas):**
```
Hit crit = 2 200 × 1.00 × 2.10 × 2.80 × 3.60 × 1.70 × 1.40 × 2.60
        = 224 000
Avg = 0.60 × 224 000 + 0.40 × 62 222 = 134 400 + 24 889 = 159 289
LMB DPS = 191 147
```

**DPS breakdown:**
| Avots | DPS |
|---|---|
| Zelta Lietus (setu 4-piece) | 471 000 (single target) |
| LMB Monētu Sitiens | 191 147 |
| Ķērājs Golems (auto) | 25 000 |
| Alkas Zvans + Sausā Sirds buff loop | 30 000 |
| Aspekti overlap | −25 000 |
| **Kopā (single target)** | **~692 000 DPS** |

**Kill times:**
- Elite 10M: 15 sec
- 100M: 2 min 25 sec
- 500M Uber: 12 min

**Peak burst:** Alkas Zvans + 3× Zelta Lietus 3s window = ~5 miljardi burst.

---

## 3.2. Build B — *Vecā Goldo Aizsegs* (Golemu Kalts)

**Setup:** 4/6 setu, Sirds no Kalna Uber, keystone *Ķērāju Karaļvalsts* → 4 Golemi max.

**Key stats:**
- Prāts: 900, Ļaunums 1 200 → mult ×2.11
- Crit Chance: 50%
- Crit Damage: 200% → mult ×2.50 (bez GA)
- Additive: +150% → ×2.50
- Minjonu Bojājums %: +120% (heavy)
- Aspects: ×2.40 (Divi Golemi, Golemu Karogs, Metāla Kāzas)

**Golemu DPS (kodolā):**
- 4× Ķērājs Golems + 2× Rāceles Ķērājs = 6 constructi
- Bāzes Golems dmg: 800 per swing, AS 1
- Bonusi: +50% Zelta Kalt pasīvs, +40% setu 4-piece, +40% Golemu Karogs Aspekts = ×3.30
- **Per Golem DPS = 800 × 3.30 = 2 640**
- Kopā Golemu DPS = 6 × 2 640 = **15 840**

Ar Ķērāju Karaļvalsts keystone +30% dmg katram = **20 592 DPS no Golemiem**.

Ja Golems mirst → 500% weap eksplozija = 5 000 dmg × cycle mult ×20 = 100 000 per die.

**Player LMB + Zelta Lietus:**
- LMB DPS: ~140 000 (less than Build A jo mazāk crit dmg)
- Zelta Lietus DPS: ~200 000 (bez 4-piece bonuss)

**DPS breakdown:**
| Avots | DPS |
|---|---|
| Golemi (6 aktīvi) | 20 592 |
| Golemu eksplozijas (die/summon cycle) | 8 000 |
| LMB Monētu Sitiens | 140 000 |
| Zelta Lietus | 200 000 |
| Alkas Zvans buff | 15 000 |
| **Kopā** | **~383 000 DPS** |

**Kill times:**
- Elite 10M: 26 sec
- 100M: 4 min 21 sec
- 500M Uber: 21 min 45 sec (AFK style, drošs)

**Peak burst:** 4 Golemi vienlaikus mirst + auto-summon = 400 000 burst instant.

---

## 3.3. Build C — *Zelta Karalis Bez Trono* (Ekonomika + Sausā Sirds)

**Setup:** 4/6 setu, Nolādētais Asinsakmens Uber Gem, keystone *Sausā Sirds Kronis*.

**Key stats:**
- Prāts: 900, Ļaunums 1 000 → mult ×2.00
- AS: 1.5/sec (Sausā Sirds +30% AS + gear bonuss)
- Crit Chance: 65%
- Crit Damage: 250% + 40% GA → mult ×3.50 × 2 (Nolādētais Asinsakmens dubultkritis) = ×7.00 efektīvs
- Additive: +200% → ×3.00
- Aspects: ×2.50
- Setu 4-piece: Sausā Sirds +30% AS, +30% MS → jau iekļauts
- Setu 6-piece: Sausā Sirds laikā hiti dubultā dropo zeltu → sniedz atlicība

**Main hit LMB (pastāvīgi Sausā Sirds aktīvs):**
```
Hit crit = 2 200 × 1.00 × 2.00 × 3.00 × 7.00 × 1.70 × 1.35 × 2.50
        = 265 200
Avg = 0.65 × 265 200 + 0.35 × 37 886 = 172 380 + 13 260 = 185 640
LMB DPS = 185 640 × 1.5 = 278 460
```

Sausā Sirds pati par sevi ×1.5 dmg → viss pievienotais ir jau iekļauts additivā.

**Zelta Lietus:**
- Ar 6-piece dubultā zelta drop + rezerves plūsma = ×2 spam
- DPS: ~350 000

**DPS breakdown:**
| Avots | DPS |
|---|---|
| LMB pastāvīgā Sausā Sirds | 278 460 |
| Zelta Lietus × 2 spam | 350 000 |
| Golemi (1-2 baseline) | 8 000 |
| Alkas Zvans loop | 20 000 |
| Aspekti overlap | −25 000 |
| **Kopā** | **~631 000 DPS** |

**Kill times:**
- Elite 10M: 16 sec
- 100M: 2 min 39 sec
- 500M Uber: 13 min 12 sec

**Peak burst:** Sausā Sirds 60s window ir jau pastāvīgi. Peak ir kad **5M zelta bilance** + Sausā Sirds Kronis + 6-piece = ~1 miljards per Zelta Lietus.

---

# MEŽONIS KUNGS

## 4.1. Build A — *Puvušās Sirds Kronis* (DoT Spam)

**Setup:** 4/6 setu, Ninheras Ērkšķu Piespriedu Uber, keystone *Puve Nekad Nemirst*.

**Key stats:**
- Prāts: 1 200, Ļaunums 1 000 → mult ×2.20
- Crit Chance: 45% (DoT less crit-focused)
- Crit Damage: 200% → mult ×2.50
- DoT Damage additive: +180% → ×2.80
- DoT Reizinātājs (GA): ×1.50
- Vulnerable: ×1.75 (setu 6-piece un endgame)
- Aspects: ×2.50

**Main DoT — Puvusī Migla (kanālēta):**
```
Base per tick (ik 0.5s ar Slāņa 2 Indes Skūpsts) = 2 200 × 0.35 (skill% per tick) = 770
Per tick dmg = 770 × 2.20 × 2.80 × 2.50 (avg with crit) × 1.75 × 1.50 × 2.50
            = 22 700 per tick

Puvusī Migla ilgums 8s = 16 tick per mob
Vienam mob damage total: 16 × 22 700 = 363 200

Migla radius 12m, ~8 mērķi vidēji = 8 × 363 200 = 2.9 M per Puvusī Migla cast
Cooldown ~12s → 240 000 DPS grupā
```

**Ērceles Sitiens LMB (100% weap + Poison stack):**
```
Hit crit = 2 200 × 1.00 × 2.20 × 2.80 × 2.50 × 1.75 × 1.50 × 2.50
        = 111 300
Avg per hit = 68 000
LMB DPS = 68 000 × 1.10 = 74 800
```

**Ninheras Ērkšķu Piespriedu Uber:** ienaidnieki 10m rādiusā pastāvīgi indēti → +10 DoT stack ap tevi = **efektīvs DoT DPS +50%**.

**DPS breakdown:**
| Avots | DPS |
|---|---|
| Puvusī Migla (AoE) | 240 000 (grupa) |
| Ērceles Sitiens LMB | 74 800 |
| Sakņu Slazds + Aizmirstība (DoT tuple) | 55 000 |
| Vilka Āda melee (sporadisks) | 25 000 |
| Ninheras pastāvīgā inde | 45 000 |
| Zaļči minjoni ar inde spread | 30 000 |
| **Kopā (grupa)** | **~470 000 DPS** |
| **Kopā (single target)** | **~200 000 DPS** |

**Kill times (single target):**
- Elite 10M: 50 sec
- 100M: 8 min 20 sec
- 500M Uber: 41 min 40 sec

**Peak burst:** *Puve Nekad Nemirst* keystone spread — grupā ar 30 mob katrs 3-stack DoT = **~5 miljardi 8s window**.

---

## 4.2. Build B — *Vilka Ādas Kronis* (Beast Melee)

**Setup:** 4/6 setu, Melnais Kronis Uber, keystone *Elfu Karaliene* → Vilka Āda 20s ilgums.

**Key stats:**
- Prāts: 1 000, Veiklība 900 → mult ×2.00
- AS: 1.6/sec (Vilka Ādā)
- Crit Chance: 60%
- Crit Damage: 240% + 40% GA → mult ×3.36
- Melee Damage additive: +200% → ×3.00
- Physical Reizinātājs: ×1.40
- Vulnerable: ×1.60
- Aspects: ×2.60 (Zvēra Ceļš, Lāca Sīvība, Neapstādināmais Zvērs)

**Main hit — Vilka Ādas Melee LMB (2× dmg beast mode):**
```
Base = 2 400 × 1.00 × 2.00 (beast mode) = 4 800
Hit crit = 4 800 × 2.00 × 3.00 × 3.36 × 1.60 × 1.40 × 2.60
        = 281 700
Avg per hit = 175 000
LMB DPS = 175 000 × 1.6 = 280 000
```

Vilka Ādas ilgums 20s / 30s cd = **67% uptime**. Bezmaskes formā LMB ir 100 000 DPS.

**Vidējais DPS (weighted):**
- 67% laika: 280 000 DPS (beast mode)
- 33% laika: 100 000 DPS (human mode)
- **Vidēji: 220 800 DPS**

**Setu 4-piece:** LMB pastāv AoE (3m radius) beast mode → cleave grupas.

**DPS breakdown:**
| Avots | DPS |
|---|---|
| Vilka Ādas LMB (avg weighted) | 220 800 |
| Puvusī Migla ilgstoša | 60 000 |
| Sakņu Slazds burst | 20 000 |
| Ērceles Sitiens human mode | 15 000 |
| Zaļči minjoni | 25 000 |
| **Kopā (single target)** | **~340 000 DPS** |

**Kill times:**
- Elite 10M: 29 sec
- 100M: 4 min 54 sec
- 500M Uber: 24 min 30 sec

**Peak burst:** Vilka Āda + Setu 4-piece AoE grupā ar 8 mob = **~1.8 miljardi 20s window**.

---

## 4.3. Build C — *Sakņu Zvēra Kronis* (Zone Control)

**Setup:** 6/6 setu (kritiski), Sirds no Kalna Uber, keystone *Sakņu Kronis*.

**Key stats:**
- Prāts: 1 100 → mult ×2.09
- Crit Chance: 45%
- Crit Damage: 210% + 30% GA → mult ×2.73
- Additive: +170% → ×2.70
- Vulnerable: ×1.70
- Aspects: ×2.30

**Main mehānika — Sakņu Kronis (pastāvīgs, ap tevi 15m ar setu 2-piece + keystone):**
- Katri 5s mikroeksplozija Sakņu Kroņā (setu 6-piece) = 200% weap dmg AoE
- Base = 2 400 × 2.00 = 4 800 per mob
- Mult stack (avg): ×80
- Per mob = 384 000 damage per 5s
- Vidēji 6-8 mērķi krītu = **6 × 384 000 / 5 = 460 000 DPS grupā** no eksploziju vien

**Sakņu slow -30% dmg (setu 4-piece) uz mērķiem:** tavas tiešās prasmes iesit vairāk.

**Ērceles Sitiens LMB pret Sakņu Kroņā mērķiem:**
```
Hit crit = 2 200 × 1.00 × 2.09 × 2.70 × 2.73 × 1.70 × 1.35 × 2.30 × 1.30 (slow mult)
        = 175 000
Avg per hit = 105 000
LMB DPS = 115 500 × 1.10 = 127 050
```

**DPS breakdown:**
| Avots | DPS |
|---|---|
| Sakņu Kroņa mikroeksplozijas | 460 000 (grupa) |
| LMB pret slowed mērķi | 127 050 |
| Puvusī Migla | 80 000 |
| Zaļči Sakņu Kroņā | 35 000 |
| **Kopā (grupa)** | **~700 000 DPS** |
| **Kopā (single target)** | **~200 000 DPS** |

**Kill times (single):**
- Elite 10M: 50 sec
- 100M: 8 min 20 sec
- 500M Uber: 41 min 40 sec

**Peak burst:** grupa ar 15 mob Sakņu Kroņā katrs saņem 5s mikroeksploziju = **~6 miljardi 5s window**.

---

# BEZĒNU ZAGLIS

## 5.1. Build A — *Divu Ēnu Solis* (Teleport-Backstab)

**Setup:** 4/6 setu, Melnais Kronis Uber, keystone *Divi Ēnu Klonis*.

**Key stats:**
- Veiklība: 1 400 → mult ×2.40
- Dagger, AS 1.7/sec
- Crit Chance: 65%
- Crit Damage: 240% + 50% GA → mult ×3.60
- Additive: +200% → ×3.00
- Backstab bonuss: ×2.00 (Ass Duncis Aspekts)
- Vulnerable: ×1.80
- Aspects: ×2.80 (Ass Duncis, Klona Ceļš, Vēju Solis)

**Main hit — Duncis LMB no aizmugures (garantēts crit stealth + backstab):**
```
Hit crit = 2 400 × 0.90 × 2.00 (backstab) × 2.40 × 3.00 × 3.60 × 1.80 × 1.35 × 2.80
        = 340 500

Avg per hit = 0.65 × 340 500 + 0.35 × 94 583 = 221 325 + 33 104 = 254 429
LMB DPS = 254 429 × 1.7 = 432 530
```

**Klona bonuss (setu 4-piece, klons ilgums 5s uz Ēnu Solis + keystone):**
- Klons veic 1 automātisku sitienu per Ēnu Solis
- Ēnu Solis 2 charges (setu 2-piece) → 2 klones sitieni per cikls
- Ēnu Solis cooldown 8s → 2 charges dod 4 klonu sitienus per 8s = 0.5 klona hits/sec
- Klona hit crit = 340 500 × 0.7 (klona penalty) = 238 350
- Klona DPS = 238 350 × 0.5 = 119 175

**Kaklaslazds (100 Ēna ult, dubultkritisks garantēts):**
- Bāze = 2 400 × 3.00 = 7 200
- Hit crit = 7 200 × 2.40 × 3.00 × 3.60 (base crit) × 2.00 (Kaklaslazds dubultā) × 1.80 × 1.35 × 2.80
       = 3 173 000 = **3.17 miljoni per hit**
- Uz cooldown ~30s → 105 000 DPS

**DPS breakdown:**
| Avots | DPS |
|---|---|
| LMB Duncis (garantēts crit stealth) | 432 530 |
| Klona sitieni | 119 175 |
| Kaklaslazds burst | 105 000 |
| Zaļču Signāls buff | 40 000 |
| Ēnu Solis atgriešanas cikls | 25 000 |
| **Kopā** | **~721 700 DPS** |

**Kill times:**
- Elite 10M: 14 sec
- 100M: 2 min 19 sec
- 500M Uber: 11 min 33 sec

**Peak burst:** Kaklaslazds + Nolādētais Asinsakmens double = ~10 M vienā hit. Ar Zaļču Signāls sync 8 Zaļči backstab par 1 M katrs = **20-30 miljardi 3-sekunžu combo**.

---

## 5.2. Build B — *Nakts Melnās Slēpes* (Permanent Invis)

**Setup:** 4/6 setu, Nolādētais Asinsakmens Uber Gem, keystone *Melnās Slēpes Kronis*.

**Key stats:**
- Veiklība: 1 300 → mult ×2.30
- Dagger, AS 1.6/sec
- Crit Chance: N/A — visi hiti crit (keystone + stealth)
- Crit Damage: 250% + 60% GA → mult ×4.00, ×2 no Nolādētais Asinsakmens = **×8.00**
- Additive: +200% → ×3.00
- Backstab: ×2.00
- Vulnerable: ×1.80 (setu 4-piece uzliek pastāvīgi neredzamā)
- Aspects: ×2.90 (Melnās Slēpes, Neredzamā Krunkas, Ēnas Sirds Karogs)

**Main hit — LMB pastāvīgā stealth:**
```
Hit (100% crit) = 2 400 × 0.90 × 2.00 (backstab) × 2.30 × 3.00 × 8.00 × 1.80 × 1.35 × 2.90
                = 830 700

LMB DPS = 830 700 × 1.6 = 1 329 120
```

**Bet:** setu 2-piece + keystone Ēnu regen management → tu esi neredzams tikai ~85% laika (uzkrāj Ēna caur backstab kļūst pastāvīgi cīņā).

**Weighted LMB DPS (85% stealth 100% crit + 15% cīņā ar crit ~65%):**
- 85% × 1 329 120 = 1 129 752
- 15% × 830 700 (regular crit) = 124 605
- **Vid LMB DPS = 1 254 357**

**Kaklaslazds:**
- Ar 8.0 crit mult + Kaklaslazds dubultā = ×16 crit mult efektīvs
- Peak crit ~7 M
- Cooldown 30s → 233 000 DPS

**DPS breakdown:**
| Avots | DPS |
|---|---|
| LMB (85% stealth 100% crit) | 1 254 357 |
| Kaklaslazds ult | 233 000 |
| Ēnu Solis reset chain | 40 000 |
| Zaļči minjoni | 25 000 |
| **Kopā (single target)** | **~1 552 000 DPS** |

**Kill times:**
- Elite 10M: 6 sec
- 100M: 1 min 4 sec
- 500M Uber: **5 min 22 sec** (single target king)

**Peak burst:** Nolādētais Asinsakmens + Kaklaslazds + Zaļču Signāls sync = **~50 miljardi 5s window**.

**Sensitivity:** ja Ēna izplēn (stealth downtime aug) → DPS krīt strauji. 50% stealth uptime = DPS uz pusi (750K).

---

## 5.3. Build C — *Bezēnu Karaļvalsts* (Zaļču Armija)

**Setup:** 4/6 setu, Melnais Kronis Uber, keystone *Bezēnu Karaļvalsts*.

**Key stats:**
- Veiklība: 1 200 → mult ×2.20
- Crit Chance: 55%
- Crit Damage: 220% + 40% GA → mult ×3.08
- Additive: +170% → ×2.70
- Minjonu Bojājums: +150% → ×2.50 pastāvīgi
- Aspects: ×2.60

**Zaļču DPS (kodolā):**
- 12 Zaļči (max ar Paragon)
- Bāzes Zaļča dmg: 450 (higher jo backstab-fokusēts)
- AS: 1.5/sec
- Bonusi (keystone + aspects): ×3.20
- **Per Zaļča DPS: 450 × 3.20 × 1.5 = 2 160**
- Kopā Zaļču DPS: 12 × 2 160 = **25 920**

**Setu 6-piece proc (kad Zaļču Signāls beidzas, 5s pēc — visi Zaļči sit 300% dmg reizē):**
- 12 × 2 160 × 3.00 = 77 760 burst
- Cikls 30s → 2 592 DPS papildu

**LMB Duncis:**
- Hit crit = 2 400 × 0.90 × 2.00 × 2.20 × 2.70 × 3.08 × 1.75 × 1.35 × 2.60
- = 250 800
- Avg = 155 800
- LMB DPS = 233 700

**DPS breakdown:**
| Avots | DPS |
|---|---|
| Zaļču armija baseline | 25 920 |
| Setu 6-piece proc | 2 592 |
| LMB Duncis | 233 700 |
| Ēnu Solis chain | 30 000 |
| Naktsāda sync ar Zaļču Signāls | 60 000 |
| Kaklaslazds | 80 000 |
| **Kopā** | **~432 000 DPS** |

**Kill times:**
- Elite 10M: 23 sec
- 100M: 3 min 51 sec
- 500M Uber: 19 min 17 sec

**Peak burst:** Zaļču Signāls end (setu 6) sync ar Nolādētais Asinsakmens = ~2 miljardi burst.

---

# VIENTUĻAIS SNIEGURGS

## 6.1. Build A — *Ledus Kroņa Nāve* (Freeze Nuke)

**Setup:** 4/6 setu, Melnais Kronis Uber, keystone *Sasala Kronis Ass*.

**Key stats:**
- Prāts: 1 400 → mult ×2.40
- Wand, AS 1.3/sec
- Crit Chance: 60%
- Crit Damage: 240% + 50% GA → mult ×3.60
- Frost Damage additive: +200% → ×3.00
- Frost Reizinātājs: ×1.40 (Kroņa Sirds)
- Vulnerable: ×1.75
- Aspects: ×2.70

**Main hit — Ledus Šauts (piercing):**
```
Base = 2 200 × 1.00 = 2 200
Hit crit = 2 200 × 2.40 × 3.00 × 3.60 × 1.75 × 1.40 × 2.70
        = 226 800

Avg per hit = 0.60 × 226 800 + 0.40 × 63 000 = 136 080 + 25 200 = 161 280
LMB DPS = 161 280 × 1.3 = 209 664
```

Piercing efektīvs multiplier grupā: 5 mērķi × dmg = ×5 DPS grupā.

**Sasalis Kaps (40 Sala, aoe freeze):**
- Bāze = 2 200 × 1.50 (skill%) = 3 300
- Hit crit = 3 300 × visi mults = 340 200
- AoE 10m radius, ~8 mērķi = 2.7 M per cast
- Cooldown ~12s → 227 000 DPS grupā

**Ledus Lauskas (80 Sala nuke, 20m līnijā):**
- Bāze = 2 200 × 4.00 = 8 800
- Hit crit = 8 800 × 2.40 × 3.00 × 3.60 × 1.75 × 2.00 (freeze bonuss setu 4-piece) × 1.40 × 2.70
- = 1 815 000 per single mērķi
- Kanālē 3s, cooldown 15s → 121 000 DPS

**DPS breakdown (grupa):**
| Avots | DPS |
|---|---|
| Ledus Šauts piercing | 209 664 (single), ×5 grupā |
| Sasalis Kaps AoE | 227 000 (grupa) |
| Ledus Lauskas | 121 000 |
| Zilspāri (5 aktīvi, spell power 2×) | 40 000 |
| Torņa Redzējums buff (aktīvs 10s / 60s = 17% uptime, ×2 dmg) | +50 000 avg |
| **Kopā (single)** | **~430 000 DPS** |
| **Kopā (grupa)** | **~1 500 000 DPS** |

**Kill times (single):**
- Elite 10M: 23 sec
- 100M: 3 min 52 sec
- 500M Uber: 19 min 20 sec

**Peak burst:** Ledus Lauskas + freeze all + Torņa Redzējums = **~15 miljardi 5s window**.

---

## 6.2. Build B — *Skeletu Karaļvalsts* (Necromancer)

**Setup:** 4/6 setu, Sirds no Kalna Uber, keystone *Skeletu Karaļvalsts* (10 skeleti).

**Key stats:**
- Prāts: 1 200 → mult ×2.20
- Crit Chance: 45%
- Crit Damage: 210% → mult ×2.60
- Additive: +170% → ×2.70
- Minjonu Bojājums: +150% → ×2.50 pastāvīgi
- Frost Reizinātājs (setu 4-piece skeleti frost): ×1.30
- Aspects: ×2.50

**Skeletu DPS:**
- 10 skeleti (max ar keystone), ilgums 60s
- Bāzes skeleta dmg: 550 per swing (frost proc), AS 1.2/sec
- Bonusi: minjonu Karogs +50%, setu 4-piece Frost proc +30%, aspects +80% = ×3.60
- **Per skelet DPS: 550 × 3.60 × 1.2 = 2 376**
- Kopā skeletu DPS: 10 × 2 376 = **23 760**

**Setu 6-piece proc (skelet dies → mikroeksplozija 100% weap dmg):**
- Bāze eksplozija = 2 200 dmg × cycle mult ×50 = 110 000 per die
- Skeleta die rate: ~1/5s (60s life, refresh) = 22 000 DPS from explosions

**Ledus Šauts LMB:**
- Hit crit = 2 200 × 2.20 × 2.70 × 2.60 × 1.75 × 1.35 × 2.50
- = 100 700
- Avg = 62 000
- LMB DPS = 80 600

**DPS breakdown:**
| Avots | DPS |
|---|---|
| Skeletu armija (10) | 23 760 |
| Skeletu eksplozijas (die) | 22 000 |
| LMB Ledus Šauts | 80 600 |
| Sasalis Kaps AoE + freeze | 120 000 |
| Ledus Lauskas | 90 000 |
| Zilspāri (papildu ceļ vēl skeletus) | 35 000 |
| **Kopā** | **~371 000 DPS** |

**Kill times:**
- Elite 10M: 27 sec
- 100M: 4 min 30 sec
- 500M Uber: 22 min 30 sec (AFK style, drošs)

**Peak burst:** 10 skeleti visi mirst vienā sekundē + auto-summon = 220 000 burst instant.

---

## 6.3. Build C — *Torņa Redzējuma Kronis* (Permanent Vision)

**Setup:** 6/6 setu (kritiski), Nolādētais Asinsakmens Uber Gem, keystone *Ledus Torņa Karogs*.

**Key stats:**
- Prāts: 1 400 → mult ×2.40
- AS: 1.3/sec
- Crit Chance: 60%
- Crit Damage: 250% + 60% GA → mult ×4.00 × 2 (Nolādētais) = ×8.00
- Frost additive: +220% → ×3.20
- Frost Reizinātājs: ×1.50 (setu 4-piece Torņa Redzējums 60% slow → ×1.30 dmg + Kroņa Sirds)
- Aspects: ×3.00 (Torņa Karogs, Vientuļā Sirds, Ledus Kroņa Kronis)
- Vulnerable: ×1.75

**Main hit — Ledus Šauts (pastāvīgā Torņa Redzējums = ×2 dmg + crit freezes):**
```
Base = 2 200 × 1.00 × 2.00 (Torņa Redzējums) = 4 400
Hit crit = 4 400 × 2.40 × 3.20 × 8.00 × 1.75 × 1.50 × 3.00
        = 1 065 000

Avg per hit (60% crit, but crits freeze → next hit against freeze +50%):
= 0.60 × 1 065 000 + 0.40 × 133 125 = 639 000 + 53 250 = 692 250
LMB DPS = 692 250 × 1.3 = 900 000
```

**Sasalis Kaps + Ledus Lauskas ar Torņa Redzējums buff:**
- Sasalis Kaps damage ×2 no ult buff = 500 000 per cast, cooldown 12s = 42 000 DPS grupā
- Ledus Lauskas ×2 buff = 3.6 M per kanāl, cooldown 15s = 240 000 DPS

**DPS breakdown:**
| Avots | DPS |
|---|---|
| LMB Ledus Šauts (pastāvīgā buff) | 900 000 |
| Sasalis Kaps AoE ar buff | 42 000 |
| Ledus Lauskas ar buff | 240 000 |
| Zilspāri (buff 2×) | 55 000 |
| Chain freeze detonācijas | 60 000 |
| Aspekti overlap | −65 000 |
| **Kopā** | **~1 232 000 DPS** |

**Kill times:**
- Elite 10M: 8 sec
- 100M: 1 min 21 sec
- 500M Uber: 6 min 46 sec (top-tier)

**Peak burst:** Torņa Redzējums 15s window ar Ledus Lauskas dubultā + freeze chain = **~30 miljardi burst**.

**Sensitivity:** Sala management ir viss — ja Sala krīt, Torņa Redzējums beidzas → DPS krīt no 1.2M uz 400K (Sniegurgs Build A līmenis).

---

# Kopsavilkuma tabula — visi 18 buildi

| Klase              | Build                        | Setup | Sustained DPS | Peak Burst    | Kill 500M Uber |
|--------------------|------------------------------|-------|---------------|---------------|----------------|
| Bruņinieks         | A. Falangas Tank             | 4/6+1U | 193 000       | 400 M         | 43 min         |
| Bruņinieks         | B. Heal-Tank                 | 6/6+2U | 145 000       | 1 500 M       | 57 min         |
| Bruņinieks         | C. Zvana Uguns Hibrīds       | 4/6+1U | 288 000       | 50 000 M      | 29 min         |
| Berserker          | A. Dual-Wield DPS            | 4/6+1U | 386 000       | 3 750 M       | 22 min         |
| Berserker          | B. Rage-Sustain              | 6/6+1U | 327 000       | 650 M         | 25 min         |
| Berserker          | C. Armijas Kliedzējs         | 4/6+1U | 200 000       | 500 M         | 42 min         |
| Alkas Kungs        | A. Projectile Spam           | 4/6+1U | 692 000       | 5 000 M       | 12 min         |
| Alkas Kungs        | B. Golemu Kalts              | 4/6+1U | 383 000       | 400 M         | 22 min         |
| Alkas Kungs        | C. Ekonomika Sausā Sirds     | 4/6+1U | 631 000       | 1 000 M       | 13 min         |
| Mežonis Kungs      | A. DoT Spam (single)         | 4/6+1U | 200 000       | 5 000 M grupa | 42 min         |
| Mežonis Kungs      | B. Beast Melee               | 4/6+1U | 340 000       | 1 800 M       | 25 min         |
| Mežonis Kungs      | C. Zone Control (single)     | 6/6+1U | 200 000       | 6 000 M grupa | 42 min         |
| Bezēnu Zaglis      | A. Teleport-Backstab         | 4/6+1U | 722 000       | 20-30 G       | 12 min         |
| Bezēnu Zaglis      | B. Permanent Invis           | 4/6+1U | **1 552 000** | **50 G**      | **5.4 min**    |
| Bezēnu Zaglis      | C. Zaļču Armija              | 4/6+1U | 432 000       | 2 000 M       | 19 min         |
| Vientuļais Sniegurgs | A. Freeze Nuke (single)    | 4/6+1U | 430 000       | 15 G          | 19 min         |
| Vientuļais Sniegurgs | B. Skeletu Karaļvalsts     | 4/6+1U | 371 000       | 220 M         | 23 min         |
| Vientuļais Sniegurgs | C. Torņa Redzējums         | 6/6+1U | **1 232 000** | **30 G**      | **6.8 min**    |

Legenda: **M** = miljons, **G** = miljards.

---

## Novērojumi

**Top 3 single-target DPS builds (Uber Boss fokuss):**
1. Bezēnu Zaglis Permanent Invis (1.55 M DPS, 5.4 min Uber)
2. Vientuļais Sniegurgs Torņa Redzējums (1.23 M DPS, 6.8 min Uber)
3. Alkas Kungs Projectile Spam (692 K DPS, 12 min Uber)

**Top 3 group/mob clear:**
1. Vientuļais Sniegurgs Freeze Nuke (1.5 M grupā)
2. Mežonis Zone Control (700 K grupā)
3. Alkas Kungs Projectile Spam (mult per mob)

**Top 3 tanky (survivability first):**
1. Bruņinieks Heal-Tank (nemirst pat pret Uber)
2. Berserker Rage-Sustain
3. Bruņinieks Falangas Tank

**Peak burst (broken build potential):**
1. Bezēnu Zaglis Permanent Invis — 50 G
2. Vientuļais Sniegurgs Torņa Redzējums — 30 G
3. Bezēnu Zaglis Teleport-Backstab — 20-30 G

**Balansa notes:**
- Bezēnu Zaglis un Sniegurgs Torņa ir "top DPS" — tas ir sagaidīts, glass cannon builds vienmēr tur.
- Tanks (Bruņinieks Heal-Tank, Falangas Tank) ir ~15-30% DPS vietā, apmaiņā pret 3× survivability.
- Armijas builds (Berserker C, Zaglis C, Sniegurgs B, Alkas B) ir stabili vidus, AFK-friendly.
- Peak burst starp buildiem svārstās 3× no zemākā (Falangas Tank 400 M) līdz augstākajam (Zaglis Invis 50 G) — tas ir apzināts diapazons.

---

*Skaitļi ir sākuma balansa piedāvājums. Reālā balansa tuning notiek pēc playtesting.*
