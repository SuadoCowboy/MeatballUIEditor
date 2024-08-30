# TODOS
- there should be something like a editor mode and a run mode. Editor mode can move, resize, anchor, set events that calls a command prompt string, and run mode is self explanatory

- test multiple dynamic panels together

- implement UI commands for

- ui creation example: for Console, the user have some options: mainPanel, closeButton, sendButton(optional), inputBox and OutputTextBox. They also can modify events that the Console allowed, like: mainPanel::onMove, mainPanel::onResize and mainPanel::onResizeStop. The rest of the events are handled by the ConsoleUI code. closeButton::onRelease can not be changed because by default it should set ConsoleUI.visible to false. sendButton is the same thing, it should always send to the output and clear the input text. It would look like this in cfg file:
```c++
// THIS IS STILL ON THINKING, THINGS MIGHT NOT BE LIKE THIS ON RELEASE OF THE FIRST VERSION
// Also add something related to resizing. Something like a anchor (see how game engines work).

// BETTER IDEA: It works like Lua's stack. It adds all the data needed and then we call ui_create_button <name> and the data is popped. Any data that is not needed or not recognized, a error is printed but it will still be popped to not ruin any other possible ui object.
// params: name x y width height color borderRadius
ui_color 10 10 10 255
ui_border_radius 0
ui_rect_min 0 0 10 10 // for mainPanel, (x,y) are used to determine where mainPanel can be dragged or resized to
ui_rect_max_percent width height w h // [w,width] = renderWidth; [h,height] = renderHeight
ui_create_dynamic_panel "mainPanel"

// params: name x y width height color hoveredColor BUTTON_STYLE(number)
//ui_create_button "closeButton" x y width height 235 235 235 255 255 255 255 255 1
ui_color 235 235 235 255
ui_hovered_color 255 255 255 255
// x% y% width% height%
ui_anchor "mainPanel" // mainPanel should be defined before closeButton
ui_rect_min_percent 0 0 1 1
ui_rect_max_percent 0 0 3 3 
ui_draw x // it means that we will use drawX function
ui_create_button "closeButton"

ui_set_event "closeButton" "onRelease" toggle_local_console

// params: name x y width height color textColor hoveredColor hoveredTextColor text
//ui_create_text_button "sendButton" x y width height r g b a (x4) "Send"

ui_create_input_text_box "inputBox" x y width height 20 20 20 255 "default text"

ui_create_scroll_text_box name x y width height 20 20 20 255
// OR
// OBS: This method might need to create a math interpreter but it might not bee too hard to do and maybe even the HayBCMD could have already implemented
// where inputBox is specified, it means that it gets which parameter it is(ex: xPosition) and get the member of that object that matches the parameter
ui_create_scroll_text_box name "inputBox" "inputBox - 30" "inputBox" "inputBox"

// and then if someone wants to add a random button non-related to ConsoleUI:
ui_create_text_button "smiteAllButton" x y width height r g b a

ui_finish "ConsoleUI"
// this line gather all objects created by the ui_create commands and create a complete interface with a name that the game might look for and pop from the ui storage and set a defined object using that. Makes sense? I think it's pretty simple to do this. By the way, known objects have to use a specific name so that the game that uses that interface knows who is who

/*TODOS: (those are old todos that was made for the lua project. Don't know if it's still useful now)
if game does not recognize objectName: tell user
if type is not allowed for objectName: tell user and stop running
if a required objectName is not defined in the end: tell user and stop running
*/
```

# FUTURE TODOS:
- use threads with locks, atomic and mutex; (maybe not needed or worse)
- optimize code and use the right data structures;