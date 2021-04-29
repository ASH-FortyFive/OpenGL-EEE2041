/* Map Class
 * Created by ASH for EEE2041
 * Handels the terrain and rings
 */
#ifndef Map_H_
#define Map_H_

#include "Vector.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <Model.h>
#include <string>

class Map
{
//! Member Variables
private:
    Model ring;

    //! "Skybox"
    Model right,left,top,bottom,front,back;



public:

//! Member Functions
private:

public:
    //! Constructors and Destructors 
    Map();
    ~Map();

    //! Drawing Functions
    void Draw();


    //! For Initlaizng 
    void Init(std::string);
};

#endif

