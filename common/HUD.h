/* HUD Class (and 
 * Created by ASH for EEE2041
 * Handels the Heads Up Display
 *
 */
 
#ifndef HUD_H_
#define HUD_H_

#include <GL/glew.h>
#include <GL/glut.h>
#include <string>

class HUD 
{
//! Member Variables
private:

//! Functions
private:
    
public:
    //! Constructors and Destructors     
    HUD();
    ~HUD();

    //! Basic Function for Drawing Text
    void render2dText(std::string,float,float,float,float,float);
};


#endif
