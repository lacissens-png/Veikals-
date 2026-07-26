# Paragon Boards — Visām 6 Klasēm

*Pilna Paragon dēļu specifikācija — 42 unikāli dēļi (6 klases × 7 dēļi katrai).*

Šis dokuments sīki apraksta katras klases Paragon dēļus: unikālos Zvērests un Melnais Kaisms dēļus, un piecu kopīgo dēļu (Karagājienis, Tvirtīgums, Barvedis, Nolādēts, Trono Māksla) klases-specifiskās variācijas.

Pamata Paragon sistēma — skat. `GameDesignDocument.md` §9.1.

---

## 1. Kopīgais dizains

### 1.1. Dēļu struktūra

Katrs Paragon dēlis ir **21×21 režģis** ar:
- **~80 mezgli** kopā (~50 parasti, ~20 maģiski, ~6 reti, 1 leģendārs, 1-2 rūnu ligzdas, 1-4 vārti)
- **1 leģendārais mezgls centrā** — klases-specifisks build-defining efekts
- **1-2 Rūnu ligzdas** — vietas, kur ievieto Rūnas
- **4 vārti malās** (Zvērests un lielie dēļi); **2 vārti** (Melnais Kaisms endgame)
- **Ceļš** cauri ~60-70 mezgliem, lai sasniegtu izejas vārtus

### 1.2. ASCII leģenda

```
★LEG★    Leģendārais mezgls (centrs, klases-specifisks)
▲R       Rets mezgls (ar stat slieksni)
■M       Maģisks mezgls
□        Parasts mezgls
◇G       Rūnu ligzda
▓G▓      Vārti (uz nākošo dēli)
+·×      Ceļš starp mezgliem
```

### 1.3. Katras klases dēļu komplekts

| # | Dēlis                | Tips                        | Statuss         |
|---|----------------------|-----------------------------|-----------------|
| 1 | **Zvērests**         | Sākuma, klases identitāte   | Unikāls klasei  |
| 2 | Karagājienis         | Ofensīvs                    | Kopīgs, variē   |
| 3 | Tvirtīgums           | Defensīvs                   | Kopīgs, variē   |
| 4 | Barvedis             | Minjonu bonusi              | Kopīgs, variē   |
| 5 | Nolādēts             | Riska/atlīdzības            | Kopīgs, variē   |
| 6 | Trono Māksla         | Utility/cooldown            | Kopīgs, variē   |
| 7 | **Melnais Kaisms**   | Endgame, klases-eksklīvs    | Unikāls klasei  |

---

## 2. Kopīgie dēļi ar klases variācijām

### 2.1. Karagājienis — Ofensīvs dēlis

**Standarta izkārtojums:**
```
        ▓GATE N▓
    ┌─────────────┐
    │ ▲R      ■M  │
    │             │
◇G──■M──┐   ┌──▲R─◇G
    │   │   │
▓W▓─□───□★LEG★□───□─▓E▓
    │   │   │
    │   └──■M──┘
    │             │
    │ ▲R      □   │
    └─────────────┘
        ▓GATE S▓
```

**Leģendārais mezgls (klases variācijas):**

| Klase              | Leģendārais mezgls "Kara Cīņas Vaids"                                     |
|--------------------|---------------------------------------------------------------------------|
| Bruņinieks         | Pirmās 5s cīņā +100% dmg + Grēksūdze uzkrāj 3× ātrāk                     |
| Berserker          | Pirmās 5s cīņā +150% dmg (vislielākais boost)                            |
| Alkas Kungs        | Pirmās 5s cīņā +100% dmg + savāktais zelts 2×                            |
| Mežonis Kungs      | Pirmās 5s cīņā +100% dmg + DoT stacki uzliek uzreiz max                  |
| Bezēnu Zaglis      | Pirmās 5s cīņā +100% dmg + pirmais hits garantēts crit                    |
| Vientuļais Sniegurgs | Pirmās 5s cīņā +100% dmg + visi uzbrukumi sasaldē                       |

**Retie mezgli (visām klasēm):**
- **Zvēras Cīņa** — +15% dmg vs Elite (slieksnis: Primary Attribute 500+ → +15% papildu).
- **Vardarbīga Roka** — +10% Crit Chance (slieksnis: Physical 300+ → +5% papildu).
- **Ātrums** — +8% Attack Speed (slieksnis: Vek 400+ → +4% papildu).

**Rūnu ligzdas:** 2 (viens ar pieejamu ceļu, otrs "pinch point" — vajag konkrētu ceļu).

**Vārti:** 4 (N/S/E/W).

---

### 2.2. Tvirtīgums — Defensīvs dēlis

**Leģendārais mezgls "Nepārvarams" (klases variācijas):**

| Klase              | Efekts                                                                       |
|--------------------|------------------------------------------------------------------------------|
| Bruņinieks         | 30% dmg reduction virs 80% HP + heal efficiency +50%                        |
| Berserker          | 30% dmg reduction virs 80% HP + Rage nekad nemazinās kaujā                  |
| Alkas Kungs        | 30% dmg reduction virs 80% HP + max Zelts +50 000                           |
| Mežonis Kungs      | 30% dmg reduction virs 80% HP + Poison Resistance +40%                      |
| Bezēnu Zaglis      | 30% dmg reduction virs 80% HP + Dodge Chance +15%                           |
| Vientuļais Sniegurgs | 30% dmg reduction virs 80% HP + Freeze Immunity                            |

**Retie mezgli:**
- **Bruņu Māc** — +10% Armor (slieksnis: Izturība 400+ → +5% papildu).
- **Sirds Māc** — +8% Max HP (slieksnis: Izturība 500+ → +4% papildu).
- **Barjeras Kalts** — +20% Barrier generation (slieksnis: Prāts 300+ → +10% papildu).

**Rūnu ligzdas:** 2. **Vārti:** 4.

---

### 2.3. Barvedis — Minjonu dēlis

**Leģendārais mezgls "Karaļa Karogs" (klases variācijas):**

| Klase              | Efekts                                                                   |
|--------------------|--------------------------------------------------------------------------|
| Bruņinieks         | +25% minjonu dmg + Sūkšņi ap tevi veido Falangu ar +30% vairoga bonuss  |
| Berserker          | +25% minjonu dmg + Sārtaji šauj 3 uguns bumbas per uzbrukums            |
| Alkas Kungs        | +25% minjonu dmg + visi minjoni savāc lūtu 2× ātrāk                     |
| Mežonis Kungs      | +25% minjonu dmg + Zaļči automātiski indeē visus mērķus                 |
| Bezēnu Zaglis      | +25% minjonu dmg + Zaļči ar tevi 8m rādiusā ir neredzami                |
| Vientuļais Sniegurgs | +25% minjonu dmg + Zilspāri ceļ skeletus no jebkuriem līķiem          |

**Retie mezgli:**
- **Minjonu Sirds** — +12% Minion HP (slieksnis: Ticība 300+ → +6% papildu).
- **Karaļa Loma** — +10% Minion Dmg (slieksnis: Ļaunums 500+ → +5% papildu).
- **Neraižīgums** — Minjoni +15% MS.

**Rūnu ligzdas:** 2. **Vārti:** 4.

---

### 2.4. Nolādēts — Riska/atlīdzības dēlis

**Leģendārais mezgls "Sirds no Bezdibena" (klases variācijas):**

| Klase              | Efekts                                                                     |
|--------------------|----------------------------------------------------------------------------|
| Bruņinieks         | +50% dmg, bet Grēksūdzes patēriņš prasmēm 2×                              |
| Berserker          | +50% dmg + Asins Trakums pastāvīgi, bet -50% def                           |
| Alkas Kungs        | +50% dmg, bet zelta patēriņš prasmēm 2×                                    |
| Mežonis Kungs      | +50% dmg, bet tu pats saņem DoT (2% max HP/sec cīņā)                       |
| Bezēnu Zaglis      | +50% dmg, bet tu vienmēr esi Vulnerable                                    |
| Vientuļais Sniegurgs | +50% dmg + sasaldē tevi 1s ik pēc 15s                                    |

**Retie mezgli:**
- **Overpower Kalts** — +15% Overpower dmg (slieksnis: 30 000 HP+ → +15% papildu).
- **Vulnerable Kalts** — +12% Vulnerable dmg (slieksnis: Primary 800+ → +6% papildu).
- **Melnais Sirds** — Ja HP zem 30%, dmg +25%.

**Rūnu ligzdas:** 1 (viena centrālā, spēcīgākā). **Vārti:** 3 (N/S/E).

---

### 2.5. Trono Māksla — Utility dēlis

**Leģendārais mezgls "Kunga Zibens" (klases variācijas):**

| Klase              | Efekts                                                                    |
|--------------------|---------------------------------------------------------------------------|
| Bruņinieks         | -25% cooldown visām prasmēm + Grēksūdzes Zvans cooldown -50%             |
| Berserker          | -25% cooldown + Kara Kliedziens 2s ilgāks                                |
| Alkas Kungs        | -25% cooldown + Zelta Lietus izmaksa -25%                                |
| Mežonis Kungs      | -25% cooldown + Vilka Āda 5s ilgāks                                       |
| Bezēnu Zaglis      | -25% cooldown + Ēnu Solis 3 charges vietā 2                              |
| Vientuļais Sniegurgs | -25% cooldown + Torņa Redzējums 5s ilgāks                              |

**Retie mezgli:**
- **Zibeņa Solis** — +10% MS (slieksnis: 25 000 HP+ → +5% papildu).
- **Resource Meistars** — +15% resurss regen (klases-atbilstošs).
- **Cooldown Kalts** — -8% CDR (slieksnis: Primary 600+ → -4% CDR).

**Rūnu ligzdas:** 2. **Vārti:** 4.

---

# 3. Melnais Bruņinieks — Kritušā Krusta Zvērests

## 3.1. Zvērests dēlis — *Kritušā Krusta*

**Sākuma dēlis, klases identitāte, +Str +Izturība fokuss.**

```
        ▓GATE N▓
    ┌─────────────────┐
    │  ▲R          ■M │
    │                 │
    │ ■M   ◇G     ▲R  │
    │  \  /    \  /   │
▓W▓──■M──□───□───□──■M──▓E▓
    │      \  |  /    │
    │  □────★LEG★────□│
    │      /  |  \    │
    │ ▲R  □   ◇G   ■M │
    │                 │
    │  ■M          ▲R │
    └─────────────────┘
        ▓GATE S▓
```

**Leģendārais mezgls: *Falangas Kaps***

> *Kad tu vari saglabāt vismaz 3 Sūkšņus 5m rādiusā 10 sekundes, viņi kļūst par Kritušā Krusta Zvērestiem: neatgriezenīgi, 5s neievainojami. Kad Kritušā Krusta Zvērests mirst, tu iegūsti 20 Grēksūdzi.*

**Reto mezglu piemēri:**

| Mezgls                | Bāzes efekts             | Slieksnis                            |
|-----------------------|--------------------------|--------------------------------------|
| **Sūkšņu Karogs**     | +20% Sūkšņu dmg          | Str 400+ = +10% papildu Sūkšņu dmg  |
| **Krusta Zvana Māc**  | Krusta Zvans cooldown -3s| Ticība 300+ = -2s papildu            |
| **Grēksūdzes Kalts**  | Grēksūdze uzkrāj +15%    | Izturība 400+ = +10% papildu         |
| **Bruņas Sirds**      | +12% Armor               | Str 500+ = +6% papildu               |

**Rūnu ligzdas:** 2 (viena ar 6 rare mezgli 4×4 rādiusā, otra ar 5 magic mezgli).

**Vārti:** 4 (N/S/E/W).

---

## 3.2. Melnais Kaisms dēlis — *Krustneša Kronis*

**Endgame dēlis, kampaņas fināla dropo pieejamību, ekstrēmi jaudīgs.**

```
             ▓GATE N▓
    ┌──────────────────────┐
    │  ▲R    ▲R      ▲R    │
    │   \    │       /     │
    │    ■M──■M──■M         │
    │        │              │
    │    ◇G──★LEG★──◇G      │
    │        │              │
    │    ■M──■M──■M         │
    │   /    │       \     │
    │  ▲R   ◇G       ▲R    │
    └──────────────────────┘
             ▓GATE S▓
```

**Leģendārais mezgls: *Krustnesis***

> *Kad tu izmanto Krusta Zvanu (Grēksūdzes Zvans vai Melnais Zvans), visi Sūkšņi 15m rādiusā 5 sekundes ir neatgriezenīgi un uzbrūk ar dubultu bojājumu. Šī prasme cool-down neieskaita Grēksūdzes Zvana cooldown, ja izmantoja Grēksūdzes Zvanu vismaz 8s atpakaļ.*

**Reto mezglu piemēri:**

| Mezgls                     | Efekts                                             |
|----------------------------|----------------------------------------------------|
| **Kritušā Ordeņa Karogs**  | +30% dmg pret ienaidniekiem tuvāk 3 minjoniem      |
| **Nemirstīgais Ordenis**   | Sūkšņi mirstot dod tev 5% max HP heal              |
| **Zvana Māc Kronis**       | Krusta Zvana bāzes bojājums +40%                   |
| **Krustneša Vairoga**      | Kad Sūkšņi neatgriezenīgi, tu +30% dmg reduction  |

**Rūnu ligzdas:** 3 (vienā vidū, divas malās).

**Vārti:** 2 (S/N).

---

## 3.3. Ieteicamā dēļu secība pa buildiem

Skat. `BuildGuide-MelnaisBrunininks.md` §5. Kopīgi:

| Build                    | Dēļu secība                                                    |
|--------------------------|----------------------------------------------------------------|
| A. Falangas Tank         | Zvērests → Barvedis (0°) → Tvirtīgums (90°) → Trono Māksla (0°) → Melnais Kaisms |
| B. Heal-Tank             | Zvērests → Nolādēts (90°) → Tvirtīgums (180°) → Trono Māksla (0°) → Barvedis (270°) → Melnais Kaisms |
| C. Zvana Uguns Hibrīds   | Zvērests → Karagājienis (0°) → Barvedis (90°) → Trono Māksla (0°) → Melnais Kaisms |

---

# 4. Berserkers — Arēnas Zvērests

## 4.1. Zvērests dēlis — *Arēnas Zvērests*

**Sākuma dēlis, +Vek +Ļaunums fokuss.**

```
        ▓GATE N▓
    ┌─────────────────┐
    │  ▲R    ■M    ▲R │
    │   \    │    /   │
    │    ■M──■M──■M   │
    │        │        │
▓W▓──◇G──■M──★LEG★──■M──◇G──▓E▓
    │        │        │
    │    ■M──■M──■M   │
    │   /    │    \   │
    │  ▲R   ■M    ▲R  │
    └─────────────────┘
        ▓GATE S▓
```

**Leģendārais mezgls: *Trakuma Zvērests***

> *Rage uzkrāj 30% ātrāk. Kad Rage sasniedz 100, tavs nākošais uzbrukums ir garantēts crit un uzkrāj vēl 50 Rage (max 150 uz šī cikls). Rage nekad neizzūd cīņas laikā.*

**Reto mezglu piemēri:**

| Mezgls                | Bāzes efekts                    | Slieksnis                             |
|-----------------------|---------------------------------|---------------------------------------|
| **Dubultā Zobena**    | +15% Dual-Wield dmg             | Vek 500+ = +10% papildu               |
| **Kliedziena Māc**    | Kara Kliedziens dmg +25%        | Ļaunums 400+ = +15% papildu           |
| **Rage Kalts**        | Rage uzkrāj +20%                | Vek 400+ = +10% papildu               |
| **Vēja Solis**        | +10% MS pastāvīgi               | Vek 300+ = +5% papildu                |

**Rūnu ligzdas:** 2. **Vārti:** 4.

---

## 4.2. Melnais Kaisms dēlis — *Nekad Vairs Mierā*

```
             ▓GATE N▓
    ┌──────────────────────┐
    │  ▲R  ▲R   ▲R    ▲R  │
    │    \  \   /    /    │
    │     ■M──■M──■M       │
    │         │            │
    │     ◇G──★LEG★──◇G    │
    │         │            │
    │     ■M──■M──■M       │
    │    /   /  \    \    │
    │  ◇G   ▲R  ▲R    ▲R  │
    └──────────────────────┘
             ▓GATE S▓
```

**Leģendārais mezgls: *Trakuma Kronis***

> *Asins Trakums cooldown -50%. Kad Asins Trakums beidzas, uzkrāj 75 Rage un iegūsti Barjeru = 30% max HP uz 8s.*

**Reto mezglu piemēri:**

| Mezgls                    | Efekts                                          |
|---------------------------|-------------------------------------------------|
| **Trakuma Cikls**         | Rage no visiem avotiem 2×                       |
| **Neapstādināmais**       | Asins Trakumā i-frames uz visām izvairīšanās    |
| **Kliedziena Kronis**     | Kara Kliedziens 3× radius                       |
| **Sarkano Smilšu Māc**    | Ja HP zem 30%, +60% dmg pastāvīgi               |

**Rūnu ligzdas:** 3. **Vārti:** 2.

---

## 4.3. Dēļu secība pa buildiem

Skat. `BuildGuide-Berserkers.md` §5.

| Build                    | Dēļu secība                                              |
|--------------------------|----------------------------------------------------------|
| A. Dual-Wield DPS        | Zvērests → Karagājienis (0°) → Karagājienis 2 (90°) → Nolādēts (180°) → Melnais Kaisms |
| B. Rage-Sustain          | Zvērests → Tvirtīgums (0°) → Nolādēts (90°) → Karagājienis (270°) → Melnais Kaisms |
| C. Armijas Kliedzējs     | Zvērests → Barvedis (0°) → Karagājienis (90°) → Trono Māksla (180°) → Melnais Kaisms |

---

# 5. Alkas Kungs — Zelta Zvērests

## 5.1. Zvērests dēlis — *Zelta Zvērests*

**Sākuma dēlis, +Prāts +Ļaunums fokuss.**

```
        ▓GATE N▓
    ┌─────────────────┐
    │  ▲R    ◇G    ▲R │
    │   \    │    /   │
    │    ■M──■M──■M   │
    │        │        │
▓W▓──■M──◇G──★LEG★──◇G──■M──▓E▓
    │        │        │
    │    ■M──■M──■M   │
    │   /    │    \   │
    │  ▲R   ◇G    ▲R  │
    └─────────────────┘
        ▓GATE S▓
```
(3 Rūnu ligzdas šim dēlim — Alkas Kungs get bonus slot.)

**Leģendārais mezgls: *Zelta Kalums***

> *10% no visiem savāktā zelta konvertējas tieši uz XP. +10 000 max Zelta rezerves. Kad tavs zelta bilance pieaug pār 50 000, tu iegūsti +25% dmg un +15% MS 30s.*

**Reto mezglu piemēri:**

| Mezgls              | Bāzes efekts                | Slieksnis                              |
|---------------------|-----------------------------|----------------------------------------|
| **Alkas Kalts**     | Zelta drop +20%             | Ļaunums 500+ = +10% papildu            |
| **Monētu Māc**      | Zelta Lietus izmaksa -25%   | Prāts 400+ = -15% papildu              |
| **Golemu Karogs**   | Konstrukti +30% HP          | Prāts 500+ = +15% papildu              |
| **Bagātā Sirds**    | Zelts uz zemes = +2% dmg    | 1000 zelta uz zemes = +5% papildu      |

**Rūnu ligzdas:** 3 (bonus). **Vārti:** 4.

---

## 5.2. Melnais Kaisms dēlis — *Zelta Karaļvalsts*

```
             ▓GATE N▓
    ┌──────────────────────┐
    │  ◇G    ▲R    ▲R  ◇G  │
    │   \    │    /   /    │
    │    ■M──■M──■M       │
    │        │            │
    │    ▲R──★LEG★──▲R    │
    │        │            │
    │    ■M──■M──■M       │
    │   /   /  \  \       │
    │  ▲R  ◇G  ◇G  ▲R    │
    └──────────────────────┘
             ▓GATE S▓
```
(4 Rūnu ligzdas — Alkas Kungs "collect everything" tēma.)

**Leģendārais mezgls: *Vecā Goldo Mantinieks***

> *Kad tavs zelta bilance ir virs 100 000, visas tavas prasmes ir bezmaksas (bez Zelta izmaksas). Kad esi zem 100 000, prasmju izmaksa aug par 20% par katru 10 000 trūkstošu.*

**Reto mezglu piemēri:**

| Mezgls                    | Efekts                                          |
|---------------------------|-------------------------------------------------|
| **Sausā Sirds Kronis**    | Sausā Sirds patēriņš -50%                       |
| **Zelta Zvana**           | Alkas Zvans 2× radius, dubultā uptime          |
| **Konstruktu Karaļvalsts**| Ķērājs Golems max +2                            |
| **Bagātā Kronis**         | Katrs 10 000 zelta = +2% dmg (max +50%)         |

**Rūnu ligzdas:** 4. **Vārti:** 2.

---

## 5.3. Dēļu secība pa buildiem

Skat. `BuildGuide-AlkasKungs.md` §5.

| Build                    | Dēļu secība                                          |
|--------------------------|------------------------------------------------------|
| A. Projectile Spam       | Zvērests → Karagājienis (0°) → Trono Māksla (90°) → Nolādēts (180°) → Melnais Kaisms |
| B. Golemu Kalts          | Zvērests → Barvedis (0°) → Barvedis 2 (90°) → Trono Māksla (270°) → Melnais Kaisms |
| C. Ekonomika Sausā Sirds | Zvērests → Karagājienis (0°) → Trono Māksla (90°) → Nolādēts (180°) → Melnais Kaisms |

---

# 6. Mežonis Kungs — Puves Zvērests

## 6.1. Zvērests dēlis — *Puves Zvērests*

**Sākuma dēlis, +Prāts +Ļaunums fokuss, DoT tēma.**

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
(4 Rūnu ligzdas — DoT-heavy fokusa.)

**Leģendārais mezgls: *Puvušā Vainaga***

> *Tavu DoT tick 25% ātrāk. Kad ienaidnieks mirst ar tavu DoT stack, tu uzkrāj 10 Sula un mērķa poison stacki spread uz 3 tuvākajiem ienaidniekiem 8m rādiusā.*

**Reto mezglu piemēri:**

| Mezgls                | Bāzes efekts                  | Slieksnis                              |
|-----------------------|-------------------------------|----------------------------------------|
| **Puves Kalts**       | DoT dmg +20%                  | Prāts 500+ = +10% papildu              |
| **Sakņu Kalts**       | Sakņu Slazds radius +2m       | Prāts 400+ = +1m papildu               |
| **Vilka Āda Māc**     | Vilka Āda ilgums +3s          | Ļaunums 500+ = +2s papildu             |
| **Puves Sirds**       | DoT stack cap 5 → 8           | Prāts 700+ = cap 10                    |

**Rūnu ligzdas:** 4. **Vārti:** 4.

---

## 6.2. Melnais Kaisms dēlis — *Elfu Karalienes Kronis*

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
    │  ◇G  ▲R  ▲R  ◇G    │
    └──────────────────────┘
             ▓GATE S▓
```

**Leģendārais mezgls: *Puve Nekad Nemirst Kronis***

> *Kad ienaidnieks mirst ar tavu DoT stack, DoT spread uz VISIEM ienaidniekiem 15m rādiusā (ne tikai 2 tuvākajiem). Puvusī Migla ilgums 15s vietā 8s.*

**Reto mezglu piemēri:**

| Mezgls                     | Efekts                                          |
|----------------------------|-------------------------------------------------|
| **Ninheras Ērkšķu Māc**    | Ienaidnieki tuvāk 10m pastāvīgi -25% MS         |
| **Elfu Karalienes Māc**    | Sakņu Slazds pastāvīgi aktīvs 5m ap tevi        |
| **Puvušā Meža Kronis**     | Vilka Āda + Puvusī Migla vienlaicīgi bez cd     |
| **Zaļču Karaļvalsts**      | Zaļču backstab spread DoT 4 mērķiem             |

**Rūnu ligzdas:** 4. **Vārti:** 2.

---

## 6.3. Dēļu secība pa buildiem

Skat. `BuildGuide-MezonisKungs.md` §5.

| Build                | Dēļu secība                                                  |
|----------------------|--------------------------------------------------------------|
| A. DoT Spam          | Zvērests → Karagājienis (0°) → Nolādēts (90°) → Trono Māksla (180°) → Melnais Kaisms |
| B. Beast Melee       | Zvērests → Karagājienis (0°) → Tvirtīgums (90°) → Trono Māksla (270°) → Melnais Kaisms |
| C. Zone Control      | Zvērests → Trono Māksla (0°) → Barvedis (90°) → Nolādēts (180°) → Melnais Kaisms |

---

# 7. Bezēnu Zaglis — Nakts Zvērests

## 7.1. Zvērests dēlis — *Nakts Zvērests*

**Sākuma dēlis, +Vek +Ļaunums fokuss.**

```
        ▓GATE N▓
    ┌─────────────────┐
    │  ▲R    ■M    ▲R │
    │   \    │    /   │
    │    ■M──▲R──■M   │
    │        │        │
▓W▓──◇G──■M──★LEG★──■M──◇G──▓E▓
    │        │        │
    │    ■M──▲R──■M   │
    │   /    │    \   │
    │  ▲R   ■M    ▲R  │
    └─────────────────┘
        ▓GATE S▓
```

**Leģendārais mezgls: *Ēnas Skūpsts***

> *Visi tavi backstab uzbrukumi +25% dmg. Ēna uzkrāj 2× ātrāk. Kad tu esi neredzams (Naktsāda vai Ēnu Solis), tavi hiti nespēj tikt atgrūsti — tu pabeidzi savu combo.*

**Reto mezglu piemēri:**

| Mezgls                | Bāzes efekts             | Slieksnis                              |
|-----------------------|--------------------------|----------------------------------------|
| **Duncis Māc**        | Duncis dmg +15%          | Vek 500+ = +10% papildu                |
| **Ēnu Solis Kalts**   | Ēnu Solis cooldown -20%  | Vek 400+ = -10% papildu                |
| **Backstab Kronis**   | Backstab crit chance +25%| Vek 600+ = +15% papildu                |
| **Ēnas Sirds**        | Ēna cap +25              | Ļaunums 500+ = cap +50                 |

**Rūnu ligzdas:** 2. **Vārti:** 4.

---

## 7.2. Melnais Kaisms dēlis — *Bezēnu Karaļvalsts Kronis*

```
             ▓GATE N▓
    ┌──────────────────────┐
    │  ◇G    ▲R    ▲R  ◇G  │
    │   \   /  \   /   /   │
    │    ■M──▲R──■M       │
    │        │            │
    │    ▲R──★LEG★──▲R    │
    │        │            │
    │    ■M──▲R──■M       │
    │   /   /  \  \       │
    │  ▲R  ◇G  ◇G  ▲R    │
    └──────────────────────┘
             ▓GATE S▓
```

**Leģendārais mezgls: *Divi Ēnu Klonis Ass***

> *Ēnu Solis atstāj **divus** klonus (nevis vienu). Klones dzīvo 10s (nevis 3s) un veic backstab uzbrukumus ar 100% no tava dmg (nevis 50%). Kad klons "mirst", tu iegūsti 30 Ēna.*

**Reto mezglu piemēri:**

| Mezgls                    | Efekts                                          |
|---------------------------|-------------------------------------------------|
| **Naktsāda Kronis**       | Naktsāda ilgums 8s vietā 3s                     |
| **Kaklaslazds Kronis**    | Kaklaslazds cooldown -50%                        |
| **Zaļču Signāls Māc**     | Zaļči invisibility 10s vietā 5s                  |
| **Nemirstīgs Ēna**        | Ēnu Solis 4 charges vietā 2                      |

**Rūnu ligzdas:** 4. **Vārti:** 2.

---

## 7.3. Dēļu secība pa buildiem

Skat. `BuildGuide-BezenuZaglis.md` §5.

| Build                    | Dēļu secība                                          |
|--------------------------|------------------------------------------------------|
| A. Teleport-Backstab     | Zvērests → Karagājienis (0°) → Trono Māksla (90°) → Nolādēts (180°) → Melnais Kaisms |
| B. Permanent Invis       | Zvērests → Karagājienis (0°) → Trono Māksla (90°) → Nolādēts (180°) → Melnais Kaisms |
| C. Zaļču Armija          | Zvērests → Barvedis (0°) → Karagājienis (90°) → Trono Māksla (270°) → Melnais Kaisms |

---

# 8. Vientuļais Sniegurgs — Ledus Zvērests

## 8.1. Zvērests dēlis — *Ledus Zvērests*

**Sākuma dēlis, +Prāts fokuss, Frost tēma.**

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
(4 Rūnu ligzdas — Sniegurgs "control" tēma prasa daudz rūnu.)

**Leģendārais mezgls: *Ledus Kroņa Vaids***

> *Frost Damage +40%. Sasaldētie mērķi paliek freeze +1s pēc tā, kad citādi tas beigtu (piem., no tava LMB piercing atbrīvo mērķi). Kad tavs Sasalis Kaps sasalda 5+ ienaidniekus, uzkrāj 20 Sala uzreiz.*

**Reto mezglu piemēri:**

| Mezgls                | Bāzes efekts               | Slieksnis                              |
|-----------------------|----------------------------|----------------------------------------|
| **Ledus Kalts**       | Frost dmg +15%             | Prāts 500+ = +10% papildu              |
| **Sasala Sirds**      | Freeze ilgums +30%         | Prāts 400+ = +15% papildu              |
| **Skeleta Karogs**    | Skeleta Signāls +2 sk.     | Ļaunums 500+ = +3 papildu              |
| **Sala Kalts**        | Sala regen +25%            | Prāts 600+ = +15% papildu              |

**Rūnu ligzdas:** 4. **Vārti:** 4.

---

## 8.2. Melnais Kaisms dēlis — *Torņa Kronis*

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
(4 Rūnu ligzdas + endgame slots — max Sniegurgs Rūnu būvējums.)

**Leģendārais mezgls: *Sasala Kronis Kronis***

> *Sasalis Kaps neuzsāk cooldown, ja iznīcina 5+ mērķus (tas ir, tu vari spam Sasalis Kaps grupā). Ledus Lauskas kanālēšanas laikā Sala neizzūd.*

**Reto mezglu piemēri:**

| Mezgls                     | Efekts                                          |
|----------------------------|-------------------------------------------------|
| **Torņa Redzējums Kronis** | Torņa Redzējums ilgums 20s + patēriņš -50%     |
| **Ledus Lauskas Māc**      | Ledus Lauskas cooldown 5s vietā 15s             |
| **Skeletu Karaļvalsts Māc**| Skeleta max +5 (līdz 15)                        |
| **Chain Freeze Kronis**    | Sasalis Kaps eksplozija detonē 3 tuvākos       |

**Rūnu ligzdas:** 4. **Vārti:** 2.

---

## 8.3. Dēļu secība pa buildiem

Skat. `BuildGuide-VientulaisSniegurgs.md` §5.

| Build                    | Dēļu secība                                          |
|--------------------------|------------------------------------------------------|
| A. Freeze Nuke           | Zvērests → Karagājienis (0°) → Trono Māksla (90°) → Nolādēts (180°) → Melnais Kaisms |
| B. Skeletu Karaļvalsts   | Zvērests → Barvedis (0°) → Barvedis 2 (90°) → Trono Māksla (270°) → Melnais Kaisms |
| C. Torņa Redzējums       | Zvērests → Karagājienis (0°) → Trono Māksla (90°) → Nolādēts (180°) → Melnais Kaisms |

---

# 9. Kopsavilkums — visi 42 dēļi

## 9.1. Klases-eksklīvie dēļi

| Klase              | Zvērests dēļa leģendārais | Melnais Kaisms leģendārais |
|--------------------|----------------------------|-----------------------------|
| Bruņinieks         | Falangas Kaps              | Krustnesis                  |
| Berserker          | Trakuma Zvērests           | Trakuma Kronis              |
| Alkas Kungs        | Zelta Kalums               | Vecā Goldo Mantinieks       |
| Mežonis Kungs      | Puvušā Vainaga             | Puve Nekad Nemirst Kronis   |
| Bezēnu Zaglis      | Ēnas Skūpsts               | Divi Ēnu Klonis Ass         |
| Vientuļais Sniegurgs | Ledus Kroņa Vaids        | Sasala Kronis Kronis        |

## 9.2. Rūnu ligzdas kopīgi

| Klase              | Zvērests | Karagāj. | Tvirt. | Barvedis | Nolādēts | Trono | Melnais K. | Kopā  |
|--------------------|----------|----------|--------|----------|----------|-------|------------|-------|
| Bruņinieks         | 2        | 2        | 2      | 2        | 1        | 2     | 3          | **14**|
| Berserker          | 2        | 2        | 2      | 2        | 1        | 2     | 3          | **14**|
| Alkas Kungs        | 3        | 2        | 2      | 2        | 1        | 2     | 4          | **16**|
| Mežonis Kungs      | 4        | 2        | 2      | 2        | 1        | 2     | 4          | **17**|
| Bezēnu Zaglis      | 2        | 2        | 2      | 2        | 1        | 2     | 4          | **15**|
| Vientuļais Sniegurgs | 4      | 2        | 2      | 2        | 1        | 2     | 4          | **17**|

Klases ar vairāk ligzdām (Sniegurgs, Mežonis) ir "Rūnu-heavy" — viņu buildi vairāk uzticas Rūnu bonusiem.

## 9.3. Vidējais punktu iztērēšana pilnai 5-dēļu virknei

| Dēlis              | Ceļš uz izejas vārti (punkti) |
|--------------------|-------------------------------|
| Zvērests           | ~70                           |
| Karagājienis       | ~65                           |
| Tvirtīgums         | ~65                           |
| Barvedis           | ~70                           |
| Nolādēts           | ~55 (mazāks)                  |
| Trono Māksla       | ~60                           |
| Melnais Kaisms     | ~60 (endgame)                 |

**Ar Paragon 350:** 5 dēļi × 65 avg = **~325 punkti**, plus **~25 punkti "spare"** min-max izmaiņām.

---

*Skaitļi un layouts ir sākuma dizaina piedāvājums. Reāla balansa un layout tuning notiek pēc playtesting.*
