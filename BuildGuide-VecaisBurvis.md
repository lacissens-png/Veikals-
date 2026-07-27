# Build Guide — Vecais Burvis

*Torņa Zīmoglaukuma Mantinieks. Vecā Torņa valdnieks. Trio: Torins + Sasla + Vera.*

**Iedvesma:** The Wizard (Overlord I torņa boss) — visstiprākais no 7 varoņiem, arch-mage, kritušais no lepnības.

Ģenerāļu detaļas — skat. `Generals.md` §5.

---

## 1. Klases pamati

### 1.1. Identitāte

**Vecais Burvis ir tīrs distances arch-mage — glass cannon ar realitātes manipulāciju un arkānais trio.**

- **Nekad neiet tuvcīņā** ja iespējams.
- **Reality manipulation** — teleports, time slow, gravitācijas maiņa.
- **Torins (spellblade), Sasla (frost mage), Vera (teleport support)** — arkānais chaos trio.
- **Vienā teikumā:** *"Realitāte ir mans nastīgs materiāls. Mans trio to lauzu ar mani kopā."*

### 1.2. Bāzes stati (Level 1)

| Atribūts       | Vērtība | Aug par līmeni |
|----------------|---------|----------------|
| Spēks          | 4       | +0.5           |
| Veiklība       | 8       | +1             |
| Izturība       | 7       | +0.5           |
| Prāts          | 18      | +3             |
| Ticība         | 9       | +1             |
| Ļaunums        | 10      | +1.5           |
| **Dzīvība**    | 150     | +22            |
| **Ark. Enerģija** | 0/100| +max 5/līmenis |
| **Kustība**    | Normāla | —              |

### 1.3. Signāles resurss — Arkānā Enerģija

- **Uzkrāj:** 5 pt / crit, 3 pt / spell cast, 10 pt / kill ar spell.
- **Nekad neizzūd** kaujā (perfekts sustained caster spēlei).
- **Izlādē:**
  - **40 pt** → **Arkāns Zvans** (E, tavas nākošās 3 prasmes dubultā dmg).
  - **80 pt** → **Realitātes Plaisa** (Q upgrade, 15m teleports + AoE 200% spell dmg).
  - **100 pt** → **Reality Rift** (100pt ult, 15s time slow apkārtējiem 50% + tavas prasmes 2× dmg un 2× ātrāk).

### 1.4. Ģenerāļu trio

Vecā Burvja ģenerāļi (skat. `Generals.md` §5):

| Ģenerālis | Loma            | Bāzes efekts ap Vecu Burvi (aura, 8m)                     |
|-----------|-----------------|-----------------------------------------------------------|
| **Torins**| Spellblade Hybrid| Pēc viņa sitiena, tavs nākošais spell +50% dmg 5s        |
| **Sasla** | Frost Mage      | Ienaidnieki 8m ap Saslu -20% MS + -10% dmg               |
| **Vera**  | Teleport Support| Grupa (tu + 3 ģenerāļi) +15% dodge chance + 15% MS       |

Trio kopīgais spēks: Torins ir melee bruiser priekšā, Sasla ir zone control lock, Vera ir mobility catalyst.

---

## 2. Prasmju koks

### Slānis 1 (level 1–20) — Pamati
- **Arkānā Lauska** (LMB, 1) — projectile 30m, 140% spell dmg
- **Realitātes Plaisa** (Q, 3) — 8m teleports + 6m AoE explosion
- **Time Wave** (E, 8) — 20m sfērs, ienaidnieki -30% MS 4s + saņem 100% spell dmg
- **Arkāns Karogs** (R, 12) — 10s pasīvs, tavas prasmes +25% dmg
- **Arkāns Sirds** (pasīvs, 5) — Arkānā Enerģija uzkrāj +25%
- **Torņa Māte** (pasīvs, 10) — Vera Realitātes Plaisa cooldown -20% (ģenerāļa buff)

### Slānis 2 (level 20–40) — Specializācija

**Reality Rift zars:**
- **Plaisas Māc** — pasīvs, Realitātes Plaisa cooldown -30%
- **Divi Plaisi** — pasīvs, Realitātes Plaisa 2 charges
- **Reality Sirds** — pasīvs, teleports uzliek Vulnerable 4s

**Chain zars:**
- **Arkānā Ķēde** — pasīvs, Arkānā Lauska lec 3 mērķiem (chain)
- **Zibens Kalts** — pasīvs, chain lēciens +50% dmg (nevis -20%)
- **Realitātes Kalts** — pasīvs, Time Wave arī chain 3 mērķiem

**Time zars:**
- **Time Māte** — pasīvs, Time Wave slow +50% (kopā 80% slow)
- **Time Sirds** — pasīvs, Reality Rift ilgums +5s (kopā 20s)
- **Time Karogs** — pasīvs, Reality Rift laikā tavi hiti auto-freeze mērķi 1s

**Trio zars:**
- **Torins Karogs** — pasīvs, Torins Arkānais Sitiens +50% spell dmg proc
- **Sasla Karogs** — pasīvs, Sasala Torņa dubultā ilgums (30s)
- **Vera Karogs** — pasīvs, Vera Realitātes Plaisa 2 charges

### Slānis 3 (level 40–60) — Meistrības
- **Realitātes Ass** — pasīvs, tavi spell kritiski 3× dmg (nevis 2×)
- **Torņa Kronis** — pasīvs, kad Torins + Sasla + Vera visi aktīvi, tu +25% dmg
- **Vecākais Māc** — pasīvs, kad Arkānā Enerģija max, tavas prasmes bezmaksas 5s

### Slānis 4 (level 60–80) — Keystone (1 no 3)

| Keystone            | Efekts                                                                                              |
|---------------------|-----------------------------------------------------------------------------------------------------|
| **Realitātes Karaļvalsts** | Reality Rift pastāvīgi aktīvs (Ark. Enerģija patēriņš 3/sec), time slow 80%                 |
| **Arkānā Sinerģija** | Sasla + Vera signāles prasmes cooldown -50%, bet Torins deactivated                            |
| **Torņa Ass**       | Katrs spell cast dubultā (viens skill = 2 cast automātiski), bet Arkānā Enerģija patēriņš 2×      |

---

## 3. Trīs setu buildi

### 3.1. Build A — *Realitātes Karaļvalsts* (Permanent Time Slow God)

> Tu vienmēr esi maģiskā laika kluslumā. Ienaidnieki lēni, tu triple dmg. Trio buff visu.

**Keystone:** *Realitātes Karaļvalsts*.

**Setu bonusi:**
- **2 daļas:** Reality Rift patēriņš -50% (1.5/sec)
- **4 daļas:** Reality Rift laikā tavi hiti pastāvīgi freezes
- **6 daļas:** Reality Rift laikā tavi crits triple (nevis dubultā)

**Prioritie stati:**
- Spell Damage > Crit Dmg Reizinātājs > Arkānā Enerģija Regen > Cooldown Reduction

**Signāles Aspekti:**
- *Realitātes Kronis* — Reality Rift patēriņš vēl -25% (kopā 0.75/sec)
- *Vientuļā Sirds* — Kad esi 15m+ no visiem sabiedrotajiem, +30% dmg
- *Torņa Karogs* — Time slow +20% (kopā 100% ap tevi 5m)

**Ideālais Uber Mistiskais:**
- **Nolādētais Asinsakmens** (Uber Gem) — dubultkritis
- **Melnais Kronis** (2. Uber slots) — prasmes +2

**Cīņas plūsma:**
1. Grupā 3 ģenerāļi (Sasla 15m turret vieta, Vera aizmugurē, Torins flanks).
2. Aktivē Reality Rift → pastāvīgi 80% slow visiem.
3. Arkānā Lauska LMB spam — visi hiti crit + freeze.
4. Realitātes Plaisa teleports uz nākošo grupu.
5. Vera Bond 10 procs auto-repeat Realitātes Plaisa.
6. Cikls nekad neizzūd (crit → Arkānā Enerģija → sustain).

**Best against:** garas boss cīņas, Uber Bosi, single-target.
**Vājākais pret:** situācijas ar zemu crit chance (Ark. Enerģija sāk nolikties).

---

### 3.2. Build B — *Arkānā Sinerģija* (Sasla + Vera Storm)

> Torins vienmēr aizmugurē, Sasla + Vera ir čempioni. Ice turrets + teleport chaos.

**Keystone:** *Arkānā Sinerģija*.

**Setu bonusi:**
- **2 daļas:** Sasala Torņa signāles prasmes cooldown -50% (22.5s vietā 45s)
- **4 daļas:** Sasla 3 ledus torņi vienlaikus max
- **6 daļas:** Kad Sasla ledus torni ievieto, tavs Ark. Enerģija +25 uzreiz

**Prioritie stati:**
- Spell Damage > Frost Dmg > Ģenerāļu Bojājums > CDR

**Signāles Aspekti:**
- *Sasala Sirds* — Sasala Torņa dmg +50%
- *Vera Blink Karogs* — Vera teleport party freq -30%
- *Sinerģija Kronis* — Kad Sasla + Vera aktīvi, tu +30% spell dmg

**Ideālais Uber Mistiskais:**
- **Melnais Kronis** — spells +2, Sasla + Vera skills stronger

**Cīņas plūsma:**
1. Grupā Sasla (turret vieta) + Vera (aizmugurē).
2. Tu Arkānā Lauska - 3× Sasala Torņa spam par 22.5s cooldown.
3. Chain freeze caur visu grupu.
4. Ienaidnieki mirst — trio triple mirst — Ark. Enerģija spike.
5. Reality Rift burst window.

**Best against:** mob density, dungeon runs.
**Vājākais pret:** vietas kur turret nav laika ievietoties (fast-moving bosi).

---

### 3.3. Build C — *Torņa Ass* (Double Cast God)

> Katrs tavs spell iet 2×. Katras 4 sekundes izsauc pilnu apocalypse. Trio kordinē.

**Keystone:** *Torņa Ass*.

**Setu bonusi:**
- **2 daļas:** Ark. Enerģija patēriņš samazināts uz 1.5× (nevis 2×)
- **4 daļas:** Kad kastē 2× spell, otrs ir vienmēr crits
- **6 daļas:** Ik reizi kad izmanto Arkāns Zvans, tu iegūsti 60 Ark. Enerģija uzreiz

**Prioritie stati:**
- Ark. Enerģija Regen > Cast Speed > Crit Dmg Reizinātājs > CDR

**Signāles Aspekti:**
- *Divu Cast Kronis* — Ark. Enerģija patēriņš vēl -25%
- *Vecākais Kronis* — Arkāns Zvans buff ilgums +5s
- *Kā Māte Māca* — Otrs cast bezmaksas, ja tavas crit chance ir virs 60%

**Ideālais Uber Mistiskais:**
- **Nolādētais Asinsakmens** — dubultkritis (2× crit no jau 2× cast = 4× dmg efektīvs)

**Cīņas plūsma:**
1. Uzkrāj Ark. Enerģija pirmās 20s (LMB spam).
2. Torins pēkšņi teleports melee - Arkānais Sitiens buff tavai spell.
3. Arkāns Zvans → tavas nākošās 3 prasmes dubultā dmg.
4. Realitātes Plaisa 2 charges → 4 hits ar Torņa Ass double cast.
5. Vera auto-triger Realitātes Plaisa reset ar Bond 10.
6. Reality Rift ult par lielo burst.

**Best against:** boss burst races, spike windows.
**Vājākais pret:** sustain cīņas ar zemu Ark. Enerģija regen.

---

## 4. Level progression 1 → 80

| Levels | Reģions            | Fokuss                                                                 |
|--------|--------------------|------------------------------------------------------------------------|
| 1–8    | Vecais Tornis      | Arkānā Lauska, Realitātes Plaisa, Torins atslēdz                       |
| 8–15   | Vecais Tornis dziļāk | Time Wave, Sasla atslēdz, first boss (Selriss)                      |
| 15–25  | 2. reģions         | Vera atslēdz (visi 3 ģenerāļi complete)                                |
| 25–35  | 3. reģions         | Arkāns Karogs, slāņa 2 specializācija                                  |
| 35–50  | Melnais Kalns      | Slāņa 3 pasīvās; pirmie Reliktu                                        |
| 50–65  | Endgame priekšskaitis | Paragon 50; Bond 3-5                                                |
| 65–75  | Reģions grinds     | Setu 4-piece; Bond 5-7                                                 |
| 75–80  | Fināls             | Uber chase; Bond 7-9                                                    |

---

## 5. Paragon 80 → 350

### 5.1. Build A (Realitātes Karaļvalsts)

1. **Zvērests** — sākuma dēlis
2. **Karagājienis** (0°) — spell dmg + crit
3. **Trono Māksla** (90°) — resurs + CDR
4. **Nolādēts** (180°) — power ar cenu
5. **Melnais Kaisms** — leg mezgls "Realitātes Kronis"

Rūnas: *Nolādētais Sirdsdedzis* (dubultē visu, cena piedodāma), *Slēgtais Vārds* (Prāts slieksnis)

### 5.2. Build B (Sasla + Vera Storm)

1. **Zvērests**
2. **Ģenerāļu Sardzes** (0°) — trio bonusi (Sasla + Vera focus)
3. **Karagājienis** (90°) — spell dmg
4. **Trono Māksla** (270°)
5. **Melnais Kaisms**

Rūnas: *Asinssaite* (Ģenerāļu bonusi), *Sasala Zīme*

### 5.3. Build C (Double Cast)

1. **Zvērests**
2. **Karagājienis** (0°) — crit dmg
3. **Trono Māksla** (90°) — resurs
4. **Nolādēts** (180°)
5. **Melnais Kaisms**

Rūnas: *Vainaga Māc* (ult uptime), *Zvana Skaņa* (crit)

---

## 6. BiS ekipējums pa buildiem

### Build A (Realitātes Karaļvalsts)
- **Ierocis:** Sena wand ar Spell Dmg + Crit Dmg Reizinātājs
- **Ķivere:** **Melnais Kronis**
- **Ēnu slot:** **Nolādētais Asinsakmens**
- **Vera gear:** Realitātes Vaids Uber (teleport spam)
- **Gemi:** Ametists (weapons), Topāzs (jewellery), Dimants (armor)

### Build B (Sasla + Vera Storm)
- **Ierocis:** Sena wand ar Frost Dmg + Ģenerāļu Bojājums Reizinātājs
- **Ķivere:** Setu daļa
- **Sasla gear:** Ledus Kroņis Uber (3 torņi max)
- **Vera gear:** Realitātes Vaids Uber
- **Gemi:** Safīrs (weapons), Safīrs (jewellery), Dimants (armor)

### Build C (Double Cast)
- **Ierocis:** Sena wand ar Cast Speed + Crit Dmg
- **Ķivere:** Setu daļa
- **Ēnu slot:** **Nolādētais Asinsakmens**
- **Torins gear:** Arkānais Glaive Uber (spell buff sync)
- **Gemi:** Ametists (weapons), Topāzs (jewellery), Dimants (armor)

---

## 7. Endgame chase

1. Paragon 80 → 200
2. Setu 6 daļas — Selriss boss farm
3. Rūnu upgrade
4. Ģenerāļu Bond farmings — visi 3 uz Bond 10
5. Ģenerāļu Uber Mistiskie chase (Arkānais Glaive + Ledus Kroņis + Realitātes Vaids)
6. Paragon 200 → 350

---

## 8. Kurš build ir tev?

| Ja tev patīk...                              | Izvēlies                       |
|----------------------------------------------|--------------------------------|
| Time slow, mūžīga jauda, single-target       | **A — Realitātes Karaļvalsts** |
| Ice turrets + teleport chaos                 | **B — Sasla + Vera Storm**     |
| Combo, double-cast, spike burst              | **C — Double Cast**            |
| Vieglākais start                             | **B**                          |
| Sarežģītākais bet visspēcīgākais            | **A**                          |
