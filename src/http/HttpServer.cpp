#include <softadastra/registry/http/HttpServer.hpp>

namespace softadastra::registry::http
{
    HttpServer::HttpServer(std::uint16_t port)
        : port_(port)
    {
    }

    void HttpServer::setupRoutes()
    {
        // Healthcheck endpoint
        app_.get("/health", [](auto &, auto &res)
                 { res.json({"status", "ok"}); });

        // Basic root endpoint (temporary hello world)
        app_.get("/", [](auto &, auto &res)
                 { res.json({"message", "Softadastra Registry is running"}); });

        // TODO:
        //  - /api/packages/:name
        //  - /api/packages/:name/:version
        //  - /api/packages/publish
        //  - /api/search
    }

    void HttpServer::run()
    {
        setupRoutes();
        app_.run(port_);
    }
} // namespace softadastra::registry::http
