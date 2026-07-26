# Aspektu Katalogs

*80 Aspektu pilna specifikācija — mehānika, drop avoti, ideālie buildi, broken potenciāls.*

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

# 3. Klases-Specifiskie Aspekti (48)

## 3.1. Bruņinieks (8)

### #33. **Krusta Karogs**
*Bruņinieks · Ieroči · Multiplicative · ★★★★☆*
> Krusta Zvans radius **+[30-60%]**, dmg **×[+15-25%]**.
- Kodekss: Kritušā Ordeņa Kripta (Debesu Virsotne, Murgu T40+)
- Builds: visi Bruņinieks buildi

### #34. **Falangas Ābece**
*Bruņinieks · Rumpis · Transformation · ★★★★★*
> Sūkšņu pastāvība 2× (mirst uz pusi retāk).
- Reliktu-only
- Builds: Falangas Tank (kritiski)

### #35. **Zvana Atbalss**
*Bruņinieks · Ieroči · Proc · ★★★★☆*
> Vairoga Sitiens hit **3** mērķus, katrs ģenerē **[3-6]** Grēksūdzi.
- Kodekss: Zvana Sirds Kripta (Debesu Virsotne)
- Builds: Heal-Tank (resurs generation)

### #36. **Kritušā Ordeņa Karogs**
*Bruņinieks · Amulets · Multiplicative · ★★★★★*
> Kad tu block ar vairogu, minjoni tuvāk 8m **×[+20-35%]** dmg **3s**.
- Kodekss: Ordeņa Karogs Kripta (Debesu Virsotne)
- Builds: Falangas Tank (kritiski)

### #37. **Grēksūdzes Piespriedu**
*Bruņinieks · Rokas · Additive · ★★★☆☆*
> Kad tu blockē, **+[3-6]** Grēksūdze papildu.
- Kodekss: Bīskapa Roku Kripta (Debesu Virsotne)
- Builds: Heal-Tank

### #38. **Nemirstīgā Zvana**
*Bruņinieks · Amulets · Proc · ★★★★★*
> Kad HP zem **25%**, Grēksūdzes Zvans auto-trigers 1× per cīņa (bezmaksas).
- Reliktu-only
- Builds: Heal-Tank (kritiski)

### #39. **Katedrāles Vēls**
*Bruņinieks · Rumpis · Multiplicative · ★★★☆☆*
> Barrier dod **[15-25%]** papildu HP heal, kad izzūd.
- Kodekss: Katedrāles Kripta (Debesu Virsotne)
- Builds: Heal-Tank

### #40. **Zvana Sprādzien**
*Bruņinieks · Ieroči · Transformation · ★★★★☆*
> Zvana Metiens eksplodē pret **3** tuvākajiem mērķiem (AoE mainīgums).
- Kodekss: Uguns Zvana Kripta (Debesu Virsotne)
- Builds: Zvana Uguns Hibrīds

---

## 3.2. Berserker (8)

### #41. **Vēja Ceļš**
*Berserker · Ieroči · Proc · ★★★★☆*
> Vēja Vējš dod **+[35-50%]** AS **4s** pēc izmantošanas.
- Kodekss: Vēju Māja (Asins Arēna)
- Builds: Dual-Wield DPS

### #42. **Divu Krāsu Asmeņi**
*Berserker · Ieroči · Multiplicative · ★★★★★*
> Otrs sitiens combo (Divu Zobenu Deja) dubultā dmg (ne 60%, bet 120% weap).
- Reliktu-only
- Builds: Dual-Wield DPS (kritiski)

### #43. **Asins Ātruma**
*Berserker · Amulets · Multiplicative · ★★★★☆*
> Ja tev virs **80** Rage, AS **×[+15-25%]**.
- Kodekss: Asins Kripta (Asins Arēna)
- Builds: visi

### #44. **Neapstādināmais**
*Berserker · Ķivere · Transformation · ★★★★☆*
> Asins Trakuma defensive penalty samazināts uz **-[15-20%]** (bija -50%).
- Kodekss: Trakuma Sirds Kripta (Asins Arēna)
- Builds: Dual-Wield DPS, Rage-Sustain

### #45. **Karoga Sirds**
*Berserker · Amulets · Additive · ★★★☆☆*
> Kliedziena Signāls dod **+[15-25%]** DR grupai **6s**.
- Kodekss: Karoga Kripta (Asins Arēna)
- Builds: Armijas Kliedzējs, grupas kontenta

### #46. **Trakuma Katlaputiņa**
*Berserker · Ieroči · Multiplicative · ★★★★★*
> Kad Rage max, **+[20-40%]** overpower dmg. Iesaistīts broken build combos.
- Reliktu-only
- Builds: Rage-Sustain (broken combo)

### #47. **Kliedziena Vējš**
*Berserker · Amulets · Additive · ★★★★☆*
> Kara Kliedziens cooldown **-[35-50%]**.
- Kodekss: Kliedziena Kripta (Asins Arēna)
- Builds: Armijas Kliedzējs

### #48. **Karaļa Zvans**
*Berserker · Amulets · Additive · ★★★☆☆*
> Visi minjoni ap tevi **+[10-20%]** max life.
- Kodekss: Karaļa Māja (Asins Arēna)
- Builds: Armijas Kliedzējs

---

## 3.3. Alkas Kungs (8)

### #49. **Bagātā Sirds**
*Alkas Kungs · Ieroči · Transformation · ★★★★★*
> Zelta Lietus monētas ranged dubultā (30m vietā 15m); jo vairāk zelts zemē, jo lielāks tavs dmg.
- Reliktu-only
- Builds: Projectile Spam (kritiski)

### #50. **Alkas Aura**
*Alkas Kungs · Amulets · Multiplicative · ★★★★☆*
> Alkas Zvana buff dubultā ilgums (**30s** vietā 15s).
- Kodekss: Zvana Kripta (Zelta Raktuves)
- Builds: visi

### #51. **Monētu Sirds**
*Alkas Kungs · Josta · Utility · ★★★☆☆*
> Zelts pastāv uz zemes **2× ilgāk** pirms izzūd.
- Kodekss: Monētu Kripta (Zelta Raktuves)
- Builds: Projectile Spam (sinerģē ar Bagātā Sirds)

### #52. **Sausās Sirds**
*Alkas Kungs · Ķivere · Multiplicative · ★★★★★*
> Sausā Sirds patēriņš vēl **-[20-40%]** (kopā ar keystone = 20 zelta/sec).
- Reliktu-only
- Builds: Ekonomika (kritiski)

### #53. **Zelta Sirdsdedzis**
*Alkas Kungs · Amulets · Proc · ★★★★☆*
> Pie **0 zelta**, Sausā Sirds automātiski aktīvs **5s** bezmaksas.
- Kodekss: Bezcelis Kripta (Zelta Raktuves)
- Builds: Ekonomika (emergency oh-shit button)

### #54. **Alkas Zvana Kronis**
*Alkas Kungs · Amulets · Additive · ★★★☆☆*
> Alkas Zvans arī uzkrāj Zelta **[300-500]/sec** **15s**.
- Kodekss: Alkas Kripta (Zelta Raktuves)
- Builds: Ekonomika (feeds Sausā Sirds)

### #55. **Divi Golemi**
*Alkas Kungs · Ieroči · Transformation · ★★★★★*
> Rāceles Ķērājs sauc **2** vienlaikus (nevis 1).
- Kodekss: Konstrukta Kripta (Zelta Raktuves, Murgu T50+)
- Builds: Golemu Kalts (kritiski)

### #56. **Metāla Kāzas**
*Alkas Kungs · Amulets · Transformation · ★★★★☆*
> Golemi apstāj tevi kā vairogus (agro magnēts) — DR **+[20-35%]** ja Golems tuvāk 3m.
- Kodekss: Kāzu Kripta (Zelta Raktuves)
- Builds: Golemu Kalts

---

## 3.4. Mežonis Kungs (8)

### #57. **Puves Migla Aspekts**
*Mežonis · Ieroči · Additive · ★★★★☆*
> Puvusī Migla radius **+[20-35%]**.
- Kodekss: Puves Migla Kripta (Puvušais Mežs, Murgu T30+)
- Builds: DoT Spam, Zone Control

### #58. **Puves Karogs**
*Mežonis · Amulets · Transformation · ★★★★★*
> DoT stack cap **5 → 8**.
- Kodekss: Karogs Kripta (Puvušais Mežs)
- Builds: DoT Spam (kritiski)

### #59. **Ērceles Ceļš**
*Mežonis · Ieroči · Additive · ★★★☆☆*
> Ērceles Sitiens uzliek **2** poison stackus vietā 1.
- Kodekss: Ērceles Kripta (Puvušais Mežs)
- Builds: DoT Spam

### #60. **Zvēra Ceļš**
*Mežonis · Zābaki · Multiplicative · ★★★★☆*
> Vilka Ādā MS **+[70-100%]** (10m vietā 8m per second).
- Kodekss: Zvēra Kripta (Puvušais Mežs)
- Builds: Beast Melee

### #61. **Lāca Sīvība**
*Mežonis · Ieroči · Multiplicative · ★★★★★*
> Vilka Ādā melee dmg vēl **×[+15-25%]**.
- Kodekss: Lāca Sirds Kripta (Ledus Kroņi)
- Builds: Beast Melee (kritiski)

### #62. **Neapstādināmais Zvērs**
*Mežonis · Ķivere · Transformation · ★★★★☆*
> Vilka Ādā imunī pret roots/stuns.
- Kodekss: Zvēra Kronis Kripta (Puvušais Mežs)
- Builds: Beast Melee (QoL, kritiskās situācijās)

### #63. **Sakņu Karogs**
*Mežonis · Amulets · Additive · ★★★★☆*
> Sakņu Kroņa radius vēl **+[2-3]m** (13-14m ar keystone).
- Kodekss: Sakņu Kripta (Puvušais Mežs)
- Builds: Zone Control (kritiski)

### #64. **Aizmirsta Zeme**
*Mežonis · Gredzens · Multiplicative · ★★★★☆*
> Ienaidnieki Sakņu Kroņā **-[15-25%]** MS papildu.
- Kodekss: Aizmirsta Kripta (Puvušais Mežs)
- Builds: Zone Control

---

## 3.5. Bezēnu Zaglis (8)

### #65. **Divu Soļu Karogs**
*Zaglis · Amulets · Transformation · ★★★★★*
> Ēnu Solis charges **+1** (2 → 3 ar keystone).
- Reliktu-only
- Builds: Teleport-Backstab (kritiski)

### #66. **Klona Ceļš**
*Zaglis · Ieroči · Multiplicative · ★★★★★*
> Klons backstab dubultā dmg (100% weap vietā 50%).
- Reliktu-only
- Builds: Teleport-Backstab (kritiski)

### #67. **Ēnu Solis Ass**
*Zaglis · Zābaki · Proc · ★★★★☆*
> Ēnu Solis reset cooldown, ja mērķis mirst **3s** laikā.
- Kodekss: Vēju Solis Kripta (Tuksneša Bāzars)
- Builds: Teleport-Backstab (chain reakcijas)

### #68. **Melnās Slēpes Aspekts**
*Zaglis · Ķivere · Multiplicative · ★★★★★*
> Naktsāda Ēna izmaksa **-[35-50%]** (efektīvs 15-20 Ēna).
- Reliktu-only
- Builds: Permanent Invis (kritiski)

### #69. **Neredzamā Krunkas**
*Zaglis · Ieroči · Proc · ★★★★☆*
> Visi hiti neredzamā stāvoklī stackē Vulnerable **1s** uz mērķi.
- Kodekss: Neredzamā Kripta (Tuksneša Bāzars)
- Builds: Permanent Invis (Vulnerable spam)

### #70. **Ēnas Sirds Karogs**
*Zaglis · Amulets · Additive · ★★★★☆*
> Ēna regen **+[15-30%]** pastāvīgi.
- Kodekss: Ēna Sirds Kripta (Tuksneša Bāzars)
- Builds: Permanent Invis, Teleport-Backstab

### #71. **Zaļču Karogs**
*Zaglis · Amulets · Multiplicative · ★★★★☆*
> Zaļči **×[+20-40%]** dmg neredzamā stāvoklī.
- Kodekss: Zaļču Karogs Kripta (Tuksneša Bāzars)
- Builds: Zaļču Armija

### #72. **Ēnas Sinerģija**
*Zaglis · Gredzens · Proc · ★★★☆☆*
> Zaļču crits dod Ēna **2× ātrāk** (papildu 2 pt per crit).
- Kodekss: Sinerģija Kripta (Tuksneša Bāzars)
- Builds: Zaļču Armija

---

## 3.6. Vientuļais Sniegurgs (8)

### #73. **Sasalis Karogs**
*Sniegurgs · Ieroči · Additive · ★★★★☆*
> Sasalis Kaps ilgums **+[1-2]s** (kopā 5-7s).
- Kodekss: Sasalis Kripta (Ledus Kroņi, Murgu T30+)
- Builds: Freeze Nuke (kritiski)

### #74. **Ledus Šauta Ceļš**
*Sniegurgs · Ieroči · Multiplicative · ★★★★★*
> Ledus Šauts piercing **+[30-50%]** dmg katrā mērķī pēc pirmā (exponenciāls stack).
- Reliktu-only
- Builds: Freeze Nuke (kritiski)

### #75. **Kroņa Sirds**
*Sniegurgs · Amulets · Multiplicative · ★★★★☆*
> Frost dmg **×[+15-25%]** pret ienaidniekiem ar zemu HP (<40%).
- Kodekss: Kroņa Sirds Kripta (Ledus Kroņi)
- Builds: Freeze Nuke, Torņa Redzējums

### #76. **Skeleta Karogs**
*Sniegurgs · Amulets · Transformation · ★★★★★*
> Skeletu skaits **+2** (kopā ar keystone = 12 skeleti).
- Reliktu-only
- Builds: Skeletu Karaļvalsts (kritiski)

### #77. **Sasala Karogs (Sk)**
*Sniegurgs · Sekundārais · Proc · ★★★★☆*
> Skeletu Frost hits pastāvīgi sasaldē mērķi (**1s** per hit).
- Kodekss: Skeletu Sasala Kripta (Ledus Kroņi)
- Builds: Skeletu Karaļvalsts

### #78. **Torņa Karogs**
*Sniegurgs · Ķivere · Multiplicative · ★★★★★*
> Torņa Redzējums patēriņš vēl **-[15-25%]** (efektīvs 1/sec).
- Reliktu-only
- Builds: Torņa Redzējums (kritiski)

### #79. **Vientuļā Sirds**
*Sniegurgs · Amulets · Multiplicative · ★★★★☆*
> Kad esi **15m+** no visiem sabiedrotajiem, **×[+20-35%]** dmg.
- Kodekss: Vientuļā Kripta (Ledus Kroņi)
- Builds: Torņa Redzējums

### #80. **Ledus Kroņa Kronis**
*Sniegurgs · Rumpis · Additive · ★★★★☆*
> Visas ledus prasmes **+[10-20%]** dmg.
- Kodekss: Ledus Kroņa Kripta (Ledus Kroņi)
- Builds: Freeze Nuke, Torņa Redzējums

---

# 4. Kodekss Dungeon Map (kur farmot)

Katrs Kodekss ir pieejams **specifiskā dungeonā** — pabeidzot to pirmoreiz, atslēdz Aspektu Kodeksa versijā (75% max roll).

## 4.1. Pēc reģiona

| Reģions            | Kodekss Aspekti Count | Ievērojami                          |
|--------------------|-----------------------|-------------------------------------|
| Debesu Virsotne    | 12                    | Bruņinieks + universālie ofensīvi   |
| Asins Arēna        | 9                     | Berserker + AS/rage                 |
| Zelta Raktuves     | 11                    | Alkas Kungs + defensīvie            |
| Puvušais Mežs      | 10                    | Mežonis + DoT/spread                |
| Tuksneša Bāzars    | 9                     | Zaglis + shadow/dodge               |
| Ledus Kroņi        | 8                     | Sniegurgs + frost/resistance        |
| Melnais Kalns      | 2                     | Endgame utility (Nāves, Kalts)      |

Kopā: **61 Aspekti Kodeksā**. **19 Aspekti tikai Reliktu-only** (skat. §5).

## 4.2. Farm ceļvedis pa buildiem

Katras build build guide ir sarakstā **3-5 galvenie Aspekti** — vairāki no tiem ir Kodeksā, daži Reliktu-only. Ideāls farm order:

1. **Kodekss versija first** (garantēts, 75% roll).
2. **Reliktu chase** — turpini Murgu Dungeonus, cerē uz drop ar labāko roll.
3. **GA chase** — kad drop nāk, cerē uz 4 GA.
4. **Extract pie Okultista** — 100% roll no Reliktu → imprint uz Sen ar 4 GA.

---

# 5. Reliktu-only Aspekti (nav Kodeksā)

19 Aspekti nekad neatslēdzas Kodeksā — tikai dropo kā Reliktu items. Šie ir **retākie, spēles-maiņas** Aspekti:

| # | Aspekts                    | Klase          |
|---|----------------------------|----------------|
| 8 | Bezdibeņa Solis            | Universāls     |
| 21| Zvana Karogs (nāves izvairīšanās) | Universāls |
| 32| Melnā Sirds (M)            | Universāls (minjonu) |
| 34| Falangas Ābece             | Bruņinieks     |
| 38| Nemirstīgā Zvana           | Bruņinieks     |
| 42| Divu Krāsu Asmeņi          | Berserker      |
| 46| Trakuma Katlaputiņa        | Berserker      |
| 49| Bagātā Sirds               | Alkas Kungs    |
| 52| Sausās Sirds               | Alkas Kungs    |
| 55| Divi Golemi                | Alkas Kungs (Kodeksā T50+, arī Reliktu) |
| 58| Puves Karogs               | Mežonis        |
| 61| Lāca Sīvība                | Mežonis        |
| 65| Divu Soļu Karogs           | Zaglis         |
| 66| Klona Ceļš                 | Zaglis         |
| 68| Melnās Slēpes Aspekts      | Zaglis         |
| 74| Ledus Šauta Ceļš           | Sniegurgs      |
| 76| Skeleta Karogs             | Sniegurgs      |
| 78| Torņa Karogs               | Sniegurgs      |

**Drop chance:** ~2-3% no augsta tiera Reliktu drops.
**Farm laiks:** vidēji **20-40h per Reliktu-only Aspekts**.

---

# 6. Build sinerģiju kopsavilkums

Kuras Aspekta trio ir must-have katram buildam? Skat. build guide dokumentos §3 katrai klasei.

**Top 10 Aspekti kopā par visiem buildiem (broken potenciāls):**

| Vieta | Aspekts                | Klase       | Iesaistīts kombu |
|-------|------------------------|-------------|--------------------|
| 1     | Melnā Sirds (M)        | Universāls  | Broken build "Melnā Sirds stacking" |
| 2     | Divi Ēnu Klonis Ass    | Zaglis (Paragon) | Bezēnu Zaglis Ēnu Kaskade |
| 3     | Falangas Ābece         | Bruņinieks  | Falangas Tank sustain |
| 4     | Nemirstīgā Zvana       | Bruņinieks  | Heal-Tank Grēksūdzes Bomba |
| 5     | Trakuma Katlaputiņa    | Berserker   | Rage-Sustain overpower |
| 6     | Bagātā Sirds           | Alkas Kungs | Projectile Spam |
| 7     | Puves Karogs           | Mežonis     | DoT stack 8 → cascade |
| 8     | Klona Ceļš             | Zaglis      | Teleport-Backstab kaskade |
| 9     | Ledus Šauta Ceļš       | Sniegurgs   | Freeze Nuke piercing |
| 10    | Bezdibeņa Solis        | Universāls  | Multiple broken buildi |

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
