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


class Skybox
{
//! Member Variables
private:
    GLuint cubemapTextureID;
    GLuint vertexPositionBuffer;
public:
//! Functions
private:
    bool loadCubemapTexture(std::string*);
public:
    void Draw(Matrix4x4 ModelViewMatrix, GLuint MVMatrixUniformLocation, GLuint vertexPositionAttribute);
    void Init();

    //! Constructors and Destructors 
    Skybox(std::string*);
    ~Skybox();   
}; 



#endif