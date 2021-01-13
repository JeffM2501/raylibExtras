# Raylib Extras
Utilities and shared components for use with raylib (https://www.raylib.com/)

# RLGeoTools
Geometry tools to extend raylib functionality

## GenMeshCustom
Creartes a mesh using a callback to generate the geomtry and bind it to openGL.
### Useage
Crearte a function that matches the siguanture of MeshGenerateCallback

void (*MeshGenerateCallback)(Mesh* mesh, void* userData);

Call GenMeshCustom with the callback
GenMeshCustom(yourcallback,NULL);

The second paramater is just an arbitary paramater that will be passed to the callback in case you need to pass any additional data to your generation fucntion.

Inside your function, you will be given a pointer to a mesh to fill out.
Call AllocateMeshData with the mesh given to you, and the counts for vertecies, triangles and colors, and flags for how many uv channels you want.

Then fill out the data for the mesh and let the callback return.

The mesh returned by GenMeshCustom will have the data set in the callback and be bound to openGL just like a regular map and will be manged by raylib as normal.
It can be bound to a model as normal.

## DrawVector functions
DrawVector3Text and DrawVector2Text will draw the contents of a vector as text at the defined position.

## Float Draw functions
These functions are versions of line, circle, and rectangle drawing functions that take raw float parameras intead of vectors for speed.