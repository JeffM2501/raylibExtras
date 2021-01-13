# Raylib Extras
Utilities and shared components for use with raylib (https://www.raylib.com/)

# Raylib Sprites
Basic Sprite management system. It does require C++ because it treats sprites as objects.

## Features

* Multiple Sprite Sheets per sprite
* Animated sequences
* Support for different directions in the same sequence
* Sprite Flipping
* Sprite Instancing
* Scaling and Rotation
* Sprite Origins
* Layering and Tinting
* Event callbacks

## Concepts

### Sprite
A collection of sprite sheets and frames that make up a single 'thing'. Contains SpriteImages and lists of frames that are referenced by Sequences. Frames can have negative width or height to flip the frame for display. Sprite frames may be used by multiple sequences.

### Sequence
A named series of frames that are executed in some order at some speed to play an animation.

### Instance
One or more sprite layers drawn at a postion/rotatation/scale on the screen executing some animation sequence.

# Example
See example.cpp