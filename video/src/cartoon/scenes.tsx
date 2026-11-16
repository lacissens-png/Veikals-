import { Bakery, Cloud, Door, Meadow, Night, Shelf, Sunburst, Sunset, Tint } from "./backgrounds";
import { Ant, Baker, Bird, Cookie, Crumb, Hearts, MiniCookie, Mood } from "./characters";
import { MORAL, SceneId, THE_END, TITLE } from "./script";
import { font, GROUND, pop, tw } from "./util";

type P = { t: number };
const BROWN = "#7a4420";

const Title: React.FC<P> = ({ t }) => (
  <g>
    <Sunburst t={t} />
    {TITLE.map((line, i) => (
      <text
        key={line}
        x={540}
        y={210 + i * 160}
        textAnchor="middle"
        fontFamily={font}
        fontWeight={900}
        fontSize={150}
        fill={BROWN}
        opacity={tw(t, 0.6 + i * 0.5, 1.2 + i * 0.5, 0, 1)}
        transform={`translate(0 ${(1 - pop(t, 0.6 + i * 0.5)) * 60})`}
      >
        {line}
      </text>
    ))}
    <Cookie x={540} y={700} s={1.4 * pop(t, 1.8, 0.7)} t={t} arm={t > 3 ? "wave" : "down"} />
  </g>
);

const Oven: React.FC<P> = ({ t }) => {
  const z = tw(t, 12, 15, 1, 2.1);
  const trayX = tw(t, 6.5, 9.5, 20, 470);
  const mood: Mood = t < 11 ? "sleep" : t < 14 ? "wow" : "happy";
  const glow = 0.55 + Math.sin(t * 5) * 0.2;
  return (
    <g transform={`translate(${(540 - 970 * z) * ((z - 1) / 1.1)} ${(600 - 690 * z) * ((z - 1) / 1.1)}) scale(${z})`}>
      <Bakery />
      <rect x={430} y={724} width={1300} height={GROUND - 724} fill="#9a6238" />
      <rect x={430} y={724} width={1300} height={16} fill="#7f4f2b" />
      <g transform={`translate(${trayX} 0)`}>
        <rect x={0} y={700} width={580} height={24} rx={8} fill="#aab4bf" />
        {[62, 172, 282, 392].map((cx) => (
          <MiniCookie key={cx} x={cx} y={652} r={48} t={t} />
        ))}
        <Cookie x={505} y={630} s={0.5} t={t} mood={mood} arm={t > 17 ? "wave" : "down"} sit />
      </g>
      <rect x={-520} y={360} width={1010} height={GROUND - 360} rx={26} fill="#8d99a6" />
      <rect x={-520} y={360} width={1010} height={60} rx={26} fill="#77828f" />
      <rect x={110} y={470} width={320} height={250} rx={18} fill="#3b2a22" />
      <rect x={110} y={470} width={320} height={250} rx={18} fill="#ff9a3c" opacity={t < 6.5 ? glow : 0.25} />
      <rect x={110} y={740} width={320} height={16} rx={8} fill="#5d6772" />
      {[150, 230, 310, 390].map((kx) => (
        <circle key={kx} cx={kx} cy={392} r={14} fill="#5d6772" />
      ))}
      {t > 5.8 && t < 8.2 && (
        <text
          x={270}
          y={300}
          textAnchor="middle"
          fontFamily={font}
          fontWeight={900}
          fontSize={110 * pop(t, 5.8)}
          fill="#f08a2b"
          transform={`rotate(${Math.sin(t * 30) * 4} 270 300)`}
        >
          DZIN!
        </text>
      )}
    </g>
  );
};

const ShelfScene: React.FC<P> = ({ t }) => {
  const leave = [6, 10, 14, 18];
  const mood: Mood = t < 15 ? "happy" : "sad";
  return (
    <g>
      <Bakery evening={tw(t, 19, 25, 0, 1)} />
      <Shelf y={640} />
      {[210, 370, 530, 690].map((cx, i) => {
        const lt = t - leave[i];
        return (
          <g key={cx}>
            <MiniCookie x={cx} y={582 - (lt > 0 ? lt * lt * 260 : 0)} r={58} t={t} o={lt > 0 ? Math.max(0, 1 - lt * 1.2) : 1} />
            {lt > 0 && lt < 1.2 && (
              <text x={cx} y={520 - lt * 60} textAnchor="middle" fontFamily={font} fontWeight={900} fontSize={70} fill="#f2b632" opacity={1 - lt / 1.2}>
                ✦
              </text>
            )}
          </g>
        );
      })}
      <Cookie
        x={860}
        y={568}
        s={0.64}
        t={t}
        mood={mood}
        sit
        arm={t > 9 && t < 14.5 ? "wave" : "down"}
        look={t < 19 ? -1 : 0}
      />
      <Tint color="#2a1a5e" o={tw(t, 19, 25, 0, 0.38)} />
    </g>
  );
};

const Leaving: React.FC<P> = ({ t }) => {
  const mood: Mood = t < 6.5 ? "sad" : t < 11 ? "wow" : "happy";
  const jp = tw(t, 13.5, 14.8, 0, 1);
  const wx = tw(t, 15.2, 22, 0, 1);
  const x = 860 - jp * 160 - wx * 520;
  const y = 568 + jp * 136 - Math.sin(jp * Math.PI) * 150;
  return (
    <g>
      <Bakery evening={1} />
      <Door x={70} glow="#ffcf70" />
      <Shelf y={640} />
      <Cookie
        x={x}
        y={y}
        s={0.64 * (1 - tw(t, 20, 22.5, 0, 0.5))}
        t={t}
        mood={mood}
        sit={t < 13.5}
        walk={t > 15.2 && t < 22}
        look={t > 6.5 ? -1 : 0}
        arm={t > 11 && t < 13.5 ? "up" : "down"}
      />
      {t > 2 && t < 6.2 && (
        <g transform={`translate(760 400) scale(${pop(t, 2)})`}>
          <circle cx={62} cy={78} r={9} fill="white" />
          <circle cx={40} cy={50} r={15} fill="white" />
          <ellipse cx={0} cy={0} rx={70} ry={54} fill="white" />
          <text x={0} y={26} textAnchor="middle" fontFamily={font} fontWeight={900} fontSize={76} fill={BROWN}>
            ?
          </text>
        </g>
      )}
      <Tint color="#2a1a5e" o={0.3} />
    </g>
  );
};

const AntScene: React.FC<P> = ({ t }) => {
  const given = t > 14;
  const out = tw(t, 24.5, 30, 0, 1);
  const kx = tw(t, 0, 5, -140, 360) + out * 1000;
  const ax = tw(t, 0, 5, 930, 740) + out * 1000;
  return (
    <g>
      <Meadow t={t} />
      <Cookie
        x={kx}
        y={GROUND - 150}
        t={t}
        walk={t < 5 || t > 24.5}
        bites={t > 13 ? 1 : 0}
        look={1}
        arm={t > 11.5 && t < 14 ? "up" : t > 17 && t < 22 ? "wave" : "down"}
      />
      <Ant
        x={ax}
        y={GROUND}
        t={t}
        s={1.5}
        flip={t < 24}
        walk={t < 5 || t > 24.5}
        carry={!given}
        mood={given ? "happy" : "sad"}
        hop={given && t < 22 ? Math.abs(Math.sin(t * 7)) * 22 : 0}
      />
      {!given && t > 5 && (
        <g fill="#7cc8ff">
          <circle cx={ax - 30} cy={GROUND - 120 - ((t * 40) % 30)} r={7} />
          <circle cx={ax + 10} cy={GROUND - 135 - ((t * 40 + 15) % 30)} r={6} />
        </g>
      )}
      <Crumb t={t} start={13.2} from={[430, 600]} to={[690, GROUND - 60]} />
      <Hearts x={(kx + ax) / 2} y={470} t={t} start={15.5} n={3} />
      <Hearts x={(kx + ax) / 2} y={470} t={t} start={20} n={2} />
    </g>
  );
};

const Rain: React.FC<P> = ({ t }) => {
  const dark = Math.min(tw(t, 1, 4, 0, 1), tw(t, 19, 23, 1, 0));
  const raining = tw(t, 3, 4.5, 0, 1) * tw(t, 18.5, 20.5, 1, 0);
  const run = tw(t, 11, 14, 0, 1);
  const kx = tw(t, 0, 3, -140, 280) + run * 470;
  const ax = tw(t, 0, 3, 20, 440) + run * 470;
  const mood: Mood = t > 4 && t < 14.5 ? "scared" : t < 22 ? "happy" : "wow";
  return (
    <g>
      <Meadow t={t} dark={dark} />
      <g opacity={tw(t, 21.5, 25, 0, 0.9)}>
        {["#ff6b6b", "#ffa94d", "#ffe066", "#69db7c", "#4dabf7", "#b197fc"].map((c, i) => (
          <circle key={c} cx={540} cy={GROUND} r={720 - i * 34} fill="none" stroke={c} strokeWidth={34} />
        ))}
      </g>
      <rect x={-520} y={GROUND} width={2120} height={1500} fill="#6fbd54" />
      <Tint color="#27304d" o={dark * 0.3} />
      {[0, 1, 2, 3, 4].map((i) => (
        <Cloud key={i} x={-380 + i * 440 + Math.sin(t + i) * 20} y={120 + (i % 2) * 70 - (1 - dark) * 700} s={1.9} fill="#5d6680" />
      ))}
      <g stroke="#a9d6ff" strokeWidth={6} strokeLinecap="round" opacity={raining * 0.85}>
        {Array.from({ length: 90 }).map((_, i) => {
          const rx = ((i * 137.5) % 2100) - 510;
          const ry = ((t * 950 + i * 211) % 1500) - 300;
          if (rx > 600 && rx < 1010 && ry > 500) return null;
          if (ry > GROUND + 500) return null;
          return <line key={i} x1={rx} y1={ry} x2={rx - 10} y2={ry + 46} />;
        })}
      </g>
      <rect x={770} y={560} width={76} height={GROUND - 560} rx={30} fill="#f6ead6" />
      <path d={`M 590 580 Q 610 380 808 370 Q 1006 380 1026 580 Z`} fill="#e5484d" />
      <circle cx={700} cy={500} r={26} fill="white" />
      <circle cx={820} cy={440} r={30} fill="white" />
      <circle cx={930} cy={520} r={22} fill="white" />
      <Cookie x={kx} y={GROUND - 150 * 0.95} s={0.95} t={t} bites={1} mood={mood} walk={t < 3 || (t > 11 && t < 14)} look={t > 14 ? -1 : 1} />
      <Ant
        x={ax}
        y={GROUND}
        t={t}
        s={1.5}
        walk={t < 3 || (t > 11 && t < 14)}
        flip={t > 8 && t < 11}
        hop={t > 8.5 && t < 11 ? Math.abs(Math.sin(t * 9)) * 26 : 0}
      />
    </g>
  );
};

const BirdScene: React.FC<P> = ({ t }) => {
  const land = tw(t, 1, 4, 0, 1);
  const bx = 1250 - land * 550 - tw(t, 6, 8.5, 0, 60);
  const by = GROUND - (1 - land) * 900;
  const fed = t > 19;
  const kMood: Mood = t < 3 ? "happy" : t < 11.5 ? "scared" : t < 15 ? "wow" : "happy";
  return (
    <g>
      <Meadow t={t} />
      <rect x={900} y={330} width={64} height={GROUND - 330} fill="#8a5a34" />
      <path d="M 920 520 Q 840 500 760 470" stroke="#8a5a34" strokeWidth={30} fill="none" strokeLinecap="round" />
      <circle cx={930} cy={230} r={170} fill="#5fae4a" />
      <circle cx={790} cy={300} r={110} fill="#6fbd54" />
      <circle cx={1070} cy={300} r={120} fill="#58a444" />
      {[-48, 0, 48].map((dx, i) => {
        const open = !fed && Math.sin(t * 9 + i * 2) > 0;
        return (
          <g key={dx} transform={`translate(${770 + dx} ${440 - (fed ? Math.abs(Math.sin(t * 6 + i)) * 10 : 0)})`}>
            <circle r={28} fill="#f6d84a" />
            <circle cx={-9} cy={-6} r={4} fill="#111" />
            <circle cx={9} cy={-6} r={4} fill="#111" />
            {open ? (
              <polygon points="-12,4 12,4 0,26" fill="#f39c34" />
            ) : (
              <polygon points="-9,6 9,6 0,15" fill="#f39c34" />
            )}
          </g>
        );
      })}
      <path d="M 680 452 Q 770 540 860 452 Z" fill="#9a6b3c" />
      <path d="M 680 452 L 860 452" stroke="#7b5129" strokeWidth={10} strokeLinecap="round" />
      <Ant x={150} y={GROUND} t={t} s={1.5} hop={fed ? Math.abs(Math.sin(t * 7)) * 18 : 0} />
      <Cookie
        x={340}
        y={GROUND - 150 * 0.92}
        s={0.92}
        t={t}
        bites={t > 16.5 ? 2 : 1}
        mood={kMood}
        look={1}
        arm={t > 15 && t < 19 ? "up" : fed ? "wave" : "down"}
      />
      <Bird x={bx} y={by} t={t} s={1.35} fly={t < 4} mood={fed ? "happy" : "neutral"} hop={fed ? Math.abs(Math.sin(t * 5)) * 16 : 0} />
      {[15.5, 16.6, 17.7].map((s, i) => (
        <Crumb key={s} t={t} start={s} from={[400, 560]} to={[722 + i * 48, 430]} dur={1.3} />
      ))}
      <Hearts x={770} y={370} t={t} start={19.2} n={3} />
      <Hearts x={bx - 60} y={GROUND - 290} t={t} start={22} n={3} />
    </g>
  );
};

const Flight: React.FC<P> = ({ t }) => {
  const by = 640 + Math.sin(t * 1.6) * 28;
  const worried = t > 17.5;
  return (
    <g>
      <Sunset t={t} />
      <g transform={`rotate(${Math.sin(t * 1.6 + 1) * 3} 540 ${by})`}>
        <Bird x={540} y={by + 212} t={t} s={2.5} flip fly mood="happy" />
        <Ant x={372} y={by - 128} t={t} s={1.05} />
        <Cookie x={520} y={by - 205} s={0.72} t={t} bites={2} sit mood={worried ? "sad" : "happy"} arm={worried ? "down" : "up"} look={worried ? 0 : 1} />
      </g>
    </g>
  );
};

const NightScene: React.FC<P> = ({ t }) => {
  const smile = t > 24.5;
  return (
    <g>
      <Night t={t} moonSmile={tw(t, 25.5, 27, 0, 1)} />
      <Ant x={300} y={GROUND} t={t} s={1.5} hop={t > 5 && t < 9.5 ? Math.abs(Math.sin(t * 7)) * 16 : 0} />
      <Cookie
        x={500}
        y={GROUND - 112 * 0.85}
        s={0.85}
        t={t}
        bites={2}
        sit
        mood={smile ? "happy" : "sad"}
        look={smile ? 1 : t > 5 && t < 10 ? -1 : 0}
        arm={smile ? "up" : "down"}
      />
      <Bird x={770} y={GROUND} t={t} s={1.3} mood="happy" hop={t > 15 && t < 24 ? Math.abs(Math.sin(t * 5)) * 12 : 0} />
      <Hearts x={300} y={GROUND - 150} t={t} start={20} n={2} />
      <Hearts x={730} y={GROUND - 260} t={t} start={21} n={2} />
      <Hearts x={500} y={GROUND - 260} t={t} start={25} n={3} />
    </g>
  );
};

const Return: React.FC<P> = ({ t }) => {
  const inn = tw(t, 1.5, 8, 0, 1);
  const bMood = t < 8.5 ? "neutral" : t < 14 ? "wow" : "happy";
  return (
    <g>
      <Bakery windowX={380} />
      <Door x={790} glow="#fff0b8" />
      <Baker x={230} y={GROUND} t={t} mood={bMood} />
      {t > 8.5 && t < 13 && (
        <text x={230} y={180} textAnchor="middle" fontFamily={font} fontWeight={900} fontSize={130 * pop(t, 8.5)} fill="#e5484d">
          !
        </text>
      )}
      {t > 20 && (
        <g transform={`translate(230 150) scale(${pop(t, 20)})`}>
          <circle r={95} fill="#ffe066" opacity={0.35 + Math.sin(t * 6) * 0.15} />
          <path d="M -34 -8 A 36 36 0 1 1 34 -8 Q 22 12 20 34 L -20 34 Q -22 12 -34 -8 Z" fill="#ffd43b" stroke="#e8a800" strokeWidth={5} />
          <rect x={-18} y={36} width={36} height={20} rx={5} fill="#868e96" />
        </g>
      )}
      <Bird x={1500 - inn * 560} y={GROUND} t={t} s={1.1} mood="happy" hop={t < 8 ? Math.abs(Math.sin(t * 8)) * 20 : 0} />
      <Ant x={1330 - inn * 560} y={GROUND} t={t} s={1.4} flip walk={t < 8} />
      <Cookie
        x={1150 - inn * 560}
        y={GROUND - 150 * 0.85}
        s={0.85}
        t={t}
        bites={2}
        walk={t > 1.5 && t < 8}
        look={-1}
        arm={t > 9 ? "wave" : "down"}
      />
      <Hearts x={410} y={330} t={t} start={14.5} n={3} />
    </g>
  );
};

const Family: React.FC<P> = ({ t }) => (
  <g>
    <Bakery windowX={-400} />
    <g transform={`translate(540 ${130 - (1 - pop(t, 0.5, 0.8)) * 400})`}>
      <rect x={-400} y={-70} width={800} height={130} rx={30} fill="#e5484d" />
      <rect x={-384} y={-54} width={768} height={98} rx={20} fill="none" stroke="#ffd9a8" strokeWidth={6} />
      <text x={0} y={22} textAnchor="middle" fontFamily={font} fontWeight={900} fontSize={78} fill="white">
        Krikšķa cepumi
      </text>
    </g>
    <Shelf y={500} />
    {[190, 320, 760, 890].map((cx, i) => (
      <MiniCookie
        key={cx}
        x={cx}
        y={446 - Math.abs(Math.sin(t * 4 + i * 1.3)) * 14 - (1 - pop(t, 2 + i * 0.5)) * 500}
        r={54}
        notch
        t={t}
      />
    ))}
    <Cookie x={540} y={500 - 150 * 0.8} s={0.8 * pop(t, 4.5)} t={t} bites={2} arm="wave" />
    {[0, 1, 2, 3].map((i) => {
      const px = tw(t, 8 + i * 1.2, 12 + i * 1.2, 1700, 620 + i * 125);
      const col = ["#4dabf7", "#f783ac", "#69db7c", "#ffa94d"][i];
      const bob = Math.abs(Math.sin(t * 3 + i)) * 10;
      return (
        <g key={i} transform={`translate(${px} ${GROUND - bob})`}>
          <rect x={-55} y={-160} width={110} height={160} rx={42} fill={col} />
          <circle cx={0} cy={-205} r={44} fill="#f2c9a0" />
          <circle cx={-16} cy={-210} r={5} fill="#2b1a10" />
          <circle cx={8} cy={-210} r={5} fill="#2b1a10" />
          <path d="M -18 -192 Q -4 -180 12 -192" stroke="#7a3030" strokeWidth={4} fill="none" strokeLinecap="round" />
        </g>
      );
    })}
    <Ant x={130} y={GROUND} t={t} s={1.4} hop={Math.abs(Math.sin(t * 6)) * 14} />
    <Bird x={360} y={GROUND} t={t} s={1.05} flip mood="happy" hop={Math.abs(Math.sin(t * 5 + 1)) * 12} />
    <Hearts x={255} y={640} t={t} start={14} n={3} />
    <Hearts x={255} y={640} t={t} start={19} n={3} />
  </g>
);

const End: React.FC<P> = ({ t }) => (
  <g>
    <Sunburst t={t} />
    {MORAL.map((line, i) => (
      <text
        key={line}
        x={540}
        y={170 + i * 100}
        textAnchor="middle"
        fontFamily={font}
        fontWeight={800}
        fontSize={80}
        fill={BROWN}
        opacity={tw(t, 0.5 + i * 1.2, 1.3 + i * 1.2, 0, 1)}
      >
        {line}
      </text>
    ))}
    <Cookie x={540} y={560} s={1.15} t={t} bites={2} arm="wave" />
    <text
      x={540}
      y={950}
      textAnchor="middle"
      fontFamily={font}
      fontWeight={900}
      fontSize={150 * pop(t, 5.5, 0.7)}
      fill="#e5484d"
    >
      {THE_END}
    </text>
  </g>
);

export const sceneArt: Record<SceneId, React.FC<P>> = {
  title: Title,
  oven: Oven,
  shelf: ShelfScene,
  leaving: Leaving,
  ant: AntScene,
  rain: Rain,
  bird: BirdScene,
  flight: Flight,
  night: NightScene,
  return: Return,
  family: Family,
  end: End,
};
