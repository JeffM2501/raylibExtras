# Raylib Extras - FPCamera (C++ Version)
A simple first person camera for raylib

## About
This is a basic first person camera system. It is built to function similar to the build in CAMERA_FIRST_PERSON built in mode in raylib but has several enhancments and features.

* Uses time for movement not frames, so it works at any frame rate.
* Tracks the "feet" of the character not the eye point
* Supports configurable speeds
* Enable/Diable head bobble
* Sprint

This code is intended to be used as a starting point for an optimized camera for a first person view game.

## Useage
Add the 2 files (one .cpp and one .h) to your game proejct.

Create a FPCamera object in your game. This will hold all the camera data.

FPCamera MyCamera;

Setup the camera by calling the Setup method field of view, and inital postion.

MyCamera.Setup(45,Vector3{0,0,0});

Each frame call the Update method, this will apply all input and anmiation updates to the camera.

MyCamera.Update();

When you call BeginMode3D, pass in the value returned by the GetCamera member.

BeginMode3D(MyCamera.GetCamera());

If your game window is resizeable, call ViewResized each time the window is resized.

To get the current player postions (feet of the player) call GetCameraPosition. 

## Options

UseMouseX, UseMouseY
Enables/disables use of the mouse for looking in each axis

FPCamera::MoveSpeed
The speed in units per second that the camera moves in each axis

FPCamera::TurnSpeed
The speed in degrees per second that the camera rotates

FPCamera::ViewBobbleFreq
How much the view bobbles when it moves, 0 means no bobble

FPCamera::ValidateCamPostion
A callback function that is called before any move is applied, can be used to do collision detection and responce.