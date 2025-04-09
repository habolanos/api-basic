#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <fstream>
#include <string>

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

void setup_logger() {
    auto rotating_logger = spdlog::rotating_logger_mt("api_logger", "/var/log/api-basic.log", 1048576 * 5, 3);
    spdlog::set_default_logger(rotating_logger);
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S] [%l] %v");
    spdlog::info("Logger initialized");
}

std::string load_swagger_file() {
    std::ifstream file("/app/swagger.json");
    if (!file.is_open()) {
        spdlog::error("Failed to open swagger.json");
        return "<h1>Error: Swagger file not found</h1>";
    }
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    return content;
}

void handle_add(http_request request) {
    auto path = request.relative_uri().split_path();
    if (path.size() != 3 || path[0] != "add") {
        request.reply(status_codes::BadRequest, "Invalid path. Use /add/num1/num2");
        return;
    }

    try {
        int num1 = std::stoi(path[1]);
        int num2 = std::stoi(path[2]);
        int result = num1 + num2;
        json::value response;
        response[U("result")] = json::value::number(result);
        response[U("message")] = json::value::string(U("Addition successful"));
        spdlog::info("Addition: {} + {} = {}", num1, num2, result);
        request.reply(status_codes::OK, response);
    } catch (const std::exception& e) {
        spdlog::error("Error in addition: {}", e.what());
        request.reply(status_codes::BadRequest, "Invalid numbers");
    }
}

void handle_subtract(http_request request) {
    auto path = request.relative_uri().split_path();
    if (path.size() != 3 || path[0] != "subtract") {
        request.reply(status_codes::BadRequest, "Invalid path. Use /subtract/num1/num2");
        return;
    }

    try {
        int num1 = std::stoi(path[1]);
        int num2 = std::stoi(path[2]);
        int result = num1 - num2;
        json::value response;
        response[U("result")] = json::value::number(result);
        response[U("message")] = json::value::string(U("Subtraction successful"));
        spdlog::info("Subtraction: {} - {} = {}", num1, num2, result);
        request.reply(status_codes::OK, response);
    } catch (const std::exception& e) {
        spdlog::error("Error in subtraction: {}", e.what());
        request.reply(status_codes::BadRequest, "Invalid numbers");
    }
}

void handle_health(http_request request) {
    json::value response;
    response[U("status")] = json::value::string(U("OK"));
    spdlog::info("Health check requested");
    request.reply(status_codes::OK, response);
}

void handle_swagger(http_request request) {
    auto content = load_swagger_file();
    http_response response(status_codes::OK);
    response.headers().set_content_type(U("application/json"));
    response.set_body(content);
    request.reply(response);
}

int main() {
    setup_logger();
    spdlog::info("Starting API server...");

    http_listener add_listener(U("http://0.0.0.0:8080/add"));
    http_listener subtract_listener(U("http://0.0.0.0:8080/subtract"));
    http_listener health_listener(U("http://0.0.0.0:8080/health"));
    http_listener swagger_listener(U("http://0.0.0.0:8080/"));

    add_listener.support(methods::GET, handle_add);
    subtract_listener.support(methods::GET, handle_subtract);
    health_listener.support(methods::GET, handle_health);
    swagger_listener.support(methods::GET, handle_swagger);

    try {
        add_listener.open().wait();
        subtract_listener.open().wait();
        health_listener.open().wait();
        swagger_listener.open().wait();
        spdlog::info("API server running on port 8080");
        std::string line;
        std::getline(std::cin, line); // Wait for input to keep server running
    } catch (const std::exception& e) {
        spdlog::error("Error starting server: {}", e.what());
    }

    return 0;
}