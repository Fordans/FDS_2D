#include "FDS_Config.h"

#include "nlohmann/json.hpp"
#include "spdlog/spdlog.h"

#include <fstream>
#include <filesystem>

namespace fds
{
    Config::Config(std::string_view filepath)
    {
        loadFromFile(filepath);
    }

    bool Config::loadFromFile(std::string_view filepath)
    {
        auto path = std::filesystem::path(filepath);
        std::ifstream file(path);
        if (!file.is_open())
        {
            spdlog::warn("Config file {} not found, use default", filepath);
            if (!saveToFile(filepath))
            {
                spdlog::error("Failed to create default config file, {}", filepath);
                return false;
            }
            return false;
        }

        try
        {
            nlohmann::json j;
            file >> j;
            fromJson(j);
            spdlog::info("Load successfully from {}", filepath);
            return true;
        }
        catch (const std::exception &e)
        {
            spdlog::error("Failed to load config from {}, {}, use default", filepath, e.what());
        }
        return false;
    }

    bool Config::saveToFile(std::string_view filepath)
    {
        auto path = std::filesystem::path(filepath);
        std::ofstream file(path);
        if (!file.is_open())
        {
            spdlog::error("Failed to open config file to write, {}", filepath);
            return false;
        }

        try
        {
            nlohmann::ordered_json j = toJson();
            file << j.dump(4);
            spdlog::info("Config saved successfully to {}", filepath);
            return true;
        }
        catch (const std::exception &e)
        {
            spdlog::error("Failed to save to {}, {}", filepath, e.what());
        }
        return false;
    }

    void Config::fromJson(const nlohmann::json &j)
    {
        if (j.contains("window"))
        {
            const auto &window_config = j["window"];
            window_title_ = window_config.value("title", window_title_);
            window_width_ = window_config.value("width", window_width_);
            window_height_ = window_config.value("height", window_height_);
            window_resizable_ = window_config.value("resizable", window_resizable_);
        }
        if (j.contains("graphics"))
        {
            const auto &graphics_config = j["graphics"];
            vsync_enabled_ = graphics_config.value("vsync", vsync_enabled_);
        }
        if (j.contains("performance"))
        {
            const auto &perf_config = j["performance"];
            target_fps_ = perf_config.value("target_fps", target_fps_);
            if (target_fps_ < 0)
            {
                spdlog::warn("Attempted to set target FPS to a non-positive value");
                target_fps_ = 0;
            }
        }
        if (j.contains("audio"))
        {
            const auto &audio_config = j["audio"];
            music_volume_ = audio_config.value("music_volume", music_volume_);
            sound_volume_ = audio_config.value("sound_volume", sound_volume_);
        }

        if (j.contains("input_mappings") && j["input_mappings"].is_object())
        {
            const auto &mappings_json = j["input_mappings"];
            try
            {
                auto input_mappings = mappings_json.get<std::unordered_map<std::string, std::vector<std::string>>>();
                input_mappings_ = std::move(input_mappings);
                spdlog::trace("Input mapping load successfully");
            }
            catch (const std::exception &e)
            {
                spdlog::warn("Exception occured when reading input-mapping, {}, use default", e.what());
            }
        }
        else
        {
            spdlog::trace("Failed to trace input-mapping, use default");
        }
    }

    nlohmann::ordered_json Config::toJson() const
    {
        return nlohmann::ordered_json{
            {"window", {{"title", window_title_}, {"width", window_width_}, {"height", window_height_}, {"resizable", window_resizable_}}},
            {"graphics", {{"vsync", vsync_enabled_}}},
            {"performance", {{"target_fps", target_fps_}}},
            {"audio", {{"music_volume", music_volume_}, {"sound_volume", sound_volume_}}},
            {"input_mappings", input_mappings_}};
    }
}