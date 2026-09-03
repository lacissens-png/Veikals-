import "dotenv/config";
import path from "node:path";
import { defineConfig, env } from "prisma/config";

/**
 * Prisma 7 konfigurācija. Kopš Prisma 7 datubāzes URL vairs netiek norādīts
 * `schema.prisma` failā — migrācijas/introspekcijas komandas to lasa no šejienes,
 * bet PrismaClient izmanto driver adapteri (skat. `src/lib/prisma.ts`).
 */
export default defineConfig({
  schema: path.join("prisma", "schema.prisma"),
  migrations: {
    path: path.join("prisma", "migrations"),
  },
  datasource: {
    url: env("DATABASE_URL"),
  },
});
