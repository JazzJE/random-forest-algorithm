#define _WIN32_WINNT 0x0A00  // Windows 10
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include "httplib.h"
#include "json.hpp"

#include "server.h"
#include "math_functions.h"
#include "utils.h"
#include "containers.h"
#include "Constants.h"
#include "CSVDatasetFileContent.h"
#include "exceptions.h"
#include "api_responses.h"

void start_server() {
    // Create uploads directory if it doesn't exist
    std::filesystem::create_directories("uploads");

    httplib::Server svr;

    std::optional<CSVDatasetFileContent> dataset_content;
    std::optional<RandomForestModel> current_model;

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

        j["DAT_FILE_IDENTIFIER"] = Constants::DAT_FILE_IDENTIFIER;

        res.set_content(j.dump(), "application/json");
    });

    // Generate an algorithm in the backend, and then send a JSON object that will allow the frontend to display each tree
    svr.Post("/generate_random_forest_algorithm", [&](const httplib::Request& req, httplib::Response& res)
    {
        // Helper function to for fail responses
        auto fail = [&](const std::string& msg) 
        {
            res.status = 400;

            nlohmann::json res_json;
            res_json["status"] = modelGenerateStatusToString(APIResponseTypes::ModelGenerateStatus::ModelGenerationError);
            res_json["message"] = msg;

            res.set_content(res_json.dump(), "application/json");
        };

        // Check if there is a dataset to genreate trees from
        if (!dataset_content.has_value()) 
        {
            fail("No dataset content detected in backend");
            return;
        }
        // Check if the number of bootstrapped samples per tree field exists
        if (!req.form.has_field("number_of_bootstrapped_samples_per_tree")) 
        {
            fail("No 'number_of_bootstrapped_samples_per_tree' data found");
            return;
        }
        // Check if the number of trees field exists
        if (!req.form.has_field("number_of_trees")) 
        {
            fail("No 'number_of_trees' data found");
            return;
        }
        // Check if the max depth level field exists
        if (!req.form.has_field("maximum_depth_level")) 
        {
            fail("No 'maximum_depth_level' data found");
            return;
        }
        // Check if the min sample split field exists
        if (!req.form.has_field("min_sample_split")) 
        {
            fail("No 'min_sample_split' data found");
            return;
        }
        // Check if the max leaf nodes field exists
        if (!req.form.has_field("max_leaf_nodes")) 
        {
            fail("No 'max_leaf_nodes' data found");
            return;
        }
        // Check if the min purity split field exists
        if (!req.form.has_field("min_entropy"))
        {
            fail("No 'min_entropy' data found");
            return;
        }

        // Parse the data in the form
        size_t number_of_boostrapped_samples_per_tree = std::stoul(req.form.get_field("number_of_bootstrapped_samples_per_tree"));
        size_t number_of_trees = std::stoul(req.form.get_field("number_of_trees"));
        size_t maximum_depth_level = std::stoul(req.form.get_field("maximum_depth_level"));
        size_t min_sample_split = std::stoul(req.form.get_field("min_sample_split"));
        size_t max_leaf_nodes = std::stoul(req.form.get_field("max_leaf_nodes"));
        double min_entropy = std::stod(req.form.get_field("min_entropy"));

        try 
        {
            ModelConfigs current_configs = ModelConfigs(
                number_of_boostrapped_samples_per_tree, number_of_trees, 
                maximum_depth_level, min_sample_split, max_leaf_nodes, min_entropy);

            // Generate the model
            /*
            current_model.emplace(
                dataset_content.value(),
                current_configs
            );
            */
           printModelConfigs(current_configs);

            APIResponseTypes::ModelGenerateStatus status = APIResponseTypes::ModelGenerateStatus::Success;

            nlohmann::json res_json;
            res_json["status"] = modelGenerateStatusToString(status);
            res_json["message"] = "Model successfully generated";

            res.set_content(res_json.dump(), "application/json");
        }
        catch (...)
        {
            APIResponseTypes::ModelGenerateStatus status = APIResponseTypes::ModelGenerateStatus::ModelGenerationError;

            nlohmann::json res_json;
            res_json["status"] = modelGenerateStatusToString(status);
            res_json["message"] = "Model failed to generate due to unknown error";

            res.set_content(res_json.dump(), "application/json");
        }
    });


    // Uploading CSV dataset to the project
    svr.Post("/upload_dataset", [&](const httplib::Request& req, httplib::Response& res) 
    {
        // Helper function to for fail responses
        auto fail = [&](const std::string& msg) 
        {
            res.status = 400;

            nlohmann::json res_json;
            res_json["status"] =  fileUploadStatusToString(APIResponseTypes::FileUploadStatus::CSVParseError);
            res_json["message"] = msg;

            res.set_content(res_json.dump(), "application/json");
        };

        // Check if this is multipart form data
        if (!req.is_multipart_form_data()) 
        {
            fail("Request is not multipart/form-data");
            return;
        }
        // Check if the file is missing
        if (!req.form.has_file("file")) 
        {
            fail("No 'file' data found");
            return;
        }
        // Check if the headers array is missing
        if (!req.form.has_field("headers"))
        {   
            fail("No 'headers' array data found");
            return;
        }
        // Check if the continuous_features array field is missing
        if (!req.form.has_field("continuous_features"))
        {
            fail("No 'continuous_features' set data found");
            return;
        }
        // Check if the target label field is missing
        if (!req.form.has_field("target_label"))
        {
            fail("No 'target_label' data found");
            return;
        }
        
        // Get the file using req.form.get_file()
        const auto& file = req.form.get_file("file");
        // Get the form fields
        std::string headers_string_array = req.form.get_field("headers");
        std::string continuous_features_string_array = req.form.get_field("continuous_features");
        std::string target_label = req.form.get_field("target_label");

        // Parse the file into the program
        DynamicArray<std::string> headers_array = parseStringArray(headers_string_array);
        DynamicArray<std::string> continuous_features_headers_array = parseStringArray(continuous_features_string_array);
        
        try 
        {
            dataset_content.emplace(
                processCSVDatasetFile(
                    file.content.data(),
                    headers_array,
                    continuous_features_headers_array,
                    target_label
                )
            );

            // Testing purposes; delete later
            printDatasetMetadata(*dataset_content);

            APIResponseTypes::FileUploadStatus  status = APIResponseTypes::FileUploadStatus::Success;

            nlohmann::json res_json;
            res_json["status"] = fileUploadStatusToString(status);
            res_json["message"] = std::string("File uploaded successfuly - ") + file.filename;

            res.set_content(res_json.dump(), "application/json");
        }

        // Empty the dataset content and return the line in which the error occurred
        catch (const CSVParseException& error)
        {
            dataset_content.emplace(CSVDatasetFileContent());

            APIResponseTypes::FileUploadStatus status = APIResponseTypes::FileUploadStatus::CSVParseError;

            nlohmann::json res_json;
            res_json["status"] = fileUploadStatusToString(status);
            res_json["message"] = std::string("File failed to upload - ") + error.what();

            res.set_content(res_json.dump(), "application/json");
        }

        // Empty the dataset content if failed to parse at all
        catch (...) 
        {
            dataset_content.emplace(CSVDatasetFileContent());

            APIResponseTypes::FileUploadStatus status = APIResponseTypes::FileUploadStatus::CSVParseError;

            nlohmann::json res_json;
            res_json["status"] = fileUploadStatusToString(status);
            res_json["message"] = "File failed to upload due to unknown error";

            res.set_content(res_json.dump(), "application/json");
        }
    });

    std::cout << "Server running on http://localhost:8080\n";
    svr.listen("0.0.0.0", 8080);
}