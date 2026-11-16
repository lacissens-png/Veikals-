import { Easing, interpolate } from "remotion";

// Krāsas ņemtas no veikala Style.css
export const colors = {
  dark: "#1a1a1a",
  blue: "#007bff",
  white: "#ffffff",
  card: "#f9f9f9",
  line: "#e6e6e6",
  muted: "#8a8f98",
};

export const fontFamily = "Inter, sans-serif";

export const FPS = 30;
export const WIDTH = 1080;
export const HEIGHT = 1920;

export const easeOut = Easing.bezier(0.16, 1, 0.3, 1);

/** 0 → 1 no kadra `start` līdz `start + duration`, ar mīkstu nobeigumu. */
export const appear = (frame: number, start: number, duration = 20) =>
  interpolate(frame, [start, start + duration], [0, 1], {
    extrapolateLeft: "clamp",
    extrapolateRight: "clamp",
    easing: easeOut,
  });

/** Ainas izdzišana pēdējos kadros. */
export const fadeOut = (frame: number, total: number, duration = 10) =>
  interpolate(frame, [total - duration, total], [1, 0], {
    extrapolateLeft: "clamp",
    extrapolateRight: "clamp",
  });
