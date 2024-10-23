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

    bool LogParser::IsDirExist(const fs::path &path_to_dir) const
    {
        return IsFileExist(path_to_dir);
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
        static float parsed_temperature = UNREAL_LOW_TEMPERATURE;
        try
        {
            parsed_temperature = std::stof(str.substr(temp_first_char_index, temp_last_char_index));
        }
        catch (const std::invalid_argument &e)
        {
        }
        return parsed_temperature;
    }

    const TimePoint LogParser::ParseTime(std::string &str)
    {
        size_t time_first_char_index = 0; // : + space
        size_t time_last_char_index = str.find(".");
        std::string absolute_time_str = str.substr(time_first_char_index, time_last_char_index);

        int64_t hour_int, minutes_int, seconds_int = 0;
        try
        {
            sscanf(absolute_time_str.data(), "%ld:%ld:%ld", &hour_int, &minutes_int, &seconds_int);
        }
        catch (const std::exception &e)
        {
        }

        hours_t parsed_hours = std::chrono::hours(hour_int);
        minutes_t parsed_minutes = std::chrono::minutes(minutes_int);
        seconds_t parsed_seconds = std::chrono::seconds(seconds_int);

        return TimePoint(parsed_hours, parsed_minutes, parsed_seconds); // TODO: implement
    }

    const int LogParser::ParseVoltage(const std::string &str)
    {
        static int parsed_voltage = ZERO_ADC_VOLTAGE;
        try
        {
            parsed_voltage = std::stoi("plug");
        }
        catch (const std::invalid_argument &e)
        {
        }
        return parsed_voltage; // TODO: implement
    }

    const Hardware::ThermistorModel LogParser::ParseThermistorModel(const std::string &str)
    {
        int int_var = 0;
        return static_cast<Hardware::ThermistorModel>(int_var);
    } // TODO: implement

    bool LogParser::IsStringContainVoltage(std::string &str) const
    {
        return str.find("cali data") != std::string::npos;
    }

    bool LogParser::IsStringContainThermistorModel(std::string &str) const
    {
        return str.find("using thermistor model") != std::string::npos;
    }
    bool LogParser::IsStringContainTemperature(std::string &str) const
    {
        return str.find("converted temperature") != std::string::npos;
    }

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
        TimePoint parsed_timepoint{};
        int parsed_voltage{};
        float parsed_temperature{};
        Hardware::ThermistorModel parsed_thermistor_model{};
        std::map<std::string, std::shared_ptr<Channel>> channels;
        std::map<const std::string_view, std::ofstream> output_channel_fstreams;

        const bool needs_write_in_files = this->GetSettings().create_save_in_diffrent_files_;

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

            parsed_timepoint = ParseTime(var_str);

            if (!channels[parsed_channel_name]->GetLogPoint(parsed_timepoint))
            {
                channels[parsed_channel_name]->AddPoint(LogPoint(parsed_timepoint));
            }

            LogPoint *target_logpoint = channels[parsed_channel_name]->GetLogPoint(parsed_timepoint).value();

            if (IsStringContainVoltage(var_str))
            {
                parsed_voltage = ParseVoltage(var_str);
                target_logpoint->SetVoltage(parsed_voltage);
                continue;
            }

            if (IsStringContainThermistorModel(var_str))
            {
                parsed_thermistor_model = ParseThermistorModel(var_str);
                target_logpoint->SetThermistor(parsed_thermistor_model);
                continue;
            }

            if (IsStringContainTemperature(var_str))
            {
                parsed_temperature = ParseTemperature(var_str);
                target_logpoint->SetTemperature(parsed_temperature);
                continue;
            }
        }
    }
} // namespace Data
