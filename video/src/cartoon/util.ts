import { Easing, interpolate } from "remotion";

export const GROUND = 800;
export const font = "Inter, sans-serif";

/** Vērtība no x līdz y laikā no a līdz b (sekundēs), ar mīkstu sākumu un beigām. */
export const tw = (t: number, a: number, b: number, x: number, y: number) =>
  interpolate(t, [a, b], [x, y], {
    extrapolateLeft: "clamp",
    extrapolateRight: "clamp",
    easing: Easing.inOut(Easing.cubic),
  });

/** "Izlēkšana" ar nelielu pārsniegumu. */
export const pop = (t: number, a: number, dur = 0.5) =>
  interpolate(t, [a, a + dur], [0, 1], {
    extrapolateLeft: "clamp",
    extrapolateRight: "clamp",
    easing: Easing.out(Easing.back(1.8)),
  });

/** Pseido-gadījuma skaitlis 0..1 no indeksa (vienmēr vienāds). */
export const rnd = (i: number) => {
  const v = Math.sin(i * 127.1 + 311.7) * 43758.5453;
  return v - Math.floor(v);
};
