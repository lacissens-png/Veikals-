# Build Guide — Vientuļais Sniegurgs

*Ledus Zvērests. Ledus Kroņu mantinieks. Auksta liels lepnums.*

---

## 1. Klases pamati

### 1.1. Identitāte

**Vientuļais Sniegurgs ir distances maģija ar ledus + nekromantijas hibrīdu.**

- **Nekad neiet tuvcīņā**, ja var izvairīties.
- **Kontrolē lauku** ar sasalšanu, slow, un freezes.
- **Sauc skeletus** no līķiem (temp minjoni, papildu spēle).
- **Zilspāri** ap viņu iegūst dubultu spell power.

**Vienā teikumā:** *Es nelaušu tavas rokas, es tās vienkārši sasalšu.*

### 1.2. Bāzes stati (Level 1)

| Atribūts       | Vērtība | Aug par līmeni |
|----------------|---------|----------------|
| Spēks          | 5       | +0.5           |
| Veiklība       | 7       | +1             |
| Izturība       | 9       | +1             |
| Prāts          | 16      | +2.5           |
| Ticība         | 10      | +1.5           |
| Ļaunums        | 11      | +1.5           |
| **Dzīvība**    | 160     | +25            |
| **Sala (resource)** | 0/100 | +max 5/līmenis |
| **Kustība**    | Normāla | —              |

### 1.3. Signāles resurss — Sala

- **Uzkrāj** kad ienaidnieks tavas ledus prasmes iet cauri (2 pt/tick), kad **sasalis mērķis mirst** (10 pt/kill), vai kad tu **zaudē dzīvību** (1 pt/20 HP).
- **Izzūd** 1 pt/sec ārpus kaujas.
- **Izlādē:**
  - **40 pt** → **Sasalis Kaps** (10m rādiuss, freeze visus 3s + līķi kļūst par skeletiem 15s).
  - **80 pt** → **Ledus Lauskas** (kanālēts 3s nuke, milzīgs dmg 20m līnijā).
  - **100 pt** → **Torņa Redzējums** (10s: redzi visu karti + slow visi ienaidnieki 30% + tavas prasmes 2× dmg).

### 1.4. Minjonu afinitāte

- **Zilspāri** ir Vientuļā Sniegurga dzimtā vienība.
- Ap tevi ~10m: Zilspāri **spell power 2×**, viņu heal 50% stiprāks.
- **Zilspāri var celt skeletus** no līķiem tavā tuvumā (papildu tavai Sasalis Kaps mehanikai).
- Ja tavs Sala virs 50, Zilspāri **iegūst +30% cast speed**.

---

## 2. Prasmju koks

### Slānis 1 (level 1–20) — Pamati

| Prasme              | Tips     | Level | Efekts                                                            |
|---------------------|----------|-------|-------------------------------------------------------------------|
| **Ledus Šauts**     | Pamata   | 1     | LMB, projectile 20m, 100% weap dmg, +5% slow 3s                   |
| **Ledus Vājam**     | Aktīvs   | 3     | Q, cone 8m, dmg + freeze 2s                                       |
| **Sasalis Kaps**    | Aktīvs   | 8     | E, 40 Sala, area freeze (skat. augšā)                             |
| **Skeleta Signāls** | Aktīvs   | 12    | R, sauc 3 skeletus no līķiem (30s ilgums)                         |
| **Sala Sirds**      | Pasīvs   | 5     | Sala uzkrāj +25%                                                  |
| **Freeze Kronis**   | Pasīvs   | 10    | Sasaldētie mērķi +30% dmg no tavām prasmēm                        |

### Slānis 2 (level 20–40) — Specializācija

**Ledus zars:**
- **Ledus Ceļš** — pasīvs, Ledus Šauts caururbj (piercing).
- **Sasala Kronis** — pasīvs, Sasalis Kaps ilgums 5s.
- **Ledus Sirds** — pasīvs, freeze mērķi +50% dmg (papildu Freeze Kronis).

**Nekro zars:**
- **Skeleta Karogs** — pasīvs, skeleti +50% dmg.
- **Divi Skeleti** — pasīvs, Skeleta Signāls sauc 5 vietā 3.
- **Nekro Sirds** — pasīvs, ienaidnieku līķi paliek uz zemes 2× ilgāk.

**Lauka kontroles zars:**
- **Slow Kronis** — pasīvs, ienaidnieki tavas prasmes ietvarā -20% MS papildu.
- **Ledus Sfērs** — pasīvs, ap tevi 5m rādiusā pastāvīga slow aura.
- **Vientuļais Kronis** — pasīvs, jo tuvāk esi bez sabiedrotiem, jo lielāks tavs dmg (max +50%).

### Slānis 3 (level 40–60) — Meistrības

- **Ledus Lauskas Māc** — pasīvs, Ledus Lauskas cooldown -30%.
- **Torņa Redzējums Māc** — pasīvs, Torņa Redzējums ilgums 15s.
- **Sasala Kronis** — pasīvs, freezes vairs neizzūd no dmg (līdz 3s).

### Slānis 4 (level 60–80) — Keystone (1 no 3)

| Keystone            | Efekts                                                                                            |
|---------------------|---------------------------------------------------------------------------------------------------|
| **Sasala Kronis Ass**| Visas tavas prasmes sasaldē mērķi 1s papildu; sasaldēti mērķi mirstot dropo Ledus Kristu (5 Sala)|
| **Ledus Torņa Karogs**| Torņa Redzējums pastāvīgi aktīvs (Sala patēriņš 3/sec kaujā)                                   |
| **Skeletu Karaļvalsts**| Skeleta Signāls sauc 10 skeletus (nevis 3), tie dzīvo 60s; katrs saņem +20% dmg per līķi tuvumā |

---

## 3. Trīs setu buildi

### 3.1. Build A — *Ledus Kroņa Nāve* (Freeze Nuke)

> Sasaldē visu, tad Ledus Lauskas eksplodē cauri visai grupai.

**Keystone:** **Sasala Kronis Ass**.

**Setu bonusi:**
- **2 daļas:** Sasalis Kaps radius +5m.
- **4 daļas:** Freeze mērķi -30% dmg reduction (tu iesit vēl vairāk).
- **6 daļas:** Ledus Lauskas dubultā dmg pret freeze.

**Prioritie stati:**
- Frost Dmg > Crit Dmg Reizinātājs > Vulnerable Dmg > Cooldown Reduction.

**Signāles Aspekti:**
- *Sasalis Karogs* — Sasalis Kaps ilgums +2s.
- *Ledus Šauta Ceļš* — Ledus Šauts piercing +50% dmg katrā mērķī pēc pirmā.
- *Kroņa Sirds* — Frost dmg +25% pret jebkuru ienaidnieku ar zemu HP.

**Ideālais Uber Mistiskais:**
- **Melnais Kronis** — prasmes +2, viss stiprāks.

**Cīņas plūsma:**
1. Grupā ienaidnieku — Sasalis Kaps → visi freeze 3-5s.
2. Ledus Lauskas kanālē pret grupu — masveida nuke.
3. Ledus Šauts piercing pieskaita.
4. Bosos — 100 Sala → Torņa Redzējums lielā burst window.

**Best against:** grupas, mob-heavy, freeze-vulnerable bosi.
**Vājākais pret:** freeze-immūni ienaidnieki (dažas endgame bosi).

---

### 3.2. Build B — *Skeletu Karaļvalsts* (Necromancer)

> 10 skeletu armija, tu stāv aizmugurē un tikai kontrolē.

**Keystone:** **Skeletu Karaļvalsts**.

**Setu bonusi:**
- **2 daļas:** Skeleti +30% HP.
- **4 daļas:** Skeleti autoattack ar Frost dmg (papildu freeze).
- **6 daļas:** Kad skelets mirst, atmet mikro-eksploziju 100% weap dmg.

**Prioritie stati:**
- Minion Dmg > Minion HP > Frost Dmg > Cooldown Reduction.

**Signāles Aspekti:**
- *Skeleta Karogs* — Skeletu skaits +2.
- *Sasala Karogs* — skeletu Frost hits pastāvīgi sasaldē (1s freeze/hit).
- *Nekro Kronis* — līķi paliek 3× ilgāk (vairāk fuel skeletiem).

**Ideālais Uber Mistiskais:**
- **Sirds no Kalna** — otra chance, skeleti turpina.

**Cīņas plūsma:**
1. Skeleta Signāls → 10 skeleti.
2. Skeleti aizsargā tevi + fokusē mērķi.
3. Tu Ledus Šauts + Ledus Vājam LMB spam.
4. Skeleti mirst → tu sauc no jaunām līķiem.
5. Bosos — Sasalis Kaps papildu freeze fāze.

**Best against:** boss cīņas, garas.
**Vājākais pret:** ātrs mob clear (skeletu setup laiks).

---

### 3.3. Build C — *Torņa Redzējuma Kronis* (Permanent Vision God)

> Tu stāvi centrā, redzi visu karti, tavas prasmes ir 2× visu laiku. Ekonomikas puzzles ir viss.

**Keystone:** **Ledus Torņa Karogs**.

**Setu bonusi:**
- **2 daļas:** Torņa Redzējums patēriņš -50% (1.5/sec).
- **4 daļas:** Torņa Redzējuma slow 60% (nevis 30%).
- **6 daļas:** Torņa Redzējuma laikā visi tavi crits pastāvīgi freezes.

**Prioritie stati:**
- Sala Regen > Frost Dmg > Crit Chance > Crit Dmg Reizinātājs.

**Signāles Aspekti:**
- *Torņa Karogs* — Torņa Redzējums patēriņš -25%.
- *Vientuļā Sirds* — kad esi 15m+ no visiem sabiedrotajiem, +30% dmg.
- *Ledus Kroņa Kronis* — visas ledus prasmes +15% dmg.

**Ideālais Uber Mistiskais:**
- **Nolādētais Asinsakmens** — dubultkritis (feed back freeze cikls).

**Cīņas plūsma:**
1. Aktivē Torņa Redzējums pirms grupas.
2. Sala paust 1.5/sec — jāuztur ar Sasalis Kaps freezes.
3. LMB Ledus Šauts spam — visi crits freeze.
4. Freeze mērķi mirst → Sala uzkrāj → Torņa Redzējums neapstājas.
5. Ekonomikas puzzles — mērķi bez freeze avotiem = Torņa Redzējums beidzas.

**Best against:** open world farm ar high density.
**Vājākais pret:** vientuļš bosss (nav Sala fuel).

---

## 4. Level progression 1 → 80

| Levels | Reģions            | Fokuss                                                                 |
|--------|--------------------|------------------------------------------------------------------------|
| 1–8    | Ledus Kroņi        | Ledus Šauts, pirmie Sūkšņi                                            |
| 8–15   | Ledus Kroņi dziļāk | Ledus Vājam, first boss (Ievas Turaņa)                                |
| 15–25  | 2. reģions         | Otrs minjonu tips (Zilspāri, ja nesāc tur)                            |
| 25–35  | 3. reģions         | Sasalis Kaps, slāņa 2 specializācija                                  |
| 35–50  | 4.–5. reģions      | Slāņa 3 pasīvās                                                        |
| 50–65  | Endgame priekšskaitis | Paragon 50                                                          |
| 65–75  | Reģions 6          | Setu 4-piece                                                           |
| 75–80  | Fināls             | Uber chase                                                              |

---

## 5. Paragon 80 → 350

### 5.1. Build A (Freeze Nuke)

1. **Zvērests** — Prāta.
2. **Karagājienis** (0°) — crit dmg, frost dmg.
3. **Trono Māksla** (90°) — CDR uz Ledus Lauskas.
4. **Nolādēts** (180°).
5. **Melnais Kaisms**.

Rūnas: *Slēgtais Vārds* (Prāta), *Zvana Skaņa* (crit).

### 5.2. Build B (Skeletu Karaļvalsts)

1. **Zvērests**.
2. **Barvedis** (0°) — skeletu bonusi.
3. **Barvedis 2** (90°) vai **Karagājienis**.
4. **Trono Māksla** (270°).
5. **Melnais Kaisms**.

Rūnas: *Asinssaite*.

### 5.3. Build C (Torņa Redzējums)

1. **Zvērests**.
2. **Karagājienis** (0°).
3. **Trono Māksla** (90°) — resurss + CDR.
4. **Nolādēts** (180°).
5. **Melnais Kaisms**.

Rūnas: *Nolādētais Sirdsdedzis*.

---

## 6. BiS ekipējums pa buildiem

### Build A (Freeze Nuke)
- **Ierocis:** Sena ar Frost Dmg + Crit Dmg Reizinātājs.
- **Ķivere:** Setu daļa.
- **Amulets:** Frost Dmg + Crit Chance GA.
- **Gemi:** Safīrs visos.

### Build B (Skeletu Karaļvalsts)
- **Ierocis:** Sena ar Minjonu Dmg + Frost.
- **Ķivere:** **Melnais Kronis** vai setu daļa.
- **Amulets:** Minjonu Bojājums Reizinātājs GA.
- **Gemi:** Rubīns (weapons), Safīrs (jewellery), Dimants (armor).

### Build C (Torņa Redzējums)
- **Ierocis:** Sena ar Sala Regen + Crit Dmg.
- **Ķivere:** Setu daļa.
- **Ēnu slots:** **Nolādētais Asinsakmens**.
- **Gemi:** Safīrs (weapons), Topāzs (jewellery resource), Dimants (armor).

---

## 7. Endgame chase

1. Paragon 80 → 200.
2. Setu 6 daļas — Ievas Turaņa farm.
3. Rūnu upgrade.
4. Uber chase — Melnais Kronis un Sirds no Kalna.
5. Masterworking.
6. Paragon 200 → 350.

---

## 8. Kurš build ir tev?

| Ja tev patīk...                        | Izvēlies                          |
|----------------------------------------|-----------------------------------|
| Sasaldē visu, tad nuke                 | **A — Freeze Nuke**               |
| 10 skeleti apkārt, mierīga plūsma      | **B — Skeletu Karaļvalsts**       |
| Kontroli visai kartei, ekonomika       | **C — Torņa Redzējums**           |
| Vieglākais                             | **B**                             |
| Sarežģītākais bet visdziļākais         | **C**                             |
