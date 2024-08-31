#include "EditorMode.h"

#include <Meatball/Utils/Defaults.h>
#include <Meatball/Utils/DrawFuncs.h>

bool editorMode = true;

Editor::Editor() {
    toolsPanel.config = &Defaults::dynamicPanelConfig;
    toolsPanelScrollBar.config = &Defaults::scrollBarConfig;

    toolsPanel.rect.width = GetScreenWidth()*0.25f;
    toolsPanel.rect.height = GetScreenHeight();

    toolsPanel.rect.x = GetScreenWidth()-toolsPanel.rect.width;
    toolsPanel.rect.y = 0.0f;

    toolsPanel.onMove = [&]() {
        toolsPanelScrollBar.setPosition(
            toolsPanel.rect.x+toolsPanel.rect.width-toolsPanelScrollBar.getRect().width,
            toolsPanel.rect.y
        );
    };

    toolsPanel.onResize = [&]() {
        toolsPanelScrollBar.setSize(toolsPanel.rect.width*0.25f, toolsPanel.rect.height);

        toolsPanel.onMove();
    };

    toolsPanel.onResize();
}

void Editor::update() {
    toolsPanel.update();
    toolsPanelScrollBar.update(toolsPanel.rect);
}

void Editor::draw() {
    drawRect(toolsPanel.rect, toolsPanel.config->color);
    toolsPanelScrollBar.draw();
}

void Editor::onResize(float ratioWidth, float ratioHeight) {
    toolsPanel.rect.x *= ratioWidth;
    toolsPanel.rect.y *= ratioHeight;

    toolsPanel.rect.width *= ratioWidth;
    toolsPanel.rect.height *= ratioHeight;
    
    toolsPanel.onResize();
}