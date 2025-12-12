#include <softadastra/registry/App.hpp>

#include <iostream>
#include <cstdlib>
#include <stdexcept>

namespace softadastra::registry
{
    using softadastra::registry::db::Database;
    using softadastra::registry::db::DatabaseConfig;

    const char *App::resolveConfigPath()
    {
        if (const char *env = std::getenv("REGISTRY_CONFIG"))
        {
            if (*env != '\0')
            {
                return env;
            }
        }

        return "config/config.dev.json";
    }

    static DatabaseConfig makeDbConfigFromFileConfig(const vix::config::Config &cfg)
    {
        DatabaseConfig dbCfg{};

        const std::string host = cfg.getString("database.default.HOST", "localhost");
        const int port = cfg.getInt("database.default.PORT", 3306);
        const std::string user = cfg.getString("database.default.USER", "root");
        const std::string pass = cfg.getString("database.default.PASSWORD", "");
        const std::string name = cfg.getString("database.default.NAME", "registry");

        dbCfg.host = "tcp://" + host + ":" + std::to_string(port);
        dbCfg.user = user;
        dbCfg.password = pass;
        dbCfg.database = name;

        dbCfg.poolMin = 1;
        dbCfg.poolMax = 8;

        return dbCfg;
    }

    Database App::initDatabase(const vix::config::Config &cfg)
    {
        if (const char *envHost = std::getenv("REGISTRY_DB_HOST"))
        {
            if (envHost && *envHost != '\0')
            {
                return Database::fromEnv("REGISTRY_DB_");
            }
        }

        auto dbCfg = makeDbConfigFromFileConfig(cfg);
        return Database{dbCfg};
    }

    App::App()
        : config_(resolveConfigPath()), db_(initDatabase(config_))
    {
        port_ = static_cast<std::uint16_t>(
            config_.getInt("http.port", config_.getServerPort()));

        server_ = std::make_unique<http::HttpServer>(port_);
    }

    App::~App() = default;

    int App::run()
    {
        std::cout << "[registry] Starting Softadastra Registry on port "
                  << port_ << "..." << std::endl;

        try
        {
            db_.testConnection();
            std::cout << "[registry] Database connection OK." << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cerr << "[registry] Database connection FAILED: "
                      << e.what() << std::endl;
            return 1;
        }

        server_->run();
        return 0;
    }

} // namespace softadastra::registry
