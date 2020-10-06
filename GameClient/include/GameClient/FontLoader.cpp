//
// Created by Michal_Marszalek on 01.10.2020.
//

#include "FontLoader.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui-SFML.h>
#include "IconsFontAwesome5_c.h"

FontLoader::FontLoader(const Argv_options &options) {
    (void) (options);

    ImGuiIO &io = ImGui::GetIO();

    io.Fonts->Clear();

    io.Fonts->AddFontDefault();

    ImFontConfig config;
    config.MergeMode = true;
    config.PixelSnapH = true;
    config.OversampleH = 1;
    config.GlyphOffset = {0, 1};

    static const ImWchar icons_ranges[] = {ICON_MIN_FA, ICON_MAX_FA, 0};

    io.Fonts->AddFontFromFileTTF("data/fonts/" FONT_ICON_FILE_NAME_FAS, 13.f, &config,
                                 icons_ranges);

    //io.Fonts->Build();
    ImGui::SFML::UpdateFontTexture();
}
