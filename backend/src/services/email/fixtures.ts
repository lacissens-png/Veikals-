import type { EmailMessage } from "./types.js";

/**
 * Testa vēstules mock režīmam.
 *
 * Sedz tieši to, kāpēc e-pasta piekļuve vispār ir vajadzīga:
 *   - bezmaksas izmēģinājums, kas drīz sāks maksāt (bankā vēl nav nekā)
 *   - cenas paziņojums pirms norēķina
 *   - gada abonements, kas iekritis ārpus 3 mēnešu loga
 *   - parasts rēķins par jau zināmu abonementu
 *   - krāpnieciskas vēstules, ko lietotne brīdina, bet NEAIZTIEK Gmail
 *
 * Krāpšanas piemēri atdarina tipiskas NL shēmas: viltus ING/Ziggo rēķins,
 * pīķšķerēšana ar "abonements beidzies, apstiprini maksājumu", un abonementu
 * slazds, kur "balva" patiesībā ir ikmēneša maksa sīkā drukā.
 */

function daysAgo(days: number): Date {
  const date = new Date();
  date.setUTCDate(date.getUTCDate() - days);
  date.setUTCHours(9, 30, 0, 0);
  return date;
}

function daysAhead(days: number): string {
  const date = new Date();
  date.setUTCDate(date.getUTCDate() + days);
  return date.toISOString().slice(0, 10);
}

export function generateMockEmails(): EmailMessage[] {
  return [
    {
      externalId: "mock-mail-trial-audible",
      receivedAt: daysAgo(2),
      senderName: "Audible",
      senderAddress: "no-reply@audible.nl",
      subject: "Je gratis proefperiode eindigt binnenkort",
      body:
        `Beste klant,\n\nJe gratis proefperiode van Audible eindigt op ${daysAhead(5)}. ` +
        "Daarna wordt automatisch € 9,95 per maand in rekening gebracht. " +
        "Je kunt op elk moment opzeggen via je accountinstellingen.\n\nMet vriendelijke groet,\nAudible",
    },
    {
      externalId: "mock-mail-price-ziggo",
      receivedAt: daysAgo(9),
      senderName: "Ziggo",
      senderAddress: "klantenservice@ziggo.nl",
      subject: "Wijziging van je maandbedrag per 1 volgende maand",
      body:
        "Beste klant,\n\nPer volgende maand wijzigt je maandbedrag van € 37,22 naar € 41,50. " +
        `De nieuwe prijs geldt vanaf ${daysAhead(21)}. ` +
        "Je hebt het recht om je abonnement kosteloos op te zeggen binnen 30 dagen.\n\nZiggo",
    },
    {
      externalId: "mock-mail-annual-domain",
      receivedAt: daysAgo(14),
      senderName: "TransIP",
      senderAddress: "facturen@transip.nl",
      subject: "Automatische verlenging van je domeinnaam",
      body:
        `Je domeinnaam wordt automatisch verlengd op ${daysAhead(30)} voor € 89,00 per jaar. ` +
        "Wil je dit niet? Zet automatische verlenging uit in je control panel.",
    },
    {
      externalId: "mock-mail-invoice-essent",
      receivedAt: daysAgo(6),
      senderName: "Essent",
      senderAddress: "noreply@essent.nl",
      subject: "Je maandelijkse termijnbedrag",
      body:
        "Je termijnbedrag van € 150,00 wordt afgeschreven van rekening eindigend op 0144. " +
        "Bekijk je verbruik in Mijn Essent.",
    },
    {
      externalId: "mock-mail-scam-fake-invoice",
      receivedAt: daysAgo(1),
      senderName: "ING Bank Nederland",
      senderAddress: "security-ing@ing-verificatie.net",
      subject: "DRINGEND: openstaande factuur € 249,00 - betaal binnen 24 uur",
      body:
        "Geachte klant, er staat een onbetaalde factuur open van € 249,00. " +
        "Betaal binnen 24 uur via onderstaande link om incassokosten te voorkomen. " +
        "https://ing-verificatie.net/betaal?id=88213 " +
        "Log in met uw pasnummer en pincode om de betaling te bevestigen.",
    },
    {
      externalId: "mock-mail-scam-expired",
      receivedAt: daysAgo(3),
      senderName: "Netflix Support",
      senderAddress: "support@netflix-account-verify.com",
      subject: "Your subscription has expired - update payment details now",
      body:
        "We were unable to process your last payment. Your account will be suspended within 48 hours. " +
        "Please confirm your credit card number, expiry date and CVV here: " +
        "http://netflix-account-verify.com/update",
    },
    {
      externalId: "mock-mail-scam-subscription-trap",
      receivedAt: daysAgo(5),
      senderName: "Prijzenfestival",
      senderAddress: "winnaar@prijzenfestival-nl.com",
      subject: "Gefeliciteerd! Je hebt een iPhone gewonnen",
      body:
        "Claim je prijs binnen 12 uur! Vul je gegevens in en betaal slechts € 1,95 verzendkosten. " +
        "Door deelname ga je akkoord met het proefabonnement van € 49,95 per maand, " +
        "opzegbaar per kwartaal.",
    },
  ];
}
