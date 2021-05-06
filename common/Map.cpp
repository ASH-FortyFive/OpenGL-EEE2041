#include <Map.h>

//! Constructors and Destructors 
Map::Map()
{}
Map::~Map()
{}

void Map::Draw(MasterShader shader)
{

}

bool Map::Init(std::string mapFile)//, Model ring)
{
    std::fstream newFile;
    newFile.open(mapFile, std::ios::in);
    if(!newFile.good())
	{
		std::cerr<<"Failed to open file at "<<mapFile<<std::endl;
		return false;
	}

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

        new Model;
    }

    return true;
}


std::istream& operator>>(std::istream& in, Map& map)
{
    int num;
    Vector3f location, rotation, scale;

}