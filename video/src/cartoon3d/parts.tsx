import { useThree } from "@react-three/fiber";
import { useLayoutEffect, useMemo } from "react";
import * as THREE from "three";
import { cookieBump, cookieColor, plankColor } from "./textures";

export type V3 = [number, number, number];
export type Mood = "happy" | "sad" | "scared" | "wow" | "sleep";

const BODY = "#dd9d4f";
const CHIP = "#4f2c14";
const CHOC = "#3a1d0c";
const LIMB = "#8a5526";
const UP = new THREE.Vector3(0, 1, 0);

/** Kamera: novieto to un pagriež pret mērķi. Vertikālajā kadrā plašāks skata leņķis. */
export const Rig: React.FC<{ pos: V3; look: V3 }> = ({ pos, look }) => {
  const camera = useThree((s) => s.camera) as THREE.PerspectiveCamera;
  const size = useThree((s) => s.size);
  useLayoutEffect(() => {
    camera.fov = size.height > size.width ? 45 : 30;
    camera.position.set(pos[0], pos[1], pos[2]);
    camera.lookAt(look[0], look[1], look[2]);
    camera.updateProjectionMatrix();
  });
  return null;
};

export const Ball: React.FC<{
  p?: V3;
  r?: number;
  s?: V3;
  c: string;
  glow?: boolean;
  o?: number;
  rough?: number;
  metal?: number;
}> = ({ p = [0, 0, 0], r = 1, s = [1, 1, 1], c, glow = false, o = 1, rough = 0.75, metal = 0 }) => (
  <mesh position={p} scale={[s[0] * r, s[1] * r, s[2] * r]} castShadow={!glow}>
    <sphereGeometry args={[1, 28, 20]} />
    {glow ? (
      <meshBasicMaterial color={c} fog={false} transparent={o < 1} opacity={o} />
    ) : (
      <meshStandardMaterial color={c} roughness={rough} metalness={metal} />
    )}
  </mesh>
);

export const Box: React.FC<{
  p: V3;
  s: V3;
  c: string;
  glow?: boolean;
  rot?: V3;
  rough?: number;
  metal?: number;
  wood?: boolean;
}> = ({ p, s, c, glow = false, rot, rough = 0.85, metal = 0, wood = false }) => (
  <mesh position={p} rotation={rot} castShadow={!glow} receiveShadow>
    <boxGeometry args={s} />
    {glow ? (
      <meshBasicMaterial color={c} />
    ) : (
      <meshStandardMaterial color={c} roughness={rough} metalness={metal} map={wood ? plankColor() : null} />
    )}
  </mesh>
);

/** Cepuma mīklas materiāls ar nelīdzenu, apceptu virsmu. */
const Dough: React.FC = () => (
  <meshStandardMaterial color="#ffffff" map={cookieColor()} bumpMap={cookieBump()} bumpScale={2.2} roughness={0.95} />
);

/** Šokolādes gabaliņi: neregulāri un nedaudz spīdīgi. */
const Chips: React.FC<{ n?: number }> = ({ n = CHIPS.length }) => (
  <>
    {CHIPS.slice(0, n).map((c, i) => (
      <mesh key={i} position={c} rotation={[i * 1.7, i * 2.3, i]} scale={[0.14 + (i % 3) * 0.02, 0.12, 0.09]} castShadow>
        <icosahedronGeometry args={[1, 1]} />
        <meshStandardMaterial color={CHOC} roughness={0.32} flatShading />
      </mesh>
    ))}
  </>
);

export const Limb: React.FC<{ a: V3; b: V3; r?: number; c?: string }> = ({ a, b, r = 0.065, c = LIMB }) => {
  const va = new THREE.Vector3(...a);
  const vb = new THREE.Vector3(...b);
  const d = vb.clone().sub(va);
  const len = d.length();
  const q = new THREE.Quaternion().setFromUnitVectors(UP, d.normalize());
  return (
    <mesh position={va.add(vb).multiplyScalar(0.5)} quaternion={q} castShadow>
      <capsuleGeometry args={[r, len, 4, 10]} />
      <meshStandardMaterial color={c} roughness={0.8} />
    </mesh>
  );
};

const NOTCHES = [
  { a: 45, r: 0.38 },
  { a: 195, r: 0.28 },
  { a: 285, r: 0.26 },
];

/** Cepuma forma: aplis ar izkostiem robiem, izspiests biezumā. */
const useCookieGeometry = (notches: number) =>
  useMemo(() => {
    const shape = new THREE.Shape();
    const N = 180;
    for (let i = 0; i <= N; i++) {
      const ang = (i / N) * Math.PI * 2;
      const dx = Math.cos(ang);
      const dy = Math.sin(ang);
      let rho = 1;
      for (const n of NOTCHES.slice(0, notches)) {
        const cx = Math.cos((n.a * Math.PI) / 180) * 1.08;
        const cy = Math.sin((n.a * Math.PI) / 180) * 1.08;
        const dc = dx * cx + dy * cy;
        const disc = dc * dc - (cx * cx + cy * cy) + n.r * n.r;
        if (disc > 0 && dc > 0) rho = Math.min(rho, dc - Math.sqrt(disc));
      }
      if (i === 0) shape.moveTo(dx * rho, dy * rho);
      else shape.lineTo(dx * rho, dy * rho);
    }
    const g = new THREE.ExtrudeGeometry(shape, {
      depth: 0.2,
      bevelEnabled: true,
      bevelThickness: 0.1,
      bevelSize: 0.08,
      bevelSegments: 8,
      curveSegments: 8,
    });
    g.translate(0, 0, -0.1);
    return g;
  }, [notches]);

const CHIPS: V3[] = [
  [-0.58, 0.52, 0.17],
  [0.1, 0.68, 0.17],
  [0.66, -0.48, 0.17],
  [-0.62, -0.56, 0.17],
  [0.08, -0.8, 0.17],
  [0.78, 0.02, 0.17],
  [-0.2, 0.1, -0.17],
  [0.4, 0.4, -0.17],
  [-0.4, -0.4, -0.17],
  [-0.22, 0.42, 0.18],
  [0.42, 0.3, 0.18],
  [0.3, -0.62, 0.18],
  [-0.78, 0.02, 0.17],
];

const Face: React.FC<{ mood: Mood; t: number }> = ({ mood, t }) => {
  const blink = mood === "sleep" || t % 3.1 < 0.12;
  const big = mood === "scared" || mood === "wow" ? 1.25 : 1;
  return (
    <group position={[0, 0, 0.17]}>
      <Ball p={[-0.55, -0.2, 0]} r={0.14} s={[1, 0.8, 0.25]} c="#ee8a7c" />
      <Ball p={[0.55, -0.2, 0]} r={0.14} s={[1, 0.8, 0.25]} c="#ee8a7c" />
      {[-0.32, 0.32].map((ex) => (
        <group key={ex} position={[ex, 0.16, 0.02]} scale={[big, blink ? 0.12 : big, 1]}>
          <Ball r={0.18} s={[1, 1, 0.45]} c="#ffffff" rough={0.15} />
          <Ball p={[0, -0.01, 0.07]} r={0.095} s={[1, 1, 0.5]} c="#201208" rough={0.08} />
          <Ball p={[0.03, 0.04, 0.11]} r={0.03} c="#ffffff" glow />
        </group>
      ))}
      {mood === "sad" && (
        <>
          <Box p={[-0.36, 0.45, 0.04]} s={[0.3, 0.06, 0.05]} c={CHIP} rot={[0, 0, 0.3]} />
          <Box p={[0.36, 0.45, 0.04]} s={[0.3, 0.06, 0.05]} c={CHIP} rot={[0, 0, -0.3]} />
        </>
      )}
      {(mood === "happy" || mood === "sleep") && (
        <mesh position={[0, -0.22, 0.03]} rotation={[0, 0, Math.PI]} scale={mood === "sleep" ? 0.4 : 1}>
          <torusGeometry args={[0.28, 0.04, 8, 24, Math.PI]} />
          <meshStandardMaterial color={CHIP} />
        </mesh>
      )}
      {mood === "sad" && (
        <mesh position={[0, -0.6, 0.03]}>
          <torusGeometry args={[0.24, 0.04, 8, 24, Math.PI]} />
          <meshStandardMaterial color={CHIP} />
        </mesh>
      )}
      {(mood === "scared" || mood === "wow") && <Ball p={[0, -0.44, 0.02]} r={0.15} s={[1, 1.15, 0.3]} c={CHIP} />}
    </group>
  );
};

/** Galvenais varonis. p = vieta, kur viņš stāv (vai sēž) uz zemes. */
export const Cookie3: React.FC<{
  p: V3;
  t: number;
  s?: number;
  mood?: Mood;
  walk?: boolean;
  bites?: number;
  arm?: "down" | "up" | "wave";
  sit?: boolean;
  turn?: number;
}> = ({ p, t, s = 1, mood = "happy", walk = false, bites = 0, arm = "down", sit = false, turn = 0 }) => {
  const geo = useCookieGeometry(1 + bites);
  const cyc = Math.sin(t * 11);
  const bob = walk ? Math.abs(cyc) * 0.14 : Math.sin(t * 2.2) * 0.03;
  const shake = mood === "scared" ? Math.sin(t * 45) * 0.04 : 0;
  const sw = walk ? cyc * 0.28 : 0;
  const w = Math.sin(t * 9) * 0.18;
  const armL: V3 = arm === "down" ? [-1.36, -0.5, 0.1] : [-1.4, 0.5, 0.1];
  const armR: V3 = arm === "down" ? [1.36, -0.5, 0.1] : arm === "up" ? [1.4, 0.5, 0.1] : [1.38 + w * 0.4, 0.55 + w, 0.1];
  const cy = (sit ? 1.12 : 1.5) * s;
  return (
    <group position={[p[0] + shake, p[1] + cy + bob * s, p[2]]} rotation={[0, turn + Math.sin(t * 1.3) * 0.1, 0]} scale={s}>
      <mesh geometry={geo} castShadow receiveShadow>
        <Dough />
      </mesh>
      <Chips />
      <Face mood={mood} t={t} />
      {sit ? (
        <>
          <Limb a={[-0.34, -0.85, 0]} b={[-0.5, -1.08, 0.4]} />
          <Limb a={[0.34, -0.85, 0]} b={[0.5, -1.08, 0.4]} />
        </>
      ) : (
        <>
          <Limb a={[-0.34, -0.82, 0]} b={[-0.34, -1.4, sw]} />
          <Limb a={[0.34, -0.82, 0]} b={[0.34, -1.4, -sw]} />
          <Ball p={[-0.34, -1.42, sw + 0.1]} r={0.2} s={[1, 0.5, 1.4]} c={LIMB} />
          <Ball p={[0.34, -1.42, -sw + 0.1]} r={0.2} s={[1, 0.5, 1.4]} c={LIMB} />
        </>
      )}
      <Limb a={[-0.92, 0.05, 0]} b={armL} />
      <Limb a={[0.92, 0.05, 0]} b={armR} />
      <Ball p={armL} r={0.11} c={LIMB} />
      <Ball p={armR} r={0.11} c={LIMB} />
    </group>
  );
};

/** Mazs cepums bez rokām un kājām; p = centrs. */
export const Mini3: React.FC<{ p: V3; t: number; r?: number; notch?: boolean; turn?: number }> = ({
  p,
  t,
  r = 0.55,
  notch = false,
  turn = 0,
}) => {
  const geo = useCookieGeometry(notch ? 1 : 0);
  if (r <= 0.001) return null;
  return (
    <group position={p} scale={r} rotation={[0, turn, 0]}>
      <mesh geometry={geo} castShadow receiveShadow>
        <Dough />
      </mesh>
      <Chips n={9} />
      <Face mood="happy" t={t + p[0]} />
    </group>
  );
};

/** Skudra Tipa; p = vieta uz zemes. Pēc noklusējuma skatās pa labi. */
export const Ant3: React.FC<{
  p: V3;
  t: number;
  s?: number;
  flip?: boolean;
  walk?: boolean;
  happy?: boolean;
  carry?: boolean;
  hop?: number;
}> = ({ p, t, s = 1.5, flip = false, walk = false, happy = true, carry = false, hop = 0 }) => {
  const c = "#4a2f2a";
  const sw = walk ? Math.sin(t * 16) * 0.1 : 0;
  return (
    <group position={[p[0], p[1] + hop, p[2]]} rotation={[0, flip ? Math.PI + 0.45 : -0.45, 0]} scale={s}>
      {[-1, 1].map((z) => (
        <group key={z}>
          <Limb a={[-0.18, 0.28, z * 0.1]} b={[-0.32 + sw * z, 0, z * 0.34]} r={0.03} c={c} />
          <Limb a={[-0.02, 0.28, z * 0.1]} b={[-0.02 - sw * z, 0, z * 0.38]} r={0.03} c={c} />
          <Limb a={[0.12, 0.28, z * 0.1]} b={[0.26 + sw * z, 0, z * 0.34]} r={0.03} c={c} />
          <Limb a={[0.4, 0.7, z * 0.08]} b={[0.62, 1.0, z * 0.2]} r={0.022} c={c} />
          <Ball p={[0.5, 0.56, z * 0.14]} r={0.085} c="#ffffff" />
          <Ball p={[0.56, 0.56, z * 0.17]} r={0.045} c="#111111" rough={0.1} />
        </group>
      ))}
      <Ball p={[-0.5, 0.34, 0]} r={1} s={[0.36, 0.27, 0.28]} c={c} rough={0.25} />
      <Ball p={[-0.04, 0.32, 0]} r={0.21} c={c} rough={0.25} />
      <Ball p={[0.36, 0.52, 0]} r={0.26} c={c} rough={0.25} />
      {happy && (
        <mesh position={[0.55, 0.45, 0]} rotation={[0, Math.PI / 2, Math.PI]}>
          <torusGeometry args={[0.09, 0.02, 6, 16, Math.PI]} />
          <meshBasicMaterial color="#ffffff" />
        </mesh>
      )}
      {carry && <Ball p={[-0.25, 0.8, 0]} r={1} s={[0.45, 0.26, 0.3]} c="#e9c768" />}
    </group>
  );
};

/** Zīlīte Čiepa; p = vieta uz zemes (lidojot — zem vēdera). Pēc noklusējuma skatās pa kreisi. */
export const Bird3: React.FC<{
  p: V3;
  t: number;
  s?: number;
  flip?: boolean;
  fly?: boolean;
  hop?: number;
}> = ({ p, t, s = 1.3, flip = false, fly = false, hop = 0 }) => {
  const flap = fly ? Math.sin(t * 14) * 0.9 : Math.sin(t * 2) * 0.05;
  return (
    <group position={[p[0], p[1] + hop, p[2]]} rotation={[0, flip ? Math.PI - 0.4 : 0.4, 0]} scale={s}>
      {!fly &&
        [-0.16, 0.16].map((z) => (
          <group key={z}>
            <Limb a={[0, 0.4, z]} b={[0, 0.03, z]} r={0.035} c="#e58a2b" />
            <Limb a={[-0.16, 0.03, z]} b={[0.1, 0.03, z]} r={0.03} c="#e58a2b" />
          </group>
        ))}
      <Ball p={[0, 0.85, 0]} r={1} s={[0.78, 0.58, 0.6]} c="#f6d84a" />
      <mesh position={[0.95, 0.95, 0]} rotation={[0, 0, -1.25]} scale={[1, 1, 0.3]} castShadow>
        <coneGeometry args={[0.28, 0.9, 12]} />
        <meshStandardMaterial color="#4a6fa5" />
      </mesh>
      <Ball p={[-0.58, 1.33, 0]} r={0.42} c="#2f4a7a" />
      {[-1, 1].map((z) => (
        <group key={z}>
          <Ball p={[-0.66, 1.24, z * 0.26]} r={1} s={[0.26, 0.2, 0.18]} c="#ffffff" />
          <Ball p={[-0.76, 1.38, z * 0.33]} r={0.07} c="#111111" rough={0.08} />
          <group position={[0.05, 1.05, z * 0.5]} rotation={[z * (fly ? 0.9 + flap * 0.7 : flap - 1.2), 0, 0]}>
            <Ball p={[fly ? 0.3 : 0.15, fly ? 0.55 : 0.35, 0]} r={1} s={fly ? [0.45, 0.8, 0.08] : [0.5, 0.42, 0.1]} c="#5b7fb5" />
          </group>
        </group>
      ))}
      <mesh position={[-1.08, 1.27, 0]} rotation={[0, 0, Math.PI / 2]} castShadow>
        <coneGeometry args={[0.12, 0.36, 12]} />
        <meshStandardMaterial color="#f39c34" />
      </mesh>
    </group>
  );
};

/** Maiznieks; p = vieta starp pēdām. */
export const Baker3: React.FC<{ p: V3; t: number; mood?: "neutral" | "wow" | "happy" }> = ({ p, t, mood = "neutral" }) => {
  const up = mood !== "neutral";
  const bob = Math.sin(t * 2) * 0.03 + (mood === "happy" ? Math.abs(Math.sin(t * 6)) * 0.12 : 0);
  const hl: V3 = up ? [-1.5, 3.9, 0.2] : [-1.15, 1.8, 0.3];
  const hr: V3 = up ? [1.5, 3.9, 0.2] : [1.15, 1.8, 0.3];
  return (
    <group position={[p[0], p[1] + bob, p[2]]} rotation={[0, 0.35, 0]}>
      <Box p={[-0.32, 0.6, 0]} s={[0.42, 1.2, 0.45]} c="#4d5b7a" />
      <Box p={[0.32, 0.6, 0]} s={[0.42, 1.2, 0.45]} c="#4d5b7a" />
      <Ball p={[-0.34, 0.12, 0.15]} r={1} s={[0.34, 0.14, 0.5]} c="#33384a" />
      <Ball p={[0.34, 0.12, 0.15]} r={1} s={[0.34, 0.14, 0.5]} c="#33384a" />
      <mesh position={[0, 2.15, 0]} castShadow>
        <capsuleGeometry args={[0.95, 1.1, 8, 20]} />
        <meshStandardMaterial color="#ffffff" roughness={0.9} />
      </mesh>
      <Ball p={[0, 2.5, 0.93]} r={0.08} c="#c9c2b6" />
      <Ball p={[0, 2.0, 0.95]} r={0.08} c="#c9c2b6" />
      <Limb a={[-0.85, 2.9, 0]} b={hl} r={0.17} c="#ffffff" />
      <Limb a={[0.85, 2.9, 0]} b={hr} r={0.17} c="#ffffff" />
      <Ball p={hl} r={0.22} c="#f2c9a0" />
      <Ball p={hr} r={0.22} c="#f2c9a0" />
      <Ball p={[0, 3.95, 0]} r={0.72} c="#f2c9a0" />
      <mesh position={[0, 4.75, 0]} castShadow>
        <cylinderGeometry args={[0.6, 0.6, 0.6, 20]} />
        <meshStandardMaterial color="#ffffff" />
      </mesh>
      <Ball p={[-0.42, 5.1, 0]} r={0.42} c="#ffffff" />
      <Ball p={[0.42, 5.1, 0]} r={0.42} c="#ffffff" />
      <Ball p={[0, 5.28, 0]} r={0.48} c="#ffffff" />
      <Ball p={[-0.26, 4.05, 0.64]} r={mood === "wow" ? 0.12 : 0.085} c="#2b1a10" />
      <Ball p={[0.26, 4.05, 0.64]} r={mood === "wow" ? 0.12 : 0.085} c="#2b1a10" />
      <Ball p={[-0.22, 3.72, 0.68]} r={1} s={[0.26, 0.1, 0.1]} c="#7a4a2a" />
      <Ball p={[0.22, 3.72, 0.68]} r={1} s={[0.26, 0.1, 0.1]} c="#7a4a2a" />
      {mood === "wow" && <Ball p={[0, 3.5, 0.66]} r={0.11} c="#7a3030" />}
      {mood === "happy" && (
        <mesh position={[0, 3.6, 0.66]} rotation={[0, 0, Math.PI]}>
          <torusGeometry args={[0.2, 0.035, 8, 20, Math.PI]} />
          <meshStandardMaterial color="#7a3030" />
        </mesh>
      )}
    </group>
  );
};

const useHeartGeometry = () =>
  useMemo(() => {
    const h = new THREE.Shape();
    h.moveTo(0, 0.1);
    h.bezierCurveTo(-0.25, 0.45, -0.7, 0.1, 0, -0.42);
    h.bezierCurveTo(0.7, 0.1, 0.25, 0.45, 0, 0.1);
    const g = new THREE.ExtrudeGeometry(h, { depth: 0.12, bevelEnabled: true, bevelThickness: 0.05, bevelSize: 0.05, bevelSegments: 3 });
    g.translate(0, 0, -0.06);
    return g;
  }, []);

/** Sirsniņas, kas parādās brīdī `start` un ceļas augšup. */
export const Hearts3: React.FC<{ p: V3; t: number; start: number; n?: number }> = ({ p, t, start, n = 3 }) => {
  const geo = useHeartGeometry();
  return (
    <>
      {Array.from({ length: n }).map((_, i) => {
        const lt = t - start - i * 0.3;
        if (lt < 0 || lt > 2.2) return null;
        const sc = Math.min(1, lt * 4) * Math.min(1, (2.2 - lt) * 3) * 0.7;
        return (
          <mesh
            key={i}
            geometry={geo}
            position={[p[0] + (i - (n - 1) / 2) * 0.6 + Math.sin(lt * 3 + i) * 0.1, p[1] + lt * 0.8, p[2]]}
            rotation={[0, Math.sin(lt * 2 + i) * 0.6, 0]}
            scale={sc}
          >
            <meshStandardMaterial color="#ff5d73" roughness={0.4} />
          </mesh>
        );
      })}
    </>
  );
};

/** Drupatiņa, kas lokā lido no vienas vietas uz otru. */
export const Crumb3: React.FC<{ t: number; start: number; from: V3; to: V3; dur?: number }> = ({ t, start, from, to, dur = 1 }) => {
  const k = (t - start) / dur;
  if (k < 0 || k > 1) return null;
  return (
    <mesh
      position={[
        from[0] + (to[0] - from[0]) * k,
        from[1] + (to[1] - from[1]) * k + Math.sin(k * Math.PI) * 1.6,
        from[2] + (to[2] - from[2]) * k,
      ]}
      rotation={[k * 8, k * 5, 0]}
      castShadow
    >
      <dodecahedronGeometry args={[0.17]} />
      <meshStandardMaterial color={BODY} roughness={0.95} flatShading />
    </mesh>
  );
};
