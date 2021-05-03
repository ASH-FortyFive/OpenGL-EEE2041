/* Model Class (and Helper Functions)
 * Created by ASH for EEE2041
 * Adds a skybox (along side the new shaders)
 */
#ifndef Skybox_H_
#define Skybox_H_

#include <GL/glew.h>
#include <GL/glut.h>
#include <string>
#include <Texture.h>
#include <Shader.h>
#include <Matrix.h>
#include <Model.h>
#include <ShaderClass.h>


class Skybox
{
//! Member Variables
private:
     //! Skybox
    GLuint cubeTextureID[6];
    Model cube[6];

public:
//! Functions
private:
    void makeBox();
public:
    void Draw(Vector3f, Matrix4x4, MasterShader);
    void Init(GLuint, std::string*);

    //! Constructors and Destructors 
    Skybox();
    ~Skybox();   
}; 



#endif