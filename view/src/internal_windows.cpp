//
// Created by sproper on 4/1/25.
//

#include "../include/internal_windows.h"

namespace AppView {
    void ShowWelcomeWindow(bool *const window_show_flag) {
        ImGui::Begin("Welcome!", window_show_flag);
        char buf[255];
        memset(buf, 0, 255);
        ImGui::InputTextWithHint("Device name", "Input device name here", buf,
                                 254);
        ImGui::TextColored(
                {1, 0.2, 0.5, 1},
                std::string(
                        "Online mode is not available now! Sorry, we are working on it!")
                        .data());

        if (ImGui::Button("Continue in offline mode", {150, 20})) {
            *(window_show_flag) = false;
        }

        ImGui::End();
    }

    void ShowAboutWindow(bool *const window_show_flag) {
        ImGui::Begin("About ESP_good_therm", window_show_flag);
        ImGui::TextDisabled(description_str.data());
        ImGui::Separator();
        ImGui::Separator();

        static float row_length = 60.0f;
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * row_length);
        ImGui::TextDisabled(MIT_license_str.data());
        ImGui::PopTextWrapPos();
        ImGui::Separator();
        ImGui::Separator();

        ImGui::End();
    }

} // namespace AppView
