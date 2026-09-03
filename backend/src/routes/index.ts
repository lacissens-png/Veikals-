import { Router } from "express";
import { accountRouter } from "./account.routes.js";
import { authRouter } from "./auth.routes.js";
import { bankRouter } from "./bank.routes.js";
import { emailRouter } from "./email.routes.js";
import { subscriptionsRouter } from "./subscriptions.routes.js";
import { transactionsRouter } from "./transactions.routes.js";

export const apiRouter = Router();

apiRouter.use("/auth", authRouter);
apiRouter.use("/bank", bankRouter);
apiRouter.use("/email", emailRouter);
apiRouter.use("/transactions", transactionsRouter);
apiRouter.use("/subscriptions", subscriptionsRouter);
apiRouter.use(accountRouter);
