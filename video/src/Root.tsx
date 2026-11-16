import { Composition } from "remotion";
import { Ad, AD_DURATION } from "./Ad";
import { FPS, HEIGHT, WIDTH } from "./theme";

export const RemotionRoot: React.FC = () => {
  return (
    <Composition
      id="VeikalsAd"
      component={Ad}
      durationInFrames={AD_DURATION}
      fps={FPS}
      width={WIDTH}
      height={HEIGHT}
    />
  );
};
