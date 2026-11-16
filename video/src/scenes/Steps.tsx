import { AbsoluteFill, useCurrentFrame, useVideoConfig } from "remotion";
import { steps } from "../content";
import { appear, colors, fadeOut, fontFamily } from "../theme";

export const Steps: React.FC = () => {
  const frame = useCurrentFrame();
  const { durationInFrames } = useVideoConfig();

  return (
    <AbsoluteFill
      style={{
        backgroundColor: colors.card,
        fontFamily,
        padding: "0 90px",
        justifyContent: "center",
        gap: 44,
        opacity: fadeOut(frame, durationInFrames),
      }}
    >
      <div
        style={{
          fontSize: 100,
          fontWeight: 800,
          letterSpacing: -2,
          color: colors.dark,
          marginBottom: 30,
          opacity: appear(frame, 0),
        }}
      >
        Tikai <span style={{ color: colors.blue }}>3 soļi</span>
      </div>
      {steps.map((s, i) => {
        const a = appear(frame, 18 + i * 28, 24);
        return (
          <div
            key={s.title}
            style={{
              display: "flex",
              alignItems: "center",
              gap: 40,
              padding: 44,
              borderRadius: 40,
              backgroundColor: colors.white,
              boxShadow: "0 16px 50px rgba(0,0,0,0.08)",
              opacity: a,
              translate: `${(1 - a) * 220}px 0px`,
            }}
          >
            <div
              style={{
                flexShrink: 0,
                width: 130,
                height: 130,
                borderRadius: 65,
                backgroundColor: colors.blue,
                color: colors.white,
                fontSize: 72,
                fontWeight: 800,
                display: "flex",
                justifyContent: "center",
                alignItems: "center",
              }}
            >
              {i + 1}
            </div>
            <div>
              <div style={{ fontSize: 62, fontWeight: 800, color: colors.dark, lineHeight: 1.1 }}>
                {s.title}
              </div>
              <div style={{ fontSize: 44, color: colors.muted, marginTop: 10 }}>{s.text}</div>
            </div>
          </div>
        );
      })}
    </AbsoluteFill>
  );
};
