//
// Created by sproper on 4/1/25.
//
#pragma once

#include <string>
#include <string_view>
#include <memory>

#include "imgui.h"

using namespace std::literals;

namespace AppView{

    class Window {
    public:
        Window() = delete;
        explicit Window(bool window_show_flag)
        : window_show_flag_(std::make_unique<bool>(window_show_flag)) {}
        std::string_view SetWindowTitle(std::string& new_title);
        std::string_view GetWindowTitle() const;

        virtual ~Window() = default;

    private:
        std::string title_ = "Untitled"s;
        std::unique_ptr<bool> window_show_flag_;
    };

/**
 * @brief Show the welcome window
 * @param window_show_flag
 */
    void ShowWelcomeWindow(bool *const window_show_flag);

/**
 * @brief Show the welcome window
 * @param window_show_flag
 */
    const std::string description_str(
            "THERE SHOULD BE SOME KIND OF DESCRIPTION HERE, BUT RIGHT NOW IT IS JUST THIS ...\n");
    const std::string MIT_license_str(
            "Permission is hereby granted, free of charge, to any person obtaining a copy"
            "\nof this software and associated documentation files (the \"Software\"), to deal"
            "\nin the Software without restriction, including without limitation the rights"
            "\nto use, copy, modify, merge, publish, distribute, sublicense, and/or sell"
            "\ncopies of the Software, and to permit persons to whom the Software is"
            "\nfurnished to do so, subject to the following conditions:"

            "\n\nThe above copyright notice and this permission notice shall be included in"
            "\nall copies or substantial portions of the Software."

            "\nTHE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR"
            "\nIMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,"
            "\nFITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE"
            "\nAUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER"
            "\nLIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,"
            "\nOUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN"
            "\nTHE SOFTWARE.\n");

    void ShowAboutWindow(bool *const window_show_flag); // TODO: needs to add imgui license according their requirements

} // namespace AppView