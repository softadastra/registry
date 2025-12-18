#include <gtest/gtest.h>
#include <softadastra/registry/db/Database.hpp>

TEST(Database, CanConnectFromEnv)
{
    auto db = softadastra::registry::db::Database::fromEnv("REGISTRY_DB_");
    EXPECT_NO_THROW(db.testConnection());
}
