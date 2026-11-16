import * as THREE from "three";
import { pop, rnd, tw } from "../cartoon/util";
import { Ant3, Baker3, Ball, Bird3, Box, Cookie3, Crumb3, Hearts3, Mini3, Mood, Rig } from "./parts";
import { Scene3Id } from "./script3d";
import { Bakery3, Cloud3, Door3, Ground, Lights, Meadow3, Mushroom3, Shelf3, Sign3, Sky, Tree3 } from "./world";

type P = { t: number };

const Warm: React.FC<P & { bites?: number }> = ({ t, bites = 0 }) => (
  <>
    <Sky top="#ffd9a0" bottom="#fff3dc" />
    <Lights />
    <Ground c="#ffe2b4" />
    <Rig pos={[Math.sin(t * 0.5) * 2.5, 3.2, 15]} look={[0, 2.3, 0]} />
    <Cookie3
      p={[0, 0, 0]}
      t={t}
      s={1.45 * pop(t, 0.5, 0.6)}
      turn={tw(t, 0.5, 2.2, -Math.PI * 2, 0)}
      arm={t > 2.2 ? "wave" : "up"}
      bites={bites}
    />
  </>
);

const Oven: React.FC<P> = ({ t }) => {
  const trayX = tw(t, 1.4, 3.4, -3.2, 1.6);
  const z = tw(t, 4.4, 6, 0, 1);
  const mood: Mood = t < 4.2 ? "sleep" : t < 5.6 ? "wow" : "happy";
  const glow = 0.6 + Math.sin(t * 6) * 0.25;
  return (
    <>
      <Bakery3 windowX={3} />
      <Rig pos={[z * 3.9, 3.2 - z * 0.7, 21 - z * 13.5]} look={[z * 3.9, 2.2 - z * 0.45, 0]} />
      <Box p={[4, 0.5, -0.4]} s={[12, 1, 2.6]} c="#9a6238" />
      <Box p={[trayX, 1.08, -0.3]} s={[5.8, 0.14, 1.7]} c="#aab4bf" />
      {[-2.2, -1.1, 0, 1.1].map((dx) => (
        <Mini3 key={dx} p={[trayX + dx, 1.68, -0.3]} t={t} r={0.5} turn={0.2} />
      ))}
      <Cookie3 p={[trayX + 2.3, 1.15, -0.3]} s={0.5} t={t} sit mood={mood} arm={t > 6.6 ? "wave" : "down"} />
      <Box p={[-4.3, 2.2, -0.6]} s={[4.4, 4.4, 3.6]} c="#8d99a6" />
      <Box p={[-4.3, 4.15, -0.55]} s={[4.5, 0.5, 3.7]} c="#77828f" />
      <Box p={[-4.3, 1.9, 1.22]} s={[3.2, 2.5, 0.06]} c={t < 1.6 ? new THREE.Color("#ff9a3c").multiplyScalar(glow + 0.4).getStyle() : "#5a3a26"} glow />
      {[-5.4, -4.7, -3.9, -3.2].map((kx) => (
        <Ball key={kx} p={[kx, 4.15, 1.3]} r={0.14} c="#5d6772" />
      ))}
    </>
  );
};

const ShelfScene: React.FC<P> = ({ t }) => {
  const leave = [1.5, 3, 4.5, 6];
  const eve = tw(t, 6, 8.5, 0, 1);
  return (
    <>
      <Bakery3 windowX={-2.5} evening={eve} dim={1 - eve * 0.55} />
      <Rig pos={[tw(t, 0, 9, -1.5, 2), 3.4, 21 - eve * 5]} look={[eve * 1.6, 2.6, 0]} />
      <Shelf3 y={1.8} />
      {[-3.3, -1.7, -0.1, 1.5].map((x, i) => {
        const lt = Math.max(0, t - leave[i]);
        return <Mini3 key={x} p={[x, 2.45 + lt * lt * 7, -1.6 + lt * 3]} t={t} r={0.6 * Math.max(0, 1 - lt * 0.9)} turn={lt * 6} />;
      })}
      <Cookie3 p={[3.2, 1.8, -1.6]} s={0.64} t={t} sit mood={t < 5.6 ? "happy" : "sad"} arm={t > 2.4 && t < 5.2 ? "wave" : "down"} turn={t < 6 ? -0.4 : 0} />
    </>
  );
};

const Leaving: React.FC<P> = ({ t }) => {
  const mood: Mood = t < 1.4 ? "sad" : t < 2.6 ? "wow" : "happy";
  const j = tw(t, 3, 3.8, 0, 1);
  const w = tw(t, 4, 6.6, 0, 1);
  return (
    <>
      <Bakery3 windowX={-0.5} evening={1} dim={0.5} />
      <Door3 x={-4.2} glow="#ffcf70" />
      <Rig pos={[1 - w * 2.5, 3.2, 20]} look={[1 - w * 2.5, 2.2, 0]} />
      <Shelf3 y={1.8} x={1.5} w={6} />
      <Cookie3
        p={[3.2 - j * 1.6 - w * 5.4, 1.8 * (1 - j) + Math.sin(j * Math.PI) * 1.4, -1.6 + j * 1.2 - w * 1.6]}
        s={0.64}
        t={t}
        mood={mood}
        sit={t < 3}
        walk={t > 4 && t < 6.6}
        arm={t > 2.4 && t < 3 ? "up" : "down"}
        turn={t > 1.4 ? -0.8 : 0}
      />
    </>
  );
};

const AntScene: React.FC<P> = ({ t }) => {
  const given = t > 5.1;
  const out = tw(t, 8, 10, 0, 9);
  const kx = tw(t, 0, 2, -7.5, -1.8) + out;
  const ax = tw(t, 0, 2, 4.8, 2) + out;
  return (
    <>
      <Meadow3 t={t} />
      <Rig pos={[Math.sin(t * 0.35) * 2.5, 3.4, 20 - tw(t, 2, 5, 0, 3)]} look={[0, 2, 0]} />
      <Cookie3 p={[kx, 0, 0]} t={t} walk={t < 2 || t > 8} bites={t > 4 ? 1 : 0} turn={0.55} arm={t > 3.3 && t < 4.6 ? "up" : t > 5.5 && t < 8 ? "wave" : "down"} />
      <Ant3 p={[ax, 0, 0.3]} t={t} flip={t < 7.8} walk={t < 2 || t > 8} carry={!given} happy={given} hop={given && t < 7.8 ? Math.abs(Math.sin(t * 7)) * 0.25 : 0} />
      <Crumb3 t={t} start={4.1} from={[-0.9, 2.1, 0.2]} to={[1.4, 0.7, 0.3]} />
      <Hearts3 p={[0.2, 3.4, 0.5]} t={t} start={5.3} />
    </>
  );
};

const Rain: React.FC<P> = ({ t }) => {
  const dark = Math.min(tw(t, 0.4, 1.4, 0, 1), tw(t, 5.4, 6.6, 1, 0));
  const run = tw(t, 3, 4.2, 0, 1);
  const kx = tw(t, 0, 1.2, -7.5, -2.8) + run * 4.6;
  const ax = tw(t, 0, 1.2, -5.5, -0.8) + run * 4.6;
  const mood: Mood = t > 1.3 && t < 4.3 ? "scared" : t > 6 ? "wow" : "happy";
  const rain = tw(t, 1, 1.6, 0, 1) * tw(t, 5.3, 5.9, 1, 0);
  const bow = tw(t, 6, 7.4, 0, 1);
  return (
    <>
      <Meadow3 t={t} dim={1 - dark * 0.8} clouds={false} />
      <Rig pos={[1 + run * 1.5, 3.4, 20 - run * 3]} look={[1 + run * 1.2, 2.2, 0]} />
      {[-9, -4.5, 0, 4.5, 9].map((x, i) => (
        <Cloud3 key={x} p={[x + Math.sin(t + i) * 0.3, 9.5 + (i % 2) + (1 - dark) * 12, -6]} s={2.2} c="#5d6680" />
      ))}
      {rain > 0 &&
        Array.from({ length: 140 }).map((_, i) => {
          const x = rnd(i) * 26 - 13;
          const z = rnd(i + 200) * 9 - 5;
          const y = 12 - ((t * 13 + rnd(i + 77) * 13) % 13);
          if (Math.hypot(x - 2.7, z) < 2.1 && y < 3.6) return null;
          return (
            <mesh key={i} position={[x, y, z]} rotation={[0, 0, 0.12]}>
              <cylinderGeometry args={[0.02, 0.02, 0.5, 4]} />
              <meshBasicMaterial color="#b9ddff" transparent opacity={rain * 0.8} />
            </mesh>
          );
        })}
      {bow > 0 &&
        ["#ff6b6b", "#ffa94d", "#ffe066", "#69db7c", "#4dabf7", "#b197fc"].map((c, i) => (
          <mesh key={c} position={[0, -1, -20]}>
            <torusGeometry args={[14 - i * 0.6, 0.3, 8, 60, Math.PI]} />
            <meshBasicMaterial color={c} transparent opacity={bow * 0.85} />
          </mesh>
        ))}
      <Mushroom3 p={[2.7, 0, -0.6]} />
      <Cookie3 p={[kx, 0, 0.4]} s={0.95} t={t} bites={1} mood={mood} walk={t < 1.2 || (t > 3 && t < 4.2)} turn={t > 4.3 ? -0.3 : 0.5} />
      <Ant3 p={[ax, 0, 0.9]} t={t} walk={t < 1.2 || (t > 3 && t < 4.2)} flip={t > 2 && t < 3} hop={t > 2 && t < 3 ? Math.abs(Math.sin(t * 9)) * 0.3 : 0} />
    </>
  );
};

const BirdScene: React.FC<P> = ({ t }) => {
  const land = tw(t, 0.4, 1.7, 0, 1);
  const bx = 8 - land * 6.6 - tw(t, 2.2, 3.2, 0, 0.5);
  const fed = t > 7.3;
  const kMood: Mood = t < 1.2 ? "happy" : t < 4.2 ? "scared" : t < 5.5 ? "wow" : "happy";
  return (
    <>
      <Meadow3 t={t} />
      <Rig pos={[Math.sin(t * 0.3) * 2 + 0.5, 3.8, 21]} look={[0.5, 2.9, 0]} />
      <Tree3 p={[4.3, 0, -1.6]} t={t} fed={fed} />
      <Ant3 p={[-4.1, 0, 0.5]} t={t} hop={fed ? Math.abs(Math.sin(t * 7)) * 0.2 : 0} />
      <Cookie3 p={[-2.2, 0, 0]} s={0.92} t={t} bites={t > 6 ? 2 : 1} mood={kMood} turn={0.5} arm={t > 5.4 && t < 7 ? "up" : fed ? "wave" : "down"} />
      <Bird3 p={[bx, (1 - land) * 9, 0.2]} t={t} s={1.35} fly={t < 1.7} hop={fed ? Math.abs(Math.sin(t * 5)) * 0.2 : 0} />
      {[5.6, 6.0, 6.4].map((s, i) => (
        <Crumb3 key={s} t={t} start={s} from={[-1.4, 2.3, 0.2]} to={[2.3 + i * 0.42, 4.0, -1.2]} dur={0.9} />
      ))}
      <Hearts3 p={[2.7, 4.9, -1]} t={t} start={7.4} />
      <Hearts3 p={[bx - 0.6, 3.4, 0.6]} t={t} start={8.6} />
    </>
  );
};

const Flight: React.FC<P> = ({ t }) => {
  const by = 2.2 + Math.sin(t * 1.6) * 0.3;
  const worried = t > 4.6;
  return (
    <>
      <Sky top="#5b4b9a" bottom="#ffc46b" />
      <Lights color="#ffd0a0" from={[-8, 6, 10]} />
      <Ball p={[0, 2, -60]} r={9} c="#fff1b8" glow />
      <Rig pos={[Math.sin(t * 0.4) * 4, 4.2, 21 - tw(t, 4, 6, 0, 5)]} look={[0, 4.2, 0]} />
      {[0, 1, 2, 3, 4].map((i) => (
        <Cloud3 key={i} p={[18 - ((t * (2 + i * 0.7) + i * 9) % 40), 3 + i * 2.2, -8 - i * 3]} s={1.3 + (i % 3) * 0.4} c="#ffd9d0" />
      ))}
      {Array.from({ length: 22 }).map((_, i) => {
        const row = i % 2;
        const x = 34 - ((t * 3.2 + i * 3.4) % 68);
        const h = 5 + rnd(i) * 5;
        const z = -3 - row * 7;
        return (
          <group key={i} position={[x, -13, z]}>
            <Box p={[0, h / 2, 0]} s={[3, h, 3]} c={row ? "#4a3760" : "#3a2a4a"} />
            <mesh position={[0, h + 0.9, 0]} rotation={[0, Math.PI / 4, 0]} castShadow>
              <coneGeometry args={[2.5, 1.8, 4]} />
              <meshStandardMaterial color="#2c1f3a" />
            </mesh>
            <Box p={[-0.6, h - 1, 1.52]} s={[0.6, 0.8, 0.05]} c="#ffd94a" glow />
            {i % 3 !== 0 && <Box p={[0.6, h - 1, 1.52]} s={[0.6, 0.8, 0.05]} c="#ffd94a" glow />}
          </group>
        );
      })}
      <group position={[0, by, 0]} rotation={[0, 0, Math.sin(t * 1.6 + 1) * 0.05]}>
        <Bird3 p={[0, 0, 0]} t={t} s={2.1} flip fly />
        <Ant3 p={[-1.5, 2.55, 0.2]} t={t} s={1.05} />
        <Cookie3 p={[-0.1, 2.85, 0.1]} s={0.72} t={t} bites={2} sit mood={worried ? "sad" : "happy"} arm={worried ? "down" : "up"} turn={0.3} />
      </group>
    </>
  );
};

const NightScene: React.FC<P> = ({ t }) => {
  const smile = t > 8.3;
  return (
    <>
      <Sky top="#070b24" bottom="#2c3a78" />
      <ambientLight intensity={0.9} color="#8a9bff" />
      <directionalLight position={[6, 10, -4]} intensity={1.6} color="#cfd8ff" castShadow shadow-mapSize={[2048, 2048]}>
        <orthographicCamera attach="shadow-camera" args={[-12, 12, 12, -12, 0.1, 50]} />
      </directionalLight>
      <pointLight position={[0, 3, 6]} intensity={30} color="#ffe2b0" distance={20} />
      <Rig pos={[Math.sin(t * 0.25) * 2, 3.4, 21 - tw(t, 0, 11, 0, 5)]} look={[0, 2.6, 0]} />
      {Array.from({ length: 90 }).map((_, i) => (
        <Ball
          key={i}
          p={[rnd(i) * 90 - 45, 3 + rnd(i + 99) * 34, -55]}
          r={0.1 + rnd(i + 7) * 0.14}
          c="#ffffff"
          glow
          o={0.4 + 0.6 * Math.abs(Math.sin(t * (0.8 + rnd(i + 3)) + i))}
        />
      ))}
      <Ball p={[5.5, 11, -40]} r={4} c="#fff1a8" glow />
      <Ball p={[4.2, 12, -36.4]} r={0.7} c="#eeda86" glow />
      <Ball p={[6.6, 10, -36.3]} r={0.9} c="#eeda86" glow />
      <Ball p={[5.2, 9.2, -36.4]} r={0.5} c="#eeda86" glow />
      <mesh rotation={[-Math.PI / 2 + 0.12, 0, 0]} position={[0, 0, 0]} receiveShadow>
        <planeGeometry args={[400, 200]} />
        <meshStandardMaterial color="#2a3263" roughness={1} />
      </mesh>
      {Array.from({ length: 9 }).map((_, r) => (
        <Box key={r} p={[0, -0.03 - r * 0.12, r * 1.0 + 0.6]} s={[400, 0.08, 0.1]} c="#39427e" />
      ))}
      <Box p={[-5.2, 1, -1.5]} s={[1.1, 2.4, 1.1]} c="#343c74" />
      <Box p={[-5.2, 2.3, -1.5]} s={[1.4, 0.3, 1.4]} c="#3f4888" />
      <Ant3 p={[-2.6, 0, 0.3]} t={t} hop={t > 0.5 && t < 3 ? Math.abs(Math.sin(t * 7)) * 0.16 : 0} />
      <Cookie3 p={[-0.3, 0, 0]} s={0.85} t={t} bites={2} sit mood={smile ? "happy" : "sad"} arm={smile ? "up" : "down"} turn={smile ? 0.3 : 0} />
      <Bird3 p={[2.5, 0, 0.2]} t={t} hop={t > 1 && t < 7.5 ? Math.abs(Math.sin(t * 5)) * 0.12 : 0} />
      <Hearts3 p={[-2.3, 1.6, 0.6]} t={t} start={5.6} n={2} />
      <Hearts3 p={[1.8, 2.8, 0.6]} t={t} start={6.2} n={2} />
      <Hearts3 p={[-0.3, 2.9, 0.6]} t={t} start={8.6} n={3} />
    </>
  );
};

const Home: React.FC<P> = ({ t }) => {
  const inn = tw(t, 0.2, 2.6, 0, 1);
  const bMood = t < 2.6 ? "neutral" : t < 4 ? "wow" : "happy";
  return (
    <>
      <Bakery3 windowX={-8} />
      <Door3 x={7.5} glow="#fff0b8" />
      <Rig pos={[0.4 + Math.sin(t * 0.4) * 1.5, 3.6, 22]} look={[0.4, 3.1, 0]} />
      <Baker3 p={[-3.6, 0, -0.6]} t={t} mood={bMood} />
      <Shelf3 y={3.5} x={1.9} w={6.6} z={-2.1} />
      {[-0.5, 1.1, 2.7, 4.3].map((x, i) => (
        <Mini3 key={x} p={[x, 4.06 + Math.abs(Math.sin(t * 4 + i * 1.3)) * 0.12, -2.1]} t={t} r={0.52 * pop(t, 4.6 + i * 0.35)} notch />
      ))}
      <Sign3 p={[1.9, 6.6 + (1 - pop(t, 4.1, 0.6)) * 5, -2.7]} text="Krikšķa cepumi" s={t > 4.1 ? 1 : 0} />
      <Cookie3 p={[9 - inn * 8.6, 0, 0.4]} s={0.85} t={t} bites={2} walk={t > 0.2 && t < 2.6} turn={-0.5} arm={t > 2.8 ? "wave" : "down"} />
      <Ant3 p={[10.6 - inn * 8.6, 0, 0.9]} t={t} flip walk={t < 2.6} hop={t > 4 ? Math.abs(Math.sin(t * 6)) * 0.15 : 0} />
      <Bird3 p={[12.4 - inn * 8.6, 0, 0.3]} t={t} s={1.1} hop={Math.abs(Math.sin(t * 6 + 1)) * 0.18} />
      <Hearts3 p={[-1.2, 4.6, 0.5]} t={t} start={4.2} />
      <Hearts3 p={[1.2, 2.9, 1]} t={t} start={6} />
    </>
  );
};

const End: React.FC<P> = ({ t }) => <Warm t={t + 3} bites={2} />;

export const sceneArt3: Record<Scene3Id, React.FC<P>> = {
  title: Warm,
  oven: Oven,
  shelf: ShelfScene,
  leaving: Leaving,
  ant: AntScene,
  rain: Rain,
  bird: BirdScene,
  flight: Flight,
  night: NightScene,
  home: Home,
  end: End,
};
