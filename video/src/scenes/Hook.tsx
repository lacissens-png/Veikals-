import { AbsoluteFill, useCurrentFrame, useVideoConfig } from "remotion";
import { hook } from "../content";
import { appear, colors, fadeOut, fontFamily } from "../theme";

export const Hook: React.FC = () => {
  const frame = useCurrentFrame();
  const { durationInFrames } = useVideoConfig();

  return (
    <AbsoluteFill
      style={{
        backgroundColor: colors.dark,
        fontFamily,
        justifyContent: "center",
        padding: 90,
        opacity: fadeOut(frame, durationInFrames),
      }}
    >
      <div
        style={{
          width: 140 * appear(frame, 0, 25),
          height: 14,
          borderRadius: 7,
          backgroundColor: colors.blue,
          marginBottom: 50,
        }}
      />
      {hook.map((line, i) => (
        <div
          key={line}
          style={{
            fontSize: 124,
            fontWeight: 800,
            lineHeight: 1.1,
            letterSpacing: -3,
            color: i === 1 ? colors.blue : colors.white,
            opacity: appear(frame, 8 + i * 22),
            translate: `0px ${(1 - appear(frame, 8 + i * 22)) * 70}px`,
          }}
        >
          {line}
        </div>
      ))}
    </AbsoluteFill>
  );
};
