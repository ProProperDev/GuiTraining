#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <string_view>
#include <tuple>
#include <regex>
#include <utility>
#include <string_view>
#include <exception>
#include <chrono>

#include "channel.hpp"
// /home/sproper/Desktop/origin_log30.08.24.txt

namespace fs = std::filesystem;

#ifndef UNREAL_LOW_TEMPERATURE
#define UNREAL_LOW_TEMPERATURE (-273.0f)
#endif

#ifndef ZERO_ADC_VOLTAGE
#define ZERO_ADC_VOLTAGE (0)
#endif
/*
enum class Match
{
    NOT_FOUND,
    ADC_CH1,
    ADC_CH3,
    ADC_CH4
};

const Match SearchNameMatch(std::string &str)
{
    if (str.find("temperature") == std::string::npos)
    {
        return Match::NOT_FOUND;
    }
    if (str.find("ADC1_CH1:") != std::string::npos)
    {
        return Match::ADC_CH1;
    }
    if (str.find("ADC1_CH3:") != std::string::npos)
    {
        return Match::ADC_CH3;
    }
    if (str.find("ADC1_CH4:") != std::string::npos)
    {
        return Match::ADC_CH4;
    }
    return Match::NOT_FOUND;
}

std::ostream &WriteTimeAndTempInStream(std::ostream &out, const std::string &time, const std::string temp)
{
#ifdef DEBUG
    std::cout << "Time: " << time << "   Temperature: " << temp << std::endl;
#endif
    out << "Time: " << time << "   Temperature: " << temp << "\n";
    return out;
}

const std::string ParseTime(std::string &str)
{
    size_t time_last_char_index = str.find(".");
    return str.substr(0, time_last_char_index);
}

const std::string ParseTemperature(std::string &str)
{
    size_t temp_first_char_index = str.rfind(":") + 2; // : + space
    size_t temp_last_char_index = str.substr(temp_first_char_index).find(" ");
    return str.substr(temp_first_char_index, temp_last_char_index);
}

void InputFileToConvertPath()
{
    std::string input_str{};
    std::getline(std::cin, input_str);

    const fs::path path_to_file{input_str};
    if (!fs::exists(path_to_file))
    {
        std::cout << "file not found" << std::endl;
        return;
    }
    std::ifstream input_stream_file;
    std::ofstream output_file_ch1;
    std::ofstream output_file_ch3;
    std::ofstream output_file_ch4;
    input_stream_file.open(path_to_file);
    output_file_ch1.open("parsed_results/ADC_CH1", std::ios::out);
    output_file_ch3.open("parsed_results/ADC_CH3", std::ios::out);
    output_file_ch4.open("parsed_results/ADC_CH4", std::ios::out);
    std::string var_str{};
    int match_str_counter = 0;
    while (std::getline(input_stream_file, var_str) && match_str_counter < 3 * 50000)
    {
        const Match match = SearchNameMatch(var_str);
        if (match == Match::NOT_FOUND)
        {
            continue;
        }
        auto [time, temperature] = std::make_pair(ParseTime(var_str), ParseTemperature(var_str));
        switch (match)
        {
        case Match::ADC_CH1:
            WriteTimeAndTempInStream(output_file_ch1, time, temperature);
            break;
        case Match::ADC_CH3:
            WriteTimeAndTempInStream(output_file_ch3, time, temperature);
            break;
        case Match::ADC_CH4:
            WriteTimeAndTempInStream(output_file_ch4, time, temperature);
            break;
        };
        ++match_str_counter;
    }

    input_stream_file.close();
    output_file_ch1.close();
    output_file_ch3.close();
    output_file_ch4.close();
}

int main()
{
    InputFileToConvertPath();
    return 0;
}
*/
namespace Data
{

    struct LogSettings
    {
        LogSettings();
        std::string channel_reg_expr_str_{"ADC1_CH[0-9]"};
        std::string parsed_files_dir_name_{"parsed_files"};
        bool create_save_in_diffrent_files_ = false;
        bool parse_all_strings_ = true;
        const std::regex &GetChannelNameRegExpr() const;

    private:
        std::regex channel_name_reg_expr_;
    };

    class LogParser // TODO: do we need to use a singleton pattern?
    {
    public:
        LogParser();
        std::map<std::string, std::shared_ptr<Channel>> ParseLogFile(const fs::path &path_to_file);
        void SetSettings(LogSettings &&new_settings);
        const LogSettings &GetSettings() const;

    private:
        bool IsStringContainThermistorModel(std::string &str) const;
        bool IsStringContainTemperature(std::string &str) const;
        bool IsStringContainVoltage(std::string &str) const;
        const float ParseTemperature(std::string &str);
        const TimePoint ParseTime(std::string &str);
        const int ParseVoltage(const std::string &str);
        const Hardware::ThermistorModel ParseThermistorModel(const std::string &str);

        bool IsFileExist(const fs::path &path_to_file) const;
        bool IsDirExist(const fs::path &path_to_dir) const;
        fs::path CreateChannelLogFile(const std::string &filename);

        LogSettings settings_;
        std::vector<fs::path> out_files_paths_;
    };

} // namespace Data