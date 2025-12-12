#pragma once

#include <cstdint>
#include <vix.hpp>

namespace softadastra::registry::http
{
    class HttpServer
    {
    public:
        explicit HttpServer(std::uint16_t port);
        ~HttpServer() = default;

        void run();
        vix::App &app() { return app_; }

    private:
        void setupRoutes();
        std::uint16_t port_{8080};
        vix::App app_;
    };
} // namespace softadastra::registry::http
