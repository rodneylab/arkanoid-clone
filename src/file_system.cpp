#include "file_system.h"

#include "components.h"

#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <spdlog/spdlog.h>

#include <cstddef>
#include <fstream>
#include <map>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

void parse_level_bricks_file(std::string_view json_path,
                             nlohmann::json &json_data)
{
    std::ifstream json_file{json_path.data()};
    if (!json_file)
    {
        spdlog::error("Unable to open {} for reading.", json_path);
    }

    try
    {
        json_data = nlohmann::json::parse(json_file);
    }
    catch (nlohmann::json::parse_error &exception)
    {
        spdlog::error("Error parsing JSON from `{}`: {}.",
                      json_path,
                      exception.what());
        throw exception;
    }
}

std::map<BrickType, LevelBrick> load_brick_properties()
{
    const std::string json_path{ASSETS_PATH "level_bricks.json"};
    nlohmann::json json_data;
    parse_level_bricks_file(json_path, json_data);

    std::map<BrickType, LevelBrick> result{};
    const auto bricks_json_data = json_data["bricks"];
    if (bricks_json_data.empty())
    {
        spdlog::error("Error in JSON data from `{}`: expected at least one "
                      "element in bricks array.",
                      json_path);
    }
    for (const auto &element : bricks_json_data)
    {
        try
        {
            const std::string colour_field{element.at("colour")};
            const BrickType brick_type =
                LevelBrick::get_string_to_brick_type_map().at(colour_field);
            result.emplace(brick_type,
                           LevelBrick{brick_type,
                                      element.at("value"),
                                      element.value("hitsToDestroy", 1)});
            spdlog::trace("Added BrickType: '{}' from JSON data in `{}`.  "
                          "Loaded data: {}",
                          std::string(element["colour"]),
                          json_path,
                          to_string(result.at(brick_type)));
        }
        catch (const std::out_of_range &exception)
        {
            const std::string element_value{element["colour"]};
            spdlog::error(
                "Error in JSON data from `{}`: unknown BrickType: '{}'.",
                json_path,
                element_value);
            throw exception;
        }
    }

    return result;
}

std::vector<BrickType> load_level_data()
{
    const std::string json_path{ASSETS_PATH "level_bricks.json"};

    nlohmann::json json_data;
    parse_level_bricks_file(json_path, json_data);

    const int level_1_rows{
        static_cast<int>(json_data["levelBricks"][0]["rows"].size())};
    if (level_1_rows == 0)
    {
        spdlog::error("Error in JSON data from `{}`: expected at least one row "
                      "of bricks in levelBricks[0].rows.",
                      json_path);
    }

    std::vector<BrickType> brick_rows{};
    brick_rows.reserve(static_cast<size_t>(level_1_rows));
    for (const auto &element : json_data["levelBricks"][0]["rows"])
    {
        try
        {
            brick_rows.emplace_back(
                LevelBrick::get_string_to_brick_type_map().at(element));
        }
        catch (const std::out_of_range &exception)
        {
            spdlog::error(
                "Error in JSON data from `{}`: unknown BrickType: '{}'.",
                json_path,
                std::string(element));
            throw exception;
        }
    }

    return brick_rows;
}
