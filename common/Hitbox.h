/* Model Class (and Helper Functions)
 * Created by ASH for EEE2041
 * Expands the provided Mesh Class to allow for Transforms and Textures
 *
 * Also handels the loading of textures (as the Texture.h does not do this)
 */
#ifndef hitbox_H_
#define hitbox_H_

#include <GL/glew.h>
#include <GL/glut.h>

#include <math.h>
#include <iostream>
#include <Vector.h>
#include <Matrix.h>
#include <Mesh.h>
#include <list>
#include <vector>
#include <iterator>

class Hitbox{
//! Member Variables
private:


//! Functions
public:
    bool doCollsions(Hitbox&);

    void Draw(Matrix4x4 ModelViewMatrix, GLuint MVMatrixUniformLocation, GLuint vertexPositionAttribute, Vector3f colour);

    void loadHitboxes();

    Hitbox();
    ~Hitbox();
};


#endif