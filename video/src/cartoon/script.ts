// Multenes "Cepumiņš Krikšķis" scenārijs: ainu garumi (sekundēs) un teicēja teksts.
// Maini tekstus šeit — titri pielāgosies automātiski.
export type SceneId =
  | "title"
  | "oven"
  | "shelf"
  | "leaving"
  | "ant"
  | "rain"
  | "bird"
  | "flight"
  | "night"
  | "return"
  | "family"
  | "end";

export type SceneDef = { id: SceneId; seconds: number; lines: string[] };

export const TITLE = ["Cepumiņš", "Krikšķis"];
export const MORAL = ["Nav jābūt perfektam,", "lai būtu īpašs."];
export const THE_END = "Beigas";

export const scenes: SceneDef[] = [
  { id: "title", seconds: 10, lines: [] },
  {
    id: "oven",
    seconds: 26,
    lines: [
      "Kādā agrā rītā mazā ceptuvē smaržoja pēc vaniļas un šokolādes.",
      "Krāsns nozvanīja — dzin! — un no tās izslīdēja paplāte ar cepumiem.",
      "Pats pēdējais uz paplātes bija Krikšķis.",
      "Viņam viena maliņa bija nedaudz nodrupusi.",
      "Bet Krikšķis par to vēl neko nezināja. Viņš vienkārši priecājās par pasauli.",
    ],
  },
  {
    id: "shelf",
    seconds: 28,
    lines: [
      "Cepumus salika plauktā, un drīz ieradās pirmie pircēji.",
      "Apaļie un gludie cepumi cits pēc cita devās prom.",
      "— Mani! Izvēlieties mani! — sauca Krikšķis.",
      "Taču visi skatījās uz viņa nodrupušo maliņu un izvēlējās citu.",
      "Vakarā plauktā bija palicis viņš viens.",
    ],
  },
  {
    id: "leaving",
    seconds: 24,
    lines: [
      "— Varbūt es neesmu pietiekami labs cepums? — viņš nopūtās.",
      "Bet tad Krikšķis ieraudzīja pa durvīm spīdam vakara sauli.",
      "— Ja neviens neatnāk pie manis, es pats došos pasaulē!",
      "Viņš nolēca no plaukta un devās ceļā.",
    ],
  },
  {
    id: "ant",
    seconds: 30,
    lines: [
      "Pļavā Krikšķis satika skudru Tipu.",
      "Tipa stiepa smagu graudu un bija ļoti izsalkusi.",
      "— Man nav daudz, bet es varu padalīties, — teica Krikšķis.",
      "Viņš nolauza mazu drupatiņu un pasniedza to skudrai.",
      "— Tas ir gardākais, ko esmu ēdusi! — iesaucās Tipa.",
      "Tā Krikšķis ieguva savu pirmo draugu.",
    ],
  },
  {
    id: "rain",
    seconds: 28,
    lines: [
      "Pēkšņi debesis satumsa, un sāka līt.",
      "Cepumiem lietus ir bīstams — tie kļūst mīksti!",
      "— Skrien man līdzi! — sauca Tipa.",
      "Abi paslēpās zem lielas sēnes cepures.",
      "Kad lietus pārstāja, debesīs iemirdzējās varavīksne.",
    ],
  },
  {
    id: "bird",
    seconds: 30,
    lines: [
      "Tad no debesīm nolaidās zīlīte Čiepa.",
      "— Kāds kraukšķīgs cepums! — viņa teica un pienāca tuvāk.",
      "Krikšķis nobijās. Bet tad viņš sadzirdēja čiepstam mazuļus ligzdā.",
      "— Tavi bērni ir izsalkuši? Ņem, man drupatiņu pietiks visiem!",
      "Mazuļi paēda, un Čiepa vairs negribēja Krikšķi apēst.",
      "— Tu esi labākais cepums pasaulē. Kāp man mugurā!",
    ],
  },
  {
    id: "flight",
    seconds: 28,
    lines: [
      "Čiepa pacēla draugus augstu virs pilsētas jumtiem.",
      "Krikšķis vēl nekad nebija redzējis tik skaistu saulrietu.",
      "Bet tad viņš paskatījās uz sevi.",
      "— Es esmu kļuvis mazāks... Ja dalīšos vēl, no manis nekas nepaliks.",
    ],
  },
  {
    id: "night",
    seconds: 30,
    lines: [
      "Naktī draugi apsēdās uz jumta zem zvaigznēm.",
      "— Kāpēc tu esi bēdīgs? — jautāja Tipa.",
      "— Man jau tā trūka maliņas. Tagad manis ir vēl mazāk.",
      "— Tu neesi kļuvis mazāks, — teica Čiepa. — Tu esi kļuvis lielāks.",
      "— Daļiņa no tevis tagad ir katrā no mums.",
      "Krikšķis paskatījās uz mēnesi un pirmo reizi pasmaidīja.",
    ],
  },
  {
    id: "return",
    seconds: 28,
    lines: [
      "No rīta draugi aizveda Krikšķi atpakaļ uz ceptuvi.",
      "Maiznieks nevarēja noticēt savām acīm.",
      "Viņa cepums bija atgriezies — kopā ar draugiem!",
      "Un tad maizniekam radās ideja.",
    ],
  },
  {
    id: "family",
    seconds: 26,
    lines: [
      "Kopš tās dienas ceptuvē cep īpašus cepumus.",
      "Katram no tiem viena maliņa ir nodrupusi — tāpat kā Krikšķim.",
      "Pircēji pēc tiem stāv garā rindā.",
      "Bet Krikšķis sēž goda vietā un sagaida katru ciemiņu.",
    ],
  },
  { id: "end", seconds: 12, lines: [] },
];

export const CARTOON_FPS = 30;
export const CARTOON_SECONDS = scenes.reduce((a, s) => a + s.seconds, 0);
