# Build Guide — Otrs Kungs

*Otrā Zīmoglaukuma Mantinieks. Nordbergas Impērijas mantinieks. Trio: Kaspars + Fay + Juno.*

**Iedvesma:** The Second Overlord (Overlord II) — pirmā dēls, iekaroja Nordberga un Glorius Empire, hibrīds warrior-mage.

Ģenerāļu detaļas — skat. `Generals.md` §4.

---

## 1. Klases pamati

### 1.1. Identitāte

**Otrs Kungs ir hibrīds tuvcīņa + tumšas maģijas prasmes ar formation warfare trio.**

- **Melee combo + spell combo** — pārslēdzies starp abām stilēm.
- **Kaspars front (tank) + Fay ranged (magic archer) + Juno flank (assassin)** — formation warfare.
- **Impēriskā Dusma** aug iekarot ienaidniekus.
- **Vienā teikumā:** *"Mans tēvs bija pirmais. Es esmu labākais. Trīs zvēresti, viena impērija."*

### 1.2. Bāzes stati (Level 1)

| Atribūts       | Vērtība | Aug par līmeni |
|----------------|---------|----------------|
| Spēks          | 11      | +1.5           |
| Veiklība       | 8       | +1             |
| Izturība       | 10      | +1.5           |
| Prāts          | 12      | +2             |
| Ticība         | 6       | +0.5           |
| Ļaunums        | 12      | +2             |
| **Dzīvība**    | 195     | +32            |
| **Imp. Dusma** | 0/100   | +max 5/līmenis |
| **Kustība**    | Normāla | —              |

### 1.3. Signāles resurss — Impēriskā Dusma

- **Uzkrāj:** 10 pt / "gaišā" ordeņa ienaidnieka kill (paladīns, mūks, priesteris), 5 pt / cita, 25 pt / dungeon boss.
- **Nekad neizzūd** kaujas laikā (perfekts hibrīdai spēlei).
- **Izlādē:**
  - **40 pt** → **Melnās Zibenis** (Q, chain lightning + shadow, 5 mērķi).
  - **80 pt** → **Impērisks Zvans** (E, apkārtējiem ģenerāļiem +50% dmg 8s + tavas prasmes -30% CDR 8s).
  - **100 pt** → **Impērija Vaidēs** (100pt ult, 15s maģiska forma: tu peldēt, prasmes 2× dmg, spell cast time -50%, kustība +30%).

### 1.4. Ģenerāļu trio

Otrā Kunga ģenerāļi (skat. `Generals.md` §4):

| Ģenerālis  | Loma           | Bāzes efekts ap Otro Kungu (aura, 8m)                   |
|------------|----------------|---------------------------------------------------------|
| **Kaspars**| Heavy Tank     | +25% armor grupai + Impēriskais Sitiens dubultā stun    |
| **Fay**    | Ranged DPS/Mage| +25% party crit chance + Elfu Redzējums silver DPS boost|
| **Juno**   | Melee Assassin | +20% party MS + backstab dmg +30% aurā                  |

Trio kopīgā sinerģija: kad Kaspars tur agro, Fay + Juno flankē no aizmugures = ideāls formation warfare setup.

---

## 2. Prasmju koks

### Slānis 1 (level 1–20) — Pamati
- **Impēriskais Sitiens** (LMB, 1) — 110% weap dmg + 10% shadow proc, uzkrāj 3 Impēriskā Dusma
- **Impēriskā Cirte** (Q, 3) — melee cleave 90°, 150% weap dmg
- **Shadow Bolt** (E, 8) — projectile, 130% spell dmg, ranged 20m
- **Impērisks Karogs** (R, 12) — 10s pasīvs, tavi 3 ģenerāļi +25% dmg
- **Dusmu Sirds** (pasīvs, 5) — Impēriskā Dusma uzkrāj +20%
- **Formation Karogs** (pasīvs, 10) — kad Kaspars 15m priekšā un Fay/Juno flankē, tavas prasmes +15% dmg

### Slānis 2 (level 20–40) — Specializācija

**Warrior zars:**
- **Melnais Zobens** — pasīvs, melee prasmju dmg +30%
- **Karagājienis** — pasīvs, tuvcīņa dmg pret grupu +40%
- **Impēriskais Trakums** — pasīvs, kad HP zem 50%, melee AS +30%

**Mage zars:**
- **Shadow Kalts** — pasīvs, spell dmg +30%
- **Zibens Ceļš** — pasīvs, Melnās Zibenis lec 7 mērķiem (5 vietā)
- **Arkānais Prāts** — pasīvs, Prāts +100 pastāvīgi

**Formation zars:**
- **Kaspars Karogs** — pasīvs, Kaspars +40% aggro magnetism + Impēriskais Vairogs +25% absorb
- **Fay Karogs** — pasīvs, Fay bultas piercing (chain 3 mērķi)
- **Juno Karogs** — pasīvs, Juno backstab dmg +50% + Bleed ilgums +2s

**Hybrid zars:**
- **Divu Rokas** — pasīvs, kad melee hit, nākošais spell dubultā crit chance
- **Impēriskais Elks** — pasīvs, ja izmanto gan melee gan spell 5s laikā, +50% dmg 5s
- **Zvēresta Sinerģija** — pasīvs, kad 3 ģenerāļi aktīvi, +15% dmg

### Slānis 3 (level 40–60) — Meistrības
- **Nordbergas Vaids** — pasīvs, tavi hiti pret elite +40% dmg
- **Impērijas Kronis** — pasīvs, Impēriskā Dusma uzkrāj 2× ātrāk pret bosiem
- **Šķelmju Vaids** — pasīvs, kad ienaidnieks mirst tavā prasmē, tavas nākošās 2s prasmes bezmaksas

### Slānis 4 (level 60–80) — Keystone (1 no 3)

| Keystone            | Efekts                                                                                              |
|---------------------|-----------------------------------------------------------------------------------------------------|
| **Nordbergas Vaids Ass** | Kad Kaspars, Fay, Juno visi aktīvi (nav down), tu iegūsti pastāvīgu +50% dmg buff              |
| **Melnās Zibens Kronis** | Melnās Zibenis lec bezgalīgi (līdz nav vairāk mērķu vai 15 lēcieni); katrs lēciens -10% dmg    |
| **Impēriskā Karaļvalsts** | Impērija Vaidēs pastāvīgi aktīvs (Impēriskā Dusma patēriņš 3/sec), tavas prasmes vienmēr 2× dmg |

---

## 3. Trīs setu buildi

### 3.1. Build A — *Nordbergas Vaids Impērija* (Hybrid Formation)

> Klasiskais Otrs Kungs. Kaspars tur agro, Fay skaida no distances, Juno slīdē no aizmugures, tu combo tikai kopā.

**Keystone:** *Nordbergas Vaids Ass*.

**Setu bonusi:**
- **2 daļas:** Trio aura bonuss vēl +15%
- **4 daļas:** Kad melee hit un spell hit vienlaicīgi (0.5s laikā), +100% dmg 3s
- **6 daļas:** Impērisks Karogs pastāvīgi aktīvs

**Prioritie stati:**
- Physical Dmg % > Shadow Dmg % > Ģenerāļu Bojājums > Crit Dmg

**Signāles Aspekti:**
- *Divu Rokas Kronis* — melee → spell combo dubultā dmg (nevis 1.5×)
- *Formation Sirds* — kad visi 3 ģenerāļi aktīvi, +25% dmg
- *Zibens Ceļš Karogs* — Shadow Bolt piercing 3 mērķi

**Ideālais Uber Mistiskais:**
- **Melnais Kronis** — prasmes +2, viss stiprāks

**Cīņas plūsma:**
1. Grupā 3 ģenerāļi (Kaspars front, Fay 15m ranged, Juno flank).
2. Iebrauc, Impēriskais Sitiens melee combo.
3. Shadow Bolt spell no distances.
4. Combo buff (setu 4-piece) aktīvs pastāvīgi.
5. Impērija Vaidēs bosos par lielo burst.

**Best against:** balansētas boss cīņas ar adds.
**Vājākais pret:** ļoti šauri koridori, kur Fay nevar novietoties.

---

### 3.2. Build B — *Melnās Zibens Ķēde* (Chain Lightning Nuke)

> Melnās Zibenis lec pa visu grupu. Fay chains kā zīme. Boss ar adds = insta-clear.

**Keystone:** *Melnās Zibens Kronis*.

**Setu bonusi:**
- **2 daļas:** Melnās Zibenis dmg +50%
- **4 daļas:** Katrs Zibens lēciens uzliek 1s freeze
- **6 daļas:** Kad Zibens lec pēdējais reize, tas eksplodē par 500% weap dmg AoE

**Prioritie stati:**
- Shadow Dmg > Lightning Dmg > Crit Chance > CDR

**Signāles Aspekti:**
- *Zibens Ceļš Kronis* — max lēcieni +5 (kopā 20)
- *Sasala Karogs* — freeze ilgums +2s
- *Impēriskā Sirds* — Melnās Zibens cooldown -30%

**Ideālais Uber Mistiskais:**
- **Nolādētais Asinsakmens** (Uber Gem, Ēnu slot) — dubultkritis uz Zibens

**Cīņas plūsma:**
1. Grupā 3 ģenerāļi (Fay chain-fokusa gear).
2. Iebrauc mob grupā (15+ ienaidnieki ideāls).
3. Melnās Zibenis → lec bezgalīgi caur visiem.
4. Fay Elfu Bulta Kaskade piercing sync.
5. Katrs lēciens freezes + damages.
6. Bosos: pievieno Impēriskais Sitiens combo.

**Best against:** mob density, dungeon clear, elite grupas.
**Vājākais pret:** vientuļš boss (Zibens tikai 1 mērķim = mazāks dmg).

---

### 3.3. Build C — *Impēriskā Karaļvalsts* (Permanent Empress Form)

> Tu esi peldošs, mūžīgi maģiskā formā. Ģenerāļi apstāj bosu, tu peldi virsū un iznīcini.

**Keystone:** *Impēriskā Karaļvalsts*.

**Setu bonusi:**
- **2 daļas:** Impērija Vaidēs patēriņš -50% (1.5/sec)
- **4 daļas:** Peldošā formā, tu esi imunī pret melee attacks
- **6 daļas:** Peldošā formā, tavas prasmes triple dmg (nevis 2×)

**Prioritie stati:**
- Spell Dmg > Impēriskā Dusma Regen > Crit Dmg Reizinātājs > Movement Speed

**Signāles Aspekti:**
- *Impērijas Karogs* — Impērija Vaidēs patēriņš vēl -25%
- *Peldošā Sirds* — peldošā formā, DR +30%
- *Impēriskā Māte* — kad Impēriskā Dusma max, Impērija Vaidēs auto-triger bezmaksas

**Ideālais Uber Mistiskais:**
- **Nolādētais Asinsakmens** + **Sirds no Kalna** (double Uber setup)

**Cīņas plūsma:**
1. Sāc cīņu, uzkrāj Impēriskā Dusma pirmās 30s.
2. Kaspars front, Fay + Juno flank uz galveno mērķi.
3. Impērija Vaidēs → peldošā formā.
4. Pastāvīgs 3× dmg, imunī melee, +30% MS.
5. Cikls nekad neizzūd, ja tu turpini uzkrāj Impērisko Dusmu (bosi + elite kills).

**Best against:** open world farm, Murgu ar boss + adds sadalījumu.
**Vājākais pret:** situācijas ar zemu Impēriskā Dusma pieplūduma (piem., boss race bez adds).

---

## 4. Level progression 1 → 80

| Levels | Reģions            | Fokuss                                                                 |
|--------|--------------------|------------------------------------------------------------------------|
| 1–8    | Nordberga           | Impēriskais Sitiens + Impēriskā Cirte, Kaspars atslēdz                |
| 8–15   | Nordberga dziļāk    | Shadow Bolt, Juno atslēdz, first boss (Solariuss)                     |
| 15–25  | 2. reģions         | Fay atslēdz (visi 3 ģenerāļi complete)                                 |
| 25–35  | 3. reģions         | Impērisks Karogs, slāņa 2 (formation zars)                             |
| 35–50  | 4. reģions         | Slāņa 3 pasīvās                                                        |
| 50–65  | Endgame priekšskaitis | Paragon 50; Bond 3-5                                                |
| 65–75  | Reģions grinds     | Setu 4-piece; Bond 5-7                                                 |
| 75–80  | Fināls             | Uber chase; Bond 7-9                                                   |

---

## 5. Paragon 80 → 350

### 5.1. Build A (Formation Hybrid)

1. **Zvērests** — sākuma dēlis
2. **Karagājienis** (0°) — hybrid dmg
3. **Ģenerāļu Sardzes** (90°) — trio buffi
4. **Trono Māksla** (180°) — CDR
5. **Melnais Kaisms** — leg mezgls "Nordbergas Vaids"

Rūnas: *Kritušā Vaids*, *Slēgtais Vārds*

### 5.2. Build B (Chain Lightning)

1. **Zvērests**
2. **Karagājienis** (0°) — spell dmg
3. **Karagājienis 2** (90°)
4. **Nolādēts** (180°)
5. **Melnais Kaisms**

Rūnas: *Zibens Solis*, *Zvana Skaņa*

### 5.3. Build C (Permanent Empress)

1. **Zvērests**
2. **Karagājienis** (0°)
3. **Trono Māksla** (90°) — resurs + CDR
4. **Nolādēts** (180°)
5. **Melnais Kaisms**

Rūnas: *Nolādētais Sirdsdedzis*, *Vainaga Māc*

---

## 6. BiS ekipējums pa buildiem

### Build A (Formation Hybrid)
- **Ierocis:** Sena ar Physical + Shadow Dmg + Crit
- **Ķivere:** **Melnais Kronis**
- **Amulets:** Hybrid Damage GA
- **Kaspars gear:** Impērijas Kronis Uber
- **Fay gear:** Elfu Karalienes Bulta Uber
- **Juno gear:** Sniedznieces Duncis Uber
- **Gemi:** Rubīns (weapons), Topāzs (jewellery), Dimants (armor)

### Build B (Chain Lightning)
- **Ierocis:** Sena ar Shadow Dmg + Lightning Dmg
- **Ķivere:** Setu daļa
- **Ēnu slot:** **Nolādētais Asinsakmens**
- **Fay gear:** Elfu Karalienes Bulta Uber (chain sync)
- **Gemi:** Topāzs (weapons), Safīrs (jewellery), Rubīns (armor)

### Build C (Empress Form)
- **Ierocis:** Sena ar Spell Dmg + Impēriskā Dusma Regen
- **Ķivere:** Setu daļa
- **Amulets:** **Sirds no Kalna**
- **Kaspars gear:** Impērijas Kronis Uber (tur agro kamēr tu peldi)
- **Gemi:** Ametists (weapons), Topāzs (jewellery), Dimants (armor)

---

## 7. Endgame chase

1. Paragon 80 → 200
2. Setu 6 daļas — Solariuss boss farm
3. Rūnu upgrade
4. Ģenerāļu Bond farmings — visi 3 uz Bond 10
5. Ģenerāļu Uber Mistiskie chase (Impērijas Kronis + Elfu Karalienes Bulta + Sniedznieces Duncis)
6. Paragon 200 → 350

---

## 8. Kurš build ir tev?

| Ja tev patīk...                       | Izvēlies                       |
|---------------------------------------|--------------------------------|
| Balansēts, formation warfare          | **A — Formation Hybrid**       |
| Chain lightning caur grupām           | **B — Chain Lightning**        |
| Peldoša maģiska forma pastāvīgi       | **C — Empress Form**           |
| Vieglākais                            | **A**                          |
| Visspēkīgākais burst                  | **B**                          |
| Sarežģītākais bet iznīcinošs          | **C**                          |
