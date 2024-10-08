// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "../implot/implot.h"
#include "../implot/implot_internal.h"
#include <stdio.h>
#include <cmath>
#include <string_view>
#include <string>
#include <vector>
#include <array>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

// This example can also compile and run with Emscripten! See 'Makefile.emscripten' for details.
#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif

static void glfw_error_callback(int error, const char *description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

#define _CRT_SECURE_NO_WARNINGS
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Simple helper function to load an image into a OpenGL texture with common settings
bool LoadTextureFromMemory(const void *data, size_t data_size, GLuint *out_texture, int *out_width, int *out_height)
{
    // Load from file
    int image_width = 0;
    int image_height = 0;
    unsigned char *image_data = stbi_load_from_memory((const unsigned char *)data, (int)data_size, &image_width, &image_height, NULL, 4);
    if (image_data == NULL)
        return false;

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload pixels into texture
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    *out_texture = image_texture;
    *out_width = image_width;
    *out_height = image_height;

    return true;
}

// Open and read a file, then forward to LoadTextureFromMemory()
bool LoadTextureFromFile(const char *file_name, GLuint *out_texture, int *out_width, int *out_height)
{
    FILE *f = fopen(file_name, "rb");
    if (f == NULL)
        return false;
    fseek(f, 0, SEEK_END);
    size_t file_size = (size_t)ftell(f);
    if (file_size == -1)
        return false;
    fseek(f, 0, SEEK_SET);
    void *file_data = IM_ALLOC(file_size);
    fread(file_data, 1, file_size, f);
    bool ret = LoadTextureFromMemory(file_data, file_size, out_texture, out_width, out_height);
    IM_FREE(file_data);
    return ret;
}

// Main code
int main(int, char **)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

        // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char *glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char *glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char *glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    GLFWwindow *window = glfwCreateWindow(1280, 720, "ESP_good_therm", nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable Multi-Viewport / Platform Windows
    // io.ConfigViewportsNoAutoMerge = true;
    // io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    // ImGui::StyleColorsDark();
    ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle &style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
#ifdef __EMSCRIPTEN__
    ImGui_ImplGlfw_InstallEmscriptenCallbacks(window, "#canvas");
#endif
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    // - Our Emscripten build process allows embedding fonts to be accessible at runtime from the "fonts/" folder. See Makefile.emscripten for details.
    // io.Fonts->AddFontDefault();
    // io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    // ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    // IM_ASSERT(font != nullptr);

    // Our state

    int my_image_width = 0;
    int my_image_height = 0;
    GLuint my_image_texture = 0;
    bool ret = LoadTextureFromFile("small_logo.png", &my_image_texture, &my_image_width, &my_image_height);
    IM_ASSERT(ret);

    enum class PlotType
    {
        SOLID_LINE,
        SINGLE_POINTS,
        STAIRS,
        SHADED_AREA,
        BARS,
        BAR_GROUPS,
        ERROR_BARS,
        STEMS,
        INF_VERTICAL_LINES,
        PIE_CHART,
        HEAT_MAP,
        HISTOGRAM,
        HISTOGRAM_2D,
        DIGITAL,
        IMAGE,
        TEXT,
        DUMMY
    };

    bool show_about_app_window = true;
    bool show_storage_window = false;
    bool show_items = false;
    bool show_statistic_window = false;
    bool show_another_window = false;
    bool my_tool_active = false;
    bool show_adc_ch_0 = false;
    bool show_adc_ch_1 = false;
    bool show_adc_ch_2 = false;
    bool show_adc_ch_3 = false;
    bool show_adc_ch_4 = false;
    bool show_adc_ch0_points = false;
    bool show_adc_ch1_points = false;
    bool show_adc_ch2_points = false;
    bool show_adc_ch3_points = false;
    bool show_adc_ch4_points = false;
    bool exit_app_desire = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
#ifdef __EMSCRIPTEN__
    // For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
    // You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
    io.IniFilename = nullptr;
    EMSCRIPTEN_MAINLOOP_BEGIN
#else
    while (!glfwWindowShouldClose(window))
#endif
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Storage"))
                {
                    show_storage_window = true;
                }
                ImGui::Separator();
                if (ImGui::MenuItem("Items"))
                {
                    show_items = true;
                }
                ImGui::Separator();
                if (ImGui::MenuItem("Statistic"))
                {
                    show_statistic_window = true;
                }
                ImGui::Separator();
                if (ImGui::MenuItem("Channel curves Online"))
                {
                    my_tool_active = true;
                }
                ImGui::Separator();
                if (ImGui::MenuItem("Exit"))
                {
                    exit_app_desire = true;
                }

                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Online mode"))
            {
                if (ImGui::BeginMenu("Open Channel..."))
                {
                    if (ImGui::MenuItem("ADC Channel 0", "", false, false))
                    {
                        show_adc_ch_0 = true;
                    }
                    if (ImGui::MenuItem("ADC Channel 1"))
                    {
                        show_adc_ch_1 = true;
                    }
                    if (ImGui::MenuItem("ADC Channel 2"))
                    {
                        show_adc_ch_2 = true;
                    }
                    if (ImGui::MenuItem("ADC Channel 3"))
                    {
                        show_adc_ch_3 = true;
                    }
                    if (ImGui::MenuItem("ADC Channel 4", "", false, false))
                    {
                        show_adc_ch_4 = true;
                    }
                    ImGui::EndMenu();
                }
                ImGui::SameLine();
                ImGui::TextDisabled("Info about Channel  ");
                ImGui::Separator();
                if (ImGui::MenuItem("Redo", "CTRL+Y", false, false))
                {
                } // Disabled item
                ImGui::Separator();
                if (ImGui::MenuItem("Open all channels"))
                {
                    show_adc_ch_0 = true;
                    show_adc_ch_1 = true;
                    show_adc_ch_2 = true;
                    show_adc_ch_3 = true;
                    show_adc_ch_4 = true;
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("About"))
            {
                show_about_app_window = true;
                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }

        if (exit_app_desire)
        {
            ImGui::Begin("Close App", &exit_app_desire);
            const std::string exit_msg = "Are you sure you want to exit?";
            auto [w, h] = ImGui::GetWindowSize();
            ImGui::SetCursorPos({(w / 2) - exit_msg.length() / 2, h / 2 - 50});
            ImGui::Text(exit_msg.data());
            char buf[255];
            ImGui::InputTextWithHint("INput goodbye", "input desires here", buf, 254);
            ImGui::SetCursorPos({(w / 2) - 100 / 2, h / 2 + 5});
            if (ImGui::Button("No", {50, 20}))
            {
                exit_app_desire = false;
            }
            ImGui::SameLine();
            ImGui::SetCursorPos({(w / 2) + 90 / 2, h / 2 + 5});
            if (ImGui::Button("Yes", {50, 20}))
            {
                glfwSetWindowShouldClose(window, GLFW_TRUE);
            }
            ImGui::End();
        }

        if (show_storage_window)
        {
            ImGui::Begin("Storage", &show_storage_window, ImGuiWindowFlags_MenuBar);
            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("Edit"))
                {
                    if (ImGui::MenuItem("Add new item group", "Ctrl+O"))
                    { /* Do stuff */
                    }
                    if (ImGui::MenuItem("Delete item group", "Ctrl+D"))
                    { /* Do stuff */
                    }
                    ImGui::EndMenu();
                }

                ImGui::EndMenuBar();
            }
            if (ImGui::CollapsingHeader("Shoes"))
            {
                if (ImGui::TreeNode("Sneakers"))
                {
                    static ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;

                    if (ImGui::BeginTable("Sneakers", 5, flags))
                    {
                        ImGui::TableSetupColumn("Manufacture", ImGuiTableColumnFlags_WidthFixed);
                        ImGui::TableSetupColumn("Model", ImGuiTableColumnFlags_WidthFixed);
                        ImGui::TableSetupColumn("Origin country", ImGuiTableColumnFlags_WidthFixed);
                        ImGui::TableSetupColumn("Price", ImGuiTableColumnFlags_WidthFixed);
                        ImGui::TableSetupColumn("Stock balance", ImGuiTableColumnFlags_WidthStretch);
                        ImGui::TableHeadersRow();
                        for (int row = 0; row < 300; row++)
                        {
                            ImGui::TableNextRow();
                            for (int column = 0; column < 4; column++)
                            {
                                ImGui::TableSetColumnIndex(column);
                                if (row < 100 && column == 0)
                                {
                                    ImGui::Text("Other item");
                                }
                                else if (row >= 100 && column == 0)
                                {
                                    ImGui::Text("Bosco");
                                }
                            }
                        }
                        ImGui::EndTable();
                    }
                    ImGui::TreePop();
                }

                if (ImGui::TreeNode("Boots"))
                {
                    ImGui::Text("Empty");
                    ImGui::TreePop();
                }
            }
            ImGui::SeparatorText("Last distribution 08.07.2024");
            ImGui::CollapsingHeader("Clothes");
            ImGui::SeparatorText("Last distribution 16.06.2024");
            ImGui::End();
        }

        if (show_statistic_window)
        {
            ImGui::Begin("Statistic", &show_statistic_window);
            ImGui::BeginListBox("one");
            ImGui::EndListBox();
            ImGui::End();
        }

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        // if (show_demo_window)
        //     ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        // {
        //     static float f = 0.0f;
        //     static int counter = 0;

        //     ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        //     ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        //     //ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
        //     ImGui::Checkbox("Another Window", &show_another_window);

        //     ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        //     ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        //     if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        //         counter++;
        //     ImGui::SameLine();
        //     ImGui::Text("counter = %d", counter);

        //     ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        //     ImGui::End();
        // }

        if (show_about_app_window)
        {
            ImGui::Begin("About ESP_good_therm", &show_about_app_window);
            ImGui::TextDisabled("Some description...");
            ImGui::Separator();
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window); // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

        // 4. Show custom measuring window
        if (my_tool_active)
        {
            ImGui::Begin("My First Tool", &my_tool_active, ImGuiWindowFlags_None);
            // ImGui::Checkbox("Show ADC_CH1", &show_adc_ch_1);
            // ImGui::SameLine();
            // ImGui::Checkbox("Show ADC_CH3", &show_adc_ch_3);
            // ImGui::SameLine();
            // ImGui::Checkbox("Show ADC_CH4", &show_adc_ch_4);ImGui::Begin("OpenGL Texture Text");
            ImGui::Text("pointer = %x", my_image_texture);
            ImGui::Text("size = %d x %d", my_image_width, my_image_height);
            ImGui::Image((void *)(intptr_t)my_image_texture, ImVec2(my_image_width, my_image_height));
            ImGui::End();
            if (show_adc_ch_1)
            {
                // Generate samples and plot them
                std::vector<float> samples(500);
                for (int n = 0; n < 500; n++)
                    samples[n] = sinf(n * 0.2f + ImGui::GetTime() * 1.5f);
                // void ImGui::PlotLines(const char* label, const float* values, int values_count, int values_offset, const char* overlay_text, float scale_min, float scale_max, ImVec2 graph_size, int stride)
                ImGui::Button("Nothing to do");
                ImGui::Button("Nothing to do either");
                ImGui::PlotLines("ADC_CH1 Graph", samples.data(), 200, 0, "FFFGGG", -10, 10, {500, 200});
            }
            // Display contents in a scrolling region
            ImGui::TextColored(ImVec4(1, 1, 0, 1), "Important Stuff");
            ImGui::BeginChild("Scrolling");
            for (int n = 0; n < 50; n++)
                ImGui::Text("%04d: Some text", n);
            ImGui::EndChild();
            ImGui::End();
        }
        if (show_adc_ch_1)
        {
            ImGui::Begin("ADC Channel 1", &show_adc_ch_1);
            if (ImPlot::BeginPlot("Plot"))
            {
                std::array<float, 10> exm;
                std::array<float, 10> exm1;
                std::array<float, 10> exm2;
                exm = {0, 1, 2.2, 3.3, 4.4, 5.5, 6, 7, 8, 9};
                exm1 = {0, -1, -2.2, -3.3, -4.4, -5.5, -6, -7, -8, -9};
                exm2 = {5, 8, 2, 6, 7, 2, 5.5, 2, 9, 6};

                // ImPlot::PlotScatter("My Bar Plot", exm.data(), 11);
                ImPlot::PlotStairs("ADC Channel 1", exm.data(), exm1.data(), 9);
                ImPlot::PlotStairs("ADC Channel 2", exm1.data(), exm.data(), 9);
                ImPlot::PlotStairs("ADC Channel 3", exm1.data(), exm2.data(), 9);
                ImPlot::EndPlot();

                ImGui::SeparatorText("ADC_CH1 (X: time, s; Y: temperature, C)");
                ImGui::RadioButton("Show points", show_adc_ch1_points);
                ImGui::SameLine();
                ImGui::Button("Stop writing this channel");
            }
            ImGui::End();
        }

        if (show_adc_ch_2)
        {
            ImGui::Begin("ADC Channel 2", &show_adc_ch_2);
            if (ImPlot::BeginPlot("My Plot"))
            {
                std::array<float, 10> exm;
                std::array<float, 10> exm1;
                exm = {10, 111, 22.2, 13.3, 64.4, 85.5, 46, 87, 78, 89};
                exm1 = {0, -14, -2.27, -3.38, -46.4, -54.5, -62, -79, -81, -93};
                ImPlot::PlotScatter("My Bar Plot", exm.data(), 11);
                ImPlot::PlotLine("My Line Plot", exm.data(), exm1.data(), 9);
                ImPlot::EndPlot();

                ImGui::SeparatorText("ADC_CH2 (x: time, s; y: temperature, C)");
                ImGui::RadioButton("Show points", show_adc_ch1_points);
                ImGui::SameLine();
                ImGui::Button("Stop writing this channel");
            }
            ImGui::End();
        }

        if (show_adc_ch_3)
        {
            ImGui::Begin("ADC Channel 3", &show_adc_ch_3);
            std::array<uint, 10> exm;
            exm[0] = 1;
            std::array<uint, 10> exm1;
            if (exm[0] == 0 || exm1.empty())
            {
                ImGui::TextDisabled("No data");
            }
            else
            {

                if (ImPlot::BeginPlot("My Plot"))
                {
                    ImPlot::PlotScatter("My Bar Plot", exm.data(), 11);
                    ImPlot::PlotLine("My Line Plot", exm.data(), exm1.data(), 9);
                    ImPlot::EndPlot();
                }
            }
            ImGui::SeparatorText("ADC_CH3 (x: time, s; y: temperature, C)");
            if (ImGui::RadioButton("Show points", show_adc_ch3_points))
            {
                show_adc_ch3_points = !show_adc_ch3_points;
            }; // Toogle the state
            ImGui::SameLine();
            ImGui::Button("Stop writing this channel");
            if (show_adc_ch3_points)
            {

                ImGui::BeginChild("Next point enumering in cycle");
                if (ImGui::BeginTabBar("MyTabBar"))
                {
                    if (ImGui::BeginTabItem("Avocado"))
                    {
                        ImGui::Text("This is the Avocado tab!\nblah blah blah blah blah");
                        ImGui::EndTabItem();
                    }
                    if (ImGui::BeginTabItem("Broccoli"))
                    {
                        ImGui::Text("This is the Broccoli tab!\nblah blah blah blah blah");
                        ImGui::EndTabItem();
                    }
                    if (ImGui::BeginTabItem("Cucumber"))
                    {
                        ImGui::Text("This is the Cucumber tab!\nblah blah blah blah blah");
                        ImGui::EndTabItem();
                    }
                    ImGui::EndTabBar();
                }
                ImGui::Text("Date: 15.04.2023 Time: 16:47:13 Temperature: 34 C");
                ImGui::Text("Date: 15.04.2023 Time: 16:47:14 Temperature: 36 C");
                ImGui::Text("Date: 15.04.2023 Time: 16:47:15 Temperature: 40 C");
                ImGui::EndChild();
            }

            ImGui::End();
        }

        // ImGui::Begin("OpenGL Texture Text");
        //  ImGui::Text("pointer = %x", my_image_texture);
        //  ImGui::Text("size = %d x %d", my_image_width, my_image_height);
        // ImGui::SetWindowSize({500, 400});
        // ImGui::Image((void *)(intptr_t)my_image_texture, ImVec2(50 /*my_image_width*/, 50 /*my_image_height*/));
        // ImGui::End();
        // auto MAIN_WINDOW = ImGuiWindowFlags_NoMove
        //     //  ImGuiWindowFlags_NoTitleBar |
        //     //  ImGuiWindowFlags_NoBringToFrontOnFocus |
        //     // ImGuiWindowFlags_NoInputs |
        //     // ImGuiWindowFlags_NoCollapse |
        //     // ImGuiWindowFlags_NoResize;
        //     ;
        // int w, h;
        // glfwGetWindowSize(window, &w, &h);
        // ImVec2 size(w, h);
        // ImGui::SetNextWindowPos(ImVec2());
        // ImGui::SetNextWindowSize(size);
        // if (ImGui::Begin("Logo", nullptr, MAIN_WINDOW))
        // {

        //     glfwGetWindowSize(window, &w, &h);
        //     ImGui::Image((void *)(intptr_t)my_image_texture, size);
        //     ImGui::End();
        // }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow *backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        glfwSwapBuffers(window);
    }
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END;
#endif

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
