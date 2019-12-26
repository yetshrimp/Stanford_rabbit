#ifndef DEF_H
#define DEF_H

#include <string>

#include "gl.h"

using namespace std;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const float nearClipPlane = 0.1f;
const float farClipPlane = 100.0f;

const string bunnyFile = "bunny.ply";

const string rabbitVertexPath = "shader/rabbit.vs";
const string rabbitFragmentPath = "shader/rabbit.fs";

const string lightVertexPath = "shader/light.vs";
const string lightFragmentPath = "shader/light.fs";

extern float lightVertices[];

extern unsigned int lightIndices[];

extern glm::vec3 pointLightPositions[];

extern glm::vec3 pointLightScales[];

const int lightVertexNum = 36;

#endif // DEF_H
