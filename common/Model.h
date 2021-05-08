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
#include <Hitbox.h>

class Model : public Mesh {
//! Member Variables
private:
	//!Uniform Locations
	GLuint TextureMapUniformLocation; 	// Matrix Uniform Location 

	//! Transform Varibles (used to create ModelMatrix each Draw)
	float scaleFactor;
	Vector3f position;
	Vector3f rotationAngles;	
	
	//! Visuals
	GLuint textureID;

	//! Hitboxes
	std::string hitboxPath; //! For a very lazy way of having copies keep the hitboxes

	//! Axis
	Vector3f abosluteAxis[3] = //! Always in order of x aixs, y axis then z axis
	{
		Vector3f(1,0,0),
		Vector3f(0,1,0),
		Vector3f(0,0,1)
	};
protected:
	//! Personal ModelViewMatrix	
	Matrix4x4 ModelMatrix;
public:
	std::vector<Hitbox*> hitboxes;

	Vector3f relativeAxis[3] = //! Always in order of x aixs, y axis then z axis
	{
		Vector3f(1,0,0),
		Vector3f(0,1,0),
		Vector3f(0,0,1)
	};
//! Functions
public:
    //! Overloads functions from Mesh to Allow for Transforms, Textures, and Colours (BROKEN)
	void Draw(MasterShader shader);
	void DrawHitboxes(MasterShader shader);
 
	void Reset();
    
    //Load an OBJ mesh from File, uses parent function
    bool loadOBJ(
		std::string filename, 
		GLuint TextureMapUniformLocation = -1,
		GLuint texture = -1);

	//Load an set of hitboxes from File
    bool loadHitbox(
		std::string filename);
	//Load an set of hitboxes, used for something bad and should be redone
    bool reloadHitbox();

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
	Matrix4x4 getMatrix();
	Vector3f getRotation();
	//! Overrides Mesh Function to get Centre of Translated Model
	Vector3f getMeshCentroid();

	//! Debug Functions

	//! Constructors and Destructors 
  	Model();
  	~Model();

private:
};

//! Helper Function
namespace ModelHelper {
	// Load Texture into Given Texture ID
	void initTexture(	std::string filename, 
						GLuint & textureID, 
						GLuint TEXTURE_TYPE = GL_TEXTURE_2D);
} 

#endif