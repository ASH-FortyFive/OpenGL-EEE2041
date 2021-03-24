/* Model Class (and Helper Functions)
 * Created by ASH for EEE2041
 * Expands the provided Mesh Class to allow for Transforms and Textures
 *
 * Also handels the loading of textures (as the Texture.h does not do this)
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
	//GLuint MVMatrixUniformLocation; 	// Matrix Uniform Location 
	GLuint TextureMapUniformLocation; 	// Matrix Uniform Location 

	//! Personal ModelViewMatrix	
	Matrix4x4 ModelMatrix;

	//! Transform Varibles (used to create ModelMatrix each Draw)
	float scaleFactor;
	Vector3f position;
	Vector3f rotationAxis;
	float rotationAngle;

	//! New Rotations
	Vector3f rotationAngles;
	
	//! Visuals
    Vector3f colour;
	GLuint textureID;

//! Functions
public:
    //! Overloads functions from Mesh to Allow for Transforms, Textures, and Colours (BROKEN)
  	void Draw(
		Matrix4x4 ModelViewMatrix,
		GLuint MVMatrixUniformLocation,
        GLuint vertexPositionAttribute, 
        GLuint vertexNormalAttribute = -1, 
        GLuint vertexTexcoordAttribute = -1);

    
    //Load and OBJ mesh from File, uses parent function
    bool loadOBJ(
		std::string filename, 
		GLuint TextureMapUniformLocation = -1,
		GLuint texture = -1);
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
	void rotate(Vector3f); //Increments Rotation by Input (and sets Axis)
	void setRotation(Vector3f); //Sets Rotation and Axis to Input
	void setColour(float, float, float); //Sets Colour to Input (RGB)
	
	//! Accessors 
	Vector3f facing();
	//! Overrides Mesh Function to get Centre of Translated Model
	Vector3f getMeshCentroid();

	//! Debug Functions
	void printMatrix(); //Shows Models Matrix

	//! Constructors and Destructors 
  	Model();
	Model( 												//Used to set inital Values (CURRENTLY BROKEN)
		Vector3f initalPos, 
		float initalSize = 1.0f,
		Vector3f initalRot = Vector3f(0.0f,0.0f,0.0f),
		Vector3f initalAngles = Vector3f(0.0f,0.0f,0.0f), 
		Vector3f initalColour = Vector3f(0.5f, 0.5f, 0.5f));
  	~Model();
};

//! Helper Function
namespace ModelHelper {
	// Load Texture into Given Texture ID
	void initTexture(std::string filename, GLuint & textureID);
} 

#endif
