# Unreal Engine Macro System

This is very unstable and buggy, and is more a proof of concept than anything substantially useful.
It's built on UE5.2, and uses Slate heavily (only the macro editor is exposed as a `UWidget`).
There's little documentation, but more will come as I flesh it out. Feel free to use or extend this as you wish.

- [How to setup](#how-to-setup)
- [Creating custom macros](#creating-custom-macros)
- [Screenshots](#screenshots)

## How to setup

1. Download or clone the repository,
2. Put it in your project's plugins folder (e.g. `MyProject/Plugins/MacroSystem`),
3. Open the editor and enable it in the plugins window (`Edit > Plugins > Macro System`),
4. Restart the editor,
5. Follow [Creating custom macros](#creating-custom-macros) for how to create your own macros.

## Creating custom macros

Macros are just `UObject`s, so you can inherit from the base `UMacro` Blueprint class to create your own.
Fill out the class defaults, and override `ExecuteCustomParameters`, which is where the magic happens.
In this event, you can do whatever you want, as macros have a valid world reference. For example,
the built-in `Delay` macro sets a timer for the user-specified length of time before finishing.
After you do whatever you wanted to do, call `FinishExecute` to finish the macro.
The `bSuccess` pin is purely to indicate to *conditional macros* (like if statements and while loops)
whether to run their actions, or skip them. If you don't intend on your macro being used in a conditional macro,
then don't worry about the value of `bSuccess`. You can just leave it as true.

>Control is handed over completely to the macro, so make sure all your code paths finish correctly!

In order to use your macro at runtime, you need to add it to the macro settings in a category. Go to
`Project Settings > Engine > Macro Settings` and add a category for your macros. Give it a unique key,
and add your macro to it. It will now show up in the context menu when adding an action to a user macro.

>The built-in macros (like If, While, For, Delay, etc.) are not added in the settings by default.
>I still need to work out how to do that.

## Screenshots

![Screenshot1](Screenshot1.png)
![Screenshot2](Screenshot2.png)
![Screenshot3](Screenshot3.png)
