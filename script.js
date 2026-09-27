// Nebijušu Lietu Veikals — katra prece tiek izgudrota tieši tajā brīdī, kad to ieraugi.

// ---------- Nejaušība ar sēklu, lai katra prece vienmēr izskatītos vienādi ----------
function mulberry32(seed) {
    return function () {
        seed |= 0; seed = (seed + 0x6D2B79F5) | 0;
        let t = Math.imul(seed ^ (seed >>> 15), 1 | seed);
        t = (t + Math.imul(t ^ (t >>> 7), 61 | t)) ^ t;
        return ((t ^ (t >>> 14)) >>> 0) / 4294967296;
    };
}
const pick = (rnd, arr) => arr[Math.floor(rnd() * arr.length)];

// ---------- Vārdnīca ----------
// Lietvārdi ar dzimti (v — vīriešu, s — sieviešu)
const NOUNS = [
    ["mākonis", "v"], ["saulriets", "v"], ["lietussargs", "v"], ["klusums", "v"],
    ["vējš", "v"], ["sapnis", "v"], ["pulkstenis", "v"], ["kompass", "v"],
    ["spogulis", "v"], ["mirklis", "v"], ["zibens", "v"], ["smiekls", "v"],
    ["ēna", "s"], ["atbalss", "s"], ["atslēga", "s"], ["zvaigzne", "s"],
    ["karte", "s"], ["smarža", "s"], ["melodija", "s"], ["varavīksne", "s"],
    ["pēcpusdiena", "s"], ["lāsteka", "s"], ["nopūta", "s"], ["jūra", "s"]
];
// Īpašības vārdi [vīriešu, sieviešu]
const ADJECTIVES = [
    ["Salocāms", "Salocāma"], ["Kabatas", "Kabatas"], ["Neredzams", "Neredzama"],
    ["Vakardienas", "Vakardienas"], ["Pusizlietots", "Pusizlietota"], ["Dziedošs", "Dziedoša"],
    ["Atpakaļejošs", "Atpakaļejoša"], ["Pazaudēts", "Pazaudēta"], ["Rezerves", "Rezerves"],
    ["Svētdienas", "Svētdienas"], ["Pieradināts", "Pieradināta"], ["Kvantu", "Kvantu"],
    ["Mēnessgaismas", "Mēnessgaismas"], ["Ievārīts", "Ievārīta"], ["Otrreizējs", "Otrreizēja"],
    ["Divkāršs", "Divkārša"], ["Nepabeigts", "Nepabeigta"], ["Iekonservēts", "Iekonservēta"]
];
const USES = [
    "kad pirmdiena pienāk pārāk agri",
    "kad gribas pateikt kaut ko svarīgu, bet vārdi aizmirsušies",
    "lai paņemtu līdzi uz vietām, kur vēl neesi bijis",
    "kad uz ielas ir par daudz pelēkā",
    "ja nepieciešams uz brīdi pazust",
    "kad kaķis skatās uz tukšu stūri",
    "garām rindām pasta nodaļā",
    "lai atcerētos smaržu, kuru nekad neesi saodis",
    "vēlām sarunām virtuvē",
    "kad vajag vēl piecas minūtes miega"
];
const FEATURES = [
    "Darbojas arī otrdienās",
    "Nav nepieciešamas baterijas, tikai iztēle",
    "Mazgājams 30° temperatūrā",
    "Garantija: līdz aizmiršanai",
    "Izgatavots no pārpalikušiem sapņiem",
    "Nedaudz mirdz tumsā",
    "Saderīgs ar visiem gadalaikiem",
    "Pēc lietošanas atgriežas pats",
    "Klusi dūc, ja to paglauda",
    "Sver mazāk par domu"
];

// ---------- Preču izgudrošana ----------
function inventProduct(seed) {
    const rnd = mulberry32(seed);
    const [noun, gender] = pick(rnd, NOUNS);
    const adjective = pick(rnd, ADJECTIVES)[gender === "v" ? 0 : 1];
    const f1 = pick(rnd, FEATURES);
    let f2 = pick(rnd, FEATURES);
    while (f2 === f1) f2 = pick(rnd, FEATURES);
    return {
        id: seed,
        name: `${adjective} ${noun}`,
        use: `Noderēs, ${pick(rnd, USES)}.`,
        features: [f1, f2],
        price: Math.round((1 + rnd() * 98) * 100 + (rnd() < 0.3 ? 0 : 7)) / 100,
        serial: seed.toString(36).toUpperCase().padStart(7, "0"),
        art: drawArt(rnd)
    };
}

// Ģeneratīvs SVG attēls — neviens nav tieši tāds pats kā cits
function drawArt(rnd) {
    const hue = Math.floor(rnd() * 360);
    const hue2 = (hue + 40 + Math.floor(rnd() * 140)) % 360;
    const id = "g" + Math.floor(rnd() * 1e9);
    let shapes = "";
    const count = 3 + Math.floor(rnd() * 5);
    for (let i = 0; i < count; i++) {
        const x = 20 + rnd() * 160, y = 20 + rnd() * 110, r = 8 + rnd() * 45;
        const h = rnd() < 0.5 ? hue : hue2;
        const op = (0.35 + rnd() * 0.5).toFixed(2);
        if (rnd() < 0.65) {
            shapes += `<circle cx="${x.toFixed(1)}" cy="${y.toFixed(1)}" r="${r.toFixed(1)}" fill="hsl(${h} 85% 70% / ${op})"/>`;
        } else {
            shapes += `<ellipse cx="${x.toFixed(1)}" cy="${y.toFixed(1)}" rx="${(r * 1.6).toFixed(1)}" ry="${(r * 0.4).toFixed(1)}" fill="none" stroke="hsl(${h} 90% 85% / ${op})" stroke-width="2" transform="rotate(${Math.floor(rnd() * 180)} ${x.toFixed(1)} ${y.toFixed(1)})"/>`;
        }
    }
    let stars = "";
    for (let i = 0; i < 12; i++) {
        stars += `<circle cx="${(rnd() * 200).toFixed(1)}" cy="${(rnd() * 150).toFixed(1)}" r="${(0.5 + rnd() * 1.2).toFixed(1)}" fill="white" opacity="${(0.4 + rnd() * 0.6).toFixed(2)}"/>`;
    }
    return `<svg viewBox="0 0 200 150" role="img" aria-hidden="true">
        <defs><linearGradient id="${id}" x1="0" y1="0" x2="1" y2="1">
            <stop offset="0" stop-color="hsl(${hue} 60% 22%)"/>
            <stop offset="1" stop-color="hsl(${hue2} 55% 38%)"/>
        </linearGradient></defs>
        <rect width="200" height="150" fill="url(#${id})"/>${stars}${shapes}
    </svg>`;
}

const newSeed = () => (Math.random() * 2 ** 31) >>> 0;

// ---------- Stāvoklis ----------
let products = Array.from({ length: 6 }, newSeed).map(inventProduct);
let cart = loadCart();

function loadCart() {
    try {
        const saved = JSON.parse(localStorage.getItem("nebijusu-grozs") || "[]");
        return Array.isArray(saved) ? saved.map(inventProduct) : [];
    } catch { return []; }
}
function saveCart() {
    try { localStorage.setItem("nebijusu-grozs", JSON.stringify(cart.map(p => p.id))); } catch { }
}

const euro = n => n.toFixed(2);

// ---------- Attēlošana ----------
function renderProducts() {
    const grid = document.getElementById("product-grid");
    grid.innerHTML = "";
    products.forEach(p => {
        const inCart = cart.some(c => c.id === p.id);
        const card = document.createElement("article");
        card.className = "product-card";
        card.innerHTML = `
            <div class="art">${p.art}</div>
            <div class="body">
                <h3>${p.name}</h3>
                <p class="use">${p.use}</p>
                <ul class="features">${p.features.map(f => `<li>${f}</li>`).join("")}</ul>
                <div class="meta">
                    <span class="price">€${euro(p.price)}</span>
                    <span class="stock">Noliktavā: 1 (vienīgais pasaulē)</span>
                </div>
                <button ${inCart ? "disabled" : ""}>${inCart ? "Jau grozā" : "Ielikt grozā"}</button>
            </div>`;
        card.querySelector("button").addEventListener("click", () => addToCart(p));
        grid.appendChild(card);
    });

    const inventCard = document.createElement("button");
    inventCard.className = "invent-card";
    inventCard.innerHTML = `<span class="plus">✦</span><span>Izgudrot vēl vienu lietu</span>`;
    inventCard.addEventListener("click", () => {
        products.push(inventProduct(newSeed()));
        renderProducts();
    });
    grid.appendChild(inventCard);
}

function renderCart() {
    const list = document.getElementById("cart-items");
    list.innerHTML = "";
    if (cart.length === 0) {
        list.innerHTML = `<li class="empty">Grozs ir tukšs. Tik tukšs, ka tur varētu dzīvot klusums.</li>`;
    }
    cart.forEach(p => {
        const li = document.createElement("li");
        li.innerHTML = `<span>${p.name}</span><span>€${euro(p.price)}</span><button class="remove" aria-label="Izņemt">×</button>`;
        li.querySelector(".remove").addEventListener("click", () => removeFromCart(p.id));
        list.appendChild(li);
    });
    document.getElementById("cart-count").textContent = cart.length;
    document.getElementById("total-price").textContent = euro(cart.reduce((s, p) => s + p.price, 0));
}

function addToCart(p) {
    if (cart.some(c => c.id === p.id)) return;
    cart.push(p);
    saveCart();
    renderAll();
}
function removeFromCart(id) {
    cart = cart.filter(p => p.id !== id);
    saveCart();
    renderAll();
}
function renderAll() { renderProducts(); renderCart(); }

// ---------- Pasūtījuma noformēšana ----------
function checkout() {
    if (cart.length === 0) {
        showReceipt(`<p>Tu mēģināji nopirkt neko. Diemžēl “nekas” ir izpārdots jau kopš 1998. gada.</p>`);
        return;
    }
    const total = cart.reduce((s, p) => s + p.price, 0);
    const lines = cart.map(p => `<tr><td>${p.name}<small>Nr. ${p.serial}</small></td><td>€${euro(p.price)}</td></tr>`).join("");
    showReceipt(`
        <p class="receipt-lead">Paldies! Tavs pasūtījums ir pieņemts un tiks piegādāts brīdī, kad par to vismazāk domāsi.</p>
        <table>${lines}<tr class="total"><td>Kopā</td><td>€${euro(total)}</td></tr></table>
        <p class="fine">Šīs lietas vairs nekad netiks izgudrotas atkārtoti. Apmaiņa un atgriešana nav iespējama, jo tās pirms šī brīža nemaz nepastāvēja.</p>`);
    cart = [];
    saveCart();
    products = Array.from({ length: 6 }, newSeed).map(inventProduct);
    renderAll();
}

function showReceipt(html) {
    const dialog = document.getElementById("receipt");
    document.getElementById("receipt-body").innerHTML = html;
    dialog.showModal();
}

document.getElementById("close-receipt").addEventListener("click", () => document.getElementById("receipt").close());
renderAll();
