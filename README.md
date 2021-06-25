# SDL2 Tutorials

![SDL Logo](./md/preview.png)
## GitHub [repository](https://github.com/jmparis/SDL2-lazy-foo)

This tutorial is based on the [SDL lazy' foo](https://lazyfoo.net/tutorials/SDL/index.php) tutorials series.


|  # | Title | Description |
|---:|:------|:------------|
| 00 | [Installation](./lesson-00/README.md)                | In this tutorial we will be setting up the SDL library. |
| 01 | [Hello SDL](./lesson-01/README.md)                   | In this tutorial we will be creating our first window. |
| 02 | [Getting an Image on the screen](./lesson-02/README.md)  | Now that we can get a window to appear, lets blit an image onto it. |
| 03 | [Event Driven Programming](./lesson-03/README.md)    | Here we'll start handling user input by allowing the user to X out the window.	 |
| 04 | [Key Presses](./lesson-04/README.md)                 | Here we'll learn to handle keyboard input. |
| 05 | [Optimized Surface Loading and Soft Stretching](./lesson-05/README.md)    | Now that we know how to load and blit surfaces, it's time to make our blits faster. We'll also take a smaller image and stretch it to fit the screen. |
| 06 | [Extension Libraries and Loading Other Image Formats](./lesson-06/README.md) | Here we'll be using the SDL_image extension library to load png images. |
| 07 | [Texture Loading and Rendering](./lesson-07/README.md)   | A big new feature in SDL 2.0 is hardware accelerated texture based 2D rendering. Here we'll be loading an image to render it using textures. |
| 08 | [Geometry Rendering](./lesson-08/README.md)          | Another new feature in SDL 2.0 is hardware accelerated primitive rendering. Here we'll be using it to render some common shapes. |
| 09 | [The Viewport](./lesson-09/README.md)                | SDL 2.0 also lets you control where you render on the screen using the viewport. We'll be using the viewport to create subscreens. |
| 10 | [Color Keying](./lesson-10/README.md)                | Here we'll use color keying to give textures transparent backgrounds. |
| 11 | [Clip Rendering and Sprite Sheets](./lesson-11/README.md)    | Using clip rendering, you can keep multiple images on one texture and render the part you need. We'll be using this to render individual sprites from a sprite sheet. |
| 12 | [Color Modulation](./lesson-12/README.md)            | We'll be altering the color of rendered textures using color modulation. |
| 13 | [Alpha Blending](./lesson-13/README.md)              | Here we'll be using SDL 2.0 new hardware accelerated alpha blending. |
| 14 | [Animated Sprites and Vsync](./lesson-14/README.md)  | Here we'll be using a sequence of sprites to animate them. |
| 15 | [Rotation and Flipping](./lesson-15/README.md)       | Here we'll be using SDL 2.0's new texture rotation and flipping. |
| 16 | [True Type Fonts](./lesson-16/README.md)             | Here we'll be rendering text from true type fonts using `SDL_ttf`. |
| 17 | [Mouse Events](./lesson-17/README.md)                | Here we'll learn to read mouse input using mouse events. |
| 18 | [Key States](./lesson-18/README.md)                  | There are other ways to read the keys besides event polling. Here we will get the current state of the keyboard using key states. |
| 19 | [Gamepads and Joysticks](./lesson-19/README.md)      | Here we'll learn to read input from a game controller. |
| 20 | [Force Feedback](./lesson-20/README.md)              | Another new feature for SDL 2.0 is rumble support using the SDL haptics. We'll make our controller rumble when a button is pressed. |
| 21 | [Sound Effects and Music](./lesson-21/README.md)     | Here we'll be using `SDL_mixer` to add music and sound to our SDL App. |
| 22 | [Timing](./lesson-22/README.md)                      | Here we'll be using SDL's time capabilites. |
| 23 | [Advanced Timers](./lesson-23/README.md)             | Here we'll extend SDL time capabilities to make our own custom timer. |
| 24 | [Calculating Frame Rate](./lesson-24/README.md)      | Here we'll use the timers we built to measure frame rate. |
| 25 | [Capping Frame Rate](./lesson-25/README.md)          | If you need a constant frame rate when vsync isn't available, frame rate capping can be used as a fall back. |
| 26 | [Motion](./lesson-26/README.md)                      | Here we'll be taking what we learned about rendering and handling input to make a dot move around the screen. |
| 27 | [Collision Detection](./lesson-27/README.md)         | Here we'll have two objects interact with each other using bounding box collision detection. |
| 28 | [Per-pixel Collision Detection](./lesson-28/README.md)   | Here we'll have two objects collide using per-pixel collision detection. |
| 29 | [Circular Collision Detection](./lesson-29/README.md)    | Here we'll learn to detect collisions with circles and boxes. |
| 30 | [Scrolling](./lesson-30/README.md) | Here we'll be implementing a camera to scroll levels larger than the screen. |
| 31 | [Scrolling Backgrounds](./lesson-31/README.md)       | Here we'll be using a scrolling background to give the illusion of an infinite level. |
| 32 | [Text Input and Clipboard Handling](./lesson-32/README.md)     | Here we'll be using SDL 2.0's new way of handling text input and its new clipboard handling feature. |
| 33 | [File Reading and Writing](./lesson-33/README.md)    | Here we'll be using SDL's RWOps API to do binary file IO. |
| 34 | [Audio Recording](./lesson-34/README.md)             | SDL 2.0.8 supports audio recording. Here we'll be copying from the audio device to record and copying to the audio device to play back. |
| 35 | [Window Events](./lesson-35/README.md)               | Here we'll be handling events from a resizable window. |
| 36 | [Multiple Windows](./lesson-36/README.md)            | A new feature in SDL is the ability to support more than one window. Here we'll make an application that has 3 windows. |
| 37 | [Multiple Displays](./lesson-37/README.md)           | Another new feature of SDL 2.0 is the ability to handle more than one physical display. Here we'll make our window jump from display to display. |
| 38 | [Particle Engines](./lesson-38/README.md)            | Here we'll use a simple particle effect to create a simple trail effect. |
| 39 | [Tiling](./lesson-39/README.md)                      | Here we'll make a simple level using a tiling engine.  |
| 40 | [Texture Manipulation](./lesson-40/README.md)        | Here we'll be directly accessing and manipulating a texture's pixels.  |
| 41 | [Bitmap Fonts](./lesson-41/README.md)                | Here we'll be using a texture as a font using bitmap font techniques.  |
| 42 | [Texture Streaming](./lesson-42/README.md)           | Here we'll be rendering from a streaming data source using texture streaming.  |
| 43 | [Render to Texture](./lesson-43/README.md)           | Here we'll be taking a scene and rendering it to a texture.  |
| 44 | [Frame Independent Movement](./lesson-44/README.md)  | Here we'll be making the dot move independent of the current frame rate. |
| 45 | [Timer Callbacks](./lesson-45/README.md)             | SDL has another timing mechanism called timer callbacks. Here we'll be setting a function to be called back after a certain amount of time. |
| 46 | [Multithreading](./lesson-46/README.md)              | Multithreading allows your program to do things simultaneously. Here we'll make things print to the console from outside our main thread. |
| 47 | [Semaphores](./lesson-47/README.md)                  | A major issue in multithreaded applications is that you need to make sure that they don't try to access the same data at the same time. Semaphores are a way to make sure only a certain amount of threads are performing an action at the same time. |
| 48 | [Atomic Operations](./lesson-48/README.md)           | Atomic operations are another way to synchronize threads. Here we'll be redoing the previous tutorial with atomic counters. |
| 49 | [Mutexes and Conditions](./lesson-49/README.md)      | Mutexes and conditions are yet another way to synchronize threads. Here we'll be using the added benefit that they allow threads to communicate with each other. |
| 50 | [SDL and OpenGL 2](./lesson-50/README.md)            | SDL is a powerful tool when combined with OpenGL. If you're just starting out with OpenGL or want to maximize compatibility, you can use SDL with OpenGL 2.1. In this tutorial we will make a minimalist OpenGL 2.1 program. |
| 51 | [SDL and Modern OpenGL](./lesson-51/README.md)       | SDL 2.0 now has support for OpenGL 3.0+ with context controls. Here we'll be making a minimalist OpenGL 3+ core program. |
