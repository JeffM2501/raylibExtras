# Raylib Extras - FPCamera (C Version)
A simple first person camera for raylib

## About
This is a basic first person camera system. It is built to function similar to the build in CAMERA_FIRST_PERSON built in mode in raylib but has several enhancments and features.

* Uses time for movement not frames, so it works at any frame rate.
* Tracks the "feet" of the character not the eye point
* Supports configurable speeds
* Enable/Disable head bobble
* Sprint

This code is intended to be used as a starting point for an optimized camera for a first person view game.

## Useage
Add the 2 files (one .c and one .h) to your game proejct.

Create a FPCamera variable in your game. This will hold all the camera data.

FPCamera MyCamera;

Setup the camera by calling 
InitFPCamera with your camera, field of view, and inital postion.

InitFPCamera(&MyCamera,45,(Vector3){0,0,0});

Each frame call UpdateFPCamera and pass in a pointer to the camera. This will apply all input and anmiation updates to the camera.

UpdateFPCamera(&MyCamera);

When you call BeginMode3D, pass in the ViewCamera member of your camera variable

BeginMode3D(MyCamera->ViewCamera);

If your game window is resizeable, call ResizeFPCameraView each time the window is resized.

To get the current player postions (feet of the player) call GetFPCameraPosition

## Options

void UseFPCameraMouse(FPCamera* camera, bool useMouse);
Enables/disables use of the mouse for looking.

FPCamera::MoveSpeed
The speed in units per second that the camera moves in each axis

FPCamera::TurnSpeed
The speed in degrees per second that the camera rotates

FPCamera::ViewBobbleFreq
How much the view bobbles when it moves, 0 means no bobble
