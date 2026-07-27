# Damage Math — Visi 9 Buildi T80 Paragon 200

*Kompakts aprēķinu doc visiem 9 buildiem (3 klases × 3 varianti). Level 80, Paragon 200, 4/6 setu daļas, 1 Uber Mistiskais.*

Pilna damage pipeline formula (multiplicative D4-style):

```
FINAL_DMG =
    Base_Skill × (1 + Additive) × Attribute_Mult
  × Crit_Mult × Vulnerable_Mult
  × Physical_Reizinātājs × Aspect_Multipliers
```

- **Weapon base:** 2400 (iP 925 Sena)
- **Attribute formula:** 1 + (Primary − 10) × 0.001
- **Attack Speed base:** 1.0 (2h), 1.4 (dual), 1.5 (wand)

---

# PIRMAIS KUNGS

## 1.1. Build A — *Meistars pāri Visiem* (Balanced Command)

**Key stats:** Str 1200 → ×2.19, Crit 55% × Crit Dmg 240% × Reizinātājs GA 40% → mult ×3.36, Additive 180% → ×2.80, Physical R ×1.30, Vuln ×1.75, Aspects ×2.60.

**LMB (Kunga Sitiens, 120% weap) crit vs Vulnerable elite:**
```
Base = 2400 × 1.20 = 2880
Crit = 2880 × 2.19 × 2.80 × 3.36 × 1.75 × 1.30 × 2.60 = 218 000
Avg = 0.55 × 218 000 + 0.45 × 64 880 = 148 100
LMB DPS = 148 100 × 1.15 = 170 315
```

**Minjonu DPS (26 minjoni ar aurām):**
- Vidēji per minjons: 350 × 2.85 (aurās) × 1 = 998
- Kopā: 26 × 998 = **25 948**

**DPS breakdown:**
| Avots | DPS |
|---|---|
| LMB | 170 315 |
| Master's Push (Q) | 45 000 |
| Signāles Zvans (E) | 15 000 |
| Kunga Zvana ult | 22 000 |
| Minjonu armija (26) | 25 948 |
| **Kopā** | **~278 000** |

**Kill times:** Elite 10M: 36s · 100M: 6:00 · 500M Uber: 29:58.
**Peak burst:** ~800M (Kunga Zvans 8s + Master's Call sync).

---

## 1.2. Build B — *Sardzes Zvērests* (Ultimate Tank)

**Key stats:** Str 1100 → ×2.09, Crit 45%, Crit Dmg 210% → ×2.65, Additive 150% → ×2.50, Aspects ×2.30.

**LMB crit:** = 2880 × 2.09 × 2.50 × 2.65 × 1.75 × 1.30 × 2.30 = **137 000**. Avg = 89 000. **LMB DPS = 102 350.**

**DPS breakdown:**
| Avots | DPS |
|---|---|
| LMB | 102 350 |
| Master's Push (Q) | 32 000 |
| Minjoni (30 max ar aurām) | 32 000 |
| Aura procs | 10 000 |
| **Kopā** | **~176 000** |

**Kill times:** Elite 10M: 57s · 100M: 9:28 · 500M Uber: 47:20.
**Peak burst:** ~250M (Kunga Zvans + all minjoni imunī 3s window).
**Survivability:** ekstrēma (HP 55K + minjonu shared dmg).

---

## 1.3. Build C — *Kunga Impērija* (35+ Armija)

**Key stats:** Str 1050 → ×2.04, Crit 40%, Crit Dmg 200% → ×2.40, Additive 140% → ×2.40, Aspects ×2.40.

**LMB crit:** = 2880 × 2.04 × 2.40 × 2.40 × 1.75 × 1.30 × 2.40 = **116 500**. Avg = 71 300. **LMB DPS = 82 000.**

**Minjonu DPS (38 minjoni):**
- Vidēji per minjons: 350 × 2.75 = 963
- Kopā: 38 × 963 = **36 594**
- Ar setu 6-piece +50% grupā 30+ minjoniem = **54 891**

**DPS breakdown:**
| Avots | DPS |
|---|---|
| LMB | 82 000 |
| Master's Push (Q) | 25 000 |
| Minjonu armija (38) | 54 891 |
| Dominance Wave debuff | 15 000 |
| **Kopā** | **~177 000** |

**Kill times:** Elite 10M: 57s · 100M: 9:24 · 500M Uber: 47:00.
**Peak burst:** ~1.2G (Kunga Zvans → 38 minjoni max buffed 8s).

---

# OTRS KUNGS

## 2.1. Build A — *Nordbergas Vaids Impērija* (Hybrid Balance)

**Key stats:** Str 1000 + Prāts 800 → ×2.79 combined mult, AS 1.15/sec, Crit 55%, Crit Dmg 240% + 40% GA → ×3.36, Additive 200% → ×3.00, Physical R ×1.35, Shadow R ×1.30, Vuln ×1.75, Aspects ×2.80.

**LMB (Impēriskais Sitiens, 110% weap + combo proc):**
```
Base = 2400 × 1.10 = 2640
Crit = 2640 × 2.79 × 3.00 × 3.36 × 1.75 × 1.35 × 1.30 × 2.80 = 258 000
Avg = 158 000
LMB DPS = 181 700
```

**DPS breakdown:**
| Avots | DPS |
|---|---|
| LMB combo | 181 700 |
| Impēriskā Cirte (Q) | 55 000 |
| Shadow Bolt (E) | 40 000 |
| Impērija Vaidēs ult | 35 000 |
| Sūkšņi + Zilspāri (22 minjoni) | 45 000 |
| **Kopā** | **~356 700** |

**Kill times:** Elite 10M: 28s · 100M: 4:41 · 500M Uber: 23:24.
**Peak burst:** ~2G (Impērija Vaidēs 15s + combo procs).

---

## 2.2. Build B — *Melnās Zibens Ķēde* (Chain Lightning Nuke)

**Key stats:** Prāts 1300 → ×2.29, Crit 60%, Crit Dmg 250% + 50% GA → ×3.75, Additive 220% → ×3.20, Shadow R ×1.40, Lightning R ×1.35, Vuln ×1.80, Aspects ×2.90.

**Melnās Zibenis (Q, 130% spell dmg, chains 20 mērķi ar keystone):**
```
Base = 2400 × 1.30 = 3120
Per hit crit = 3120 × 2.29 × 3.20 × 3.75 × 1.80 × 1.40 × 1.35 × 2.90 = 260 500
Per hit avg = 180 500

Chain 20 hops (max) × 180 500 avg = 3 610 000 per Melnās Zibenis cast
Cooldown 6s → 601 667 DPS grupā (mob density)
Single target (1 mērķi, no chain): 180 500 × 1.15 = 207 575 DPS
```

**LMB Impēriskais Sitiens:** ~110 000 DPS

**DPS breakdown:**
| Avots | DPS |
|---|---|
| Melnās Zibenis chain | 207 575 (single) / 601 667 (grupa) |
| LMB | 110 000 |
| Zilspāri (15 minjoni) | 45 000 |
| Ults | 30 000 |
| **Kopā (single)** | **~392 000** |
| **Kopā (grupa)** | **~786 000** |

**Kill times (single):** Elite 10M: 25s · 100M: 4:15 · 500M Uber: 21:15.
**Peak burst:** ~5G (Chain lightning setu 6-piece explosion in grupā).

---

## 2.3. Build C — *Impēriskā Karaļvalsts* (Permanent Empress)

**Key stats:** Prāts 1400 → ×2.39, AS 1.4/sec (peldošā), Crit 60%, Crit Dmg 250% + 50% GA → ×3.75, Additive 210% → ×3.10, ×3 dmg peldošā formā (keystone), Aspects ×2.70.

**LMB Impēriskais Sitiens pastāvīgā Impērija Vaidēs formā (triple dmg):**
```
Base = 2400 × 1.10 × 3.00 (peldošā keystone) = 7920
Crit = 7920 × 2.39 × 3.10 × 3.75 × 1.75 × 1.35 × 2.70 = 707 000
Avg = 432 000
LMB DPS = 604 800
```

**DPS breakdown:**
| Avots | DPS |
|---|---|
| LMB (pastāvīgā peldošā) | 604 800 |
| Shadow Bolt (2×) | 90 000 |
| Melnās Zibenis (2×) | 130 000 |
| Zilspāri (18 minjoni) | 40 000 |
| **Kopā** | **~865 000** |

**Kill times:** Elite 10M: 12s · 100M: 1:56 · 500M Uber: 9:38.
**Peak burst:** ~30G (Reality Rift sync + peldošā triple).
**Vājāki:** ja Impēriskā Dusma pazūd (no adds killu pieplūduma), DPS krīt uz ~200K.

---

# VECAIS BURVIS

## 3.1. Build A — *Realitātes Karaļvalsts* (Permanent Time Slow God)

**Key stats:** Prāts 1500 → ×2.49, AS 1.5/sec, Crit 60%, Crit Dmg 250% + 60% GA → ×4.00, ×2 no Nolādētais Asinsakmens = **×8.00 efektīvs**, Additive 220% → ×3.20, Spell R ×1.45, Vuln ×1.80, Aspects ×3.00 (Realitātes Kronis + Vientuļā Sirds + Torņa Karogs).

**LMB Arkānā Lauska (140% spell) pastāvīgā Reality Rift laikā (2× dmg buff):**
```
Base = 2400 × 1.40 × 2.00 (Reality Rift) = 6720
Crit = 6720 × 2.49 × 3.20 × 8.00 × 1.80 × 1.45 × 3.00 = 1 671 000
Avg = 1 030 000 (0.60 crit)
LMB DPS = 1 545 000
```

**Zilspāri (10 minjoni ar ×3.5 spell power):**
- Per Zilspāris DPS: 500 × 3.5 × 2.5 × 1.5 = 6 563
- Kopā: 10 × 6 563 = **65 630**

**DPS breakdown:**
| Avots | DPS |
|---|---|
| LMB (pastāvīgā Reality Rift buff) | 1 545 000 |
| Realitātes Plaisa AoE | 120 000 |
| Time Wave debuff dmg | 80 000 |
| Zilspāri | 65 630 |
| **Kopā** | **~1 810 000** |

**Kill times:** Elite 10M: 5s · 100M: 55s · **500M Uber: 4:36** (top-tier).
**Peak burst:** ~50G (Reality Rift + freeze detonation chain).

---

## 3.2. Build B — *Arkānā Sinerģija* (Zilspāri Storm)

**Key stats:** Prāts 1300 → ×2.29, Crit 50%, Crit Dmg 220% → ×2.75, Additive 180% → ×2.80, Minjonu Spell Power +200% → ×3.00, Aspects ×2.80 (Torņa Sirds + Arkānā Sinerģija + Sinerģija Kronis).

**Zilspāri DPS (10 minjoni max ar keystone, spell power ×5 keystone):**
- Per Zilspāris crit: 500 × 5.00 × 2.29 × 2.80 × 2.75 × 3.00 (setu 4-piece 3×hits) = **131 000** per attack cycle
- Per Zilspāris DPS (avg 0.5 crit): 131 000 × 0.5 × 1.4/sec = 91 700
- Wait that's way too high. Let me redo.

Actually Zilspāri hits are much smaller than main character. Let me use simpler math:
- Per Zilspāris base attack: 500 dmg
- With keystone ×5 spell power: 2500
- With Torņa Sirds ×4 (Aspect): actually keystone replaces this, so ×5
- Cast speed 1.5/sec ar bonusiem
- Kopā per Zilspāris: 2500 × 1.5 = 3 750 DPS
- Ar setu 4-piece (3× hits per attack): 3 750 × 3 = 11 250 DPS
- 10 Zilspāri kopā: **112 500 DPS**

**LMB Arkānā Lauska:**
- Base = 2400 × 1.40 = 3360
- Crit = 3360 × 2.29 × 2.80 × 2.75 × 1.75 × 1.35 × 2.80 = 250 000
- Avg = 145 000
- LMB DPS = 217 500

**DPS breakdown:**
| Avots | DPS |
|---|---|
| Zilspāri Storm (10 max) | 112 500 |
| LMB | 217 500 |
| Realitātes Plaisa | 60 000 |
| Chain lightning (setu 6 proc) | 40 000 |
| **Kopā** | **~430 000** |

**Kill times:** Elite 10M: 23s · 100M: 3:52 · 500M Uber: 19:22.
**Peak burst:** ~4G (Reality Rift + Zilspāri chain proc).

---

## 3.3. Build C — *Torņa Ass* (Double Cast God)

**Key stats:** Prāts 1400 → ×2.39, Crit 65%, Crit Dmg 250% + 60% GA → ×4.00, Additive 200% → ×3.00, Vuln ×1.80, Aspects ×2.90 (Divu Cast Kronis + Vecākais Kronis + Kā Māte Māca).

**Arkānā Lauska ar Torņa Ass double cast (katrs spell = 2 casts):**
```
Base per cast = 2400 × 1.40 = 3360
Crit per cast = 3360 × 2.39 × 3.00 × 4.00 × 1.80 × 1.35 × 2.90 = 302 000

Ar keystone: 2 casts per LMB, otrs vienmēr crit (setu 4-piece)
Combined per LMB combo = 302 000 (first, 0.65 crit avg = 196 300) + 302 000 (second, guaranteed crit)
= 196 300 + 302 000 = 498 300 per LMB

Ar 1.5/sec base AS un Arkānā Enerģija patēriņš 1.5× (setu 2-piece):
Effective LMB DPS = 498 300 × 1.5 = 747 450
```

**DPS breakdown:**
| Avots | DPS |
|---|---|
| LMB double cast | 747 450 |
| Realitātes Plaisa 3 charges | 150 000 |
| Time Wave 2 casts | 90 000 |
| Zilspāri (12 minjoni) | 55 000 |
| Reality Rift ult burst | 45 000 |
| **Kopā** | **~1 087 000** |

**Kill times:** Elite 10M: 9s · 100M: 1:32 · 500M Uber: 7:40.
**Peak burst:** ~20G (Arkāns Zvans buff + Reality Rift + double cast).

---

# Kopsavilkuma tabula — visi 9 buildi

| Klase          | Build                          | Sustained DPS | Peak Burst   | 500M Uber Kill |
|----------------|--------------------------------|---------------|--------------|----------------|
| Pirmais Kungs  | A. Meistars pāri Visiem        | 278 000       | 800 M        | 30 min         |
| Pirmais Kungs  | B. Sardzes Zvērests            | 176 000       | 250 M        | 47 min         |
| Pirmais Kungs  | C. Kunga Impērija              | 177 000       | 1.2 G        | 47 min         |
| Otrs Kungs     | A. Hybrid Balance              | 357 000       | 2 G          | 23 min         |
| Otrs Kungs     | B. Chain Lightning (single)    | 392 000       | 5 G (grupa)  | 21 min         |
| Otrs Kungs     | C. Permanent Empress           | **865 000**   | 30 G         | 9:38 min       |
| Vecais Burvis  | A. Realitātes Karaļvalsts      | **1 810 000** | **50 G**     | **4:36 min**   |
| Vecais Burvis  | B. Zilspāri Storm              | 430 000       | 4 G          | 19 min         |
| Vecais Burvis  | C. Torņa Ass Double Cast       | 1 087 000     | 20 G         | 7:40 min       |

Legenda: **M** = miljons, **G** = miljards.

## Novērojumi

**Top 3 single-target DPS builds:**
1. Vecais Burvis Realitātes Karaļvalsts (1.81M DPS, 4:36 Uber) — top tier
2. Vecais Burvis Torņa Ass (1.09M DPS, 7:40 Uber)
3. Otrs Kungs Permanent Empress (865K DPS, 9:38 Uber)

**Top group/mob clear:**
1. Otrs Kungs Chain Lightning (786K grupā)
2. Vecais Burvis Realitātes Karaļvalsts (piercing chain)
3. Vecais Burvis Zilspāri Storm

**Top tanky:**
1. Pirmais Kungs Sardzes Zvērests (176K DPS bet nemirsti)
2. Pirmais Kungs Kunga Impērija
3. Pirmais Kungs Meistars pāri Visiem

**Top peak burst (broken potential):**
1. Vecais Burvis Realitātes Karaļvalsts — 50G
2. Otrs Kungs Permanent Empress — 30G
3. Vecais Burvis Torņa Ass — 20G

**Balansa novērojumi:**
- Vecais Burvis ir top DPS klase (glass cannon, sagaidīts).
- Pirmais Kungs ir top tanky klase (armija + heals).
- Otrs Kungs ir vidus ar plašāko diapazonu (~360K līdz 865K).
- Peak burst diapazons 250M → 50G = 200× starp zemāko un augstāko (apzināts).

---

*Skaitļi ir sākuma balansa piedāvājums. Tuning notiek pēc playtesting.*
