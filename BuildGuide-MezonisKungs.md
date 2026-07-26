# Build Guide — Mežonis Kungs

*Puves Zvērests. Puvušā Meža mantinieks. Puve nekad nemirst.*

---

## 1. Klases pamati

### 1.1. Identitāte

**Mežonis Kungs ir DoT (damage over time) specializēts, zvēru pārveidojums, area denial.**

- Nogalina nevis ar vienu sitienu, bet ar **indes migla**, kas nekad nebeidzas.
- Var **īslaicīgi pārvērsties par zvēru** (lāci, čūsku, vilku) — melee spike posmi.
- **Zaļči** ap viņu iegūst tavas indes procs.
- Cīņā **kontrolē lielas zonas** — sakņu slazdi, indes lauki, augu ierobežojumi.

**Vienā teikumā:** *Es esmu meža ēna, kas apstādina jūsu asinis un liek tām sapūt.*

### 1.2. Bāzes stati (Level 1)

| Atribūts       | Vērtība | Aug par līmeni |
|----------------|---------|----------------|
| Spēks          | 9       | +1             |
| Veiklība       | 10      | +1.5           |
| Izturība       | 11      | +1.5           |
| Prāts          | 13      | +2             |
| Ticība         | 7       | +1             |
| Ļaunums        | 10      | +1.5           |
| **Dzīvība**    | 200     | +35            |
| **Sula (resource)** | 0/100 | +max 5/līmenis |
| **Kustība**    | Normāla | —              |

### 1.3. Signāles resurss — Sula

- **Uzkrāj** kad tavas DoT prasmes tick (1 pt / tick / mērķis) vai kad **kaut kas mirst** tavas indes ietekmē (5 pt / kill).
- **Izzūd** 2 pt/sec ārpus kaujas.
- **Izlādē:**
  - **40 pt** → **Vilka Āda** (10s transformācija par zvēru: +50% MS, +80% melee dmg, dubultā HP regen).
  - **80 pt** → **Puvusī Migla** (20m rādiuss, 8s DoT + ienaidnieki tur -50% MS).
  - **100 pt** → **Elfu Karalienes Piesaukšana** (kanālēta 3s, izsauc sakņu jūru — visi ienaidnieki 30m rādiusā saites, DoT + roots 6s).

### 1.4. Minjonu afinitāte

- **Zaļči** ir Mežoņa Kunga dzimtā vienība.
- Ap Mežoni ~8m: Zaļči **inde no tavām prasmēm** pārceļas uz viņu dunčiem (Zaļču hiti indē mērķi).
- Ja Zaļči sit indēto mērķi, **inde spread** uz 2 tuvākajiem ienaidniekiem.
- Zaļči kā "backstab" atklāj ievainotu vietu — tavas prasmes pret to mērķi **+15% dmg**.

---

## 2. Prasmju koks

### Slānis 1 (level 1–20) — Pamati

| Prasme              | Tips     | Level | Efekts                                                            |
|---------------------|----------|-------|-------------------------------------------------------------------|
| **Ērceles Sitiens** | Pamata   | 1     | LMB, 100% weap dmg, uzliek Poison stack (5s DoT)                 |
| **Sakņu Slazds**    | Aktīvs   | 3     | Q, roots ienaidniekus 8m rādiusā uz 3s + 5s DoT                   |
| **Indes Migla**     | Aktīvs   | 8     | E, 12m sfērs, 8s DoT visiem, kas iet cauri                        |
| **Vilka Āda**       | Aktīvs   | 12    | R, 40 Sula (skat. augšā); transformē par zvēru                    |
| **Puves Sirds**     | Pasīvs   | 5     | DoT damage +20%                                                    |
| **Sakņu Ēna**       | Pasīvs   | 10    | Sakņu Slazds cooldown -30% pret elite                             |

### Slānis 2 (level 20–40) — Specializācija

**DoT zars:**
- **Puves Kliedziens** — pasīvs, katrs DoT stack aug līdz 5 (vietā 3).
- **Indes Skūpsts** — pasīvs, DoT tick ātrāk (0.5s vietā 1s).
- **Sarkanā Puve** — pasīvs, kad DoT nogalinā, spread uz 3 mērķiem.

**Zvēru zars:**
- **Zvēru Kalts** — pasīvs, Vilka Āda +5s.
- **Lāca Sīvība** — pasīvs, Vilka Ādā melee +50% dmg.
- **Zvēru Miesa** — pasīvs, cilvēku formā +15% max HP.

**Sakņu zars:**
- **Sakņu Jūra** — pasīvs, Sakņu Slazds radius 12m.
- **Sakņu Ērceles** — pasīvs, ienaidnieki roots gadījumā saņem 100% papildu DoT.
- **Aizmirstība** — pasīvs, roots 5s ilgumā.

### Slānis 3 (level 40–60) — Meistrības

- **Puvušā Meža Zvans** — pasīvs, Puvusī Migla cooldown -40%.
- **Elfu Karalienes Māsa** — pasīvs, Zaļču hiti pret indētiem mērķiem +50% dmg.
- **Nekad Nemirstoša Puve** — pasīvs, ja mērķis mirst ar 3+ DoT stackiem, atmetot mikro-eksploziju (30% ranged dmg).

### Slānis 4 (level 60–80) — Keystone (1 no 3)

| Keystone                 | Efekts                                                                                     |
|--------------------------|--------------------------------------------------------------------------------------------|
| **Puve Nekad Nemirst**   | DoT no tavām prasmēm spread automātiski uz 2 tuvākajiem ienaidniekiem, kad mērķis mirst   |
| **Elfu Karaliene**       | Vilka Āda ilgums 20s (2× ilgāk), un tā vietā transformē par mistisku zvēru ar 200% HP     |
| **Sakņu Kronis**         | Sakņu Slazds ir pastāvīgi aktīvs (ap tevi 5m); ienaidnieki tur pastāvīgi lēni + DoT       |

---

## 3. Trīs setu buildi

### 3.1. Build A — *Puvušās Sirds Kronis* (DoT Spam)

> Ienaidnieki nekad nedabūj svaigu gaisu. Trīs DoT vienlaicīgi katram.

**Keystone:** **Puve Nekad Nemirst**.

**Setu bonusi:**
- **2 daļas:** DoT damage +30%.
- **4 daļas:** DoT tick uzkrāj 1 papildu Sula (2 vietā 1).
- **6 daļas:** Puvusī Migla arī uzliek Vulnerable ienaidniekiem, DoT tur +50% dmg.

**Prioritie stati:**
- DoT Dmg > DoT Reizinātājs > Vulnerable Dmg > Cooldown Reduction.

**Signāles Aspekti:**
- *Puves Migla Aspekts* — Puvusī Migla radius +30%.
- *Puves Karogs* — DoT stack cap 5 → 8.
- *Ērceles Ceļš* — Ērceles Sitiens uzliek 2 stackus vietā 1.

**Ideālais Uber Mistiskais:**
- **Ninheras Ērkšķu Piespriedu** (Mežoņa class Uber) — ienaidnieki tuvāk 10m pastāvīgi indēti.

**Cīņas plūsma:**
1. Iebrauc grupā, Sakņu Slazds → roots.
2. Ērceles Sitiens uz galveno mērķi (Poison stacks).
3. Puvusī Migla no 80 Sula.
4. LMB spam turpina stackus.
5. Ienaidnieki mirst secīgi, DoT spread caur "Puve Nekad Nemirst".

**Best against:** grupas, mob-density farm, boss ar adds.
**Vājākais pret:** vientuļs bosss ar augstu HP (DoT nav burst).

---

### 3.2. Build B — *Vilka Ādas Kronis* (Beast Melee)

> Vilks 20s no 30, cirst caur mob grupas kā beast mode.

**Keystone:** **Elfu Karaliene**.

**Setu bonusi:**
- **2 daļas:** Vilka Āda uzkrāj Sula 2× ātrāk transformācijā (praktiski pastāvīgs).
- **4 daļas:** Vilka Ādā LMB pastāv AoE (bīdas radius 3m).
- **6 daļas:** Vilka Ādā iznīcinātie ienaidnieki dropo Vilka Sirdi — 5% max HP heal.

**Prioritie stati:**
- Melee Dmg > Attack Speed > Vilka Ādas Ilgums > Max HP.

**Signāles Aspekti:**
- *Zvēra Ceļš* — Vilka Ādā MS +100% (10m vietā 8m).
- *Lāca Sīvība Aspekts* — melee dmg vēl +25%.
- *Neapstādināmais Zvērs* — Vilka Ādā imunī pret roots/stuns.

**Ideālais Uber Mistiskais:**
- **Melnais Kronis** — visas prasmes +2 → Vilka Ādā ilgums vēl vairāk.

**Cīņas plūsma:**
1. Uzkrāj 40 Sula (Ērceles Sitiens ~15s).
2. Vilka Āda → 20s beast mode.
3. LMB AoE spam.
4. Beast mode beidzas → Puvusī Migla ilgai cīņai, uzkrāj vēl Sula.
5. Cikls turpinās (~30-40s intervāls).

**Best against:** ātrs mob clear, dungeon rush.
**Vājākais pret:** ranged pilnas cīņas (Vilka Āda ir melee tikai).

---

### 3.3. Build C — *Sakņu Zvēra Kronis* (Zone Control)

> Sakņu Slazds pastāvīgs. Ienaidnieki nevar iziet no tavas sfēras.

**Keystone:** **Sakņu Kronis**.

**Setu bonusi:**
- **2 daļas:** Sakņu Kroņa radius +5m (10m ap tevi).
- **4 daļas:** Ienaidnieki Sakņu Kroņā -30% dmg.
- **6 daļas:** Sakņu Kroņā katri 5s uzkrāj mikroeksploziju (visi tur saņem 200% weapon dmg).

**Prioritie stati:**
- Sakņu radius > Cooldown Reduction > DoT Dmg > Max HP.

**Signāles Aspekti:**
- *Sakņu Karogs* — Sakņu Kroņa radius vēl +2m.
- *Zaļču Māsa* — Zaļči Sakņu Kroņā +50% dmg.
- *Aizmirsta Zeme* — ienaidnieki Sakņu Kroņā -25% MS papildu.

**Ideālais Uber Mistiskais:**
- **Sirds no Kalna** — nāve = otra chance, atgriezies Sakņu Kroņā turpini.

**Cīņas plūsma:**
1. Sakņu Kronis ir vienmēr aktīvs — nav aktivējuma.
2. Iebrauc bosā, boss stāv Sakņu Kroņā.
3. Sakņu Slazds pievienojas normālam CD par extra roots.
4. Puvusī Migla + LMB spam.
5. Ap tevi ienaidnieki lēni, tu spam prasmes.

**Best against:** boss cīņas ar noteiktu pozīciju.
**Vājākais pret:** kustīgi bosi, kuri pārpuras.

---

## 4. Level progression 1 → 80

| Levels | Reģions         | Fokuss                                                                 |
|--------|-----------------|------------------------------------------------------------------------|
| 1–8    | Puvušais Mežs   | Tutoriāls, Ērceles Sitiens + Sakņu Slazds                              |
| 8–15   | Puvušais Mežs dziļāk | Indes Migla, first boss (Ninhera)                                 |
| 15–25  | 2. reģions      | 2. minjonu tips (Zaļči, ja nesāc tur)                                 |
| 25–35  | 3. reģions      | Vilka Āda, slāņa 2 specializācija                                     |
| 35–50  | 4.–5. reģions   | Slāņa 3 pasīvās; pirmie Reliktu                                        |
| 50–65  | Endgame priekšskaitis | Paragon atslēdzas 50                                              |
| 65–75  | Reģions 6       | Setu 4-piece                                                           |
| 75–80  | Fināls          | Uber chase                                                              |

---

## 5. Paragon 80 → 350

### 5.1. Build A (DoT Spam)

1. **Zvērests** — Prāta + Ļaunuma.
2. **Karagājienis** (0°) — DoT dmg.
3. **Nolādēts** (90°) — power ar cenu (DoT-friendly).
4. **Trono Māksla** (180°) — CDR.
5. **Melnais Kaisms** — leģendārais mezgls.

Rūnas: *Slēgtais Vārds* (Prāta slieksnis).

### 5.2. Build B (Beast Melee)

1. **Zvērests**.
2. **Karagājienis** (0°) — melee crit.
3. **Tvirtīgums** (90°).
4. **Trono Māksla** (270°) — Vilka Ādas CDR.
5. **Melnais Kaisms**.

Rūnas: *Zvana Skaņa* (crit).

### 5.3. Build C (Zone Control)

1. **Zvērests**.
2. **Trono Māksla** (0°) — CDR + resurs.
3. **Barvedis** (90°) — Zaļču bonusi Sakņu Kroņā.
4. **Nolādēts** (180°).
5. **Melnais Kaisms**.

Rūnas: *Asinssaite*.

---

## 6. BiS ekipējums pa buildiem

### Build A (DoT Spam)
- **Ierocis:** Sena ar DoT Dmg + DoT Reizinātājs.
- **Ķivere:** Setu daļa.
- **Amulets:** ar DoT Reizinātājs GA + Vulnerable Reizinātājs GA.
- **Gemi:** Ametists visos (weapons, jewellery, armor).

### Build B (Beast Melee)
- **Ierocis:** Sena ar Melee Dmg + AS.
- **Ķivere:** **Melnais Kronis** (Uber).
- **Amulets:** Crit Dmg Reizinātājs GA.
- **Gemi:** Smaragds (weapons), Rubīns (armor), Smaragds (jewellery).

### Build C (Zone Control)
- **Ierocis:** Sena ar Sakņu Radius + DoT.
- **Ķivere:** Setu daļa — *Sakņu Zvēra Kronis*.
- **Amulets:** Cooldown Reduction GA.
- **Gemi:** Ametists (weapons DoT), Dimants (jewellery), Rubīns (armor).

---

## 7. Endgame chase

1. Paragon 80 → 200.
2. Setu 6 daļas — Ninhera boss farm.
3. Rūnu upgrade.
4. Uber chase — **Ninheras Ērkšķu Piespriedu**.
5. Masterworking.
6. Paragon 200 → 350.

---

## 8. Kurš build ir tev?

| Ja tev patīk...                            | Izvēlies                       |
|--------------------------------------------|--------------------------------|
| Vienmērīga DoT plūsma, ienaidnieki mirst pēc laika | **A — DoT Spam**              |
| Beast mode, ātri melee, spike cikls        | **B — Beast Melee**            |
| Kontrolē zonu, boss cīnas tavā laukā       | **C — Zone Control**           |
| Vieglākais                                 | **A**                          |
| Sarežģītākais bet perfekcijas mērķis       | **C**                          |
