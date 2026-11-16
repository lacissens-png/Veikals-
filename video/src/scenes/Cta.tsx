import { AbsoluteFill, useCurrentFrame } from "remotion";
import { cta, shopName } from "../content";
import { appear, colors, fontFamily } from "../theme";

export const Cta: React.FC = () => {
  const frame = useCurrentFrame();
  const pulse = 1 + 0.04 * Math.sin(Math.max(0, frame - 45) / 6);

  return (
    <AbsoluteFill
      style={{
        backgroundColor: colors.dark,
        fontFamily,
        justifyContent: "center",
        alignItems: "center",
        padding: 90,
        textAlign: "center",
      }}
    >
      <div
        style={{
          fontSize: 116,
          fontWeight: 800,
          letterSpacing: -3,
          lineHeight: 1.08,
          color: colors.white,
          opacity: appear(frame, 0, 25),
          scale: String(0.85 + 0.15 * appear(frame, 0, 25)),
        }}
      >
        {shopName}
      </div>
      <div
        style={{
          marginTop: 90,
          padding: "44px 80px",
          borderRadius: 40,
          backgroundColor: colors.blue,
          color: colors.white,
          fontSize: 60,
          fontWeight: 800,
          opacity: appear(frame, 25),
          translate: `0px ${(1 - appear(frame, 25)) * 60}px`,
          scale: String(pulse),
        }}
      >
        {cta}
      </div>
    </AbsoluteFill>
  );
};
