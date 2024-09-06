# TODOS
- Lua (yet again) should have bindings to Meatball. The UI System should be made after lua bindings has been made. Here's some ideas I had recently and this is what I came up with:
A scene contains vectors for each ui type. **This would ease alot of work for most functions.**. We would use some kind of templates(probably made in JSON), in which would specify the required objects, if the object has a required type or a default type but it can be changed, etc. It would also be linked with the C++ code somehow, giving enough information to know some functions names that can be used when creating a object that is recognized by the template. It's good to notice that console run function should also be implemented. The way it would be setup could be a lua file, "data/interface/console.lua" for example, and place there some tables inside a array to be returned. It would look like this:
```lua
consoleUI = [ -- please note that this example is just what it can be in the future, not that it will be exactly the same.
    {
        name="mainPanel", -- not a required object but would have a default configuration for it.
        
        type="dynamicPanel", -- if type is not specified, in this case it would be fine because mainPanel by default would be dynamicPanel. But because of the way this would work, by saying the type is a rectangle would also work.
        
        -- everything works with floating percetange values instead of pixels
        
        size = [ -- size and position may or may not always be required
            0.5,
            0.5
        ]

        position = [
            0.25,
            0.15,
        ]

        --onResize = "functionA" -- this is not required and can't be put because it's already defined by default and it's only that.
        --onMove = "functionB" -- same comment from above.
    },
    {
        name="inputBox", -- required object. wouldn't need to specify in the code since it has everything by default. Nothing here would be possible to change except graphical stuff
        
        type="textBox", -- strict type, i.e. can not be changed and specifying the type here would not be required anyways since it's strict.
    },
    {
        name="outputBox", -- required object.
    }
]

return consoleUI
```

- there should be something like a editor mode and a run mode. Editor mode can move, resize, anchor, set events that calls a command prompt string, and run mode is self explanatory

- test multiple dynamic panels together

- implement UI commands

# FUTURE TODOS:
- use threads with locks, atomic and mutex; (maybe not needed or worse)
- optimize code and use the right data structures;