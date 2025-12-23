#define _WIN32_WINNT 0x0A00  // Windows 10
#include "httplib.h"
#include "json.hpp"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>

#include "server.h"
#include "math_functions.h"
#include "utils.h"
#include "containers.h"
#include "Constants.h"
#include "CSVDatasetFileContent.h"

void start_server() {
    // Create uploads directory if it doesn't exist
    std::filesystem::create_directories("uploads");

    httplib::Server svr;

    svr.set_default_headers(
        {
            {"Access-Control-Allow-Origin", "*"},
            {"Access-Control-Allow-Methods", "GET, POST, OPTIONS"},
            {"Access-Control-Allow-Headers", "Content-Type"}
        });

    svr.Get("/status", [](const httplib::Request&, httplib::Response& res) 
    {
        res.set_content("Backend running", "text/plain");
    });

    // Give the different constants the backend will use for file processing to the front end
    svr.Get("/retrieve_file_constants", [](const httplib::Request& req, httplib::Response& res) 
    {
        nlohmann::json j;

        std::string cont_feat_ident(Constants::CONTINUOUS_FEATURE_IDENTIFIER);
        j["CONTINUOUS_FEATURE_IDENTIFIER"] = cont_feat_ident;

        j["DAT_FILE_IDENTIFIER"] = Constants::DAT_FILE_IDENTIFIER;

        res.set_content(j.dump(), "application/json");
    });

    // Uploading CSV dataset to the project
    svr.Post("/upload_dataset", [](const httplib::Request& req, httplib::Response& res) 
    {
        // Check if this is multipart form data
        if (!req.is_multipart_form_data()) 
        {
            res.status = 400;
            res.set_content("Request is not multipart/form-data", "text/plain");
            return;
        }
        
        // Check if the file is missing
        if (!req.form.has_file("file")) 
        {
            res.status = 400;
            res.set_content("No file with key 'file' found", "text/plain");
            return;
        }

        // Check if the continuous_features array field is missing
        if (!req.form.has_field("continuous_features"))
        {
            res.status = 400;
            res.set_content("No 'continuous features' array found", "text/plain");
            return;
        }

        // Check if the target label field is missing
        if (!req.form.has_field("target_label"))
        {
            res.status = 400;
            res.set_content("No 'headers' array found", "text/plain");
            return;
        }
        
        // Get the file using req.form.get_file()
        const auto& file = req.form.get_file("file");
        // Get the form fields
        std::string continuous_json = req.form.get_field("continuous_features");
        std::string target_label = req.form.get_field("target_label");
        
        std::cout << "Filename: " << file.filename << std::endl;
        std::cout << "Content-Type: " << file.content_type << std::endl;
        std::cout << "Content size: " << file.content.size() << std::endl;
        std::cout << continuous_json << std::endl;
        std::cout << target_label << std::endl;
        
        // Parse the file into the program
        DynamicArray<std::string> headers_array = parseStringArray(continuous_json);
        // Print out each of the strings
        for (size_t i = 0; i < headers_array.number_of_elements; i++)
            std::cout << headers_array.elements[i] << std::endl;

        //CSVDatasetFileContent dataset_content = processCSVDatasetFile(file.content.data(), target_label);

        // If the file fails to parse, then return an error code
        std::ofstream out("uploads/" + file.filename, std::ios::binary);
        if (!out) 
        {
            res.status = 500;
            res.set_content("Failed to create file", "text/plain");
            return;
        }
        
        out.write(file.content.data(), file.content.size());
        out.close();
        
        res.set_content("File uploaded successfully: " + file.filename, "text/plain");
    });

    std::cout << "Server running on http://localhost:8080\n";
    svr.listen("0.0.0.0", 8080);
}