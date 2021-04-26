#include "HUD.h"

//! Constructors and Destructors 
HUD::HUD()
{

}

HUD::~HUD()
{

}

//! Basic (Private) Text Rendering 
void HUD::render2dText(std::string text,float r,float g,float b,float x,float y)
{
    glColor3f(r,g,b);
    glRasterPos2f(x, y); // window coordinates
    for(unsigned int i(0); i < text.size(); i++) 
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
    }
}