import * as THREE from "three";

// Procedurālas tekstūras (zīmētas uz canvas), lai virsmas izskatītos dabiskākas.
// Izmanto fiksētu "sēklu", lai katrs kadrs un katrs renderis būtu vienāds.
const prng = (seed: number) => {
  let a = seed >>> 0;
  return () => {
    a = (a + 0x6d2b79f5) >>> 0;
    let t = a;
    t = Math.imul(t ^ (t >>> 15), t | 1);
    t ^= t + Math.imul(t ^ (t >>> 7), t | 61);
    return ((t ^ (t >>> 14)) >>> 0) / 4294967296;
  };
};

const canvas = (size: number) => {
  const c = document.createElement("canvas");
  c.width = size;
  c.height = size;
  return { c, g: c.getContext("2d")! };
};

const cache = new Map<string, THREE.Texture>();
const cached = (key: string, make: () => THREE.Texture) => {
  let t = cache.get(key);
  if (!t) {
    t = make();
    cache.set(key, t);
  }
  return t;
};

const finish = (c: HTMLCanvasElement, color: boolean, repeat?: [number, number]) => {
  const t = new THREE.CanvasTexture(c);
  if (color) t.colorSpace = THREE.SRGBColorSpace;
  if (repeat) {
    t.wrapS = t.wrapT = THREE.RepeatWrapping;
    t.repeat.set(repeat[0], repeat[1]);
  }
  t.anisotropy = 8;
  return t;
};

/** Cepuma virsma: tumšākas, apceptas malas, plankumi un plaisiņas. Forma ir -1..1, tāpēc tekstūru pārbīda. */
const cookie = (bump: boolean) =>
  cached(bump ? "cookieBump" : "cookieColor", () => {
    const S = 512;
    const { c, g } = canvas(S);
    const r = prng(11);
    if (bump) {
      g.fillStyle = "#808080";
      g.fillRect(0, 0, S, S);
      for (let i = 0; i < 420; i++) {
        const v = 90 + r() * 90;
        g.fillStyle = `rgba(${v},${v},${v},0.5)`;
        g.beginPath();
        g.arc(r() * S, r() * S, 4 + r() * 22, 0, 7);
        g.fill();
      }
    } else {
      const grad = g.createRadialGradient(S / 2, S / 2, S * 0.1, S / 2, S / 2, S * 0.52);
      grad.addColorStop(0, "#e6b066");
      grad.addColorStop(0.7, "#d9984a");
      grad.addColorStop(1, "#a9662a");
      g.fillStyle = grad;
      g.fillRect(0, 0, S, S);
      for (let i = 0; i < 2600; i++) {
        g.fillStyle = r() > 0.5 ? "rgba(120,70,25,0.22)" : "rgba(255,225,160,0.2)";
        g.beginPath();
        g.arc(r() * S, r() * S, 0.6 + r() * 3, 0, 7);
        g.fill();
      }
    }
    // Plaisiņas
    g.strokeStyle = bump ? "rgba(20,20,20,0.7)" : "rgba(110,60,20,0.45)";
    for (let i = 0; i < 16; i++) {
      let x = r() * S;
      let y = r() * S;
      g.lineWidth = 1.5 + r() * 2.5;
      g.beginPath();
      g.moveTo(x, y);
      for (let k = 0; k < 6; k++) {
        x += (r() - 0.5) * 60;
        y += (r() - 0.5) * 60;
        g.lineTo(x, y);
      }
      g.stroke();
    }
    const t = finish(c, !bump);
    t.repeat.set(0.5, 0.5);
    t.offset.set(0.5, 0.5);
    return t;
  });

export const cookieColor = () => cookie(false);
export const cookieBump = () => cookie(true);

/** Koka dēļi ar šķiedru. */
const wood = (bump: boolean, repeat: [number, number], key: string) =>
  cached(key + (bump ? "B" : "C"), () => {
    const S = 1024;
    const { c, g } = canvas(S);
    const r = prng(23);
    const planks = 6;
    const ph = S / planks;
    for (let p = 0; p < planks; p++) {
      const shade = 0.82 + r() * 0.3;
      g.fillStyle = bump ? "#8a8a8a" : `rgb(${176 * shade},${118 * shade},${72 * shade})`;
      g.fillRect(0, p * ph, S, ph);
      for (let i = 0; i < 70; i++) {
        const y = p * ph + r() * ph;
        g.strokeStyle = bump ? `rgba(40,40,40,${0.1 + r() * 0.2})` : `rgba(90,52,24,${0.08 + r() * 0.2})`;
        g.lineWidth = 0.6 + r() * 2;
        g.beginPath();
        g.moveTo(0, y);
        for (let x = 0; x <= S; x += 64) g.lineTo(x, y + Math.sin(x * 0.01 + i) * 3 + (r() - 0.5) * 2);
        g.stroke();
      }
      g.fillStyle = bump ? "#101010" : "rgba(50,28,12,0.9)";
      g.fillRect(0, p * ph, S, 5);
      const seam = r() * S;
      g.fillRect(seam, p * ph, 4, ph);
    }
    return finish(c, !bump, repeat);
  });

export const floorColor = () => wood(false, [64, 32], "floor");
export const floorBump = () => wood(true, [64, 32], "floor");
export const plankColor = () => wood(false, [1, 1], "plank");

/** Zāliens: daudz īsu zaļu svītriņu dažādos toņos. */
export const grassColor = () =>
  cached("grass", () => {
    const S = 512;
    const { c, g } = canvas(S);
    const r = prng(37);
    g.fillStyle = "#5f9f3e";
    g.fillRect(0, 0, S, S);
    for (let i = 0; i < 9000; i++) {
      const x = r() * S;
      const y = r() * S;
      const l = 45 + r() * 30;
      g.strokeStyle = `hsl(${88 + r() * 30}, ${45 + r() * 25}%, ${l * 0.75}%)`;
      g.lineWidth = 1 + r() * 1.5;
      g.beginPath();
      g.moveTo(x, y);
      g.lineTo(x + (r() - 0.5) * 8, y - 4 - r() * 10);
      g.stroke();
    }
    return finish(c, true, [50, 25]);
  });

/** Sīks graudainums apmetumam un citām matētām virsmām. */
export const grainBump = (repeat: [number, number] = [40, 6]) =>
  cached("grain" + repeat.join("x"), () => {
    const S = 256;
    const { c, g } = canvas(S);
    const r = prng(51);
    g.fillStyle = "#808080";
    g.fillRect(0, 0, S, S);
    for (let i = 0; i < 6000; i++) {
      const v = 80 + r() * 100;
      g.fillStyle = `rgb(${v},${v},${v})`;
      g.fillRect(r() * S, r() * S, 1 + r() * 2, 1 + r() * 2);
    }
    return finish(c, false, repeat);
  });

export { prng };
