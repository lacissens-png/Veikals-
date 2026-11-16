import { AbsoluteFill, useCurrentFrame, useVideoConfig } from "remotion";
import { products, shopName } from "../content";
import { appear, colors, fadeOut, fontFamily } from "../theme";

// Kadri, kuros preces "tiek ieliktas grozā"
const ADD_AT = [55, 85, 115];

export const Shop: React.FC = () => {
  const frame = useCurrentFrame();
  const { durationInFrames } = useVideoConfig();
  const cartCount = ADD_AT.filter((f) => frame >= f).length;
  const lastAdd = [...ADD_AT].reverse().find((f) => frame >= f) ?? -100;
  const badgePulse = 1 + 0.35 * (1 - appear(frame, lastAdd, 15));

  return (
    <AbsoluteFill
      style={{
        backgroundColor: colors.white,
        fontFamily,
        opacity: fadeOut(frame, durationInFrames),
      }}
    >
      <div
        style={{
          position: "absolute",
          top: 130,
          left: 90,
          right: 90,
          fontSize: 92,
          fontWeight: 800,
          letterSpacing: -2,
          lineHeight: 1.08,
          color: colors.dark,
          opacity: appear(frame, 0),
          translate: `0px ${(1 - appear(frame, 0)) * 40}px`,
        }}
      >
        Ērti no <span style={{ color: colors.blue }}>telefona</span>
      </div>

      {/* Telefons */}
      <div
        style={{
          position: "absolute",
          top: 400,
          left: 150,
          width: 780,
          height: 1700,
          borderRadius: 90,
          backgroundColor: colors.dark,
          padding: 22,
          boxShadow: "0 40px 120px rgba(0,0,0,0.28)",
          translate: `0px ${(1 - appear(frame, 6, 30)) * 900}px`,
        }}
      >
        <div
          style={{
            width: "100%",
            height: "100%",
            borderRadius: 70,
            overflow: "hidden",
            backgroundColor: colors.white,
          }}
        >
          <div
            style={{
              backgroundColor: colors.dark,
              color: colors.white,
              padding: "56px 36px 34px",
            }}
          >
            <div style={{ fontSize: 44, fontWeight: 800 }}>{shopName}</div>
            <div
              style={{
                marginTop: 18,
                fontSize: 32,
                display: "flex",
                alignItems: "center",
                gap: 14,
              }}
            >
              Grozā:
              <span
                style={{
                  display: "inline-flex",
                  justifyContent: "center",
                  alignItems: "center",
                  minWidth: 56,
                  height: 56,
                  borderRadius: 28,
                  backgroundColor: colors.blue,
                  fontWeight: 800,
                  scale: String(badgePulse),
                }}
              >
                {cartCount}
              </span>
              preces
            </div>
          </div>

          <div
            style={{
              display: "grid",
              gridTemplateColumns: "1fr 1fr",
              gap: 18,
              padding: 18,
            }}
          >
            {products.map((p, i) => {
              const addFrame = ADD_AT[i];
              const added = addFrame !== undefined && frame >= addFrame;
              const press =
                addFrame === undefined
                  ? 1
                  : 1 - 0.06 * (1 - appear(frame, addFrame, 14)) * (added ? 1 : 0);
              return (
                <div
                  key={p.name}
                  style={{
                    border: `2px solid ${colors.line}`,
                    borderRadius: 26,
                    backgroundColor: colors.card,
                    padding: 18,
                    opacity: appear(frame, 22 + i * 6),
                    translate: `0px ${(1 - appear(frame, 22 + i * 6)) * 50}px`,
                    scale: String(press),
                  }}
                >
                  <div
                    style={{
                      height: 250,
                      borderRadius: 18,
                      backgroundColor: p.color,
                    }}
                  />
                  <div style={{ marginTop: 16, fontSize: 34, fontWeight: 700, color: colors.dark }}>
                    {p.name}
                  </div>
                  <div style={{ fontSize: 30, color: colors.muted, marginTop: 4 }}>
                    €{p.price}
                  </div>
                  <div
                    style={{
                      marginTop: 16,
                      padding: "16px 0",
                      borderRadius: 16,
                      textAlign: "center",
                      fontSize: 28,
                      fontWeight: 700,
                      color: colors.white,
                      backgroundColor: added ? "#1f9d55" : colors.blue,
                    }}
                  >
                    {added ? "Grozā ✓" : "Pievienot"}
                  </div>
                </div>
              );
            })}
          </div>
        </div>
      </div>
    </AbsoluteFill>
  );
};
