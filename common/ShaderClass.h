#ifndef SHADERCLASS_H_
#define SHADERCLASS_H_

#include <GL/glew.h>
#include <GL/glut.h>
#include <Shader.h>
#include <string>
#include <iostream>
/**
 * Handles input of vertex and fragment shaders from file and src
 */
class MasterShader
{
//! Member Variables
private:    
public:
    GLuint ID;	
    //! For Positions and Transforms
    GLuint vertexPositionAttribute = -1;
    GLuint vertexNormalAttribute = -1;
    GLuint ModelMatrixUniformLocation = -1;
    GLuint ViewMatrixUniformLocation = -1;
    GLuint ProjectionUniformLocation = -1;

    //! For Textures
    GLuint vertexTexcoordAttribute = -1;
    GLuint TextureMapUniformLocation = -1;

	//! For Lighting
    GLuint LightPositionUniformLocation = -1;
    GLuint AmbientUniformLocation = -1;
    GLuint SpecularUniformLocation = -1;
    GLuint SpecularPowerUniformLocation = -1;

    //! Time Related (may no longer be useful)
    GLuint TimeUniformLocation = -1;

    //! For basic colours
    GLuint ColourUniformLocation = -1;

//! Functions
public:
    //! Constructors and Destructors 
    void loadShader(std::string, std::string);

    MasterShader();
    ~MasterShader();
};

#endif
