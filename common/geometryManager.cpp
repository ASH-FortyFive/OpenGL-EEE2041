using <geometryManager.h>

#include <GL/glew.h>
#include <GL/glut.h>
#include <Shader.h>
#include <math.h>


Geometry::Geometry()
{
    static const GLfloat vertices[] = {
        0.0f,   0.0f,  0.0f,
        0.5f,   0.0f,  0.0f,
        0.0f,   0.5f,  0.0f 
    };

    

    glGenBuffers(1, &vertexPositionBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, vertexPositionBuffer);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

}

void Geometry::drawGeometry()
{
    glEnableVertexAttribArray(vertexPositionAttribute);

    glBindBuffer(GL_ARRAY_BUFFER, vertexPositionBuffer);

    glVertexAttribPointer(
        vertexPositionAttribute,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        (void*)0
    );

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(vertexPositionAttribute);
}