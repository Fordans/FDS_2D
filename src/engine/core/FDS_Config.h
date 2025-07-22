#ifndef FDS_CONFIG_H
#define FDS_CONFIG_H

#include "nlohmann/json_fwd.hpp"

#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>

namespace fds
{
    class Config final
    {
    public:
        std::string window_title_ = "FDS_2D"; // change this to your own
        int window_width_ = 1920;
        int window_height_ = 1080;
        bool window_resizable_ = false;

        bool vsync_enabled_ = true;

        int target_fps_ = 60;

        float music_volume_ = 0.5f;
        float sound_volume_ = 0.5f;

        std::unordered_map<std::string, std::vector<std::string>> input_mappings_ = {
            {"move_left", {"A", "Left"}},
            {"move_right", {"D", "Right"}},
            {"move_up", {"W", "Up"}},
            {"move_down", {"S", "Down"}},
            {"jump", {"J", "Space"}},
            {"attack", {"K", "MouseLeft"}},
            {"pause", {"P", "Escape"}},
            // more additional action
        };

    public:
        explicit Config(std::string_view filepath);

        Config(const Config &) = delete;
        Config &operator=(const Config &) = delete;
        Config(Config &&) = delete;
        Config &operator=(Config &&) = delete;

        bool loadFromFile(std::string_view filepath);
        [[nodiscard]] bool saveToFile(std::string_view filepath);

    private:
        void fromJson(const nlohmann::json &j);
        nlohmann::ordered_json toJson() const;
    };
}

#endif // FDS_CONFIG_H