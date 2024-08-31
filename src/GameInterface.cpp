#include "GameInterface.h"

#include <Meatball/Utils/DrawFuncs.h>

UIObject& createUIObject(const char* name, const UIObjectVariantType& object, std::vector<UIObject>& uiObjects) {
    uiObjects.emplace_back(object, name);
    return *uiObjects.end();
}