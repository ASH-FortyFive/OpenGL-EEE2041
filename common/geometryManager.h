#ifndef GEOMETRYMANAGER_H_
#define GEOMETRYMANAGER_H_

class Geometry {
private:
    GLuint vertexPositionAttribute;		// Vertex Position Attribute Location
    GLuint vertexPositionBuffer;		// Vertex Position Buffer  
public:
  void draw(); //Called to Draw Given Geometry 

  Geometry();
  ~Geometry();
};

#endif