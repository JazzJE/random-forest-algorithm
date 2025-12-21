#define _WIN32_WINNT 0x0A00  // Windows 10
#include "httplib.h"
#include "json.hpp"
#include <iostream>
#include <fstream>
#include <filesystem>

#include "server.h"
#include "math_functions.h"

int add(int a, int b)
{ return a + b; }

void start_server() {
    // Create uploads directory if it doesn't exist
    std::filesystem::create_directories("uploads");

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

    // Uploading CSV dataset to the project
    svr.Post("/upload_dataset", [](const httplib::Request& req, httplib::Response& res) {
        // Check if this is multipart form data
        if (!req.is_multipart_form_data()) {
            res.status = 400;
            res.set_content("Request is not multipart/form-data", "text/plain");
            return;
        }
        
        // Check if file exists
        if (!req.form.has_file("file")) {
            res.status = 400;
            res.set_content("No file with key 'file' found", "text/plain");
            return;
        }
        
        // Get the file using req.form.get_file()
        const auto& file = req.form.get_file("file");
        
        std::cout << "Filename: " << file.filename << std::endl;
        std::cout << "Content-Type: " << file.content_type << std::endl;
        std::cout << "Content size: " << file.content.size() << std::endl;
        
        // Save the file
        std::ofstream out("uploads/" + file.filename, std::ios::binary);
        if (!out) {
            res.status = 500;
            res.set_content("Failed to create file", "text/plain");
            return;
        }
        
        out.write(file.content.data(), file.content.size());
        out.close();
        
        res.set_content("File uploaded successfully: " + file.filename, "text/plain");
    });

    svr.Get("/test", [](const httplib::Request&, httplib::Response& res) {
        res.set_content("Test endpoint works!", "text/plain");
    });

    std::cout << "Server running on http://localhost:8080\n";
    svr.listen("0.0.0.0", 8080);
}