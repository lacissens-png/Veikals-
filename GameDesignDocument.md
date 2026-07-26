# Design dokuments — *Kalna Ēnas* (darba nosaukums)

**Žanrs:** Isometrisks action RPG
**Stils:** Gothic fantasy
**Engine:** Unreal Engine 5
**Skats:** Top-down / isometric (kā Diablo, Path of Exile, Hades)
**Platforma:** PC (Windows) — sākotnēji

---

## 1. Koncepta kopsavilkums

Spēlētājs iemiesojas *Sardzē* — pēdējais no zvērināta ordeņa, kas gadsimtiem sargāja pasauli no *Mūžīgā Miega* — sena lāsta, kas mirušos ceļ atpakaļ kā mēmus, sapelējušus verdzeniekus. Ordenis krita, pasaule slīd tumsā, un tu esi vienīgais, kurš vēl var doties uz **Kalnu** — sakrālo virsotni, kur lāsts sākās.

Spēle notiek septiņu procedurāli veidotu reģionu ceļojumā uz Kalnu. Katrs reģions ir mazs, blīvs, ar rokām veidots, bet secība un dažas telpas mainās katrā spēles reizē.

**Vienā teikumā:** Diablo blīvums + Bloodborne noskaņa + Hades stāstījums, viena varoņa isometriska ceļojumā uz nolādēto virsotni.

---

## 2. Pasaule un noskaņa

### Vizuālais stils
- **Krāsu palete:** ogļu melna, asins sarkana, alvas zilā, sveču zelts. Ļoti maz piesātināto krāsu.
- **Gaismas:** stipri kontrastējošas — sveces, laternas, mēness gaisma; lielas ēnas.
- **Arhitektūra:** ziemeļu gotika — smaili velvējumi, kapu akmeņi, mūru fragmenti, sapuvušas baznīcas.
- **Daba:** kailu koku silueti, migla, purvi, sniegs augstākos reģionos.

### Pasaules lore īsumā
- Pasaule saucas **Vaireja** (*Vairēja*).
- Reiz to sargāja septiņi **Zvērestvīri** — ordeņa dibinātāji. Katrs iesvētīja vienu no septiņām sardzes vietām.
- Pirms trīsdesmit gadiem viens no viņiem, **Melnais Priesteris Orins**, nokāpa Kalnā, lai atrastu lāsta sakni — un pats to izlaida.
- Kopš tā laika mirušie neguļ. Ciemi krīt, ordenis izklīst.
- Spēlētājs ir **pēdējais mācītais bērns** no ordeņa — audzināts vaidēt, cīnīties un mirt Kalnā.

### Septiņi reģioni (ceļojumā uz augšu)
1. **Pelnu Purvs** — nolaista sardzes vieta, iesācēju zona.
2. **Sausokļu Mežs** — kaltē koki, ķērtāji, pirmais boss.
3. **Klosteris Kalnpēdē** — kritis klosteris, apsēsti mūki.
4. **Kaulu Kāpnes** — mūžīgas kāpnes cauri klintīm.
5. **Ledus Kroņi** — sniegā slēptas kapenes.
6. **Melnās Baznīcas Drupas** — Orina bāze.
7. **Kalna Kalts** — pēdējais reģions, virsotne.

---

## 3. Galvenā mehānika

### 3.1. Kustība un vadība
- **Peles klikšķis** — pārvieto varoni (Diablo stils).
- **Alternatīva:** WASD tiešā vadība (spēlētājs izvēlas iestatījumos).
- **Kamera:** fiksēts isometrisks leņķis, ar iespēju rotēt 90° soļos.

### 3.2. Cīņa
Reāllaika, mērķēta, ar izvairīšanās mehāniku.
- **Vieglais uzbrukums** (LMB) — ātrs, mazs bojājums.
- **Smagais uzbrukums** (RMB) — lēns, lauž vairogus.
- **Izvairīšanās** (Space) — īsa rullēšana ar i-frames.
- **Bloks** (Shift) — patērē izturību.
- **Prasmes** (Q, E, R) — trīs aktīvas prasmes, kas mainās atkarībā no ekipējuma.

**Trīs statistikas resursu joslas:**
- **Dzīvība** (sarkana) — atjaunojas ar dziedināšanas flaķi.
- **Izturība** (zaļa) — cīņas darbībām, ātri atjaunojas.
- **Ticība** (zila) — burvestībām un lūgšanām.

### 3.3. Progresija
- **Līmeņi** — no 1 līdz 50.
- **Sešas pamata statistikas:**
  - *Spēks* — smago ieroču bojājumi.
  - *Veiklība* — vieglo ieroču bojājumi, izvairīšanās.
  - *Izturība* — dzīvības josla, izturība.
  - *Prāts* — ticības josla, burvestību jauda.
  - *Ticība* — svēto prasmju piekļuve.
  - *Ļaunums* — nolādēto prasmju piekļuve (nāk ar cenu).
- **Prasmju koks** — trīs zari: **Zobens**, **Sveka** (burvestības), **Lāsts** (nolādētās prasmes).

### 3.4. Ekipējums
- **Sešas slotes:** galva, rumpis, rokas, kājas, ierocis, sekundārais (vairogs / talismans / otrs ierocis).
- **Krāsas pēc retuma:** parasts (pelēks), rūdīts (zils), sena (violets), reliktu (zelts), nolādēts (asins sarkans — spēcīgs, bet ar sāpēm).
- **Kalt un uzlabot** — pie sētniekiem drošajās telpās.

### 3.5. Drošās telpas ("Sveces")
Kā ugunskuri Dark Souls — atjauno visu, saglabā progresu, respawn ienaidniekus. Katrā reģionā 2–4.

### 3.6. Nāve
- Nomirstot atmostas pēdējā svecē.
- Zaudē pusi savāktā **Pelna** (valūta/pieredze).
- Var to atgūt, aizsniedzot vietu, kur nomiris.

---

## 4. Stāsts un tā struktūra

### Trīs cēlieni
1. **Ceļš uz Kalnu** — reģioni 1–3. Iepazīsti pasauli, satiec ordeņa paliekas.
2. **Kāpiens** — reģioni 4–5. Atklāj, ka Orins nebija ļauns — viņš meklēja glābiņu.
3. **Virsotne** — reģioni 6–7. Izvēle: iznīcināt Kalnu (lāsts beidzas, bet arī maģija pasaulē zūd), sakausēt to sevī (kļūsti par jauno Sargu, bet mirstīgs) vai nokāpt (pieņem lāsta pastāvēšanu, izdzīvo pasauli, kāda tā ir).

### Trīs beigas
- **Klusums** — lāsts salauzts, pasaule bez brīnumiem.
- **Sargs** — spēlētājs kļūst par pusdievu Kalnā, mūžīgi vientuļš.
- **Pelni** — pasaule paliek nolādēta, spēlētājs atgriežas ciemā mierā.

### NPC (svarīgākie)
- **Sētnieks Vīlmars** — vecs kalējs, ordeņa pēdējais māceklis; kalt ekipējumu.
- **Māsa Elēna** — nabadzīga mūķene; pārdod dziedināšanu, atklāj lūgšanas.
- **Klaidonis Bēks** — sarkanmatains klejotājs; parādās katrā reģionā, tirgo dīvainas lietas.
- **Melnais Priesteris Orins** — antagonists un noslēpuma centrs.

---

## 5. Enemy dizains (piemēri)

| Nosaukums          | Reģions      | Uzvedība                                                |
|--------------------|--------------|---------------------------------------------------------|
| Purva rāpulis      | 1            | Lēns tuvcīņas ienaidnieks, indīgs.                      |
| Sapuvis kareivis   | 1–2          | Skrien uz priekšu, sitiens ar rūsainu zobenu.           |
| Kliedzēja          | 2            | Distances kliedziens — atņem izturību.                  |
| Kailās māsas       | 3            | Trio, koordinēti uzbrukumi.                             |
| Ledus lopiņš       | 5            | Slēpjas sniegā, uzbrūk no aizmugures.                   |
| **BOSS:** Bezacu Bīskaps | 3      | Zvana zvanus, sauc ģeneratīvus ienaidniekus.            |
| **BOSS:** Orins    | 7 (fināls)   | Trīs fāzes — cilvēks, pusdievs, Kalns.                  |

---

## 6. Minimālais spēlējamais prototips (MVP)

Lai pierādītu, ka koncepts strādā, mērķa MVP:

- Viens reģions (**Pelnu Purvs**).
- Viens varonis ar pilnu kustību un pamata cīņu.
- Divi ienaidnieku tipi + viens mini-boss.
- Viena droša telpa (svece) ar respawn.
- Trīs ekipējuma priekšmeti (viens ierocis, vairogs, talismans).
- Vienkāršs UI: dzīvība, izturība, ticība, mini-mapa.
- Nāves loop ar Pelna zaudēšanu/atgūšanu.

**Laika ietvars solo izstrādei:** 3–6 mēneši MVP.

---

## 7. Tehniskais stack (Unreal Engine 5)

- **UE 5.4+** ar Lumen (dinamiska gaisma — svarīga gotikai) un Nanite.
- **C++** pamatsistēmām (Character, Combat, Inventory, Save).
- **Blueprints** UI, prasmēm, quest scripting.
- **Enhanced Input** vadībai.
- **GAS (Gameplay Ability System)** prasmēm un statiem — jaudīgs, bet stāvs.
- **Behavior Trees** ienaidnieku AI.
- **Metasounds** audio.

### Ieteicamais projekta plugins
- **Advanced Locomotion System** — kustībai.
- **Electronic Nodes** — Blueprint tīrībai.
- **Rider for Unreal** vai **Visual Studio 2022** — C++ redaktoram.

---

## 8. Māksla un audio

### Māksla
Sākumam **stilizēti low-poly** ar spēcīgu apgaismojumu — reālistiska AAA māksla vienam cilvēkam nav sasniedzama, bet stils to var kompensēt (skat. *Return of the Obra Dinn*, *Death's Door*).

### Audio
- Klusa, minimāla mūzika. Bass, korālis, iestrēguši zvani.
- Vējš, tālas balsis, karkstoši koki — pastāvīgs ambient.
- Nav "battle music" — cīņā skan tikai ieroči un ienaidnieks.

---

## 9. Nākamie soļi

1. **Šis dokuments** ✅
2. **Vienas ainas mockup** — bloku pasaule ar varoņa kustību Unreal.
3. **Pamata C++ klases:** `SardzeCharacter`, `HealthComponent`, `WeaponBase`, `EnemyBase`.
4. **Cīņas prototips** ar vienu ienaidnieku.
5. **UI melnraksts** — HUD, inventārs, izvēlne.
6. **MVP saraksts** ar konkrētiem uzdevumiem.

---

*Šis ir dzīvs dokuments. Rediģē brīvi, kad kāds elements neder — labs GDD mainās ar spēli.*
