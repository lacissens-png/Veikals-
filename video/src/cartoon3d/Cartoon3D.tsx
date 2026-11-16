import { Audio } from "@remotion/media";
import { ThreeCanvas } from "@remotion/three";
import { AbsoluteFill, interpolate, Series, staticFile, useCurrentFrame, useVideoConfig } from "remotion";
import { Caption } from "../cartoon/Cartoon";
import { MORAL, THE_END, TITLE } from "../cartoon/script";
import { font, pop } from "../cartoon/util";
import { sceneArt3 } from "./scenes3d";
import { Scene3Def, scenes3 } from "./script3d";

const clamp = { extrapolateLeft: "clamp", extrapolateRight: "clamp" } as const;
const BROWN = "#7a4420";

const Scene3Frame: React.FC<{ scene: Scene3Def }> = ({ scene }) => {
  const frame = useCurrentFrame();
  const { fps, width, height } = useVideoConfig();
  const t = frame / fps;
  const vertical = height > width;
  const Art = sceneArt3[scene.id];
  const fade = interpolate(t, [0, 0.3, scene.seconds - 0.3, scene.seconds], [0, 1, 1, 0], clamp);
  const big = vertical ? 150 : 110;
  return (
    <AbsoluteFill style={{ backgroundColor: "#2b1a10" }}>
      <AbsoluteFill style={{ opacity: fade }}>
        <ThreeCanvas width={width} height={height} shadows="soft" camera={{ fov: 40, near: 0.5, far: 300, position: [0, 3, 20] }}>
          <Art t={t} />
        </ThreeCanvas>
        <Caption scene={scene} t={t} vertical={vertical} />
        {scene.id === "title" && (
          <div
            style={{
              position: "absolute",
              top: vertical ? 230 : 40,
              width: "100%",
              textAlign: "center",
              fontFamily: font,
              fontWeight: 900,
              fontSize: big,
              lineHeight: 1.05,
              color: BROWN,
              opacity: interpolate(t, [0.3, 0.9], [0, 1], clamp),
              translate: `0px ${(1 - pop(t, 0.3)) * 60}px`,
            }}
          >
            {vertical ? TITLE.map((l) => <div key={l}>{l}</div>) : TITLE.join(" ")}
          </div>
        )}
        {scene.id === "oven" && t > 1.1 && t < 2.3 && (
          <div
            style={{
              position: "absolute",
              top: vertical ? 420 : 110,
              left: vertical ? 80 : 420,
              fontFamily: font,
              fontWeight: 900,
              fontSize: 130,
              color: "#f08a2b",
              scale: String(pop(t, 1.1, 0.3)),
              rotate: `${Math.sin(t * 30) * 4}deg`,
            }}
          >
            DZIN!
          </div>
        )}
        {scene.id === "end" && (
          <>
            <div
              style={{
                position: "absolute",
                top: vertical ? 260 : 50,
                width: "100%",
                textAlign: "center",
                fontFamily: font,
                fontWeight: 800,
                fontSize: vertical ? 84 : 64,
                lineHeight: 1.2,
                color: BROWN,
                opacity: interpolate(t, [0.3, 0.9], [0, 1], clamp),
              }}
            >
              {vertical ? MORAL.map((l) => <div key={l}>{l}</div>) : MORAL.join(" ")}
            </div>
            <div
              style={{
                position: "absolute",
                bottom: vertical ? 260 : 40,
                width: "100%",
                textAlign: "center",
                fontFamily: font,
                fontWeight: 900,
                fontSize: vertical ? 170 : 120,
                color: "#e5484d",
                scale: String(pop(t, 1.6, 0.5)),
              }}
            >
              {THE_END}
            </div>
          </>
        )}
      </AbsoluteFill>
    </AbsoluteFill>
  );
};

export const Cartoon3D: React.FC = () => {
  const { fps } = useVideoConfig();
  return (
    <AbsoluteFill>
      <Audio src={staticFile("cepums-muzika-90s.mp3")} volume={0.6} />
      <Series>
        {scenes3.map((scene) => (
          <Series.Sequence key={scene.id} name={scene.id} durationInFrames={Math.round(scene.seconds * fps)}>
            <Scene3Frame scene={scene} />
          </Series.Sequence>
        ))}
      </Series>
    </AbsoluteFill>
  );
};
