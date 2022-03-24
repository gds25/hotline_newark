# gf2d
Goal:
Eliminate all enemies

(Current) Features:

- twin stick shooter mechanics
- 5 enemies, each dropping a different weapon
- health, ammo, armor pickups
- speed and invisibility powerups
- 5 stealth mechanics (to be implemented)

Controls: 


The game currently requires a dual analog controller to play (PlayStation, XBox, and Switch controllers are all compatible) 

Left Stick: move 

Right Stick: rotate 

Right Trigger: shoot 

A(XBox)/X(PS): pick up weapon 

Start: pause menu (also accessable with 'p' on keyboard)

------------------------------------------------------------------------------------------------------------------------------------------------------------

# gameframework2d (GF2D)
a collection of utlitity functions designed to facilitate creating 2D games with SDL2
This project is specifically intended to function as an educational tool for my students taking 2D Game Programming.

Currently the project is in a WIP state, however it is functional with sample projects showcasing rudimentary scrolling shooters,
platformers, and adventure games.

The master branch is kept deliberately minimal to allow students to build out systems as they are learning.
Other branches have more complete system: Collisions, menus, input abstraction.

# Build Process

Before you can build the example code we are providing for you, you will need to obtain the libraries required
by the source code
 - SDL2
 - SDL2_image
 - SDL2_mixer
 - SDL2_ttf
There are additional sub modules that are needed for this project to work as well, but they can be pulled right from within the project.
Performable from the following steps from the root of the cloned git repository within a terminal. 

Make sure you fetch submodules: `git submodule update --init --recursive`
Go into each submodule's src directory and type:
`make`
`make static`

Once each submodule has been made you can go into the base project src folder anre simply type:
`make`

You should now have a `gf2d` binary within the root of your git repository. Executing this will start your game.
