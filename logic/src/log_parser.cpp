#include "../include/log_parser.hpp"

namespace Data
{
    const std::regex &LogSettings::GetChannelNameRegExpr() const
    {
        return channel_name_reg_expr_;
    }

    LogParser::LogParser() = default;

    bool LogParser::IsFileExist(const fs::path &path_to_file) const
    {
        if (!fs::exists(path_to_file))
        {
            return false;
        }
        return true;
    }

    const LogSettings &LogParser::GetSettings() const
    {
        return settings_;
    }

    void LogParser::SetSettings(LogSettings &&new_settings)
    {
        settings_ = std::forward<LogSettings>(new_settings);
    }

    fs::path LogParser::CreateChannelLogFile(const std::string &filename)
    {
        const fs::path path_to_file{filename};
        fs::create_directory(filename);
        std::filesystem::permissions(path_to_file, std::filesystem::perms::all);
        return path_to_file;
    }

    const float LogParser::ParseTemperature(std::string &str)
    {
        size_t temp_first_char_index = str.rfind(":") + 2; // : + space
        size_t temp_last_char_index = str.substr(temp_first_char_index).find(" ");
        return std::stof(str.substr(temp_first_char_index, temp_last_char_index));
    }

    const TimePoint LogParser::ParseTime(std::string &str)
    {
        auto ParseHour = [](std::string &str) {};
        size_t time_first_char_index = 0; // : + space
        size_t time_last_char_index = str.find(".");
        std::string absolute_time_str = str.substr(time_first_char_index, time_last_char_index);

        return;
    }

    const float LogParser::ParseVoltage(const std::string &str)
    {
        float parsed_voltage = .0f;
        try
        {
            parsed_voltage = std::stof("plug");
        }
        catch (const std::invalid_argument &e)
        {
            parsed_voltage = .0f;
        }
        return parsed_voltage; // TODO: implement
    }

    const Hardware::ThermistorModel LogParser::ParseThermistorModel(const std::string &str)
    {
        int int_var = 0;
        return static_cast<Hardware::ThermistorModel>(int_var);
    } // TODO: implement

    void LogParser::ParseLogFile(const fs::path &path_to_file)
    {

        if (!IsFileExist(path_to_file))
        {
            return;
        }

        std::ifstream input_stream_file;
        input_stream_file.open(path_to_file);
        std::smatch match;
        std::string var_str{};
        std::string parsed_channel_name{};
        std::map<std::string, std::shared_ptr<Channel>> channels;

        while (std::getline(input_stream_file, var_str))
        {
            if (!std::regex_search(var_str, match, settings_.GetChannelNameRegExpr()))
            {
                continue;
            }
            parsed_channel_name = match.str();
            try
            {
                channels.at(parsed_channel_name);
            }
            catch (const std::out_of_range &e)
            {
                channels[parsed_channel_name] = std::make_shared<Channel>(parsed_channel_name);
                channels[parsed_channel_name]->SetChannelMode(ChannelMode::OFFLINE);
            }

            LogPoint log_point(ParseTime(var_str), ParseTemperature(var_str), ParseVoltage(var_str), ParseThermistorModel(var_str));
            channels[parsed_channel_name]->AddPoint(log_point);
        }
    }
} // namespace Data
