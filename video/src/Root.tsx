import { Composition } from "remotion";
import { Ad, AD_DURATION } from "./Ad";
import { Cartoon } from "./cartoon/Cartoon";
import { CARTOON_FPS, CARTOON_SECONDS } from "./cartoon/script";
import { FPS, HEIGHT, WIDTH } from "./theme";

export const RemotionRoot: React.FC = () => {
  return (
    <>
      <Composition id="VeikalsAd" component={Ad} durationInFrames={AD_DURATION} fps={FPS} width={WIDTH} height={HEIGHT} />
      <Composition
        id="CepumsVertical"
        component={Cartoon}
        durationInFrames={CARTOON_SECONDS * CARTOON_FPS}
        fps={CARTOON_FPS}
        width={1080}
        height={1920}
      />
      <Composition
        id="CepumsHorizontal"
        component={Cartoon}
        durationInFrames={CARTOON_SECONDS * CARTOON_FPS}
        fps={CARTOON_FPS}
        width={1920}
        height={1080}
      />
    </>
  );
};
