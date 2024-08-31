#pragma once

#include <variant>

#include <HayBCMD.h>
#include <Meatball/Console.h>
#include <Meatball/Interface/Button.h>
#include <Meatball/Interface/DynamicPanel.h>

#include <raylib.h>

typedef std::variant<Meatball::Button, Meatball::DynamicPanel> UIObjectVariantType;

struct UIObject {
    UIObjectVariantType object;
    const char* name;

    UIObject(const UIObjectVariantType& object, const char* name) : object(object), name(name) {}
};

struct UIOption {
    const char* text;
    Font& font;
    unsigned char type;

    UIOption(const char* text, Font& font, unsigned char type) : text(text), font(font), type(type) {}
};

UIObject& createUIObject(const char* name, const UIObjectVariantType& object, std::vector<UIObject>& uiObjects);