# Aspektu Katalogs

*56 Aspektu pilna specifikācija (32 universāli + 24 klases-specifiski) — mehānika, drop avoti, ideālie buildi, broken potenciāls.*

Pamata Aspektu mehānika — skat. `GameDesignDocument.md` §8.5.

---

## 1. Ievads

Aspekti ir mehāniski efekti, kas maina spēli. Katrs **Reliktu** items nes Aspektu; **Sen** items var iegūt Aspektu, kad Okultists to imprintē.

### 1.1. Iegūšanas ceļi

- **Kodekss (Codex of Power):** pabeidzot dungeon, atslēdz **Kodeksa versija** aspekta — 75% no max roll. Pastāvīgi pieejams re-imprintam.
- **Reliktu extract:** pie Okultista izlobā Aspektu no Reliktu item — 100% roll, bet vienreizējs (item pazūd).
- **Reliktu-only Aspekti:** ~30 aspekti nav Kodeksā, tikai no Reliktu drops (retākie, spēles-maiņas).

### 1.2. Slot restrictions

Katrs Aspekts iet konkrētā slotā:
- **Ofensīvie** → Ieroči, Rokas, Amulets, Gredzeni
- **Defensīvie** → Rumpis, Ķivere, Kājas, Vairogs, Amulets
- **Utility** → Zābaki, Josta, Amulets, Gredzeni
- **Minjonu** → Rumpis, Sekundārais, Amulets, Gredzeni

Amulets un Gredzeni ir **universāli** — jebkurš tips.

### 1.3. Multiplier style

- **Additive:** pievieno additive bucketam.
- **Multiplicative:** savs Reizinātājs multipliers pipeline.
- **Proc:** trigger-based (chance uz hit vai kondicionāls).
- **Transformation:** maina prasmes mehāniku.

### 1.4. Broken potenciāls (rating)

- ★☆☆☆☆ — Small quality-of-life, neietekmē build būtiski.
- ★★☆☆☆ — Solid choice, +5-15% DPS.
- ★★★☆☆ — Strong, +15-30% DPS, viena no top-3 slotam.
- ★★★★☆ — Build-defining, must-have kādam buildam.
- ★★★★★ — Broken potenciāls, iesaistīts trillion-damage kombos.

---

# 2. Universālie Aspekti (32)

## 2.1. Ofensīvie (8)

### #1. **Kritušā Sirds**
*Ofensīvs · Ieroči/Rotaslietas · Proc · ★★★☆☆*
> Nākošais uzbrukums pēc **3s** no cīņas sākuma ir garantēts crit ar **[+50% / +100%]** papildu dmg.
- Kodekss: Kritušā Krusta Kripta (Debesu Virsotne)
- Broken: pair ar Karagājienis Paragon "Kara Cīņas Vaids"
- Builds: visi burst DPS

### #2. **Vēja Ass**
*Ofensīvs · Ieroči · Additive · ★★★☆☆*
> Attack Speed **+[10-20%]** ja nesaņem bojājumu **5s**.
- Kodekss: Vēju Māja (Asins Arēna)
- Builds: Berserker Dual-Wield, Zaglis, Sniegurgs

### #3. **Krusta Uguns**
*Ofensīvs · Ieroči/Amulets · Additive · ★★☆☆☆*
> Elemental dmg (Fire/Frost/Lightning/Shadow/Holy) **+[10-20%]**.
- Kodekss: Uguns Kripta (Zelta Raktuves)
- Builds: hibrīdi ar elementāro fokusu

### #4. **Zvana Sirds**
*Ofensīvs · Amulets · Multiplicative · ★★★★☆*
> Kad izmanto ult, visas prasmes **×[+15-30%]** dmg **8s**.
- Kodekss: Zvana Zvēresta Kripta (Debesu Virsotne)
- Builds: visi ult-centric buildi

### #5. **Aizmirsts Vaids**
*Ofensīvs · Amulets/Gredzens · Multiplicative · ★★★★☆*
> Kritiskais dmg **×[+8-15%]** ja Primary Attribute virs **800**.
- Kodekss: Pazudušā Bīskapa Kripta (Debesu Virsotne)
- Builds: visi late-game buildi (kad Primary Attribute pieaug pār 800)

### #6. **Sasala Zvana**
*Ofensīvs · Amulets/Gredzens · Multiplicative · ★★★★☆*
> Vulnerable dmg **×[+15-25%]**.
- Kodekss: Ledus Katedrāle (Ledus Kroņi)
- Builds: visi Vulnerable-heavy — Zaglis, Sniegurgs, DoT buildi

### #7. **Nemirstīgs Karogs**
*Ofensīvs · Rokas/Amulets · Additive · ★★★☆☆*
> Kritiskā chance **+[3-7%]**.
- Kodekss: Nemirstīgā Trono Kripta (Asins Arēna)
- Builds: crit-focused DPS visi

### #8. **Bezdibeņa Solis**
*Ofensīvs · Ieroči · Proc · ★★★★★*
> Ja iesit priekš **10% max HP** damage vienā hit, nākošais hit dubultā dmg. Iesaistīts broken build combos.
- Reliktu-only (nav Kodeksā)
- Builds: Berserker Rage-Sustain (>30% HP buff), Zaglis Kaklaslazds

---

## 2.2. Defensīvie (8)

### #9. **Bruņu Vaids**
*Defensīvs · Bruņas · Additive · ★★☆☆☆*
> **+[10-20%]** Armor.
- Kodekss: Bruņu Kalts (Zelta Raktuves)
- Builds: visi tanku buildi

### #10. **Sirds Solis**
*Defensīvs · Bruņas · Additive · ★★★☆☆*
> Max HP **+[8-15%]**.
- Kodekss: Sirds Kripta (Ledus Kroņi)
- Builds: Bruņinieks Heal-Tank (kritiski), Berserker Rage-Sustain

### #11. **Zvana Barjēra**
*Defensīvs · Bruņas · Additive · ★★★☆☆*
> Barrier Gen **+[20-35%]**.
- Kodekss: Zvana Sirds Kripta (Debesu Virsotne)
- Builds: Bruņinieks Heal-Tank

### #12. **Kritušā Zvana**
*Defensīvs · Bruņas/Amulets · Multiplicative · ★★★☆☆*
> DR **+[8-15%]** ja HP virs **80%**.
- Kodekss: Krusta Sardzes Kripta (Debesu Virsotne)
- Builds: Bruņinieks Falangas Tank, Alkas Golemu Kalts

### #13. **Ledus Sirds**
*Defensīvs · Bruņas · Additive · ★★☆☆☆*
> Elemental Resistance (visi) **+[20-40%]**.
- Kodekss: Sasala Trono (Ledus Kroņi)
- Builds: universāls defensive

### #14. **Krusta Barjēra**
*Defensīvs · Bruņas · Proc · ★★★★☆*
> Kad Barjera saplīst, dziedini **[10-20%]** max HP.
- Kodekss: Nemirstīgā Bīskapa Kripta (Debesu Virsotne)
- Builds: Bruņinieks Heal-Tank (kritiski)

### #15. **Sardzes Zīme**
*Defensīvs · Bruņas · Multiplicative · ★★★★☆*
> Ja tuvāk 3 minjoniem 5m, DR **+[15-25%]**.
- Kodekss: Sardzes Kripta (Zelta Raktuves)
- Builds: visi minjonu buildi ar tank fokusa

### #16. **Vecā Vaids**
*Defensīvs · Amulets · Multiplicative · ★★★☆☆*
> Overpower dmg reduction **[15-30%]** pret tevi.
- Kodekss: Vecā Kaudze (Puvušais Mežs)
- Builds: PvP orientēti, pret bosiem ar Overpower prasmēm

---

## 2.3. Utility (8)

### #17. **Vēja Solis**
*Utility · Zābaki · Additive · ★★☆☆☆*
> MS **+[8-15%]** ārpus cīņas.
- Kodekss: Vēju Māja (Asins Arēna)
- Builds: visi (traversal QoL)

### #18. **Zvana Kronis**
*Utility · Amulets/Gredzens · Additive · ★★★☆☆*
> Cooldown reduction **+[3-7%]**.
- Kodekss: Cooldown Kripta (Ledus Kroņi)
- Builds: visi ult-centric

### #19. **Resurss Sirds**
*Utility · Josta/Gredzens · Additive · ★★★☆☆*
> Signāles resurss regen **+[15-30%]** (Grēksūdze/Rage/Zelts/Sula/Ēna/Sala).
- Kodekss: Resurss Kripta (Zelta Raktuves)
- Builds: resurss-heavy — Bruņinieks Heal-Tank, Sniegurgs Torņa

### #20. **Kritušā Solis**
*Utility · Josta · Proc · ★★★★☆*
> Nomirstot, atmostas ar **[20-40%]** HP (**30 min** cd).
- Kodekss: Nāves Kripta (Melnais Kalns)
- Builds: visi hardcore, endgame

### #21. **Zvana Karogs**
*Utility · Ķivere · Proc · ★★★★★*
> Nāves izvairīšanās **[8-15%]** šansei (nāvējošs hit tiek atsvērts).
- Reliktu-only
- Builds: hardcore mode, Uber boss cīņa

### #22. **Krusta Sirds**
*Utility · Zābaki · Additive · ★★☆☆☆*
> Dodge chance **+[5-10%]**.
- Kodekss: Krusta Solis Kripta (Debesu Virsotne)
- Builds: Zaglis buildi

### #23. **Melnais Solis**
*Utility · Zābaki · Proc · ★★★★☆*
> Kad esi neredzams (jebkurš avots), MS **+[25-40%]**.
- Kodekss: Ēnas Kripta (Tuksneša Bāzars)
- Builds: Zaglis Permanent Invis

### #24. **Bezdibeņa Zvana**
*Utility · Amulets · Utility · ★★☆☆☆*
> Kaltēšanas ātrums **+[30-70%]** pie Kalēja/Alķīmiķa/Juveliera.
- Kodekss: Melnā Kalna Kalts (Melnais Kalns)
- Builds: QoL visi

---

## 2.4. Minjonu (8)

### #25. **Karaļa Sirds**
*Minjonu · Rumpis · Additive · ★★★☆☆*
> Minjonu HP **+[15-30%]**.
- Kodekss: Karaļa Trono (Zelta Raktuves)
- Builds: visi minjonu buildi

### #26. **Zvēras Ass**
*Minjonu · Ieroči/Amulets · Multiplicative · ★★★★☆*
> Minjonu dmg **×[+10-20%]** pret Vulnerable.
- Kodekss: Vulnerable Kripta (Puvušais Mežs)
- Builds: Zaglis Zaļču Armija, Sniegurgs Skeletu

### #27. **Barveža Karogs**
*Minjonu · Amulets · Transformation · ★★★★★*
> **+1** max minjonu.
- Kodekss: Barveža Kripta (Zelta Raktuves)
- Builds: visi minjonu buildi (broken potenciāls: 2× stacking gredzeni + amulets = +3 minjoni)

### #28. **Kritušā Karogs**
*Minjonu · Rumpis · Proc · ★★★★☆*
> Kad minjons mirst, tuvāk esošie minjoni **+[8-15%]** dmg **5s**.
- Kodekss: Nāves Karogs (Puvušais Mežs)
- Builds: high-turnover minjonu buildi (Berserker Armijas)

### #29. **Zvana Karogs (M)**
*Minjonu · Sekundārais · Additive · ★★★☆☆*
> Minjonu AS **+[10-20%]**.
- Kodekss: Zvana Māja (Debesu Virsotne)
- Builds: visi minjonu

### #30. **Sirds Solis (M)**
*Minjonu · Sekundārais · Additive · ★★☆☆☆*
> Minjonu MS **+[20-40%]**.
- Kodekss: Vēju Sirds (Asins Arēna)
- Builds: QoL minjonu buildi

### #31. **Vaida Zvans**
*Minjonu · Amulets · Multiplicative · ★★★★☆*
> Minjoni pieaug **[15-30%]** ātrāk pēc nāves.
- Kodekss: Nekad Nemirst Kripta (Puvušais Mežs)
- Builds: high-turnover — Berserker Armijas, Alkas Golemu

### #32. **Melnā Sirds (M)**
*Minjonu · Amulets · Multiplicative · ★★★★★*
> Katrs aktīvais minjons dod tev **+[1-2%]** dmg pastāvīgi (max 30 minjoni = +60% dmg).
- Reliktu-only
- Builds: max-minjonu buildi (broken potenciāls milzīgs)

---

# 3. Klases-Specifiskie Aspekti (24)

*8 per klase × 3 klases = 24 kopā.*

## 3.1. Pirmais Kungs (8)

### #33. **Meistara Karogs**
*Pirmais Kungs · Ieroči · Multiplicative · ★★★★☆*
> Master's Call reset cooldown, ja **kill** notiek **3s** laikā pēc izmantošanas.
- Kodekss: Kritušā Ordeņa Kripta (Melnais Tornis)
- Builds: Meistars pāri Visiem (kritiski)

### #34. **Kunga Sirds**
*Pirmais Kungs · Amulets · Additive · ★★★★☆*
> Dominance uzkrāj **+[20-40%]** no minjonu kills.
- Kodekss: Kunga Sirds Kripta (Melnais Tornis)
- Builds: visi 3 buildi

### #35. **Ceturto Kroni**
*Pirmais Kungs · Amulets · Multiplicative · ★★★★★*
> Kad grupā ir vismaz 5 minjoni no visiem 4 tipiem, tu iegūsti **×[+20-35%]** dmg pastāvīgi.
- Reliktu-only
- Builds: Meistars pāri Visiem, Kunga Impērija (kritiski)

### #36. **Aura Māc**
*Pirmais Kungs · Rumpis · Additive · ★★★☆☆*
> Dominance Aura ilgums **+[3-6s]** (kopā 18-21s).
- Kodekss: Aura Kripta (Melnais Tornis)
- Builds: visi

### #37. **Kunga Vairogs**
*Pirmais Kungs · Vairogs/Rumpis · Proc · ★★★★☆*
> Kad minjons mirst tuvāk 5m, tu iegūsti barjeru = **[3-6%]** max HP.
- Kodekss: Vairoga Kripta (Melnais Tornis)
- Builds: Sardzes Zvērests (kritiski)

### #38. **Dominance Karogs**
*Pirmais Kungs · Ieroči · Additive · ★★★☆☆*
> Domination Wave radius **+[30-50%]**.
- Kodekss: Domination Kripta (Melnais Tornis)
- Builds: Kunga Impērija

### #39. **Universālais Kalts**
*Pirmais Kungs · Amulets · Multiplicative · ★★★★★*
> Visu 4 tipu aurā bonuss vēl **×[+10-20%]** pastāvīgi.
- Reliktu-only
- Builds: Meistars pāri Visiem, Kunga Impērija

### #40. **Kunga Zvana Māc**
*Pirmais Kungs · Ieroči · Additive · ★★★★☆*
> Kunga Zvana ilgums **+[3-6s]** un cooldown **-[15-30%]**.
- Kodekss: Zvana Kripta (Melnais Tornis)
- Builds: visi

---

## 3.2. Otrs Kungs (8)

### #41. **Nordbergas Karogs**
*Otrs Kungs · Rumpis · Multiplicative · ★★★★☆*
> Sūkšņi + Zilspāri afinitāte vēl **×[+15-25%]**.
- Kodekss: Nordbergas Kripta (Nordberga Impērija)
- Builds: Hybrid Balance (kritiski)

### #42. **Impērijas Vaids**
*Otrs Kungs · Amulets · Multiplicative · ★★★★☆*
> Impēriskā Dusma uzkrāj **×3** pret "gaisajiem" ordeņiem (paladīni, mūki).
- Kodekss: Impērijas Kripta (Nordberga)
- Builds: visi (feed ult uptime)

### #43. **Divu Rokas Kronis**
*Otrs Kungs · Ieroči · Multiplicative · ★★★★★*
> Kad melee un spell hit **0.5s** laikā, **×[+60-100%]** dmg **3s**.
- Reliktu-only
- Builds: Hybrid Balance (kritiski)

### #44. **Melnās Zibens Kronis**
*Otrs Kungs · Ieroči · Additive · ★★★★★*
> Melnās Zibenis chain lec **+[3-5]** mērķiem papildu.
- Kodekss: Zibens Kripta (Nordberga)
- Builds: Chain Lightning (kritiski)

### #45. **Impērisks Vairogs**
*Otrs Kungs · Rumpis · Multiplicative · ★★★★☆*
> Impērija Vaidēs formā DR **+[20-35%]**.
- Kodekss: Impērija Vairoga Kripta (Nordberga)
- Builds: Permanent Empress (kritiski)

### #46. **Peldošā Sirds**
*Otrs Kungs · Zābaki · Transformation · ★★★★☆*
> Peldošā formā tu esi imunī pret melee attacks.
- Kodekss: Peldošā Kripta (Nordberga)
- Builds: Permanent Empress

### #47. **Impēriskā Sirds**
*Otrs Kungs · Amulets · Multiplicative · ★★★★★*
> Peldošā formā spells triple dmg (nevis 2×).
- Reliktu-only
- Builds: Permanent Empress (broken potenciāls ekstrēms)

### #48. **Zibens Kalts**
*Otrs Kungs · Ieroči · Multiplicative · ★★★★☆*
> Chain lightning lēciens **+[30-50%]** dmg per hop (nevis -20%).
- Kodekss: Zibens Kalts Kripta (Nordberga)
- Builds: Chain Lightning

---

## 3.3. Vecais Burvis (8)

### #49. **Realitātes Karogs**
*Vecais Burvis · Ķivere · Multiplicative · ★★★★★*
> Reality Rift patēriņš **-[20-40%]** (kopā ar keystone: efektīvs 1/sec).
- Reliktu-only
- Builds: Realitātes Karaļvalsts (kritiski)

### #50. **Torņa Sirds**
*Vecais Burvis · Amulets · Multiplicative · ★★★★★*
> Zilspāri spell power **×[+0.5-1.0]** papildu (kopā ×3.5-4).
- Reliktu-only
- Builds: Zilspāri Storm (kritiski)

### #51. **Vientuļā Sirds**
*Vecais Burvis · Amulets · Multiplicative · ★★★★☆*
> Kad esi **15m+** no visiem sabiedrotajiem, **×[+20-35%]** dmg.
- Kodekss: Vientuļā Kripta (Vecais Tornis)
- Builds: Realitātes Karaļvalsts

### #52. **Ledus Šauta Ceļš**
*Vecais Burvis · Ieroči · Multiplicative · ★★★★★*
> Arkānā Lauska piercing, katrā mērķī pēc pirmā **×[+30-50%]** dmg (eksponenciāls stack).
- Reliktu-only
- Builds: Realitātes Karaļvalsts, Zilspāri Storm

### #53. **Divu Cast Kronis**
*Vecais Burvis · Amulets · Proc · ★★★★★*
> Katrs spell cast dubultā **[15-30%]** chance (bez keystone) VAI patēriņš -25% (ar keystone).
- Reliktu-only
- Builds: Torņa Ass (kritiski)

### #54. **Time Wave Māc**
*Vecais Burvis · Ieroči · Additive · ★★★★☆*
> Time Wave slow **+[30-50%]** (kopā 60-80% slow).
- Kodekss: Time Wave Kripta (Vecais Tornis)
- Builds: Realitātes Karaļvalsts

### #55. **Arkānā Sinerģija**
*Vecais Burvis · Rumpis · Multiplicative · ★★★★☆*
> Zilspāri triple hits vietā viena per attack (3 zibens hits per cast).
- Kodekss: Sinerģija Kripta (Vecais Tornis)
- Builds: Zilspāri Storm

### #56. **Reality Kronis**
*Vecais Burvis · Zābaki · Additive · ★★★★☆*
> Realitātes Plaisa charges **+1** (kopā 3 ar Trono Māksla leg).
- Kodekss: Reality Kripta (Vecais Tornis)
- Builds: Torņa Ass

---

---

# 4. Kodekss Dungeon Map (kur farmot)

Katrs Kodekss ir pieejams **specifiskā dungeonā** — pabeidzot to pirmoreiz, atslēdz Aspektu Kodeksa versijā (75% max roll).

## 4.1. Pēc reģiona

| Reģions          | Kodekss Aspekti Count | Ievērojami                            |
|------------------|-----------------------|---------------------------------------|
| Melnais Tornis   | 12                    | Pirmais Kungs + universālie ofensīvi |
| Nordberga        | 10                    | Otrs Kungs + hybrid + shadow          |
| Vecais Tornis    | 12                    | Vecais Burvis + spell + frost         |
| Melnais Kalns    | 11                    | Endgame utility + defense + minjonu   |

Kopā: **45 Aspekti Kodeksā**. **11 Aspekti tikai Reliktu-only** (skat. §5). Kopā ar **56 Aspektiem** — 32 universāli + 24 klases-specifiski.

## 4.2. Farm ceļvedis pa buildiem

Katras build build guide ir sarakstā **3-5 galvenie Aspekti** — vairāki no tiem ir Kodeksā, daži Reliktu-only. Ideāls farm order:

1. **Kodekss versija first** (garantēts, 75% roll).
2. **Reliktu chase** — turpini Murgu Dungeonus, cerē uz drop ar labāko roll.
3. **GA chase** — kad drop nāk, cerē uz 4 GA.
4. **Extract pie Okultista** — 100% roll no Reliktu → imprint uz Sen ar 4 GA.

---

# 5. Reliktu-only Aspekti (nav Kodeksā)

11 Aspekti nekad neatslēdzas Kodeksā — tikai dropo kā Reliktu items. Šie ir **retākie, spēles-maiņas** Aspekti:

| # | Aspekts                    | Klase            |
|---|----------------------------|------------------|
| 8 | Bezdibeņa Solis            | Universāls       |
| 21| Zvana Karogs (nāves izvairīšanās) | Universāls|
| 32| Melnā Sirds (M)            | Universāls (M)   |
| 35| Ceturto Kroni              | Pirmais Kungs    |
| 39| Universālais Kalts         | Pirmais Kungs    |
| 43| Divu Rokas Kronis          | Otrs Kungs       |
| 47| Impēriskā Sirds            | Otrs Kungs       |
| 49| Realitātes Karogs          | Vecais Burvis    |
| 50| Torņa Sirds                | Vecais Burvis    |
| 52| Ledus Šauta Ceļš           | Vecais Burvis    |
| 53| Divu Cast Kronis           | Vecais Burvis    |

**Drop chance:** ~2-3% no augsta tiera Reliktu drops.
**Farm laiks:** vidēji **20-40h per Reliktu-only Aspekts**.

---

# 6. Build sinerģiju kopsavilkums

Kuras Aspekta trio ir must-have katram buildam? Skat. build guide dokumentos §3 katrai klasei.

**Top 10 Aspekti kopā par visiem buildiem (broken potenciāls):**

| Vieta | Aspekts                | Klase            | Iesaistīts kombu                       |
|-------|------------------------|------------------|----------------------------------------|
| 1     | Melnā Sirds (M)        | Universāls       | Broken build "Melnā Sirds stacking"    |
| 2     | Ceturto Kroni          | Pirmais Kungs    | Meistars pāri Visiem — pastāvīgs +30%  |
| 3     | Impēriskā Sirds        | Otrs Kungs       | Permanent Empress triple dmg           |
| 4     | Ledus Šauta Ceļš       | Vecais Burvis    | Realitātes Karaļvalsts eksponenciāls   |
| 5     | Divu Cast Kronis       | Vecais Burvis    | Torņa Ass double cast sinerģija        |
| 6     | Torņa Sirds            | Vecais Burvis    | Zilspāri Storm ×4 spell power          |
| 7     | Divu Rokas Kronis      | Otrs Kungs       | Hybrid Balance combo +100%             |
| 8     | Universālais Kalts     | Pirmais Kungs    | Meistars pāri Visiem stacking          |
| 9     | Realitātes Karogs      | Vecais Burvis    | Reality Rift pastāvīgs                 |
| 10    | Bezdibeņa Solis        | Universāls       | Multiple broken buildi                 |

---

# 7. Ekonomika

## 7.1. Aspekta uzstādīšanas cena

Pie Okultista:
- **Imprint Aspekts uz Sen item:** 100 000 zelta + 5 Zvēresta Pelni.
- **Imprint uz Reliktu (overwrite):** 250 000 zelta + 10 Zvēresta Pelni.
- **Reroll Aspekta roll (Kodekss version):** 500 000 zelta + 1 Melnā Rune.
- **Extract Aspekts no Reliktu:** 50 000 zelta + item pazūd.

## 7.2. Reliktu-only Aspekta iegūšanas ekonomika

- Vidēji **50h farm** per konkrētu Reliktu-only Aspekts (target droprate).
- **4 GA versija:** vēl **10× reizes retāk** — kopā **~500h per perfect roll**.

**Šī ir hardcore endgame chase**, kas ilgst gadu.

---

*Skaitļi ir sākuma balansa piedāvājums. Tuning notiek pēc playtesting.*
