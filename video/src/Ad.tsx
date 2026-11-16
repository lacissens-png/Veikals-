import { Series } from "remotion";
import { Cta } from "./scenes/Cta";
import { Hook } from "./scenes/Hook";
import { Shop } from "./scenes/Shop";
import { Steps } from "./scenes/Steps";

export const SCENES = { hook: 100, shop: 170, steps: 150, cta: 120 };
export const AD_DURATION = SCENES.hook + SCENES.shop + SCENES.steps + SCENES.cta;

export const Ad: React.FC = () => (
  <Series>
    <Series.Sequence name="Hook" durationInFrames={SCENES.hook}>
      <Hook />
    </Series.Sequence>
    <Series.Sequence name="Shop" durationInFrames={SCENES.shop}>
      <Shop />
    </Series.Sequence>
    <Series.Sequence name="Steps" durationInFrames={SCENES.steps}>
      <Steps />
    </Series.Sequence>
    <Series.Sequence name="CTA" durationInFrames={SCENES.cta}>
      <Cta />
    </Series.Sequence>
  </Series>
);
