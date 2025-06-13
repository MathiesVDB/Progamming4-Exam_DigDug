Link to my source control depot (this github): https://github.com/MathiesVDB/Progamming4-Exam_DigDug

# Dig Dug

For my final project for Programming 4 I had to make the arcade game Dig Dug from practically scratch. This is quite the task, but luckily we got the entire semester for this task. We started with some assignments to help us get started and provide feedback on important parts of the engine that are created using design patterns.

## Our turn

After this we were on our own. 
- Gridcomponent: A component that has default values for Dig Dug, but can be adjusted to create any size grid. It holds Cells that remember coverTiles, spawnposition, centerpoint,.... Since this can be used for any game it is part of the engine.
- Level: This class will read a Dig Dug level, but only a Dig Dug level so it is part of the game instead of the engine.
- Scene/Scenemanager: I changed this to have an active scene, this way I could easily retrieve certain objects from my current active scene. This uses tags.
- Sceneswitcher: Code used to switch between scenes ensuring the switch happens between updates and not during. It also automatically clears the previous scene with all commands, colliders,...
- ColliderSystem: Checks collisions between registered collidercomponents (collidercomponents register in their constructor so user does not need to worry about this). If a collision is found it will call a collisionevent which the user can handle however they want in their game code.
- Selfdestructcomponent: Component that can be used in any game to create temporary objects that only need to be alive for a certain amount of time (I use them to make sure my scoretextures get deleted after a couple of seconds)
- SpriteComponent: Animates any spritesheet you give it. Also has option for accumulating frames so when you have a sprite that doubles every frame you can use this (I use this in my game for the Fygar fire, which gets bigger with each frame).
- Gamepad/Inputmanager: These handle the commands, they have the command class for input actions. These check wether a keybind that has a command linked to it is interacted with and reacts accordingly. The command class also has the option to create a global command which will not be deleted upon calling clearcommands so they last over multiple scenes. Gamepad also registers controllers and gives them their according indices.

The other classes I didn't mention are mainly found in the game part of my program. 
- Pooka and Fygar are AI controlled expect if a Fygar is set as controlled, then all AI logic is ignored.
- Player has a rope which gets moved out of screen if not needed.
- The digging is just placing an empty tile over a ground tile partly to give the illusion of digging part of the texture.
- Enemies will flee to the top left of the screen if they are the last enemy alive.
- In singleplayer, completing the 3 levels will allow you to give in a name and see the previous highscores and, if you got high enough, your own score.
