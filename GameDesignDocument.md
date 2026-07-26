# Design dokuments — *Kalna Kungs* (darba nosaukums)

**Žanrs:** Isometrisks action RPG ar minjonu komandēšanu
**Iedvesma:** *Overlord* (2007) × *Diablo IV*
**Engine:** Unreal Engine 5
**Skats:** Top-down / isometrisks, ar rotējamu kameru
**Platforma:** PC (Windows), vēlāk konsoles

---

## 1. Koncepts vienā teikumā

Tu esi jaunais *Kalna Kungs* — tumšas gotiskas impērijas mantinieks, kurš pavēl četru veidu **Ēnu minjoniem** izlaupīt, iekarot un pārbūvēt sabrukušu pasauli, meklējot arvien retākus artefaktus arvien draudīgākās vietās.

**Overlord daļa:** minjoni ir tavs galvenais ierocis. Tu retāk cīnies pats — tu komandē.
**Diablo 4 daļa:** blīvs loot, prasmju koki, dungeoni, world bosi, sezonas, endgame progresija.
**Gothic daļa:** viss ir tumšs, drēgns, sakrālizēts, ar melno humoru.

---

## 2. Pasaule un noskaņa

### Vizuālais stils
- **Krāsu palete:** ogļu melna, asins sarkana, alvas zila, sveču zelts, purpurs kunga zīmoglaukumos.
- **Gaisma:** stiprie kontrasti — sveces, ugunskuri, lauskas, mēness caur miglu.
- **Arhitektūra:** ziemeļu gotika — smaili velvējumi, kripтas, klosteri, akmens tilti pār bezdibeni.
- **Melnais humors:** minjoni murmina glupības, nes uz galvām ķirbjus un pannas, cīnās ar sagrautiem baznīcas svečturiem.

### Pasaules lore
- Pasaule saucas **Vaireja**.
- Pirms 300 gadiem **Pirmais Kungs** valdīja no **Melnā Kalna** — cietokšņa virsotnē. Viņa minjoni iekaroja septiņas karaļvalstis.
- Pirmais Kungs krita. Kalna vārti aizvērās. Pasaule sabruka septiņās sīkās, savstarpēji naidīgās valstiņās, kurās *tagad* valda "gaišie" ordeņi, korumpēti bīskapi un fanātiskas mūku brālības.
- Tu esi **jaunais mantinieks** — pamodies pēc trīssimt gadu miega, atradis Melno Kalnu tukšu, minjonu bedres izsalkušas, tronu drupās.
- Tavs uzdevums: atjaunot impēriju. Vai iznīcināt to. Vai kaut ko trešo — tas ir tavs stāsts.

### Septiņas karaļvalstis (spēles reģioni)
1. **Pelnu Purvs** — sabrukusi robežpils, iesācēju zona.
2. **Sausokļu Mežs** — mūku brālība sarga senu kriptu.
3. **Melnā Baznīca** — fanātiska bīskapa cietoksnis.
4. **Ledus Kroņi** — ziemeļu klaidoņu klani, vilku dievs.
5. **Sarkanās Pļavas** — lauku novads ar apsēstiem zemniekiem.
6. **Stikla Pilsēta** — noslēpumaina tirgotāju republika, alķīmiķi.
7. **Melnais Kalns** — tava paša cietoksnis (hub + fināls).

Katrs reģions:
- **Atvērts** — vairākas veiktenes, blakusuzdevumi, pasaules bosi (Diablo 4 stils).
- **Dungeoni** — 4–6 uz reģionu, iet dziļāk pēc katra passa (**Murgu Dungeoni** endgame).
- **Iekaroti reģioni** — pēc uzvaras pār reģiona bosu var tur pastāvīgi iemitināt savus minjonus, kas ienes zeltu.

---

## 3. Galvenā mehānika — Minjonu komandēšana

### Četru veidu minjoni (Overlord DNS ar gotisku pieskaņu)

| Tips        | Krāsa      | Loma                    | Īpatnības                                                                 |
|-------------|------------|-------------------------|---------------------------------------------------------------------------|
| **Sūkšņi**  | Brūni      | Tuvcīņa, tanks          | Stipri, izturīgi, dumji. Var celt smagas lietas, lauzt vārtus.            |
| **Sārtaji** | Sarkani    | Distance, uguns         | Šauj uguns bumbas, imūni pret uguni, sadedzina koka barjeras.             |
| **Zaļči**   | Zaļi       | Zaglība, inde           | Neredzami krūmos, backstab, izgādā indi, atmet slazdus.                   |
| **Zilspāri**| Zili       | Maģija, dziedināšana    | Dziedina komandu, ceļ nesenus mirušos kā īslaicīgus sabiedrotos.          |

Sākumā tev pieejami tikai **Sūkšņi**. Katrus atklāj, atgriežot minjonu bedri no izmirušas.

### Komandēšanas mehānika
- **RMB uz vietas** — *"Ejiet tur!"* — minjoni skrien uz punktu.
- **RMB uz mērķa** — *"Uzbrūkiet!"* — koncentrējas uz vienu ienaidnieku vai objektu.
- **Shift + RMB** — *"Turieties tālāk"* — minjoni atkāpjas pie tevis.
- **Q / E / R / T** — pārslēdz aktīvo minjonu tipu (kuriem sekot pavēlei).
- **F** — *"Savāc!"* — minjoni sāk lasīt loot, kur pavēl.
- **G** — *"Ekipējieties!"* — minjons paceļ ieroci/bruņas no zemes un lieto pats.

### Minjonu limits
- Sākumā: **10 minjoni** kopā (pa 2–3 tipiem).
- Uzlabojot Melno Kalnu, aug līdz **30 minjoniem** (7–8 no katra tipa).
- Minjoni mirst. Jaunus **audzē** minjonu bedrēs par savākto **dvēseļu enerģiju** (kritušie ienaidnieki izdala dvēseles).

### Tavs paša varonis
Tu **arī** cīnies, bet retāk. Tavas lomas:
- **Aura** — minjoni ap tevi ir stiprāki.
- **Personiskās prasmes** — spēcīgas AoE burvestības ar garu cooldown (Diablo ultimate stilā).
- **Diplomātija/iebiedēšana** — tu runā ar NPC, ne minjoni.

Tu vari nomirt. Ja nomirsti — minjoni bēg atpakaļ uz Melno Kalnu, tu atmostos tronī. Zaudē pusi *Pelna* (skat. tālāk).

---

## 4. Cīņas sistēma

- **LMB** — tavs personiskais uzbrukums (nūja/zizli/lāsts).
- **RMB** — minjonu komanda.
- **Space** — izvairīšanās (i-frames).
- **Q / E / R** — trīs aktīvās prasmes (mainās pēc ekipējuma un prasmju koka).
- **Shift** — sprints.

**Resursi:**
- **Dzīvība** — atjaunojas ar dziedināšanas flaķi (Zilspāri var papildināt).
- **Mana** — burvestībām.
- **Dvēseļu enerģija** — kalpo minjonu radīšanai un dažām speciālām prasmēm.

**Cīņas ritms:** ne tu, ne minjoni viens paši netiek galā ar boss. Uzvara — pareizā komandu kombinācija (piem., Zaļči backstab, kamēr Sūkšņi tur agro, tu met AoE).

---

## 5. Loot un ekipējums (Diablo 4 stils)

### Retums (krāsa)
- **Parasts** (pelēks)
- **Rūdīts** (zils) — 1 affix
- **Sens** (violets) — 2–3 affixi
- **Reliktu** (zelts, "legendary") — unikāls power ar spēles-maiņas efektu
- **Nolādēts** (asins sarkans, "unique") — vislabākais, bet ar sāpēm (piem., "-30% dzīvība, bet minjoni ir 2× stiprāki")

### Slotes (spēlētājam)
Galva, rumpis, rokas, kājas, ierocis, sekundārais, 2× gredzens, amulets = **9 slotes**.

### Minjonu ekipējums
Katrs minjons var nēsāt **3 lietas**: ķivere, ierocis, aksesuārs. Atrastas lietas — nodod minjoniem. Sūkšņi ar bruņu cepuri = dzīvi ilgāk. Sārtaji ar sadedzinātu vēdzelēm — smieklīgi bīstami.

### Kalt un pārkalt
- **Melnajā Kalnā** — kalējs, alķīmiķis, mūžīgais burvis.
- **Pārkalt affixu** (Diablo Enchanter stils) — nomaini vienu affixu par cenu, kas aug pēc katras reizes.
- **Ievietot pērles** — 5 pērļu tipi ar dažādiem bonusiem.

---

## 6. Progresija

### Līmeņi
- Spēlētājs: līmeņi 1–100 (Diablo 4 stils).
- Pēc 100. — **Paragon punkti** endgame progresijai.

### Prasmju koks
Trīs zari:
- **Kungs** — personiskās kaujas prasmes, aura, burvestības.
- **Barvedis** — minjonu bonusi (vairāk minjonu, stiprāki, ātrāk atdzimst).
- **Melnais Māksla** — nolādētās prasmes (celt mirušos, kliedziens, kas iebaida u.c.). Nāk ar cenu (mazāka dzīvība, dvēseļu maksa).

### Melnā Kalna cietoksnis (tavs hub, aug ar tevi)
- **Tronis** — galvenā telpa; NPC nāk pie tevis ar problēmām/quest.
- **Minjonu bedres** — audzē minjonus, uzlabo katra tipa max skaitu.
- **Bruņotava** — glabā ekipējumu, kalt.
- **Alķīmiķa tornis** — flakoni, pārkalšana.
- **Karte** — planē reidus un iekarojumus.
- **Trofeju halle** — bosu galvas, unikāli mīli, kosmētikas atlīdzības.
- **Melnais tronis** — sēdi, minjoni sanes tev meitenes, vīnu, un tu izvēlies **Kunga tonus** — glaimotās/naidīgās izvēles, kas ietekmē impērijas raksturu.

---

## 7. Stāsts un morāles ass

Nav "labais vs ļaunais" — ir **kāda veida ļaunais**.

### Ass: **Tirāns** ↔ **Mecēns**
Katra izvēle spēlē pabīda tevi uz vienu vai otru pusi.

- **Tirāns:** iekaro brutāli, minjoni izlaupa ciemus, NPC baidās, iedzīvotāji strādā verdziski. Ekonomika ātra, minjoni brutālāki. Beigas: draka pasauli.
- **Mecēns:** aizsargā zemniekus (jo vergu strādnieki ir vērtīgāki nekā līķi), minjoni klīst ar puķēm cepurēs, NPC lūdzas tavu vārdu. Ekonomika ilglaicīga, sabiedrotie stiprāki. Beigas: kļūsti par tumšo, bet cieņā turēto imperatoru.

### Trīs cēlieni
1. **Pamošanās** — reģioni 1–3. Atrast minjonu tipus, atgūt Kalnu.
2. **Iekarošana** — reģioni 4–6. Saduroties ar seno ordeņu paliekām.
3. **Impērija vai Bezdibenis** — reģ. 7 + fināla dungeons. Izvēlies impērijas likteni.

### Beigas (4)
- **Melnais Imperators** — pilnīgs tirāns, pasaule dreb.
- **Cieņā turētais Kungs** — mecenāta ceļš, gotiska stabilitāte.
- **Bezdibeņa Karotājs** — atbrīvo seno demonu, kas apēd Kalnu (un tevi).
- **Klusums** — atsakies no varas, atgriezies gulēt, minjoni izklīst.

---

## 8. Endgame (Diablo 4 stils)

Kad izspēlē kampaņu (~40–60 h), atveras:

- **Murgu Dungeoni** — grūtāku versiju dungeoni ar īpašiem affixiem, dropo augstākā retuma loot.
- **Iekarojuma karte** — sūti minjonu grupas uz misijām bez tevis. Idle progresija.
- **Pasaules Bosi** — parādās uz kartes ik pa laikam, sauc kopā ar draugiem (co-op vēlākā versijā).
- **Sezonas** (post-launch) — jauns mehāniskais twist ik pēc 3 mēnešiem (piem., "Sarkanā Sezona" — jauns sarkanais minjonu apakštips, jauns dungeon set, jauna kampaņa 5–10h).
- **Leaderboardi** — ātrākā Melnā Kalna atjaunošana, augstākais Murgu Dungeon.

---

## 9. Co-op (post-MVP)

- **2–4 spēlētāji.** Katrs kungs ar savu minjonu armiju.
- Šķīrējs: minjonu limits samazināts par 30% co-op režīmā (citādi ekrāns pilns).
- **PvP arēna** — kungs pret kungu, minjonu armijas duelis. Karnevāls Melnajā Kalnā.

---

## 10. Minimālais spēlējamais prototips (MVP)

- Viens reģions: **Pelnu Purvs**.
- **Sūkšņu** minjoni (10 gab. limits), pilna komandu sistēma.
- Spēlētāja pamata cīņa + viena aktīvā prasme + izvairīšanās.
- Divi ienaidnieku tipi + viens mini-boss.
- Viens dungeon (5 telpas, boss beigās).
- Melnais Kalns kā vienkāršs hub: tronis, minjonu bedre, kalējs.
- Loot ar trim retumiem (parasts, rūdīts, sens).
- Level up līdz 15.
- Saglabāšana un nāves cikls.

**Laiks solo:** 6–9 mēneši MVP.
**Laiks nelielai komandai (3–5 cilv.):** 3–4 mēneši MVP.

---

## 11. Tehniskais stack (UE5)

- **UE 5.4+** ar **Lumen** (gotikai dinamiska gaisma ir must) un **Nanite**.
- **C++** kodolam: `KungsCharacter`, `MinionBase`, `MinionAIController`, `MinionCommandComponent`, `LootManager`, `SaveGameSystem`.
- **Blueprints** UI, prasmēm, quest scripts.
- **GAS (Gameplay Ability System)** prasmēm un statiem.
- **Behavior Trees + Blackboard** — minjonu un ienaidnieku AI.
- **Enhanced Input** vadībai.
- **Niagara** minjonu partiklu efektiem (dūmi, uguns, mirdzumi).
- **Metasounds** dinamiskam audio.

### Kritiskās sistēmas dizains
- **`MinionCommandComponent`** uz spēlētāja — apstrādā RMB komandas, sūta uz minjonu grupu.
- **`MinionAIController`** — katram minjonam sava; formation, follow, attack, retreat states.
- **`MinionGroupSubsystem`** (World Subsystem) — tur globālo minjonu sarakstu, tipu limitus.
- **`LootManager`** — Diablo-style loot roll ar affix generation.

---

## 12. Māksla un audio

### Māksla
- **Stilizēti low-poly** ar spēcīgu apgaismojumu (līdzīgs Death's Door / Hades noskaņai).
- Reālistiska AAA māksla vienam cilvēkam nav sasniedzama — stils to kompensē.
- **Minjoni:** ~1m augsti, karikatūriski, ar lielām acīm un ieročiem, kas viņiem lielāki par galvu. Vienlaicīgi draudīgi un smieklīgi.
- **Bosi:** monumentāli, mazliet H.R. Giger + Bloodborne.

### Audio
- Klusa, minimāla mūzika. Bass, korālis, iestrēguši zvani, ērģeles.
- Minjoni murmina savā valodā (kaut kas starp Grot un Ork no Warcraft) — humoris tikai skaņā.
- Kad bosu cīņa — pievienojas trokšņainas ērģeles.
- **Kunga balss** izvēles brīžos — dziļa, mierīga, tumša.

---

## 13. Salīdzinājums ar iedvesmām

| Aspekts               | Overlord (2007)         | Diablo IV                | *Kalna Kungs* (mēs)         |
|-----------------------|--------------------------|--------------------------|-----------------------------|
| Skats                 | Third-person aiz muguras | Isometrisks              | **Isometrisks**             |
| Minjoni               | Jā, 4 tipi               | Nē (tikai sabiedrotie)   | **Jā, 4 gotiski tipi**      |
| Loot                  | Ierobežots               | Blīvs, definējošs        | **Blīvs, Diablo-stila**     |
| Prasmju koks          | Vienkāršs                | Sarežģīts + Paragon      | **Sarežģīts + Paragon**     |
| Endgame               | Nav                      | Murgu D., sezonas        | **Murgu D., sezonas, idle** |
| Morāle                | Labais/ļaunais ass       | Nav                      | **Tirāns/Mecēns ass**       |
| Tonis                 | Melns humors             | Nopietns, gotisks        | **Abu miksts**              |
| Hub                   | Tumšais Tornis           | Kyovashad u.c.           | **Melnais Kalns (aug)**     |

---

## 14. Nākamie soļi

1. **Šis dokuments** ✅
2. **Prototipa arhitektūra** — detalizēts C++ klašu sarakts un saites.
3. **`MinionCommandComponent`** — pirmais kritiskais kods, pierāda mehāniku.
4. **Vienkārša aina** ar spēlētāju + 3 dumji minjoni + 1 ienaidnieks.
5. **Loot generation** — pirmā versija.
6. **MVP task list** ar konkrētiem uzdevumiem un aprēķinu.

---

*Šis ir dzīvs dokuments. Katrs elements ir apspriežams — pastāsti, kas nepatīk, un mainām.*
