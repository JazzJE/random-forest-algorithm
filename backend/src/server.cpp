#define _WIN32_WINNT 0x0A00  // Windows 10
#include "httplib.h"

#include "server.h"
#include "math_functions.h"
#include <iostream>

int add(int a, int b)
{ return a + b; }

void start_server() {
    httplib::Server svr;

    svr.set_default_headers({
        {"Access-Control-Allow-Origin", "*"},
        {"Access-Control-Allow-Methods", "GET, POST, OPTIONS"},
        {"Access-Control-Allow-Headers", "Content-Type"}
        });

    svr.Get("/status", [](const httplib::Request&, httplib::Response& res) {
        res.set_content("Backend running", "text/plain");
        });

    svr.Get("/add", [](const httplib::Request& req, httplib::Response& res) {
        if (!req.has_param("a") || !req.has_param("b")) {
            res.status = 400;
            res.set_content("Missing parameters", "text/plain");
            return;
        }

        int a = std::stoi(req.get_param_value("a"));
        int b = std::stoi(req.get_param_value("b"));

        int result = add(a, b);

        res.set_content(std::to_string(result), "text/plain");
        });

    std::cout << "Server running on http://localhost:8080\n";
    svr.listen("0.0.0.0", 8080);
}