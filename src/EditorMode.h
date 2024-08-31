#pragma once

#include <Meatball/IScene.h>
#include <Meatball/Interface/DynamicPanel.h>
#include <Meatball/Interface/ScrollBar.h>

extern bool editorMode;

using namespace Meatball;

class Editor : public IScene {
public:
    Editor();

    void update();
    void draw();

    void onResize(float ratioWidth, float ratioHeight);

    DynamicPanel toolsPanel;
    ScrollBar toolsPanelScrollBar;
};