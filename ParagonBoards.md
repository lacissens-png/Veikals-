# Paragon Boards — 3 Klasēm

*Pilna Paragon dēļu specifikācija — 21 unikāls dēlis (3 klases × 7 dēļi katrai).*

Pamata Paragon sistēma — skat. `GameDesignDocument.md` §9.

---

## 1. Kopīgais dizains

### 1.1. Dēļu struktūra

Katrs Paragon dēlis ir **21×21 režģis** ar ~80 mezgli, 1 leģendāro centrā, 1-2 Rūnu ligzdām un 2-4 vārtiem.

### 1.2. ASCII leģenda

```
★LEG★    Leģendārais mezgls (centrs, klases-specifisks)
▲R       Rets mezgls (ar stat slieksni)
■M       Maģisks mezgls
□        Parasts mezgls
◇G       Rūnu ligzda
▓G▓      Vārti (uz nākošo dēli)
```

### 1.3. Katras klases dēļu komplekts

| # | Dēlis                | Tips                        | Statuss         |
|---|----------------------|-----------------------------|-----------------|
| 1 | **Zvērests**         | Sākuma, klases identitāte   | Unikāls klasei  |
| 2 | Karagājienis         | Ofensīvs                    | Kopīgs, variē   |
| 3 | Tvirtīgums           | Defensīvs                   | Kopīgs, variē   |
| 4 | Ģenerāļu Sardzes     | Ģenerāļu trio bonusi        | Kopīgs, variē   |
| 5 | Nolādēts             | Riska/atlīdzības            | Kopīgs, variē   |
| 6 | Trono Māksla         | Utility/cooldown            | Kopīgs, variē   |
| 7 | **Melnais Kaisms**   | Endgame, klases-eksklīvs    | Unikāls klasei  |

---

## 2. Kopīgie dēļi ar klases variācijām

### 2.1. Karagājienis — Ofensīvs

**Leģendārais mezgls "Kara Cīņas Vaids" (klases variācijas):**

| Klase           | Efekts                                                                  |
|-----------------|-------------------------------------------------------------------------|
| Pirmais Kungs   | Pirmās 5s cīņā +100% dmg + Dominance uzkrāj 3× ātrāk                   |
| Otrs Kungs      | Pirmās 5s cīņā +100% dmg + Impēriskā Dusma uzkrāj 3× ātrāk             |
| Vecais Burvis   | Pirmās 5s cīņā +100% dmg + Arkānā Enerģija uzkrāj 3× ātrāk             |

**Retie mezgli:** Zvēras Cīņa (+15% vs Elite), Vardarbīga Roka (+10% Crit Chance), Ātrums (+8% AS). Rūnu ligzdas: 2. Vārti: 4.

---

### 2.2. Tvirtīgums — Defensīvs

**Leģendārais mezgls "Nepārvarams" (klases variācijas):**

| Klase           | Efekts                                                                     |
|-----------------|----------------------------------------------------------------------------|
| Pirmais Kungs   | 30% DR virs 80% HP + minjonu HP +25%                                       |
| Otrs Kungs      | 30% DR virs 80% HP + Impēriskā Dusma nekad neizzūd cīņā                    |
| Vecais Burvis   | 30% DR virs 80% HP + Ark. Enerģija nekad neizzūd cīņā                      |

Rūnu ligzdas: 2. Vārti: 4.

---

### 2.3. Barvedis — Minjonu

**Leģendārais mezgls "Karaļa Karogs" (klases variācijas):**

| Klase           | Efekts                                                                     |
|-----------------|----------------------------------------------------------------------------|
| Pirmais Kungs   | +25% ģenerāļu dmg + trio (Rose/Kalts/Velvet) aura bonuss vēl +15%       |
| Otrs Kungs      | +25% ģenerāļu dmg + Kaspars/Fay/Juno formation sinerģija vēl +20%          |
| Vecais Burvis   | +25% ģenerāļu dmg + Torins/Sasla/Vera trio spell power vēl +50%            |

Rūnu ligzdas: 2. Vārti: 4.

---

### 2.4. Nolādēts — Riska/atlīdzības

**Leģendārais mezgls "Sirds no Bezdibena" (klases variācijas):**

| Klase           | Efekts                                                                     |
|-----------------|----------------------------------------------------------------------------|
| Pirmais Kungs   | +50% dmg, bet Dominance patēriņš prasmēm 2×                                |
| Otrs Kungs      | +50% dmg + Impērija Vaidēs pastāvīgi, bet -25% max HP                     |
| Vecais Burvis   | +50% dmg + Reality Rift patēriņš -50%, bet -30% max HP                    |

Rūnu ligzdas: 1. Vārti: 3.

---

### 2.5. Trono Māksla — Utility

**Leģendārais mezgls "Kunga Zibens" (klases variācijas):**

| Klase           | Efekts                                                                    |
|-----------------|---------------------------------------------------------------------------|
| Pirmais Kungs   | -25% cooldown + Master's Call 2 charges vietā 1                          |
| Otrs Kungs      | -25% cooldown + Impērisks Karogs pastāvīgs (nevis 10s buff)              |
| Vecais Burvis   | -25% cooldown + Realitātes Plaisa 3 charges (nevis 2)                    |

Rūnu ligzdas: 2. Vārti: 4.

---

# 3. Pirmais Kungs — Pirmā Zīmoglaukuma Mantinieks

## 3.1. Zvērests dēlis — *Pirmā Zvērests*

```
        ▓GATE N▓
    ┌─────────────────┐
    │  ▲R    ■M    ▲R │
    │   \    │    /   │
    │    ◇G──■M──◇G   │
    │        │        │
▓W▓──■M──▲R──★LEG★──▲R──■M──▓E▓
    │        │        │
    │    ◇G──■M──◇G   │
    │   /    │    \   │
    │  ▲R   ■M    ▲R  │
    └─────────────────┘
        ▓GATE S▓
```
(4 Rūnu ligzdas — Pirmais Kungs "master of all" tēma.)

**Leģendārais mezgls: *Meistara Sirds***

> *Visi 4 minjonu tipi aurā ap tevi iegūst papildu +20% dmg. Kad grupā ir vismaz 5 minjoni no katra 4 tipiem, tu iegūsti +30% dmg pastāvīgi.*

**Reto mezglu piemēri:**

| Mezgls              | Bāzes efekts             | Slieksnis                          |
|---------------------|--------------------------|------------------------------------|
| **Kunga Karogs**    | Max minjoni +2           | Str 400+ = +3 vietā 2              |
| **Aura Māc**        | Dominance Aura ilgums +5s| Str 500+ = +10s papildu            |
| **Master's Kalts**  | Master's Call CD -20%    | Prāts 300+ = -10% papildu          |
| **Domination Sirds**| Dominance uzkrāj +15%    | Ļaunums 500+ = +10% papildu        |

Rūnu ligzdas: 4. Vārti: 4.

---

## 3.2. Melnais Kaisms dēlis — *Meistara Kronis*

```
             ▓GATE N▓
    ┌──────────────────────┐
    │  ▲R    ▲R    ▲R  ▲R  │
    │   \    │    /    /   │
    │    ■M──■M──■M       │
    │        │            │
    │    ◇G──★LEG★──◇G    │
    │        │            │
    │    ■M──■M──■M       │
    │   /   /  \  \       │
    │  ◇G  ▲R  ▲R  ◇G     │
    └──────────────────────┘
             ▓GATE S▓
```

**Leģendārais mezgls: *Meistars pāri Visiem Kronis***

> *Kad grupā ir aktīvi visi 4 minjonu tipi, tavs Kunga Zvans cooldown -50% un ilgums +5s. Kunga Zvana laikā, minjoni imunī pret bojājumu.*

**Reto mezglu piemēri:**

| Mezgls                    | Efekts                                          |
|---------------------------|-------------------------------------------------|
| **Ceturto Kroni Māc**     | 4-tipu bonuss +25% (kopā +50% ar Zvērests)      |
| **Dominance Kronis**      | Domination Wave dubultā radius                  |
| **Meistara Māc**          | Max minjoni +5 (kopā ar Setu C = 43)            |
| **Kunga Vaids**           | Kunga Zvana laikā, ienaidnieki bail +5s papildu|

Rūnu ligzdas: 4. Vārti: 2.

---

# 4. Otrs Kungs — Otrā Zīmoglaukuma Mantinieks

## 4.1. Zvērests dēlis — *Impēriskais Zvērests*

```
        ▓GATE N▓
    ┌─────────────────┐
    │  ◇G    ▲R    ◇G │
    │   \    │    /   │
    │    ■M──■M──■M   │
    │        │        │
▓W▓──■M──▲R──★LEG★──▲R──■M──▓E▓
    │        │        │
    │    ■M──■M──■M   │
    │   /    │    \   │
    │  ▲R   ■M    ▲R  │
    └─────────────────┘
        ▓GATE S▓
```

**Leģendārais mezgls: *Nordbergas Zvērests***

> *Sūkšņi + Zilspāri sinerģija: ja grupā ir vismaz 5 katra tipa, tu iegūsti +40% dmg un tavas melee prasmes uzreiz reset spell cooldowns (un otrādi).*

**Reto mezglu piemēri:**

| Mezgls              | Bāzes efekts             | Slieksnis                          |
|---------------------|--------------------------|------------------------------------|
| **Impērijas Karogs**| Sūkšņi + Zilspāri +15%   | Str 400+ = +10% papildu            |
| **Divu Rokas Māc**  | Melee → spell combo +25% | Prāts 500+ = +15% papildu          |
| **Zibens Kalts**    | Melnās Zibenis chain +2  | Ļaunums 500+ = +3 vietā 2          |
| **Dusmu Sirds**     | Imp. Dusma uzkrāj +20%   | Str 300+ = +10% papildu            |

Rūnu ligzdas: 2. Vārti: 4.

---

## 4.2. Melnais Kaisms dēlis — *Impēriskais Kronis*

```
             ▓GATE N▓
    ┌──────────────────────┐
    │  ▲R    ▲R  ▲R    ▲R  │
    │   \   /    \    /    │
    │    ■M──■M──■M       │
    │        │            │
    │    ◇G──★LEG★──◇G    │
    │        │            │
    │    ■M──■M──■M       │
    │   /   /  \  \       │
    │  ◇G  ▲R  ▲R  ◇G     │
    └──────────────────────┘
             ▓GATE S▓
```

**Leģendārais mezgls: *Impēriskā Karaļvalsts Kronis***

> *Impērija Vaidēs pastāvīgi aktīvs (Impēriskā Dusma patēriņš 3/sec kaujā). Peldošā formā, tu esi imunī pret melee attacks un tavas prasmes triple dmg.*

**Reto mezglu piemēri:**

| Mezgls                | Efekts                                          |
|-----------------------|-------------------------------------------------|
| **Impērijas Māc**     | Impēriskā Dusma patēriņš vēl -25%               |
| **Vaidēs Sirds**      | Peldošā formā, DR +30%                          |
| **Nordbergas Māc**    | Kad ienaidnieks mirst, +5 Imp. Dusma            |
| **Impērijas Kronis**  | Melnās Zibenis lec +5 mērķiem                   |

Rūnu ligzdas: 4. Vārti: 2.

---

# 5. Vecais Burvis — Torņa Zīmoglaukuma Mantinieks

## 5.1. Zvērests dēlis — *Torņa Zvērests*

```
        ▓GATE N▓
    ┌─────────────────┐
    │  ◇G    ▲R    ◇G │
    │   \    │    /   │
    │    ■M──▲R──■M   │
    │        │        │
▓W▓──■M──▲R──★LEG★──▲R──■M──▓E▓
    │        │        │
    │    ■M──▲R──■M   │
    │   /    │    \   │
    │  ◇G   ▲R    ◇G  │
    └─────────────────┘
        ▓GATE S▓
```
(4 Rūnu ligzdas — Vecais Burvis "control" tēma.)

**Leģendārais mezgls: *Torņa Vaids***

> *Arkānā Enerģija uzkrāj 25% ātrāk. Kad tavi spell hits ir crit, tavas nākošās 2s prasmes bezmaksas (Ark. Enerģija patēriņš 0). Zilspāri ap tevi spell power ×3.5 (nevis ×3).*

**Reto mezglu piemēri:**

| Mezgls               | Bāzes efekts               | Slieksnis                          |
|----------------------|----------------------------|------------------------------------|
| **Arkānā Kalts**     | Spell dmg +20%             | Prāts 500+ = +15% papildu          |
| **Realitātes Sirds** | Realitātes Plaisa CD -25%  | Prāts 400+ = -10% papildu          |
| **Torņa Karogs**     | Max Zilspāri +3            | Prāts 600+ = +5 vietā 3            |
| **Arkānā Sirds**     | Ark. Enerģija cap +25      | Prāts 700+ = cap +50               |

Rūnu ligzdas: 4. Vārti: 4.

---

## 5.2. Melnais Kaisms dēlis — *Realitātes Kronis*

```
             ▓GATE N▓
    ┌──────────────────────┐
    │  ◇G   ◇G    ▲R  ▲R   │
    │   \    \   /    /    │
    │    ■M──■M──■M       │
    │        │            │
    │    ▲R──★LEG★──▲R    │
    │        │            │
    │    ■M──■M──■M       │
    │   /   /  \  \       │
    │  ▲R  ▲R  ◇G  ◇G     │
    └──────────────────────┘
             ▓GATE S▓
```
(4 Rūnu ligzdas — max Rūnu būvējums.)

**Leģendārais mezgls: *Realitātes Karaļvalsts Kronis***

> *Reality Rift ilgums 25s vietā 15s + patēriņš -50% (efektīvs 0.75/sec). Reality Rift laikā tavi kritiski triple dmg (nevis 2×), un sasaldētie mērķi mirstot detonē par 500% spell dmg AoE.*

**Reto mezglu piemēri:**

| Mezgls                     | Efekts                                          |
|----------------------------|-------------------------------------------------|
| **Realitātes Māc**         | Reality Rift patēriņš vēl -25%                  |
| **Torņa Māc**              | Ark. Enerģija regen +30%                        |
| **Vecākais Māc**           | Kad crit, +10 Ark. Enerģija                     |
| **Chain Freeze Kronis**    | Sasala detonācija +100% AoE                     |

Rūnu ligzdas: 4. Vārti: 2.

---

# 6. Kopsavilkums — visi 21 dēļi

## 6.1. Klases-eksklīvie dēļi

| Klase          | Zvērests dēļa leģendārais | Melnais Kaisms leģendārais    |
|----------------|----------------------------|-------------------------------|
| Pirmais Kungs  | Meistara Sirds             | Meistars pāri Visiem Kronis   |
| Otrs Kungs     | Nordbergas Zvērests        | Impēriskā Karaļvalsts Kronis  |
| Vecais Burvis  | Torņa Vaids                | Realitātes Karaļvalsts Kronis |

## 6.2. Rūnu ligzdas kopīgi

| Klase          | Zvērests | Karagāj. | Tvirt. | Barvedis | Nolādēts | Trono | Melnais K. | Kopā  |
|----------------|----------|----------|--------|----------|----------|-------|------------|-------|
| Pirmais Kungs  | 4        | 2        | 2      | 2        | 1        | 2     | 4          | **17**|
| Otrs Kungs     | 2        | 2        | 2      | 2        | 1        | 2     | 4          | **15**|
| Vecais Burvis  | 4        | 2        | 2      | 2        | 1        | 2     | 4          | **17**|

## 6.3. Ieteicamā dēļu secība pa buildiem

Skat. attiecīgo BuildGuide-*.md failu §5.

---

## 6.4. Vidējais punktu iztērēšana pilnai 5-dēļu virknei

| Dēlis              | Ceļš uz izejas vārti (punkti) |
|--------------------|-------------------------------|
| Zvērests           | ~70                           |
| Karagājienis       | ~65                           |
| Tvirtīgums         | ~65                           |
| Barvedis           | ~70                           |
| Nolādēts           | ~55                           |
| Trono Māksla       | ~60                           |
| Melnais Kaisms     | ~60                           |

**Ar Paragon 350:** 5 dēļi × 65 avg = ~325 punkti, +25 punkti "spare" min-max izmaiņām.

---

*Skaitļi un layouts ir sākuma dizaina piedāvājums. Tuning notiek pēc playtesting.*
