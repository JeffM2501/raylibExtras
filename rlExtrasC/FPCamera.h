/**********************************************************************************************
*
*   raylibExtras * Utilities and Shared Components for Raylib
*
*   RLAssets * Simple Asset Managment System for Raylib
*
*   LICENSE: MIT
*
*   Copyright (c) 2020 Jeffery Myers
*
*   Permission is hereby granted, free of charge, to any person obtaining a copy
*   of this software and associated documentation files (the "Software"), to deal
*   in the Software without restriction, including without limitation the rights
*   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*   copies of the Software, and to permit persons to whom the Software is
*   furnished to do so, subject to the following conditions:
*
*   The above copyright notice and this permission notice shall be included in all
*   copies or substantial portions of the Software.
*
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*   SOFTWARE.
*
**********************************************************************************************/


#ifndef FP_CAMERA_H
#define FP_CAMERA_H

#include "raylib.h"
#include "raymath.h"

typedef enum
{
	MOVE_FRONT = 0,
	MOVE_BACK,
	MOVE_RIGHT,
	MOVE_LEFT,
	MOVE_UP,
	MOVE_DOWN,
	TURN_LEFT,
	TURN_RIGHT,
	TURN_UP,
	TURN_DOWN,
	SPRINT,
	LAST_CONTROL
}CameraControls;

typedef struct  
{
	// keys used to control the camera
	int ControlsKeys[LAST_CONTROL];

	// the speed in units/second to move 
	// X = sidestep
	// Y = jump/fall
	// Z = forward
	Vector3 MoveSpeed;

	// the speed for turning when using keys to look
	// degrees/second
	Vector2 TurnSpeed;

	// use the mouse for looking?
	bool UseMouse;

	// how many pixels equate out to an angle move, larger numbers mean slower, more accurate mouse
	float MouseSensitivity;

	// how far down can the camera look
	float MinimumViewY;

	// how far up can the camera look
	float MaximumViewY;

	// how fast the view should bobble as you move
	// defaults to 0 for no bobble
	float ViewBobbleFreq;

	// how high up/down will the bobble be
	float ViewBobbleMagnatude;

	// how far left and right should the view bobble
	float ViewBobbleWaverMagnitude;

	// the position of the base of the camera (on the floor)
	// note that this will not be the view position because it is offset by the eye height.
	// this value is also not changed by the view bobble
	Vector3 CameraPosition;

	// how far from the base of the camera is the player's view
	float PlayerEyesPosition;

	// the field of view in X and Y
	Vector2 FOV;

	// state for mouse movement
	Vector2 PreviousMousePosition;

	// state for view movement
	float TargetDistance; 

	// state for view angles
	Vector2 ViewAngles; 

	// state for bobble
	float CurrentBobble;

	// state for window focus
	bool Focused;

	// raylib camera for use with raylib modes.
	Camera3D ViewCamera;

	Vector3 Forward;
	Vector3 Right;

	//clipping planes
	// note must use BeginModeFP3D and EndModeFP3D instead of BeginMode3D/EndMode3D for clipping planes to work
	double NearPlane;
	double FarPlane;
}FPCamera;

// called to initialize a camera to default values
RLAPI void InitFPCamera(FPCamera *camera, float fovY, Vector3 position);

// called to update field of view in X when window resizes
RLAPI void ResizeFPCameraView(FPCamera* camera);

// turn the use of mouselook on/off, also updates the cursor visibility
RLAPI void UseFPCameraMouse(FPCamera* camera, bool useMouse);

// Get the camera's position in world (or game) space
RLAPI Vector3 GetFPCameraPosition(FPCamera* camera);

// update the camera for the current frame
RLAPI void UpdateFPCamera(FPCamera* camera);

// start drawing using the camera, with near/far plane support
RLAPI void BeginModeFP3D(FPCamera* camera);

// end drawing with the camera
RLAPI void EndModeFP3D();


#endif //FP_CAMERA_H
