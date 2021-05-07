/* Model Class (and Helper Functions)
 * Created by ASH for EEE2041
 * Expands the provided Mesh Class to allow for Transforms and Textures
 *
 * Also handels the loading of textures (as the Texture.h does not do this)
 */
#ifndef model_H_
#define model_H_

#include <GL/glew.h>
#include <GL/glut.h>

#include <math.h>
#include <iostream>
#include <vector>

#include <Vector.h>
#include <Matrix.h>
#include <Mesh.h>
#include <Texture.h>
#include <ShaderClass.h>

class Model : public Mesh {
//! Member Variables
private:
	//!Uniform Locations
	GLuint TextureMapUniformLocation; 	// Matrix Uniform Location 

	//! Personal ModelViewMatrix	
	Matrix4x4 ModelMatrix;

	//! Transform Varibles (used to create ModelMatrix each Draw)
	float scaleFactor;
	Vector3f position;
	Vector3f rotationAngles;	
	
	//! Visuals
	GLuint textureID;

//! Functions
public:
    //! Overloads functions from Mesh to Allow for Transforms, Textures, and Colours (BROKEN)
	void Draw(MasterShader shader);

    
    //Load and OBJ mesh from File, uses parent function
    bool loadOBJ(
		std::string filename, 
		GLuint TextureMapUniformLocation = -1,
		GLuint texture = -1);

	//! Mutators
	void scale(float); //Increases Scale by Input
	void setScale(float); //Sets Scale to Input
	void translate(Vector3f); //Translates Position by Input
	void translate(float, float, float); //Translates Position by Input
	void setPosition(float, float, float); //Sets Position to Input
	void setPosition(Vector3f); //Sets Position to Input
		//! Incomplete Rotation Functions
	void rotate(Vector3f); //Increments Rotation by Input (and sets Axis)
	void setRotation(Vector3f); //Sets Rotation and Axis to Input

	void changeTexture(GLuint);
	
	//! Accessors 
	Vector3f facing();
	Matrix4x4 getMatrix();
	Vector3f getRotiation();
	//! Overrides Mesh Function to get Centre of Translated Model
	Vector3f getMeshCentroid();

	//! Debug Functions

	//! Constructors and Destructors 
  	Model();
	  /*
	Model( 												//Used to set inital Values (CURRENTLY BROKEN)
		Vector3f initalPos, 
		float initalSize = 1.0f,
		Vector3f initalRot = Vector3f(0.0f,0.0f,0.0f),
		Vector3f initalAngles = Vector3f(0.0f,0.0f,0.0f), 
		Vector3f initalColour = Vector3f(0.5f, 0.5f, 0.5f));
		
	
	Model(const Model &model);
	Model& operator=(const Model &model);*/
  	~Model();
};

//! Helper Function
namespace ModelHelper {
	// Load Texture into Given Texture ID
	void initTexture(	std::string filename, 
						GLuint & textureID, 
						GLuint TEXTURE_TYPE = GL_TEXTURE_2D);
} 

#endif