#ifndef GEOMERTY_H_
#define GEOMERTY_H_

#include <GL/glew.h>
#include <GL/glut.h>
#include <math.h>
#include <iostream>
#include <Matrix.h>
#include <Vector.h>

/**
 * Vector 2 Dimensions floatings
 */
class Geometry {
private:
    GLuint vertexPositionAttribute;		// Vertex Position Attribute Location
    GLuint vertexPositionBuffer;		// Vertex Position Buffer  
	GLuint vertexColourAttribute;		// Vertex Position Attribute Location
	GLuint vertexColourBuffer;		// Vertex Position Buffer  

	GLuint elementBuffer;

	GLuint MVMatrixUniformLocation; 	// Matrix Uniform Location 
	
	Matrix4x4 ModelMatrix;

	float scaleFactor;
	Vector3f position;
	Vector3f rotationAxis;
	float rotationAngle;
	
	int numberOfIndex; 
public:
	void initAttribute(GLuint posLoc, GLuint colLoc, GLuint matLoc);
	void initVerts(GLfloat vertsPosition[], GLfloat vertsColour[], GLuint vertsIndex[], int sizeVerts, int sizeIndex); //Called to Init set Geometry 
  	void draw(); //Called to Draw Given Geometry 

	void translate(Vector3f);
	void rotate(float, Vector3f);
	void scale(float);

	void printMatrix();

  	Geometry();
  	~Geometry();
};

#endif
