import { useMemo } from "react";
import * as THREE from "three";
import { rnd } from "../cartoon/util";
import { Ball, Box, V3 } from "./parts";

export const Lights: React.FC<{ dim?: number; color?: string; from?: V3 }> = ({ dim = 1, color = "#fff6e6", from = [7, 13, 11] }) => (
  <>
    <ambientLight intensity={1.1 * dim} color={color} />
    <hemisphereLight args={["#ffffff", "#b08a60", 0.5 * dim]} />
    <directionalLight position={from} intensity={2.0 * dim} color={color} castShadow shadow-mapSize={[2048, 2048]} shadow-bias={-0.0005}>
      <orthographicCamera attach="shadow-camera" args={[-16, 16, 16, -16, 0.1, 70]} />
    </directionalLight>
  </>
);

/** Debesis ar krāsu pāreju no augšas uz leju. */
export const Sky: React.FC<{ top: string; bottom: string }> = ({ top, bottom }) => {
  const geo = useMemo(() => {
    const g = new THREE.PlaneGeometry(500, 80, 1, 1);
    const a = new THREE.Color(top).toArray();
    const b = new THREE.Color(bottom).toArray();
    g.setAttribute("color", new THREE.Float32BufferAttribute([...a, ...a, ...b, ...b], 3));
    return g;
  }, [top, bottom]);
  return (
    <mesh geometry={geo} position={[0, 22, -70]}>
      <meshBasicMaterial vertexColors fog={false} toneMapped={false} />
    </mesh>
  );
};

export const Ground: React.FC<{ c: string; y?: number }> = ({ c, y = 0 }) => (
  <mesh rotation={[-Math.PI / 2, 0, 0]} position={[0, y, 0]} receiveShadow>
    <planeGeometry args={[400, 200]} />
    <meshStandardMaterial color={c} roughness={1} />
  </mesh>
);

export const Bakery3: React.FC<{ windowX?: number; evening?: number; dim?: number }> = ({ windowX = 2.2, evening = 0, dim = 1 }) => (
  <>
    <color attach="background" args={["#fbe7c6"]} />
    <Lights dim={dim} color={evening > 0.5 ? "#ffd2a0" : "#fff6e6"} />
    <Ground c="#b97a4c" />
    <mesh position={[0, 15, -3]} receiveShadow>
      <planeGeometry args={[400, 30]} />
      <meshStandardMaterial color="#fbe7c6" roughness={1} />
    </mesh>
    {Array.from({ length: 40 }).map((_, i) => (
      <mesh key={i} position={[-23.4 + i * 1.2, 15, -2.98]}>
        <planeGeometry args={[0.6, 30]} />
        <meshStandardMaterial color="#f5d9ad" roughness={1} />
      </mesh>
    ))}
    <Box p={[0, 0.25, -2.9]} s={[400, 0.5, 0.2]} c="#8a5a34" />
    <Box p={[windowX, 5.9, -2.92]} s={[3.2, 2.8, 0.18]} c="#8a5a34" />
    <mesh position={[windowX, 5.9, -2.8]}>
      <planeGeometry args={[2.8, 2.4]} />
      <meshBasicMaterial color={new THREE.Color("#9fdcff").lerp(new THREE.Color("#ff9a56"), evening)} />
    </mesh>
    <Box p={[windowX, 5.9, -2.78]} s={[0.12, 2.4, 0.06]} c="#8a5a34" />
    <Box p={[windowX, 5.9, -2.78]} s={[2.8, 0.12, 0.06]} c="#8a5a34" />
  </>
);

export const Shelf3: React.FC<{ x?: number; y: number; w?: number; z?: number }> = ({ x = 0, y, w = 8.8, z = -1.6 }) => (
  <>
    <Box p={[x, y - 0.14, z]} s={[w, 0.28, 1.6]} c="#8a5a34" />
    <Box p={[x - w / 2 + 0.8, y - 0.6, z - 0.5]} s={[0.25, 0.7, 0.6]} c="#6f4526" />
    <Box p={[x + w / 2 - 0.8, y - 0.6, z - 0.5]} s={[0.25, 0.7, 0.6]} c="#6f4526" />
  </>
);

export const Door3: React.FC<{ x: number; glow: string }> = ({ x, glow }) => (
  <>
    <Box p={[x, 2.4, -2.92]} s={[2.9, 4.8, 0.2]} c="#6f4526" />
    <mesh position={[x, 2.25, -2.78]}>
      <planeGeometry args={[2.4, 4.5]} />
      <meshBasicMaterial color={glow} />
    </mesh>
    <pointLight position={[x, 2.5, -1]} color={glow} intensity={30} distance={12} />
  </>
);

export const Cloud3: React.FC<{ p: V3; s?: number; c?: string }> = ({ p, s = 1, c = "#ffffff" }) => (
  <group position={p} scale={s}>
    <Ball p={[0, 0, 0]} r={1} s={[1.3, 0.7, 0.8]} c={c} glow />
    <Ball p={[-0.8, 0.3, 0]} r={0.6} c={c} glow />
    <Ball p={[0.1, 0.6, 0]} r={0.75} c={c} glow />
    <Ball p={[0.9, 0.3, 0]} r={0.55} c={c} glow />
  </group>
);

export const Meadow3: React.FC<{ t: number; dim?: number; clouds?: boolean }> = ({ t, dim = 1, clouds = true }) => {
  const k = (c: string) => new THREE.Color(c).multiplyScalar(0.45 + 0.55 * dim).getStyle();
  return (
    <>
      <Sky top={k("#5fb8ff")} bottom={k("#e6f7ff")} />
      <Lights dim={0.35 + 0.65 * dim} />
      <Ground c="#6fbd54" />
      <Ball p={[-9, -1.2, -14]} r={1} s={[13, 4.6, 6]} c="#93d36e" />
      <Ball p={[10, -1.4, -18]} r={1} s={[15, 5.4, 6]} c="#80c85e" />
      <Ball p={[8, 12, -45]} r={2.6} c="#ffe066" glow o={dim} />
      {clouds && (
        <>
          <Cloud3 p={[((t * 0.5 + 6) % 44) - 22, 10, -22]} s={1.6} />
          <Cloud3 p={[((t * 0.35 + 28) % 44) - 22, 7.5, -26]} s={1.2} />
        </>
      )}
      {Array.from({ length: 46 }).map((_, i) => {
        const x = rnd(i) * 26 - 13;
        const z = rnd(i + 40) * 12 - 7;
        if (Math.abs(z) < 1) return null;
        const col = ["#ff7aa8", "#ffffff", "#ffd94a", "#c79bff"][i % 4];
        return (
          <group key={i} position={[x, 0, z]} rotation={[0, 0, Math.sin(t * 2 + i) * 0.1]}>
            <mesh position={[0, 0.18, 0]}>
              <cylinderGeometry args={[0.02, 0.02, 0.36, 6]} />
              <meshStandardMaterial color="#3f8f35" />
            </mesh>
            <Ball p={[0, 0.4, 0]} r={0.13} s={[1, 0.6, 1]} c={col} />
            <Ball p={[0, 0.45, 0]} r={0.06} c="#f6b93b" />
          </group>
        );
      })}
    </>
  );
};

export const Mushroom3: React.FC<{ p: V3 }> = ({ p }) => (
  <group position={p}>
    <mesh position={[0, 1.2, 0]} castShadow>
      <cylinderGeometry args={[0.38, 0.5, 2.4, 20]} />
      <meshStandardMaterial color="#f6ead6" />
    </mesh>
    <mesh position={[0, 2.2, 0]} scale={[1, 0.8, 1]} castShadow>
      <sphereGeometry args={[2.3, 32, 16, 0, Math.PI * 2, 0, Math.PI / 2]} />
      <meshStandardMaterial color="#e5484d" roughness={0.6} side={THREE.DoubleSide} />
    </mesh>
    {[
      [-1.2, 3.1, 1.4, 0.28],
      [0.2, 3.75, 1.0, 0.32],
      [1.3, 3.0, 1.3, 0.24],
      [-0.3, 2.9, 2.0, 0.22],
      [0.9, 3.6, -0.6, 0.26],
    ].map((d, i) => (
      <Ball key={i} p={[d[0], d[1], d[2]]} r={d[3]} s={[1, 0.5, 1]} c="#ffffff" />
    ))}
  </group>
);

export const Tree3: React.FC<{ p: V3; t: number; fed: boolean }> = ({ p, t, fed }) => (
  <group position={p}>
    <mesh position={[0, 2.6, 0]} castShadow>
      <cylinderGeometry args={[0.3, 0.42, 5.2, 14]} />
      <meshStandardMaterial color="#8a5a34" />
    </mesh>
    <mesh position={[-0.9, 3.35, 0.2]} rotation={[0, 0, 1.25]} castShadow>
      <cylinderGeometry args={[0.13, 0.17, 1.9, 10]} />
      <meshStandardMaterial color="#8a5a34" />
    </mesh>
    <Ball p={[0, 6.4, 0]} r={2.0} c="#5fae4a" />
    <Ball p={[-1.5, 5.6, 0.3]} r={1.3} c="#6fbd54" />
    <Ball p={[1.5, 5.6, -0.2]} r={1.4} c="#58a444" />
    <group position={[-1.6, 3.75, 0.3]}>
      <mesh rotation={[Math.PI, 0, 0]} castShadow>
        <sphereGeometry args={[0.85, 20, 12, 0, Math.PI * 2, 0, Math.PI / 2]} />
        <meshStandardMaterial color="#9a6b3c" roughness={1} side={THREE.DoubleSide} />
      </mesh>
      {[-0.42, 0, 0.42].map((dx, i) => {
        const open = !fed && Math.sin(t * 9 + i * 2) > 0;
        return (
          <group key={dx} position={[dx, 0.12 + (fed ? Math.abs(Math.sin(t * 6 + i)) * 0.1 : 0), 0.15]}>
            <Ball r={0.27} c="#f6d84a" />
            <Ball p={[-0.09, 0.07, 0.24]} r={0.04} c="#111111" />
            <Ball p={[0.09, 0.07, 0.24]} r={0.04} c="#111111" />
            <mesh position={[0, -0.03, 0.3]} rotation={[Math.PI / 2, 0, 0]} scale={[1, 1, open ? 1.6 : 0.6]}>
              <coneGeometry args={[0.08, 0.16, 8]} />
              <meshStandardMaterial color="#f39c34" />
            </mesh>
          </group>
        );
      })}
    </group>
  </group>
);

/** Krāsains uzraksts uz plāksnes (teksts uzzīmēts tekstūrā). */
export const Sign3: React.FC<{ p: V3; text: string; s?: number }> = ({ p, text, s = 1 }) => {
  const tex = useMemo(() => {
    const c = document.createElement("canvas");
    c.width = 1024;
    c.height = 200;
    const g = c.getContext("2d")!;
    g.fillStyle = "#e5484d";
    g.fillRect(0, 0, 1024, 200);
    g.strokeStyle = "#ffd9a8";
    g.lineWidth = 8;
    g.strokeRect(16, 16, 992, 168);
    g.fillStyle = "#ffffff";
    g.font = "900 104px Inter, sans-serif";
    g.textAlign = "center";
    g.textBaseline = "middle";
    g.fillText(text, 512, 106);
    const t = new THREE.CanvasTexture(c);
    t.colorSpace = THREE.SRGBColorSpace;
    return t;
  }, [text]);
  if (s <= 0.001) return null;
  return (
    <group position={p} scale={s}>
      <Box p={[0, 0, -0.1]} s={[6.6, 1.5, 0.16]} c="#b93338" />
      <mesh>
        <planeGeometry args={[6.3, 1.23]} />
        <meshBasicMaterial map={tex} toneMapped={false} />
      </mesh>
    </group>
  );
};
