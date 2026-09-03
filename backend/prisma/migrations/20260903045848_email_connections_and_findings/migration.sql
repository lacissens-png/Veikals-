-- AlterTable
ALTER TABLE "draft_actions" ADD COLUMN     "recipient" VARCHAR(255),
ADD COLUMN     "sent_at" TIMESTAMP(6);

-- CreateTable
CREATE TABLE "email_connections" (
    "id" UUID NOT NULL,
    "user_id" UUID NOT NULL,
    "provider" VARCHAR(50) NOT NULL DEFAULT 'gmail',
    "email_address" VARCHAR(255),
    "status" VARCHAR(20) NOT NULL DEFAULT 'pending',
    "connected_at" TIMESTAMP(6) NOT NULL DEFAULT CURRENT_TIMESTAMP,
    "auth_state" VARCHAR(128),
    "access_token" TEXT,
    "refresh_token" TEXT,
    "token_expires_at" TIMESTAMP(6),
    "last_synced_at" TIMESTAMP(6),

    CONSTRAINT "email_connections_pkey" PRIMARY KEY ("id")
);

-- CreateTable
CREATE TABLE "email_findings" (
    "id" UUID NOT NULL,
    "user_id" UUID NOT NULL,
    "email_connection_id" UUID NOT NULL,
    "external_id" VARCHAR(255) NOT NULL,
    "received_at" TIMESTAMP(6) NOT NULL,
    "kind" VARCHAR(30) NOT NULL,
    "merchant_name" VARCHAR(255) NOT NULL,
    "sender_address" VARCHAR(255),
    "summary" TEXT NOT NULL,
    "amount" DECIMAL(10,2),
    "currency" VARCHAR(3),
    "effective_date" DATE,
    "confidence" DECIMAL(3,2) NOT NULL DEFAULT 0,
    "risk_reasons" JSONB,
    "status" VARCHAR(20) NOT NULL DEFAULT 'new',
    "subscription_id" UUID,
    "created_at" TIMESTAMP(6) NOT NULL DEFAULT CURRENT_TIMESTAMP,

    CONSTRAINT "email_findings_pkey" PRIMARY KEY ("id")
);

-- CreateIndex
CREATE UNIQUE INDEX "email_connections_auth_state_key" ON "email_connections"("auth_state");

-- CreateIndex
CREATE INDEX "email_connections_user_id_idx" ON "email_connections"("user_id");

-- CreateIndex
CREATE INDEX "email_findings_user_id_status_idx" ON "email_findings"("user_id", "status");

-- CreateIndex
CREATE UNIQUE INDEX "email_findings_email_connection_id_external_id_key" ON "email_findings"("email_connection_id", "external_id");

-- AddForeignKey
ALTER TABLE "email_connections" ADD CONSTRAINT "email_connections_user_id_fkey" FOREIGN KEY ("user_id") REFERENCES "users"("id") ON DELETE CASCADE ON UPDATE CASCADE;

-- AddForeignKey
ALTER TABLE "email_findings" ADD CONSTRAINT "email_findings_user_id_fkey" FOREIGN KEY ("user_id") REFERENCES "users"("id") ON DELETE CASCADE ON UPDATE CASCADE;

-- AddForeignKey
ALTER TABLE "email_findings" ADD CONSTRAINT "email_findings_email_connection_id_fkey" FOREIGN KEY ("email_connection_id") REFERENCES "email_connections"("id") ON DELETE CASCADE ON UPDATE CASCADE;

-- AddForeignKey
ALTER TABLE "email_findings" ADD CONSTRAINT "email_findings_subscription_id_fkey" FOREIGN KEY ("subscription_id") REFERENCES "subscriptions"("id") ON DELETE SET NULL ON UPDATE CASCADE;
