#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <cstring>

#include <raylib.h>

#include <HayBCMD.h>

#include <Meatball/Console.h>
#include <Meatball/Input.h>

#include <Meatball/Interface/Button.h>

#include <Meatball/Utils/Defaults.h>
#include <Meatball/Utils/DrawFuncs.h>
#include <Meatball/Utils/Utils.h>
#include <Meatball/Utils/CvarFuncs.h>
#include <Meatball/Utils/Json.h>

#include "GameInterface.h"
#include "EditorMode.h"

Font consoleGeneralFont, consoleLabelFont, defaultFont;

Meatball::ConsoleUI* pConsoleUI;

std::vector<UIObject> uiObjects;

Vector2 viewport;

Color uiColor = BLACK, uiHoveredColor = BLACK;
std::string uiAnchor = "";

bool shouldRunWindow = false;

static void resetUiCvars() {
    uiColor = BLACK;
    uiHoveredColor = BLACK;
    uiAnchor = "";
}

// TODO: fully customizable program in the future :D
// for now becuse ui stuff is not fully developed yet,
// we don't need to worry about customizable stuff in this
// program. But in the end this should be also customizable.

#pragma region commands

static void uiCreateButtonCommand(void*, HayBCMD::Command&, const std::vector<std::string>& args) {
    Meatball::Button button = Meatball::Button((Rectangle){0.0f, 0.0f, viewport.x*0.1f, viewport.y*0.06f});
    button.config = &Meatball::Defaults::buttonConfig;

    UIObject& buttonObj = createUIObject(args[0].c_str(), button, uiObjects);
    std::visit([](auto& obj) {
        if constexpr (std::is_same_v<decltype(obj), Meatball::Button&>) {
            obj.config->color = uiColor;
            obj.config->hoveredColor = uiHoveredColor;
        }
    }, buttonObj.object);

    resetUiCvars();
}

static void reloadFontsCommand(void*, HayBCMD::Command&, const std::vector<std::string>&) {
    json consoleData;
    if (!Meatball::readJSONFile("data/console.json", consoleData)) {
        Meatball::Console::print(HayBCMD::ERROR, "could not reload fonts because could not read \"data/console.json\" file\n");
        return;
    }

    std::string fontPath = "";
    GET_STRING_FROM_JSON(consoleData, "font", fontPath, "data/console.json");
    
    if (fontPath != "") {
        if (!Meatball::Defaults::loadConsoleFonts(*pConsoleUI, fontPath, consoleGeneralFont, consoleLabelFont))
            Meatball::Console::print(HayBCMD::WARNING, "something went wrong when loading console fonts\n");
        else
            Meatball::Console::print(HayBCMD::ECHO, "successfully reloaded console fonts\n");
        return;
    }

    Meatball::Console::print(HayBCMD::WARNING, "font not found in data/console.json. Using defaultFont\n");
    
    json initData;
    if (!Meatball::readJSONFile("data/init.json", initData)) {
        Meatball::Console::print(HayBCMD::ERROR, "could not reload fonts because could not read \"data/init.json\" file\n");
        return;
    }

    if (initData.count("defaultFont") == 0 || !initData["defaultFont"].is_string()) {
        Meatball::Console::print(HayBCMD::ERROR, "could not reload fonts because it's either missing defaultFont or type does not match. In \"data/init.json\" file\n");
        return;
    }

    Meatball::Defaults::loadConsoleFonts(*pConsoleUI, initData["defaultFont"], consoleGeneralFont, consoleLabelFont);
}

static void quitCommand(void*, HayBCMD::Command&, const std::vector<std::string>&) {
    shouldRunWindow = false;
}

#pragma endregion

bool stringToInt(const std::string& str, int& buffer) {
    try {
        buffer = std::stoi(str);
        return true;
    } catch (...) {
        return false;
    }
}

static void initCommands() {
    HayBCMD::Command("ui_button_create", 1, 1, uiCreateButtonCommand, "<name> - creates a button using the defined data by other ui commands");
    HayBCMD::Command("fonts_reload", 0, 0, reloadFontsCommand, "- reload fonts");
    HayBCMD::Command("quit", 0, 0, quitCommand, "closes the program");

    HayBCMD::CVARStorage::setCvar("ui_editor_mode",
        &editorMode,
        HayBCMD::CVARUtils::setBoolean,
        HayBCMD::CVARUtils::getBoolean,
        "1/0 - whether should run in editor mode or ui logic"
    );
    HayBCMD::CVARStorage::setCvar("draw_local_console",
        &pConsoleUI->visible,
        HayBCMD::CVARUtils::setBoolean,
        HayBCMD::CVARUtils::getBoolean,
        "1/0 - draws local console"
    );

    /*
    // x% y% width% height%
    ui_anchor "mainPanel" // mainPanel should be defined before closeButton
    ui_rect_min_percent 0 0 1 1
    ui_rect_max_percent 0 0 3 3 
    ui_draw x // it means that we will use drawX function
    ui_create_button "closeButton"
    */
    HayBCMD::CVARStorage::setCvar(
        "ui_color",
        &uiColor,
        Meatball::CVARFuncs::setColor,
        Meatball::CVARFuncs::getColor,
        "");
    HayBCMD::CVARStorage::setCvar(
        "ui_color_hovered",
        &uiHoveredColor,
        Meatball::CVARFuncs::setColor,
        Meatball::CVARFuncs::getColor,
        "");
    HayBCMD::CVARStorage::setCvar(
        "ui_anchor",
        &uiAnchor,
        HayBCMD::CVARUtils::setString,
        HayBCMD::CVARUtils::getString,
        "");

    Meatball::Input::allowedUiCommands.push_back("draw_local_console");
    Meatball::Input::allowedUiCommands.push_back("quit");
}

static void init(int width, int height) {
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_ALWAYS_RUN);
    InitWindow(width, height, "Meatball's Interface Creator");
    viewport = {(float)GetRenderWidth(), (float)GetRenderHeight()};

    defaultFont = GetFontDefault();

    Meatball::Defaults::init("data/init.json", defaultFont);
    
    pConsoleUI = new Meatball::ConsoleUI(Meatball::Defaults::initLocalConsole(
        {0, 0, 800 * 0.5f, 600 * 0.75f},
        "data/console.json",
        consoleGeneralFont,
        consoleLabelFont
    ));

    Meatball::Input::registerCommands();
    Meatball::Input::mapKeyboardKeys();
    Meatball::Input::mapMouseKeys();

    initCommands();
}

int main() {
    init(1280, 720);
    SetExitKey(KEY_NULL);
    pConsoleUI->visible = false;
    
    HayBCMD::execConfigFile("data/cfg/config.cfg", Meatball::Console::variables);

    Color backgroundColor = {0,0,0,255};

    const auto handleUIObject = [](auto& obj) {
        if constexpr (std::is_same_v<decltype(obj), Meatball::Button&>) {
            obj.update();
            Meatball::drawRect(obj.rect, obj.isHovered()? obj.config->hoveredColor : obj.config->color);
        } else if constexpr (std::is_same_v<decltype(obj), Meatball::DynamicPanel&>) {
            obj.update();
            Meatball::drawRect(obj.rect, obj.config->color);
        }
    };

    Editor editor;

    shouldRunWindow = true;
    while (shouldRunWindow) {
        if (WindowShouldClose()) shouldRunWindow = false;

        ClearBackground(backgroundColor);

        float dt = GetFrameTime();
        if (dt > 0.016)
            dt = 0.016;

        if (IsWindowResized()) {
            float newScreenWidth = GetRenderWidth(), newScreenHeight = GetRenderHeight();
            Vector2 ratio = { newScreenWidth / viewport.x, newScreenHeight / viewport.y };

            viewport.x = newScreenWidth;
            viewport.y = newScreenHeight;

            pConsoleUI->onResize(ratio.x, ratio.y);
            editor.onResize(ratio.x, ratio.y);
        }

        Meatball::Input::update(false);

        BeginDrawing();

        for (auto& obj : uiObjects)
            std::visit(handleUIObject, obj.object);

        if (editorMode) {
            editor.update();
            editor.draw();
        }

        pConsoleUI->update();
        pConsoleUI->draw();
        EndDrawing();
    }

    delete pConsoleUI;
    pConsoleUI = nullptr;
}