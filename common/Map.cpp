#include <Map.h>

//! Constructors and Destructors 
Map::Map()
{}
Map::~Map()
{}

void Map::Draw(MasterShader shader)
{
    for(auto ring : rings)
    {
        ring->Draw(shader);
    }
    for(auto ground : grounds)
    {
        ground->Draw(shader);
    }
    
}

void Map::DrawSkybox(Vector3f camPos,Matrix4x4 ModelViewMatrix, MasterShader shader)
{
    sky.Draw(camPos, ModelViewMatrix, shader);
}

void DrawHitbox(MasterShader shader);


bool Map::Init(std::string mapFile, MasterShader skyboxShader, MasterShader defaultShader, Model ring)
{
    std::fstream newFile;
    newFile.open(mapFile, std::ios::in);
    if(!newFile.good())
	{
		std::cerr<<"Failed to open file at "<<mapFile<<std::endl;
		return false;
	}


    //================================================//
    //! Skybox Loading
    std::cout << "Loading Skybox" << std::endl;
    std::string paths[6];
    for(int i(0); i < 6; i++)
    {
        if(newFile.eof())
        {
            return false;
        }
        newFile >> paths[i];
    }
    sky.Init(skyboxShader.TextureMapUniformLocation, paths);

    //================================================//
    //! Size of the Map
    newFile >> mapDimensions.x >> mapDimensions.y >> mapDimensions.z; 

    //================================================//
    //! Ground Loading
    std::cout << "Loading ground" << std::endl;
    std::string texturePath;
    if(newFile.eof())
        {
            return false;
        }
        newFile >> texturePath;

    GLuint texture;
    Model groundTile;

    ModelHelper::initTexture(texturePath, texture);
    groundTile.loadOBJ("../models/ground.obj", defaultShader.TextureMapUniformLocation, texture); 
    groundTile.setScale(10);   
    grounds.push_back(new Model(groundTile));

    //================================================//
    //! Ring Loading
    std::cout << "Loading Rings" << std::endl;
    int num;
    newFile >> num;

    Vector3f location, rotation;
    int scale;

    for(int i(0); i < num; i++)
    {
        if(newFile.eof())
        {
            return false;
        }
            
        newFile >> location.x   >> location.y   >> location.z  
                >> rotation.x   >> rotation.y   >> rotation.z 
                >> scale;

        std::cout << "Created Ring" << std::endl;
        rings.push_back(new Model(ring));

        //rings.back()->loadOBJ("../models/torus.obj");
        rings.back()->setPosition(location);
        rings.back()->setRotation(rotation);
        rings.back()->setScale(scale);
    }

    //! Ensures file was read succsefully
    if(newFile.eof())
    {
        return true;    
    }
    return false;
}


std::istream& operator>>(std::istream& in, Map& map)
{
    int num;
    Vector3f location, rotation, scale;

}