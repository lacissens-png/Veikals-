import { GROUND, rnd } from "./util";

// Foni tiek zīmēti krietni plašāk par 1080x1080 skatuvi,
// lai tie aizpildītu gan vertikālo, gan horizontālo kadru.
const X0 = -520;
const W = 2120;
const Y0 = -540;
const H = 2420;

export const Tint: React.FC<{ color: string; o: number }> = ({ color, o }) =>
  o <= 0 ? null : <rect x={X0} y={Y0} width={W} height={H} fill={color} opacity={o} />;

export const Bakery: React.FC<{ evening?: number; windowX?: number }> = ({ evening = 0, windowX = 620 }) => (
  <g>
    <rect x={X0} y={Y0} width={W} height={H} fill="#fbe7c6" />
    {Array.from({ length: 18 }).map((_, i) => (
      <rect key={i} x={X0 + i * 120} y={Y0} width={60} height={GROUND - Y0} fill="#f7dcb2" />
    ))}
    <rect x={windowX - 14} y={106} width={308} height={268} rx={18} fill="#8a5a34" />
    <rect x={windowX} y={120} width={280} height={240} rx={8} fill="#9fdcff" />
    <rect x={windowX} y={120} width={280} height={240} rx={8} fill="#ff9a56" opacity={evening} />
    <circle cx={windowX + 200} cy={200 + evening * 110} r={36} fill="#fff3b0" />
    <rect x={windowX + 134} y={120} width={12} height={240} fill="#8a5a34" />
    <rect x={windowX} y={234} width={280} height={12} fill="#8a5a34" />
    <rect x={X0} y={GROUND} width={W} height={H} fill="#b97a4c" />
    <rect x={X0} y={GROUND} width={W} height={14} fill="#9a6238" />
    {Array.from({ length: 12 }).map((_, i) => (
      <rect key={i} x={X0 + i * 190} y={GROUND + 14} width={5} height={H} fill="#a56c40" />
    ))}
  </g>
);

export const Shelf: React.FC<{ y: number; x1?: number; x2?: number }> = ({ y, x1 = 110, x2 = 970 }) => (
  <g>
    <rect x={x1} y={y} width={x2 - x1} height={28} rx={8} fill="#8a5a34" />
    <polygon points={`${x1 + 50},${y + 28} ${x1 + 50},${y + 90} ${x1 + 110},${y + 28}`} fill="#6f4526" />
    <polygon points={`${x2 - 50},${y + 28} ${x2 - 50},${y + 90} ${x2 - 110},${y + 28}`} fill="#6f4526" />
  </g>
);

export const Door: React.FC<{ x: number; glow: string }> = ({ x, glow }) => (
  <g>
    <rect x={x - 16} y={344} width={272} height={GROUND - 344} rx={14} fill="#6f4526" />
    <rect x={x} y={360} width={240} height={GROUND - 360} fill={glow} />
    <polygon points={`${x},${GROUND} ${x + 240},${GROUND} ${x + 330},${GROUND + 150} ${x - 90},${GROUND + 150}`} fill={glow} opacity={0.35} />
  </g>
);

export const Cloud: React.FC<{ x: number; y: number; s?: number; fill?: string; o?: number }> = ({
  x,
  y,
  s = 1,
  fill = "white",
  o = 1,
}) => (
  <g transform={`translate(${x} ${y}) scale(${s})`} fill={fill} opacity={o}>
    <ellipse cx={0} cy={0} rx={90} ry={44} />
    <circle cx={-52} cy={-24} r={42} />
    <circle cx={8} cy={-44} r={54} />
    <circle cx={62} cy={-18} r={38} />
  </g>
);

export const Meadow: React.FC<{ t: number; dark?: number }> = ({ t, dark = 0 }) => (
  <g>
    <defs>
      <linearGradient id="sky" x1="0" y1="0" x2="0" y2="1">
        <stop offset="0" stopColor="#7cc8ff" />
        <stop offset="1" stopColor="#dff4ff" />
      </linearGradient>
    </defs>
    <rect x={X0} y={Y0} width={W} height={GROUND - Y0 + 40} fill="url(#sky)" />
    <g transform={`translate(860 150) rotate(${t * 6})`}>
      {Array.from({ length: 12 }).map((_, i) => (
        <rect key={i} x={-7} y={-150} width={14} height={50} rx={7} fill="#ffd94a" transform={`rotate(${i * 30})`} />
      ))}
      <circle r={82} fill="#ffe066" />
    </g>
    <Cloud x={((t * 14 + 200) % 2300) - 620} y={150} s={1.1} />
    <Cloud x={((t * 9 + 1300) % 2300) - 620} y={300} s={0.8} o={0.9} />
    <ellipse cx={150} cy={920} rx={760} ry={270} fill="#93d36e" />
    <ellipse cx={1050} cy={950} rx={800} ry={290} fill="#80c85e" />
    <rect x={X0} y={GROUND} width={W} height={H} fill="#6fbd54" />
    {Array.from({ length: 26 }).map((_, i) => {
      const fx = X0 + rnd(i) * W;
      const fy = GROUND + 60 + rnd(i + 50) * 760;
      const col = ["#ff7aa8", "#ffffff", "#ffd94a", "#c79bff"][i % 4];
      return (
        <g key={i} transform={`translate(${fx} ${fy}) rotate(${Math.sin(t * 2 + i) * 6})`}>
          <rect x={-2} y={0} width={4} height={30} fill="#3f8f35" />
          {[0, 72, 144, 216, 288].map((a) => (
            <circle key={a} cx={Math.cos((a * Math.PI) / 180) * 10} cy={Math.sin((a * Math.PI) / 180) * 10} r={8} fill={col} />
          ))}
          <circle r={6} fill="#f6b93b" />
        </g>
      );
    })}
    <Tint color="#27304d" o={dark * 0.55} />
  </g>
);

export const Sunset: React.FC<{ t: number }> = ({ t }) => (
  <g>
    <defs>
      <linearGradient id="sunset" x1="0" y1="0" x2="0" y2="1">
        <stop offset="0" stopColor="#5b4b9a" />
        <stop offset="0.45" stopColor="#ff8f66" />
        <stop offset="0.8" stopColor="#ffd27a" />
      </linearGradient>
    </defs>
    <rect x={X0} y={Y0} width={W} height={H} fill="url(#sunset)" />
    <circle cx={540} cy={930} r={260} fill="#fff1b8" opacity={0.9} />
    {[0, 1, 2, 3, 4].map((i) => (
      <Cloud
        key={i}
        x={2300 - ((t * (50 + i * 22) + i * 520) % 2900) - 520}
        y={90 + i * 150}
        s={0.8 + (i % 3) * 0.3}
        fill="#ffd9d0"
        o={0.85}
      />
    ))}
    {Array.from({ length: 9 }).map((_, i) => {
      const hx = i * 330 - ((t * 80) % 330) - 600;
      const hh = 150 + rnd(i % 5) * 150;
      return (
        <g key={i} transform={`translate(${hx} 1090)`} fill="#3a2a4a">
          <rect x={0} y={-hh} width={240} height={hh + 900} />
          <polygon points={`-20,${-hh} 120,${-hh - 110} 260,${-hh}`} />
          <rect x={40} y={-hh + 40} width={46} height={56} fill="#ffd94a" />
          <rect x={150} y={-hh + 40} width={46} height={56} fill="#ffd94a" opacity={i % 2 ? 1 : 0.25} />
        </g>
      );
    })}
  </g>
);

export const Night: React.FC<{ t: number; moonSmile?: number }> = ({ t, moonSmile = 0 }) => (
  <g>
    <defs>
      <linearGradient id="night" x1="0" y1="0" x2="0" y2="1">
        <stop offset="0" stopColor="#0b1030" />
        <stop offset="1" stopColor="#2c3a78" />
      </linearGradient>
    </defs>
    <rect x={X0} y={Y0} width={W} height={H} fill="url(#night)" />
    {Array.from({ length: 70 }).map((_, i) => (
      <circle
        key={i}
        cx={X0 + rnd(i) * W}
        cy={Y0 + rnd(i + 99) * (GROUND - Y0 - 60)}
        r={2 + rnd(i + 7) * 3}
        fill="white"
        opacity={0.45 + 0.55 * Math.abs(Math.sin(t * (0.8 + rnd(i + 3)) + i))}
      />
    ))}
    <g transform="translate(790 230)">
      <circle r={190} fill="#fff6c9" opacity={0.12} />
      <circle r={150} fill="#fff1a8" />
      <circle cx={-60} cy={-50} r={22} fill="#eeda86" />
      <circle cx={50} cy={-70} r={15} fill="#eeda86" />
      <circle cx={70} cy={50} r={26} fill="#eeda86" />
      <circle cx={-50} cy={70} r={16} fill="#eeda86" />
      <g opacity={moonSmile}>
        <path d="M -40 -6 Q -28 -20 -16 -6" stroke="#b89a3c" strokeWidth={7} fill="none" strokeLinecap="round" />
        <path d="M 16 -6 Q 28 -20 40 -6" stroke="#b89a3c" strokeWidth={7} fill="none" strokeLinecap="round" />
        <path d="M -34 28 Q 0 62 34 28" stroke="#b89a3c" strokeWidth={7} fill="none" strokeLinecap="round" />
      </g>
    </g>
    <rect x={20} y={660} width={90} height={150} fill="#343c74" />
    <rect x={8} y={645} width={114} height={26} fill="#3f4888" />
    <rect x={X0} y={GROUND} width={W} height={H} fill="#232a52" />
    <rect x={X0} y={GROUND} width={W} height={16} fill="#39427e" />
    {Array.from({ length: 14 }).map((_, r) =>
      Array.from({ length: 16 }).map((__, c) => (
        <path
          key={`${r}-${c}`}
          d={`M ${X0 + c * 140 + (r % 2) * 70} ${GROUND + 40 + r * 62} q 70 44 140 0`}
          stroke="#2f3768"
          strokeWidth={6}
          fill="none"
        />
      )),
    )}
  </g>
);

export const Sunburst: React.FC<{ t: number }> = ({ t }) => (
  <g>
    <rect x={X0} y={Y0} width={W} height={H} fill="#ffe9c4" />
    <g transform={`translate(540 560) rotate(${t * 5})`}>
      {Array.from({ length: 12 }).map((_, i) => (
        <polygon key={i} points="0,0 -220,-2400 220,-2400" fill="#ffdca6" transform={`rotate(${i * 30})`} />
      ))}
    </g>
  </g>
);
