#pragma once

#include <functional>
#include <string>

#include <raylib.h>

#include "../Shared.h"

namespace Meatball {
    namespace Config {
        struct InputTextBox {
            Font* font = nullptr;

            Color color = {40,40,40,255},
                textColor = WHITE,
                cursorColor = WHITE,
                selectionColor = {100,100,100,55};
        };
    }

    namespace Defaults {
        extern Config::InputTextBox inputTextBoxConfig;
    }

    class InputTextBox {
    public:
        InputTextBox();
        InputTextBox(const Rectangle& rect);

        void draw();
        void update();

        Config::InputTextBox *config = &Defaults::inputTextBoxConfig;
        
        // onSend by default runs when KEY_ENTER/KEY_KP_ENTER is pressed
        std::function<void(const std::string&)> onSend, onTextChange;
        
        bool focused = false;
        size_t cursorPos = 0;

        Rectangle rect = {0.0f,0.0f,0.0f,0.0f};

        const std::string& getText() const;
        /// @return false if text size is higher than allowed
        bool setText(const std::string& text);

        // textMaxSize range is the unsigned short range from 0 to max unsigned short MINUS ONE;
        unsigned short textMaxSize = 1000;

        unsigned short fontSize = 0;
    private:
        std::string text = "";

        float offsetX = 0;

        bool mousePressed = false;

        unsigned short selectedTextStartIdx = 0;
        unsigned short selectedTextFinalIdx = 0;
    };
}