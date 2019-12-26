#ifndef PLY_H
#define PLY_H

#include <iostream>
#include <fstream>
#include <string>

#include "gl.h"

#define rabbitVertexMax 35947
#define rabbitFaceMax 69451

using namespace std;

extern float rabbitVertices[rabbitVertexMax * 6];
extern unsigned int rabbitIndices[rabbitFaceMax * 3];

extern int rabbitVertexNum;
extern int rabbitFaceNum;

typedef struct edgenode * edgeNode;

struct edgenode
{
    glm::vec3 edge;
    edgeNode next;
};

extern edgeNode edgeTable[rabbitVertexMax];

int loadFile(const string file);
void addEdge(int vertexIndex, glm::vec3 normal);
void calNormal();

#endif // PLY_H
