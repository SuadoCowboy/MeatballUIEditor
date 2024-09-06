#pragma once

#include <vector>
#include <string>

#include <Meatball/IScene.h>
#include <Meatball/Interface/DynamicPanel.h>
#include <Meatball/Interface/ScrollBar.h>
#include <Meatball/Interface/TextButton.h>
#include <Meatball/Shared.h>

extern bool editorMode;
extern Font toolsFont;

using namespace Meatball;

class Editor : public IScene {
public:
    Editor();

    void update();
    void draw();

    void onResize(float ratioWidth, float ratioHeight);

    DynamicPanel mainPanel;

    static constexpr unsigned char toolsArraySize = 6;
    static constexpr unsigned char toolsWidthDivisor = 2; // per height -> 6/2 = 3 sections
    static constexpr unsigned char toolsHeightDivisor = toolsArraySize/toolsWidthDivisor;

    Rectangle toolsRect = {0.0f,0.0f,0.0f,0.0f};
    TextButton toolsArray[toolsArraySize] = {};
    ScrollBar toolsScrollBar;
    
    unsigned char currentTool = toolsArraySize; // currentTool being toolsArraySize is the same as not using a tool 
    
    std::vector<TextButton> objectsList;
    ScrollBar objectsScrollBar;
};