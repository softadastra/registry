#pragma once

#include <memory>
#include <cstdint>

#include <vix/config/Config.hpp>
#include <softadastra/registry/http/HttpServer.hpp>
#include <softadastra/registry/db/Database.hpp>

namespace softadastra::registry
{
    class App
    {
    public:
        App();
        ~App();

        int run();

    private:
        static const char *resolveConfigPath();
        static db::Database initDatabase(const vix::config::Config &cfg);

    private:
        vix::config::Config config_;
        std::uint16_t port_{8080};
        db::Database db_;
        std::unique_ptr<http::HttpServer> server_;
    };
} // namespace softadastra::registry
