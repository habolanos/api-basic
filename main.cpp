#include <crow.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/rotating_file_sink.h>

int main() {
    auto logger = spdlog::rotating_logger_mt("api_logger", "logs/api.log", 1024 * 1024 * 5, 3);
    spdlog::set_default_logger(logger);
    spdlog::set_level(spdlog::level::info);
    spdlog::info("API iniciada");

    crow::SimpleApp app;

    // Ruta para sumar
    CROW_ROUTE(app, "/add/<int>/<int>")([](int num1, int num2) {
        spdlog::info("Solicitud recibida: /add/{}/{}", num1, num2);
        int result = num1 + num2;
        return crow::response("Resultado de la suma: " + std::to_string(result));
    });

    // Ruta para restar
    CROW_ROUTE(app, "/subtract/<int>/<int>")([](int num1, int num2) {
        spdlog::info("Solicitud recibida: /subtract/{}/{}", num1, num2);
        int result = num1 - num2;
        return crow::response("Resultado de la resta: " + std::to_string(result));
    });

    // Ruta para healthcheck
    CROW_ROUTE(app, "/healthcheck")([]() {
        spdlog::info("Healthcheck solicitado");
        return crow::response("Estado: OK");
    });

    // Ruta para swagger.yaml
    CROW_ROUTE(app, "/swagger.yaml")([]() {
        spdlog::info("Swagger.yaml solicitado");
        return crow::response(crow::mustache::load_text("swagger.yaml"));
    });

    // Ruta para Swagger UI
    CROW_ROUTE(app, "/swagger")([]() {
        spdlog::info("Swagger UI solicitado");
        return crow::response(crow::mustache::load("static/index.html"));
    });

    app.port(8080).multithreaded().run();
    return 0;
}