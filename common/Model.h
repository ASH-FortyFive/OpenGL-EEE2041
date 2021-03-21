/* Model Class (and Helper Functions)
 * Created by ASH for EEE2041
 * Expands the provided Mesh Class to allow for Transforms and Textures
 *
 * Also handels the loading of textures
 */
#ifndef moodel_H_
#define moodel_H_

#include <GL/glew.h>
#include <GL/glut.h>

#include <math.h>
#include <iostream>

#include <Vector.h>
#include <Matrix.h>
#include <Mesh.h>
#include <Texture.h>

class Model : public Mesh {
//! Member Variables
private:
    /*
	GLuint vertexColourAttribute;		// Vertex Position Attribute Location
	GLuint vertexColourBuffer;		    // Vertex Position Buffer  
    */

	//!Uniform Locations
	GLuint MVMatrixUniformLocation; 	// Matrix Uniform Location 

	//! Personal ModelViewMatrix	
	Matrix4x4 ModelMatrix;

	//! Transform Varibles (used to create ModelMatrix each Draw)
	float scaleFactor;
	Vector3f position;
	Vector3f rotationAxis;
	float rotationAngle;
	
	//! Visuals
    Vector3f colour;
	GLuint textureID;

//! Functions
public:
    //! Overloads functions from Mesh to Allow for Transforms, Textures, and Colours (BROKEN)
  	void Draw(
		Matrix4x4 ModelViewMatrix,
        GLuint vertexPositionAttribute, 
        GLuint ColourUniformLocation = -1,
        GLuint vertexNormalAttribute = -1, 
        GLuint vertexTexcoordAttribute = -1);

    
    //Load and OBJ mesh from File, uses parent function
    bool loadOBJ(std::string, GLuint);
    /*
	    //Creates geometry for a cube 
	void initCube(GLunit MVMatrixUniformLocation);
	    //Create geometry for triangle
	void initTriangle(GLunit MVMatrixUniformLocation);
	    //Create geometry for triangle
	void initQuad(GLunit MVMatrixUniformLocation);
    */

	//! Mutators
	void scale(float); //Increases Scale by Input
	void setScale(float); //Sets Scale to Input
	void translate(Vector3f); //Translates Position by Input
	void setPosition(float, float, float); //Sets Position to Input
		//! Incomplete Rotation Functions
	void rotate(float, Vector3f); //Increments Rotation by Input (and sets Axis)
	void setRotation(float, Vector3f ); //Sets Rotation and Axis to Input
	void setColour(float, float, float); //Sets Colour to Input (RGB)
	
	//! Texture Functions
	void initTexture(std::string); //Loads Texture into TextureID

	//! Debug Functions
	void printMatrix(); //Shows Models Matrix

	//! Constructors and Destructors 
  	Model();
	Model( 												//Used to set inital Values (CURRENTLY BROKEN)
		Vector3f initalPos, 
		float initalSize = 1.0f,
		Vector3f initalRot = Vector3f(0.0f,0.0f,0.0f),
		float initalAngle = 0.0f, 
		Vector3f initalColour = Vector3f(0.5f, 0.5f, 0.5f));
  	~Model();
};

#endif
