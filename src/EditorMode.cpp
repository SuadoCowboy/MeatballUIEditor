#include "EditorMode.h"

#include <Meatball/Utils/Defaults.h>
#include <Meatball/Utils/DrawFuncs.h>

bool editorMode = true;

Editor::Editor() {
    mainPanel.rect.width = GetScreenWidth()*0.25f;
    mainPanel.rect.height = GetScreenHeight();

    mainPanel.rect.x = GetScreenWidth()-mainPanel.rect.width;
    mainPanel.rect.y = 0.0f;

    toolsArray[0].text = "Button";
    toolsArray[1].text = "TextButton";
    toolsArray[2].text = "DynamicPanel";
    toolsArray[3].text = "InputTextBox";
    toolsArray[4].text = "ScrollTextBox";
    toolsArray[5].text = "ScrollBar";

    mainPanel.onResize = [&]() {
        toolsRect.width = mainPanel.rect.width-4.0f;
        toolsRect.height = mainPanel.rect.height*0.2f;

        toolsScrollBar.setSize(toolsRect.width*0.05f, toolsRect.height);

        {
            float width = (toolsRect.width - toolsScrollBar.getRect().width)/toolsWidthDivisor;
            for (unsigned char i = 0; i < toolsArraySize; ++i) {
                toolsArray[i].fontSize = toolsRect.height;

                toolsArray[i].rect.width = width;
                toolsArray[i].rect.height = toolsArray[i].fontSize + 2.0f;

                while (toolsArray[i].fontSize > 1 && measureTextWidth(
                    *toolsArray[i].config->font, toolsArray[i].fontSize, toolsArray[i].text.c_str()) > toolsArray[i].rect.width*0.5f)  {
                    toolsArray[i].fontSize -= 2;
                }
            }
        }

        objectsScrollBar.setSize(mainPanel.rect.width*0.05f, mainPanel.rect.height-toolsRect.height);

        for (auto& obj : objectsList) {
            obj.rect.width = mainPanel.rect.width-objectsScrollBar.getRect().width;
            obj.rect.height = obj.fontSize+2;
        }

        mainPanel.onMove();
    };

    mainPanel.onMove = [&]() {
        toolsRect.x = mainPanel.rect.x+2.0f;
        toolsRect.y = mainPanel.rect.y+4.0f;

        toolsScrollBar.setPosition(
            toolsRect.x+toolsRect.width-toolsScrollBar.getRect().width,
            toolsRect.y
        );

        {
            toolsArray[0].rect.x = toolsRect.x;
            toolsArray[0].rect.y = toolsRect.y;

            float xMult = 1.0f, yMult = 0.0f;
            for (unsigned char idx = 1; idx < toolsArraySize; ++idx) {
                if (idx % toolsWidthDivisor == 0) {
                    yMult += 1.0f;
                    xMult = 0.0f;
                }
                
                toolsArray[idx].rect.x = toolsRect.x + xMult * toolsArray[idx].rect.width;
                toolsArray[idx].rect.y = toolsRect.y + yMult * toolsArray[idx].rect.height * -toolsScrollBar.getScrollValue();
                toolsArray[idx].rect.y -= toolsArray[idx].rect.height * toolsScrollBar.getScrollValue();

                xMult += 1.0f;
            }
        }

        objectsScrollBar.setPosition(
            mainPanel.rect.x+mainPanel.rect.width-objectsScrollBar.getRect().width,
            toolsRect.y+toolsRect.height+1.0f
        );

        for (size_t yMultiplier = 0; yMultiplier < objectsList.size(); ++yMultiplier) {
            objectsList[yMultiplier].rect.x = mainPanel.rect.x;
            objectsList[yMultiplier].rect.y = yMultiplier * objectsList[yMultiplier].rect.height
                + objectsScrollBar.getRect().y;
        }
    };

    mainPanel.onResize();

    objectsScrollBar.visible = toolsScrollBar.visible = true;

    for (unsigned char i = 0; i < toolsArraySize; ++i)
        toolsArray[i].onRelease = [&]() {
            currentTool = i;
        };
}

void Editor::update() {
    mainPanel.update();
    
    {
        Rectangle rect = objectsScrollBar.getRect();
        rect.x = mainPanel.rect.x;
        rect.width = mainPanel.rect.width;
        objectsScrollBar.update(rect);

        float contentHeight = 0.0f;
        for (auto& object : objectsList) {
            object.update();
            contentHeight += object.rect.height;
        }

        objectsScrollBar.updateThumbHeight(rect.height, contentHeight);
    }

    {
        toolsScrollBar.update(toolsRect);
        toolsScrollBar.updateThumbHeight(toolsRect.height, toolsArray[0].rect.height*toolsHeightDivisor);

        Vector2 mousePos = GetMousePosition();

        float yMult = -1.0f;
        for (unsigned char idx = 0; idx < toolsArraySize; ++idx) {
            if (idx % toolsWidthDivisor == 0)
                yMult += 1.0f;

            if (mousePos.y >= toolsRect.y && mousePos.y <= toolsRect.y+toolsRect.height)
                toolsArray[idx].update();
            else
                toolsArray[idx].hovered = false;

            toolsArray[idx].rect.y = toolsRect.y + yMult * toolsArray[idx].rect.height
                            - toolsArray[idx].rect.height * toolsScrollBar.getScrollValue();
        }
    }
}

void Editor::draw() {
    drawRect(mainPanel.rect, mainPanel.config->color);

    for (unsigned char i = 0; i < toolsArraySize; ++i) {
        if (toolsArray[i].rect.y > toolsRect.y+toolsRect.height) break;
        
        BeginScissorMode(toolsRect.x, toolsRect.y, toolsRect.width, toolsRect.height);
        drawRect({
            toolsArray[i].rect.x,
            toolsArray[i].rect.y,
            toolsArray[i].rect.width,
            toolsArray[i].rect.height
            },
            toolsArray[i].hovered? toolsArray[i].config->hoveredColor : toolsArray[i].config->color);
        
        toolsArray[i].drawText();
        EndScissorMode();
    }

    toolsScrollBar.draw();

    BeginScissorMode(mainPanel.rect.x, objectsScrollBar.getRect().y, mainPanel.rect.width, objectsScrollBar.getRect().height);
        objectsScrollBar.draw();
        for (auto& obj : objectsList) {
            drawRect({
                obj.rect.x,
                obj.rect.y,
                obj.rect.width,
                obj.rect.height
                },
                obj.config->color);
            
            obj.drawText();
        }
    EndScissorMode();
}

void Editor::onResize(float ratioWidth, float ratioHeight) {
    mainPanel.rect.x *= ratioWidth;
    mainPanel.rect.y *= ratioHeight;

    mainPanel.rect.width *= ratioWidth;
    mainPanel.rect.height *= ratioHeight;
    
    mainPanel.onResize();
}