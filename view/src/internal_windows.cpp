//
// Created by sproper on 4/1/25.
//

#include "../include/internal_windows.h"

namespace AppView {

    std::string_view Window::SetWindowTitle(std::string &new_title) {
        title_ = std::move(new_title);
        return std::string_view(title_);
    }

    std::string_view Window::GetWindowTitle() const {
        return std::string_view(title_);
    }

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
        ImGui::SetWindowSize(ImVec2(450.0f, 300.0f));
        ImGui::TextDisabled(description_str.data());

        static float row_length = 60.0f;
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * row_length);
        //ImGui::TextDisabled(MIT_license_str.data());
        ImGui::PopTextWrapPos();

            ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysVerticalScrollbar;

            ImGui::BeginChild("License scroll", ImVec2(ImGui::GetContentRegionAvail().x, 150), ImGuiChildFlags_None, window_flags);
            ImGui::Separator();
            ImGui::Separator();
            ImGui::TextDisabled(MIT_license_str.data());
            ImGui::Separator();
            ImGui::Separator();
            ImGui::EndChild();

            auto [w, h] = ImGui::GetWindowSize();
            float button_pos_x = ImGui::GetContentRegionAvail().x * 0.5f;
            float button_pos_y = ImGui::GetCursorPos().y + (h - ImGui::GetCursorPos().y)*0.5f;
            ImGui::SetCursorPos(ImVec2(button_pos_x, button_pos_y));
            if(ImGui::Button("Ok", ImVec2(50.0f, 30.0f))) {
                *window_show_flag = false;
            }
        ImGui::End();
    }

} // namespace AppView
