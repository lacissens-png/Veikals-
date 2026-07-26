# Build Guide — Alkas Kungs

*Zelta Zvērests. Zelta Raktuvju mantinieks. Zelts ir asinis, zelts ir ierocis.*

---

## 1. Klases pamati

### 1.1. Identitāte

**Alkas Kungs ir hibrīds — sauc konstruktus, met zeltu kā projektiles, minjoni ap viņu ir loot magnēti.**

- **Zelts ir viss:** resurs prasmēm, ierocis (met kā projectile), buff (jo bagātāks, jo stiprāks).
- **Neveic afinitāti ar vienu minjonu tipu** — bet **visi tipi** ap viņu **savāc loot dubultā ātrumā** un **karo par bagātību**.
- **Sauc Zelta Golemus** — īslaicīgus konstruktus par zelta cenu.
- Cīņā vidēja distance — met monētas, sūta golemus, minjoni turpina griezt.

**Vienā teikumā:** *Katrs kritušais ienaidnieks maksā tev bagātību, kas atgriežas kā ierocis.*

### 1.2. Bāzes stati (Level 1)

| Atribūts       | Vērtība | Aug par līmeni |
|----------------|---------|----------------|
| Spēks          | 8       | +1             |
| Veiklība       | 9       | +1             |
| Izturība       | 10      | +1.5           |
| Prāts          | 11      | +1.5           |
| Ticība         | 6       | +0.5           |
| Ļaunums        | 11      | +1.5           |
| **Dzīvība**    | 200     | +35            |
| **Zelts (resource)** | 500 | +100/līmenis  |
| **Kustība**    | Normāla | —              |

### 1.3. Signāles resurss — Zelts

**Zelts ir gan valūta, gan resurss.** Atšķirībā no citām klasēm, tavs signāles resurss ir tas pats zelts, ko izmanto veikaliem un craftingam.

- **Uzkrāj** kā normālu loot no ienaidniekiem un dungeoniem.
- **Aug** par 30% no visa Alkas Kunga bāzē (klases pasīvais).
- **Izlādē:**
  - **200 zelta** → **Zelta Lietus** (5m radius, 12 monētu projectiles).
  - **1000 zelta** → **Ķērājs Golems** (20s summoned konstrukcija ar 200% weapon dmg).
  - **5000 zelta** → **Sausā Sirds** (60s buff: +30% zelta drop, +20% dmg, patērē 100 zelta/sec).

**Alkas Kungs paradox** — jo vairāk cietsirdīgs pret zelta patēriņu, jo stiprāks; bet arī jāatstāj daļu ekipējuma / gemu upgradiem.

### 1.4. Minjonu afinitāte

- Nav vienas afinitātes — visi 4 tipi ir tavi.
- Ap Alkas Kungu ~10m: visi minjoni **savāc lūtu automātiski** un **2× ātrāk**.
- Katrs ienaidnieks ap tevi tuvāk 10m, kurš nomirst, **iemet papildu 5% zelta**.
- Ja tavs Zelts >20000, visi minjoni ap tevi iegūst **+15% dmg (Zelta oreols)**.

---

## 2. Prasmju koks

### Slānis 1 (level 1–20) — Pamati

| Prasme            | Tips     | Level | Efekts                                                            |
|-------------------|----------|-------|-------------------------------------------------------------------|
| **Monētu Sitiens**| Pamata   | 1     | LMB, 100% weap dmg, dropo 3 monētas uz zemes                     |
| **Zelta Lietus**  | Aktīvs   | 3     | Q, 200 zelta izmaksa (skat. augšā)                                |
| **Rāceles Ķērājs**| Aktīvs   | 8     | E, sauc mazu konstruktu 10s (bez zelta cenas, 30s CD)             |
| **Alkas Zvans**   | Aktīvs   | 12    | R, visi ap 15m dropo 2× zeltu 15s                                 |
| **Zelta Skopums** | Pasīvs   | 5     | Zelta drop +30% pastāvīgi                                         |
| **Konstruktora Roka**| Pasīvs| 10    | Konstrukti dzīvo 25% ilgāk                                        |

### Slānis 2 (level 20–40) — Specializācija

**Konstruktu zars:**
- **Zelta Kalt** — pasīvs, konstrukts +50% dmg.
- **Divi Golemi** — pasīvs, Ķērājs Golems var būt 2 vienlaikus.
- **Metāla Skūpsts** — aktīvs, konstrukts eksplodē par 500% dmg savā nāves brīdī.

**Zelta-projekt zars:**
- **Monētu Vējš** — pasīvs, Zelta Lietus dubultā monētu skaits.
- **Ass Zelts** — pasīvs, Zelta Lietus +50% crit chance.
- **Bagātā Sirds** — pasīvs, jo vairāk zelta ap tevi (dropped), jo lielāks tavs dmg (+1% per 100 zelta uz zemes).

**Loot-fokusa zars:**
- **Vecā Ķēde** — pasīvs, visi tavi minjoni savāc loot 3× ātrāk.
- **Alkas Aura** — pasīvs, ienaidnieki 15m rādiusā dropo +50% zeltu.
- **Sudraba Uzoles** — pasīvs, tas pats attiecas uz materiāliem (Melnā Māldivara u.c.).

### Slānis 3 (level 40–60) — Meistrības

- **Zelta Sirds** — pasīvs, tavs max HP aug par 0.5% per 10000 zelta bilances (līdz 50%).
- **Vaideklis Zelts** — pasīvs, katrs uzbrukums pret ienaidnieku tuvāk 10m zeltiem uz zemes dubultkritis.
- **Kalta Bruņas** — pasīvs, katrs 1000 zelta patērēts prasmē dod +5% bruņu 10s.

### Slānis 4 (level 60–80) — Keystone (1 no 3)

| Keystone            | Efekts                                                                                        |
|---------------------|-----------------------------------------------------------------------------------------------|
| **Zelts Ir Asinis** | 20% no savāktā zelta konvertējas uz temporāro HP (līdz 2× max)                               |
| **Ķērāju Karaļvalsts**| 3 Ķērāju Golemi max, katrs +30% dmg, bet Zelta prasmes maksā 2×                             |
| **Sausā Sirds Kronis**| Sausā Sirds ir pastāvīgs (nepārtraukts patēriņš), bet visi tavi bonusi 2×                    |

---

## 3. Trīs setu buildi

### 3.1. Build A — *Zelta Monētu Karaļvalsts* (Projectile Spam)

> Monētu vējš. Zelta Lietus katras 4 sekundēs, ienaidnieki nedabū pat piecelties.

**Keystone:** *Zelts Ir Asinis*.

**Setu bonusi:**
- **2 daļas:** Zelta Lietus izmaksa -50%.
- **4 daļas:** Katra monēta ir garantēta crits.
- **6 daļas:** Zelta Lietus atslēdz burvju kaskādi — pēc katra ienaidnieka nāves, met vēl 1 papildu monētu tā vietā.

**Prioritie stati:**
- Crit Chance > Crit Dmg Reizinātājs > Zelta Uzkrāšana > Physical Dmg.

**Signāles Aspekti:**
- *Bagātā Sirds Aspekts* — Zelta Lietus monētas dubultā ranged.
- *Alkas Aura Aspekts* — Alkas Zvana buff dubultā ilgums.
- *Monētu Sirds* — Zelts pastāv uz zemes 2× ilgāk pirms tas izzūd.

**Ideālais Uber Mistiskais:**
- **Vecā Goldo Roka** (Alkas Kunga class Uber) — 30% zelta konvertējas Damage Reduction.

**Cīņas plūsma:**
1. Alkas Zvans uz grupu → +2× drop, tavs Zelts aug.
2. Zelta Lietus spam (dažas sekundes cooldown).
3. Ienaidnieki mirst, monētas iet uz zemes, tavs "Bagātā Sirds" bonus aug.
4. Ķērājs Golemi kā auto-attack backup.
5. Bosos — Sausā Sirds buff 60s + Zelta Lietus non-stop.

**Best against:** grupas, mob-density, farm.
**Vājākais pret:** vientuļš boss ar zemu adds.

---

### 3.2. Build B — *Vecā Goldo Aizsegs* (Golemu Kalts)

> Trīs Golemi + tu = klasiskais "Diablo 3 Necro" stila armija bez skeletu.

**Keystone:** *Ķērāju Karaļvalsts*.

**Setu bonusi:**
- **2 daļas:** Ķērājs Golems max +1 (kopā 3 ar keystone → 4).
- **4 daļas:** Katrs Golems +40% HP un dmg.
- **6 daļas:** Kad Golems nomirst, uzsauc otru automātiski bez cenas.

**Prioritie stati:**
- Minion Dmg % (attiecas uz konstruktiem) > Golemu HP > Cooldown Reduction > Physical Reizinātājs.

**Signāles Aspekti:**
- *Divi Golemi Aspekts* — Rāceles Ķērājs sauc 2 vienlaikus.
- *Golemu Karogs* — Golemi ap tevi +20% dmg.
- *Metāla Kāzas* — Golemi apstāv tevi kā vairogus (agro magnēts).

**Ideālais Uber Mistiskais:**
- **Sirds no Kalna** — nāve = otra chance (Golemi turpina cīņu).

**Cīņas plūsma:**
1. Sāc cīņu ar Rāceles Ķērājs + jaudīgu Ķērājs Golemu (1000 zelta).
2. Trīs Golemi apstāj bosu (viens neteikdams pieprasa agro).
3. Tu met Zelta Lietus periodiski (procs).
4. Alkas Zvans 15s intervālos — golemi arī iegūst +30% dmg no loot procs.
5. Golemi mirst → auto-summon, cikls turpinās.

**Best against:** boss cīņas, garas cīņas.
**Vājākais pret:** ātrs pack clear (Golemi lēnāki par tevi).

---

### 3.3. Build C — *Zelta Karalis Bez Trono* (Ekonomika + Sausā Sirds)

> Zelta plūsma = mūžīgs berserker režīms. Tu tērē tik ātri, cik iepelni.

**Keystone:** *Sausā Sirds Kronis*.

**Setu bonusi:**
- **2 daļas:** Sausā Sirds patēriņš -50% (50 zelta/sec).
- **4 daļas:** Sausā Sirds arī dod +30% AS un +30% MS.
- **6 daļas:** Kamēr Sausā Sirds aktīvs, visi tavi hiti dubultā dropo zeltu.

**Prioritie stati:**
- Zelta Drop % > Attack Speed > Crit Dmg Reizinātājs > Movement Speed.

**Signāles Aspekti:**
- *Sausās Sirds Aspekts* — patēriņš vēl -25%.
- *Zelta Sirdsdedzis* — pie 0 zelta, Sausā Sirds automātiski aktīvs 5s bezmaksas.
- *Alkas Zvana Kronis* — Alkas Zvans arī uzkrāj Zelta 500/sec 15s.

**Ideālais Uber Mistiskais:**
- **Nolādētais Asinsakmens** (Uber Gem) — dubultkritis + Sausā Sirds bonusi.

**Cīņas plūsma:**
1. Sāc ar 5000+ zelta rezerves.
2. Alkas Zvans → Sausā Sirds aktivē (patērē 50/sec = pastāvīgs).
3. LMB spam pastāvīgi (zelta drops feed back Sausā Sirds).
4. Zelta Lietus procs papildu dmg.
5. Ja Zelta rezerves plok — pauze, farm, restart.

**Best against:** open world nepārtraukta farm, Murgu ar high density.
**Vājākais pret:** boss cīņas bez zelta pieplūduma (Sausā Sirds izšķīst 30-40s laikā).

---

## 4. Level progression 1 → 80

| Levels | Reģions              | Fokuss                                                                 |
|--------|----------------------|------------------------------------------------------------------------|
| 1–8    | Zelta Raktuves       | Pamata Monētu Sitiens, pirmie Sūkšņi                                   |
| 8–15   | Zelta Raktuves dziļāk | Zelta Lietus, first boss (Vecais Goldo)                              |
| 15–25  | 2. reģions            | Otrs minjonu tips                                                     |
| 25–35  | 3. reģions            | Alkas Zvans, slāņa 2 specializācija                                   |
| 35–50  | 4.–5. reģions         | Slāņa 3 pasīvās; pirmie Reliktu; Kodekss                              |
| 50–65  | Endgame priekšskaitis | Paragon atslēdzas 50; Murgu T1–T20                                     |
| 65–75  | Reģions 6, Melnais Kalns | Setu 4-piece kombos; ekonomikas optimize                              |
| 75–80  | Fināls                | Uber chase                                                             |

---

## 5. Paragon 80 → 350

### 5.1. Build A (Projectile Spam)

1. **Zvērests** — Prāta + Ļaunuma.
2. **Karagājienis** (0°) — crit dmg, projectile speed.
3. **Trono Māksla** (90°) — CDR uz Zelta Lietus.
4. **Nolādēts** (180°) — power ar cenu.
5. **Melnais Kaisms** — leģendārais mezgls.

Rūnas: *Zvana Skaņa* (crit), *Slēgtais Vārds* (CDR).

### 5.2. Build B (Golemu Kalts)

1. **Zvērests**.
2. **Barvedis** (0°) — Golemi (skaitās kā minjoni).
3. **Barvedis 2** (90°) vai **Tvirtīgums**.
4. **Trono Māksla** (270°).
5. **Melnais Kaisms**.

Rūnas: *Asinssaite* (bonusi Barveža bojājumam).

### 5.3. Build C (Ekonomika)

1. **Zvērests**.
2. **Karagājienis** (0°).
3. **Trono Māksla** (90°) — CDR + resurss.
4. **Nolādēts** (180°) — Sausā Sirds cena piedodāma.
5. **Melnais Kaisms**.

Rūnas: *Nolādētais Sirdsdedzis*.

---

## 6. BiS ekipējums pa buildiem

### Build A (Projectile Spam)
- **Ierocis:** Sena ar Crit Dmg + Projectile Speed.
- **Ķivere:** Setu daļa.
- **Rotaslietas:** Amulets ar Crit Chance GA; gredzeni ar Crit Dmg Reizinātājs GA.
- **Gemi:** Smaragds visos.

### Build B (Golemu Kalts)
- **Ierocis:** **Vecā Goldo Roka** (Uber).
- **Ķivere:** Setu daļa.
- **Amulets:** Minjonu Bojājums Reizinātājs GA.
- **Gemi:** Rubīns (weapons), Smaragds (jewellery), Dimants (armor resists).

### Build C (Ekonomika)
- **Ierocis:** Sena ar Zelta Drop + AS.
- **Ķivere:** Setu daļa.
- **Ēnu slot:** **Nolādētais Asinsakmens**.
- **Gemi:** Rubīns (weapons), Topāzs (jewellery resource gen), Rubīns (armor HP).

---

## 7. Endgame chase

1. Paragon 80 → 200.
2. Setu 6 daļas — Goldo boss farm.
3. Rūnu upgrade Murgu T40+.
4. **Vecā Goldo Roka** Uber chase (visa Alkas Kunga identitāte).
5. Masterworking.
6. Paragon 200 → 350.

---

## 8. Kurš build ir tev?

| Ja tev patīk...                        | Izvēlies                          |
|----------------------------------------|-----------------------------------|
| Spam projectiles, ātrs cikls           | **A — Projectile Spam**           |
| Armija ap tevi, mierīga plūsma         | **B — Golemu Kalts**              |
| Ekonomikas optimizācija, resurs mgmt   | **C — Ekonomika**                 |
| Vieglākais                             | **A**                             |
| Sarežģītākais bet visunīcinošākais    | **C**                             |
