import { useId } from "react";

export type Mood = "happy" | "sad" | "scared" | "wow" | "sleep";

const BODY = "#dd9d4f";
const EDGE = "#b97733";
const CHIP = "#5b3418";
const LIMB = "#8a5526";

const NOTCHES = [
  { a: -45, r: 38 },
  { a: 165, r: 28 },
  { a: 75, r: 26 },
];

const Face: React.FC<{ mood: Mood; t: number; look?: number }> = ({ mood, t, look = 0 }) => {
  const blink = mood === "sleep" || t % 3.1 < 0.12;
  const big = mood === "scared" || mood === "wow";
  const r = big ? 21 : 17;
  return (
    <g>
      <circle cx={-52} cy={22} r={13} fill="#f08a7a" opacity={0.45} />
      <circle cx={52} cy={22} r={13} fill="#f08a7a" opacity={0.45} />
      {[-32, 32].map((ex) =>
        blink ? (
          <path
            key={ex}
            d={`M ${ex - 14} -14 Q ${ex} -4 ${ex + 14} -14`}
            stroke={CHIP}
            strokeWidth={6}
            fill="none"
            strokeLinecap="round"
          />
        ) : (
          <g key={ex}>
            <circle cx={ex} cy={-15} r={r} fill="white" />
            <circle cx={ex + look * 6} cy={-13} r={big ? 8 : 9.5} fill="#2b1a10" />
            <circle cx={ex + look * 6 + 3} cy={-17} r={3} fill="white" />
          </g>
        ),
      )}
      {mood === "sad" && (
        <g stroke={CHIP} strokeWidth={6} strokeLinecap="round">
          <line x1={-50} y1={-40} x2={-22} y2={-48} />
          <line x1={50} y1={-40} x2={22} y2={-48} />
        </g>
      )}
      {mood === "happy" && (
        <path d="M -28 28 Q 0 62 28 28" stroke={CHIP} strokeWidth={7} fill="none" strokeLinecap="round" />
      )}
      {mood === "sad" && (
        <path d="M -24 52 Q 0 30 24 52" stroke={CHIP} strokeWidth={7} fill="none" strokeLinecap="round" />
      )}
      {mood === "scared" && <ellipse cx={0} cy={46} rx={15} ry={17 + Math.sin(t * 30) * 2} fill={CHIP} />}
      {mood === "wow" && <circle cx={0} cy={44} r={13} fill={CHIP} />}
      {mood === "sleep" && (
        <path d="M -10 42 Q 0 49 10 42" stroke={CHIP} strokeWidth={6} fill="none" strokeLinecap="round" />
      )}
    </g>
  );
};

/** Galvenais varonis. (x, y) ir ķermeņa centrs; ar s=1 pēdas ir 150 vienības zemāk. */
export const Cookie: React.FC<{
  x: number;
  y: number;
  t: number;
  s?: number;
  mood?: Mood;
  walk?: boolean;
  bites?: number;
  arm?: "down" | "up" | "wave";
  sit?: boolean;
  look?: number;
}> = ({ x, y, t, s = 1, mood = "happy", walk = false, bites = 0, arm = "down", sit = false, look = 0 }) => {
  const id = "ck" + useId().replace(/[^a-zA-Z0-9]/g, "");
  const cyc = Math.sin(t * 11);
  const bob = walk ? -Math.abs(cyc) * 14 : Math.sin(t * 2.2) * 3;
  const shake = mood === "scared" ? Math.sin(t * 45) * 4 : 0;
  const swing = walk ? cyc * 26 : 0;
  const notches = NOTCHES.slice(0, 1 + bites);
  const wave = Math.sin(t * 9) * 18;
  const armL = arm === "down" ? { x: -138, y: 52 } : { x: -142, y: -48 };
  const armR =
    arm === "down" ? { x: 138, y: 52 } : arm === "up" ? { x: 142, y: -48 } : { x: 140 + wave * 0.4, y: -55 - wave };

  return (
    <g transform={`translate(${x + shake} ${y + bob}) scale(${s})`}>
      <defs>
        <mask id={id}>
          <rect x={-130} y={-130} width={260} height={260} fill="white" />
          {notches.map((n) => (
            <circle
              key={n.a}
              cx={Math.cos((n.a * Math.PI) / 180) * 108}
              cy={Math.sin((n.a * Math.PI) / 180) * 108}
              r={n.r}
              fill="black"
            />
          ))}
        </mask>
      </defs>
      <g stroke={LIMB} strokeWidth={13} strokeLinecap="round">
        {sit ? (
          <>
            <line x1={-34} y1={84} x2={-52} y2={112} />
            <line x1={34} y1={84} x2={52} y2={112} />
          </>
        ) : (
          <>
            <line x1={-34} y1={80} x2={-34 + swing} y2={138} />
            <line x1={34} y1={80} x2={34 - swing} y2={138} />
          </>
        )}
        <line x1={-90} y1={12} x2={armL.x} y2={armL.y} />
        <line x1={90} y1={12} x2={armR.x} y2={armR.y} />
      </g>
      {!sit && (
        <g fill={LIMB}>
          <ellipse cx={-34 + swing + 8} cy={142} rx={22} ry={10} />
          <ellipse cx={34 - swing + 8} cy={142} rx={22} ry={10} />
        </g>
      )}
      <g mask={`url(#${id})`}>
        <circle r={100} fill={BODY} stroke={EDGE} strokeWidth={9} />
        <circle cx={-18} cy={-22} r={74} fill="#e6ab60" opacity={0.55} />
        <g fill={CHIP}>
          <ellipse cx={-58} cy={-52} rx={13} ry={10} />
          <ellipse cx={10} cy={-68} rx={12} ry={9} />
          <ellipse cx={66} cy={48} rx={12} ry={10} />
          <ellipse cx={-62} cy={56} rx={11} ry={9} />
          <ellipse cx={8} cy={78} rx={12} ry={8} />
          <ellipse cx={78} cy={-2} rx={9} ry={8} />
        </g>
      </g>
      <g fill={EDGE}>
        <circle cx={96} cy={-92} r={6} />
        <circle cx={112} cy={-72} r={4} />
      </g>
      <Face mood={mood} t={t} look={look} />
    </g>
  );
};

/** Mazs cepums bez rokām un kājām. `notch` — ar nodrupušu maliņu. */
export const MiniCookie: React.FC<{ x: number; y: number; r?: number; notch?: boolean; t: number; o?: number }> = ({
  x,
  y,
  r = 55,
  notch = false,
  t,
  o = 1,
}) => {
  const id = "mc" + useId().replace(/[^a-zA-Z0-9]/g, "");
  return (
    <g transform={`translate(${x} ${y}) scale(${r / 100})`} opacity={o}>
      <defs>
        <mask id={id}>
          <rect x={-130} y={-130} width={260} height={260} fill="white" />
          {notch && <circle cx={76} cy={-76} r={38} fill="black" />}
        </mask>
      </defs>
      <g mask={`url(#${id})`}>
        <circle r={100} fill={BODY} stroke={EDGE} strokeWidth={9} />
        <g fill={CHIP}>
          <ellipse cx={-55} cy={-50} rx={13} ry={10} />
          <ellipse cx={12} cy={-66} rx={12} ry={9} />
          <ellipse cx={64} cy={46} rx={12} ry={10} />
          <ellipse cx={-60} cy={54} rx={11} ry={9} />
        </g>
      </g>
      <Face mood="happy" t={t + x * 0.01} />
    </g>
  );
};

/** Skudra Tipa. Skatās pa labi; (x, y) ir vieta, kur kājas pieskaras zemei. */
export const Ant: React.FC<{
  x: number;
  y: number;
  t: number;
  s?: number;
  flip?: boolean;
  walk?: boolean;
  mood?: "happy" | "sad";
  carry?: boolean;
  hop?: number;
}> = ({ x, y, t, s = 1, flip = false, walk = false, mood = "happy", carry = false, hop = 0 }) => {
  const c = "#4a2f2a";
  const sw = walk ? Math.sin(t * 16) * 9 : 0;
  const bob = walk ? Math.abs(Math.sin(t * 16)) * -4 : 0;
  return (
    <g transform={`translate(${x} ${y - 34 * s + bob * s - hop}) scale(${flip ? -s : s} ${s})`}>
      <g stroke={c} strokeWidth={6} strokeLinecap="round">
        <line x1={-18} y1={6} x2={-30 + sw} y2={34} />
        <line x1={-2} y1={8} x2={-2 - sw} y2={34} />
        <line x1={12} y1={6} x2={24 + sw} y2={34} />
        <path d="M 40 -42 Q 50 -70 68 -72" fill="none" />
        <path d="M 28 -44 Q 28 -74 44 -82" fill="none" />
      </g>
      <ellipse cx={-48} cy={-4} rx={34} ry={26} fill={c} />
      <ellipse cx={-4} cy={-4} rx={22} ry={18} fill={c} />
      <circle cx={34} cy={-22} r={25} fill={c} />
      {carry && <ellipse cx={-22} cy={-50} rx={42} ry={24} fill="#e9c768" stroke="#c9a23e" strokeWidth={5} transform="rotate(-8 -22 -50)" />}
      <circle cx={42} cy={-27} r={10} fill="white" />
      <circle cx={45} cy={-26} r={5} fill="#111" />
      {mood === "happy" ? (
        <path d="M 36 -10 Q 46 -2 54 -12" stroke="white" strokeWidth={4} fill="none" strokeLinecap="round" />
      ) : (
        <path d="M 36 -6 Q 46 -14 54 -6" stroke="white" strokeWidth={4} fill="none" strokeLinecap="round" />
      )}
    </g>
  );
};

/** Zīlīte Čiepa. Skatās pa kreisi; (x, y) ir vieta, kur kājas pieskaras zemei. */
export const Bird: React.FC<{
  x: number;
  y: number;
  t: number;
  s?: number;
  flip?: boolean;
  fly?: boolean;
  mood?: "happy" | "neutral";
  hop?: number;
}> = ({ x, y, t, s = 1, flip = false, fly = false, mood = "neutral", hop = 0 }) => {
  const flap = fly ? Math.sin(t * 14) * 38 - 10 : Math.sin(t * 2) * 3;
  return (
    <g transform={`translate(${x} ${y - 85 * s - hop}) scale(${flip ? -s : s} ${s})`}>
      {!fly && (
        <g stroke="#e58a2b" strokeWidth={7} strokeLinecap="round">
          <line x1={-12} y1={48} x2={-12} y2={85} />
          <line x1={16} y1={48} x2={16} y2={85} />
          <line x1={-26} y1={85} x2={0} y2={85} />
          <line x1={2} y1={85} x2={28} y2={85} />
        </g>
      )}
      <polygon points="55,-8 128,-34 120,12" fill="#4a6fa5" />
      <ellipse cx={0} cy={0} rx={76} ry={58} fill="#f6d84a" />
      <circle cx={-58} cy={-48} r={41} fill="#2f4a7a" />
      <ellipse cx={-64} cy={-38} rx={27} ry={21} fill="white" />
      <circle cx={-70} cy={-50} r={8} fill="#111" />
      <circle cx={-72} cy={-53} r={2.5} fill="white" />
      <polygon points="-95,-46 -128,-37 -95,-28" fill="#f39c34" />
      {mood === "happy" && (
        <path d="M -84 -24 Q -72 -14 -60 -24" stroke="#2f4a7a" strokeWidth={4} fill="none" strokeLinecap="round" />
      )}
      <g transform={`rotate(${flap} -18 -14)`}>
        <ellipse cx={fly ? 40 : 22} cy={-8} rx={fly ? 82 : 50} ry={fly ? 34 : 30} fill="#5b7fb5" />
      </g>
    </g>
  );
};

/** Maiznieks. (x, y) ir vieta starp pēdām. */
export const Baker: React.FC<{ x: number; y: number; t: number; mood?: "neutral" | "wow" | "happy" }> = ({
  x,
  y,
  t,
  mood = "neutral",
}) => {
  const up = mood !== "neutral";
  const bob = Math.sin(t * 2) * 3 + (mood === "happy" ? Math.abs(Math.sin(t * 6)) * -10 : 0);
  return (
    <g transform={`translate(${x} ${y + bob})`}>
      <rect x={-50} y={-115} width={40} height={115} fill="#4d5b7a" />
      <rect x={10} y={-115} width={40} height={115} fill="#4d5b7a" />
      <ellipse cx={-36} cy={-4} rx={34} ry={14} fill="#33384a" />
      <ellipse cx={36} cy={-4} rx={34} ry={14} fill="#33384a" />
      <g stroke="white" strokeWidth={34} strokeLinecap="round">
        <line x1={-80} y1={-290} x2={up ? -150 : -120} y2={up ? -380 : -180} />
        <line x1={80} y1={-290} x2={up ? 150 : 120} y2={up ? -380 : -180} />
      </g>
      <circle cx={up ? -150 : -120} cy={up ? -390 : -168} r={22} fill="#f2c9a0" />
      <circle cx={up ? 150 : 120} cy={up ? -390 : -168} r={22} fill="#f2c9a0" />
      <rect x={-95} y={-335} width={190} height={245} rx={70} fill="white" stroke="#e3ddd2" strokeWidth={5} />
      <circle cx={0} cy={-250} r={8} fill="#c9c2b6" />
      <circle cx={0} cy={-200} r={8} fill="#c9c2b6" />
      <circle cx={0} cy={-395} r={72} fill="#f2c9a0" />
      <rect x={-62} y={-505} width={124} height={62} fill="white" stroke="#e3ddd2" strokeWidth={5} />
      <circle cx={-48} cy={-512} r={40} fill="white" />
      <circle cx={48} cy={-512} r={40} fill="white" />
      <circle cx={0} cy={-530} r={46} fill="white" />
      <circle cx={-26} cy={-405} r={mood === "wow" ? 12 : 8} fill="#2b1a10" />
      <circle cx={26} cy={-405} r={mood === "wow" ? 12 : 8} fill="#2b1a10" />
      <path d="M -46 -372 Q -22 -392 0 -374 Q 22 -392 46 -372 Q 22 -360 0 -366 Q -22 -360 -46 -372 Z" fill="#7a4a2a" />
      {mood === "wow" && <circle cx={0} cy={-348} r={11} fill="#7a3030" />}
      {mood === "happy" && (
        <path d="M -22 -356 Q 0 -332 22 -356" stroke="#7a3030" strokeWidth={6} fill="none" strokeLinecap="round" />
      )}
    </g>
  );
};

export const Heart: React.FC<{ x: number; y: number; s?: number; o?: number }> = ({ x, y, s = 1, o = 1 }) => (
  <path
    transform={`translate(${x} ${y}) scale(${s})`}
    opacity={o}
    d="M 0 -8 C -22 -40 -62 -8 0 38 C 62 -8 22 -40 0 -8 Z"
    fill="#ff5d73"
  />
);

/** Peldošas sirsniņas: parādās brīdī `start` un ceļas augšup. */
export const Hearts: React.FC<{ x: number; y: number; t: number; start: number; n?: number }> = ({
  x,
  y,
  t,
  start,
  n = 3,
}) => (
  <>
    {Array.from({ length: n }).map((_, i) => {
      const lt = t - start - i * 0.5;
      if (lt < 0 || lt > 3) return null;
      return (
        <Heart
          key={i}
          x={x + (i - (n - 1) / 2) * 46 + Math.sin(lt * 3 + i) * 10}
          y={y - lt * 55}
          s={0.5 + Math.min(lt, 0.4)}
          o={Math.min(1, 3 - lt)}
        />
      );
    })}
  </>
);

/** Drupatiņa, kas lokā lido no vienas vietas uz otru. */
export const Crumb: React.FC<{ t: number; start: number; from: [number, number]; to: [number, number]; dur?: number }> = ({
  t,
  start,
  from,
  to,
  dur = 1.4,
}) => {
  const p = (t - start) / dur;
  if (p < 0 || p > 1) return null;
  const x = from[0] + (to[0] - from[0]) * p;
  const y = from[1] + (to[1] - from[1]) * p - Math.sin(p * Math.PI) * 170;
  return (
    <g transform={`translate(${x} ${y}) rotate(${p * 400})`}>
      <path d="M -16 -8 L 6 -15 L 17 2 L 4 15 L -14 9 Z" fill={BODY} stroke={EDGE} strokeWidth={4} />
      <circle cx={0} cy={0} r={4} fill={CHIP} />
    </g>
  );
};
