#ifndef VOLUMEVIS_H
#define VOLUMEVIS_H

#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <stdlib.h>
#include <GL/glut.h>
#include <time.h>
#include <GL/gl.h>

// Global Variables
#define LAND_VAL 9999.0


extern int h, w;
extern int width;
extern int height;
extern int length;
extern float scale;
extern float maxu, minu;
extern float maxv, minv;
extern float minsal, maxsal;
extern float transx, transy, transz;


void readData(char **argv);
int* getTri(int code);


class Vertex{
	float x, y, z;
	float sal;
public:
	Vertex();
	Vertex(float x_coor, float y_coor, float z_coor, float sal_val);
	float getX();
	float getY();
	float getZ();
	float getSalinity();
	void setX(float x_coor);
	void setY(float y_coor);
	void setZ(float z_coor);
	void setSalinity(float sal_val);
};

class Edge{
	Vertex *vertex1, *vertex2;
public:
	Edge();
	Edge(Vertex v1, Vertex v2);
	Vertex getVertex1();
	Vertex getVertex2();
	Vertex* interpolateP(float sal);
	Vertex* interpolateSalinity(float a,float b,float c,float d);
};

class Cube{
	float sal;
	int code, case_no;
	std::vector<Edge*> edges;
	std::vector<Vertex*> vertices;
public:
	Cube();
	Cube(std::vector<Vertex*> vrtx, std::vector<Edge*> edgs);
	void gen_Edges();
	Edge getEdge(int i);
	void addEdge(Edge e);
	Vertex getVertex(int i);
	void addVertex(Vertex v);
	std::vector<Edge*> getEdges();
	std::vector<Vertex*> getVertices();
};

#endif
