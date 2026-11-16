import { Audio } from "@remotion/media";
import { AbsoluteFill, interpolate, Series, staticFile, useCurrentFrame, useVideoConfig } from "remotion";
import { sceneArt } from "./scenes";
import { SceneDef, scenes } from "./script";
import { font } from "./util";

const clamp = { extrapolateLeft: "clamp", extrapolateRight: "clamp" } as const;

/** Teicēja teksts: katrai rindai laiks proporcionāli tās garumam. */
const Caption: React.FC<{ scene: SceneDef; t: number; vertical: boolean }> = ({ scene, t, vertical }) => {
  if (scene.lines.length === 0) return null;
  const lead = 0.7;
  const usable = scene.seconds - lead - 0.6;
  const weights = scene.lines.map((l) => l.length + 18);
  const total = weights.reduce((a, b) => a + b, 0);
  let start = lead;
  let current: { text: string; a: number; b: number } | null = null;
  for (let i = 0; i < scene.lines.length; i++) {
    const dur = (weights[i] / total) * usable;
    if (t >= start && t < start + dur) current = { text: scene.lines[i], a: start, b: start + dur };
    start += dur;
  }
  if (!current) return null;
  const o = interpolate(t, [current.a, current.a + 0.3, current.b - 0.3, current.b], [0, 1, 1, 0], clamp);
  return (
    <div
      style={{
        position: "absolute",
        left: vertical ? 60 : 210,
        right: vertical ? 60 : 210,
        top: vertical ? 1430 : undefined,
        height: vertical ? 380 : undefined,
        bottom: vertical ? undefined : 44,
        display: "flex",
        justifyContent: "center",
        alignItems: "center",
        opacity: o,
      }}
    >
      <div
        style={{
          fontFamily: font,
          fontWeight: 700,
          fontSize: vertical ? 58 : 44,
          lineHeight: 1.25,
          textAlign: "center",
          color: "#3b2414",
          backgroundColor: "rgba(255,250,240,0.94)",
          borderRadius: 36,
          padding: vertical ? "38px 44px" : "22px 40px",
          boxShadow: "0 12px 40px rgba(0,0,0,0.18)",
        }}
      >
        {current.text}
      </div>
    </div>
  );
};

const SceneFrame: React.FC<{ scene: SceneDef }> = ({ scene }) => {
  const frame = useCurrentFrame();
  const { fps, width, height } = useVideoConfig();
  const t = frame / fps;
  const vertical = height > width;
  // 1080x1080 skatuve: horizontāli centrēta, vertikālajā kadrā pabīdīta uz augšu.
  const ox = (width - 1080) / 2;
  const oy = vertical ? 440 : 0;
  const Art = sceneArt[scene.id];
  const fade = interpolate(t, [0, 0.4, scene.seconds - 0.4, scene.seconds], [0, 1, 1, 0], clamp);
  return (
    <AbsoluteFill style={{ backgroundColor: "#2b1a10" }}>
      <AbsoluteFill style={{ opacity: fade, overflow: "hidden" }}>
        <svg width={width} height={height} viewBox={`${-ox} ${-oy} ${width} ${height}`}>
          <Art t={t} />
        </svg>
        <Caption scene={scene} t={t} vertical={vertical} />
      </AbsoluteFill>
    </AbsoluteFill>
  );
};

export const Cartoon: React.FC = () => {
  const { fps } = useVideoConfig();
  return (
    <AbsoluteFill>
      <Audio src={staticFile("cepums-muzika.mp3")} volume={0.6} />
      <Series>
        {scenes.map((scene) => (
          <Series.Sequence key={scene.id} name={scene.id} durationInFrames={Math.round(scene.seconds * fps)}>
            <SceneFrame scene={scene} />
          </Series.Sequence>
        ))}
      </Series>
    </AbsoluteFill>
  );
};
