// 3D multenes "Cepumiņš Krikšķis" īsā versija (1 min 30 s): ainu garumi un titri.
export type Scene3Id =
  | "title"
  | "oven"
  | "shelf"
  | "leaving"
  | "ant"
  | "rain"
  | "bird"
  | "flight"
  | "night"
  | "home"
  | "end";

export type Scene3Def = { id: Scene3Id; seconds: number; lines: string[] };

export const scenes3: Scene3Def[] = [
  { id: "title", seconds: 5, lines: [] },
  {
    id: "oven",
    seconds: 9,
    lines: [
      "Kādā rītā mazā ceptuvē no krāsns izslīdēja paplāte ar cepumiem.",
      "Pēdējais bija Krikšķis — ar nodrupušu maliņu.",
    ],
  },
  {
    id: "shelf",
    seconds: 9,
    lines: ["Gludos cepumus pircēji izvēlējās citu pēc cita.", "Vakarā plauktā bija palicis tikai Krikšķis."],
  },
  { id: "leaving", seconds: 7, lines: ["— Ja neviens neatnāk pie manis, es pats došos pasaulē!"] },
  {
    id: "ant",
    seconds: 10,
    lines: ["Pļavā viņš satika izsalkušu skudru Tipu.", "Krikšķis nolauza drupatiņu un padalījās. Tā viņš ieguva pirmo draugu."],
  },
  {
    id: "rain",
    seconds: 8,
    lines: ["Sāka līt — cepumiem tas ir bīstami!", "Draugi paslēpās zem sēnes, līdz iemirdzējās varavīksne."],
  },
  {
    id: "bird",
    seconds: 11,
    lines: [
      "Zīlīte Čiepa gribēja Krikšķi apēst.",
      "Bet viņš pabaroja viņas izsalkušos mazuļus.",
      "— Tu esi labākais cepums pasaulē. Kāp man mugurā!",
    ],
  },
  {
    id: "flight",
    seconds: 8,
    lines: ["Viņi lidoja virs jumtiem saulrietā.", "— Bet es esmu kļuvis mazāks... — nobēdājās Krikšķis."],
  },
  {
    id: "night",
    seconds: 11,
    lines: [
      "— Tu neesi kļuvis mazāks, — teica Čiepa.",
      "— Daļiņa no tevis tagad ir katrā no mums.",
      "Un Krikšķis pirmo reizi pasmaidīja.",
    ],
  },
  {
    id: "home",
    seconds: 8,
    lines: [
      "No rīta draugi atgriezās ceptuvē.",
      "Kopš tās dienas maiznieks cep cepumus ar nodrupušu maliņu — tāpat kā Krikšķim.",
    ],
  },
  { id: "end", seconds: 4, lines: [] },
];

export const CARTOON3_SECONDS = scenes3.reduce((a, s) => a + s.seconds, 0);
