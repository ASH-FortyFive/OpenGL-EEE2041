#ifndef moodel_H_
#define moodel_H_

#include <GL/glew.h>
#include <GL/glut.h>

#include <math.h>
#include <iostream>

#include <Vector.h>
#include <Matrix.h>
#include <Mesh.h>

class Model : public Mesh {
private:
    /*
	GLuint vertexColourAttribute;		// Vertex Position Attribute Location
	GLuint vertexColourBuffer;		    // Vertex Position Buffer  
    */

	GLuint MVMatrixUniformLocation; 	// Matrix Uniform Location 
	
	Matrix4x4 ModelMatrix;

	float scaleFactor;
	Vector3f position;
	Vector3f rotationAxis;
	float rotationAngle;
	
    Vector3f colour;

public:
    //! Overloads functions from Mesh to Allow for Transforms and Colours
  	void Draw(
        GLuint vertexPositionAttribute, 
        GLuint ColourUniformLocation = -1,
        GLuint vertexNormalAttribute = -1, 
        GLuint vertexTexcordAttribute = -1);

    
        //Load and OBJ mesh from File
    bool loadOBJ(std::string, GLuint);
    /*
	    //Creates geometry for a cube 
	void initCube(GLunit MVMatrixUniformLocation);
	    //Create geometry for triangle
	void initTriangle(GLunit MVMatrixUniformLocation);
	    //Create geometry for triangle
	void initQuad(GLunit MVMatrixUniformLocation);
    */

	void translate(Vector3f);
	void rotate(float, Vector3f);
	void scale(float);

	void printMatrix();

    void setColour(Vector3f);


  	Model();
  	~Model();
};

#endif
