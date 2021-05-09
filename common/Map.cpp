#include <Map.h>

//! Constructors and Destructors 
Map::Map()
{}
Map::~Map()
{
    for(auto ring : rings)
    {
        delete ring;
    }
}

void Map::Draw(MasterShader shader)
{
    for(auto ring : rings)
    {
        ring->Draw(shader);
    }

    ground.Draw(shader);
    
}

void Map::DrawSkybox(Vector3f camPos , MasterShader shader)
{
    sky.Draw(camPos, shader);
}

void Map::DrawHitboxes(MasterShader shader)
{
    //std::cout << "=============================" <<std::endl;
    for(auto ring : rings)
    {   
        //std::cout << "Ring" <<std::endl;
        ring->DrawHitboxes(shader);
    }

    ground.DrawHitboxes(shader);
}


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
    std::cout << std::endl << " //========== Loading Skybox ==========\\\\ " << std::endl;
    std::string paths[6];
    for(int i(0); i < 6; i++)
    {
        if(newFile.eof())
        {
            newFile.close();
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
    std::cout << std::endl << " //========== Loading Ground ==========\\\\ " << std::endl;
    std::string texturePath;
    if(newFile.eof())
    {
        newFile.close();
        return false;
    }

    newFile >> texturePath;
    createGround(mapDimensions);

    GLuint texture;
    ModelHelper::initTexture(texturePath, texture);
    ground.loadOBJ("../models/newGen.obj", defaultShader.TextureMapUniformLocation, texture);

    //! Creates hitbox for ground
    Hitbox::OBB newOBB;
    newOBB.centrePoint = Vector3f(mapDimensions.x / 2, 0, mapDimensions.z / 2);
    newOBB.extents[0] = (mapDimensions.x) / 2; 
    newOBB.extents[1] = 0.1f;
    newOBB.extents[2] = (mapDimensions.z) / 2;


    ground.hitboxes.push_back(new Hitbox(newOBB, Hitbox::Obstacle)); 


    //================================================//
    //! Ring Loading
    std::cout << std::endl << "//========== Loading Rings ==========\\\\" << std::endl;
    int num;
    newFile >> num;

    Vector3f location, rotation;
    int scale;

    for(int i(0); i < num; i++)
    {
        if(newFile.eof())
        {
            newFile.close();
            return false;
        }
            
        newFile >> location.x   >> location.y   >> location.z  
                >> rotation.x   >> rotation.y   >> rotation.z 
                >> scale;

        if(!inBounds(location))
        {
            std::cout << "Ring " << i << " is Out of Bounds"<< std::endl;
        }
        
        rings.push_back(new Model(ring));

        //rings.back()->loadOBJ("../models/torus.obj");
        rings.back()->setPosition(location);
        rings.back()->setRotation(rotation);
        rings.back()->setScale(scale);

        rings.back()->reloadHitbox();
    }

    //! Ensures file was read succsefully
    if(newFile.eof())
    {
        newFile.close();
        return true;    
    }
    newFile.close();
    return false;
}

//! Create
void Map::createGround(Vector3f dimensions)
{
    std::ofstream newObj("../models/newGen.obj");
    std::ifstream baseObj("../models/ground.obj");

    std::string nextLine;

    //! Loops through baseline file and changes the objs dimensions to match the given ones
    //! Almost certainly a poor way of doing this, but the code is only run once
    std::getline(baseObj, nextLine);
    newObj << nextLine << std::endl;
    for(int i(0); i < 6; i++)
    {
        std::getline(baseObj, nextLine); //! Skips the six lines we write manually
    }
    
    newObj << "v 0.0 0.0 " << dimensions.z << std::endl;
    newObj << "v 0.0 0.0 " << dimensions.z << std::endl;
    newObj << "v " << dimensions.x <<  " 0.0 " << dimensions.z << std::endl;
    newObj << "v " << dimensions.x <<  " 0.0 " << dimensions.z << std::endl;
    newObj << "v 0.0 0.0 0.0" << std::endl;
    newObj << "v " << dimensions.x <<  " 0.0 0.0 " << std::endl;

    while(std::getline(baseObj, nextLine)) //! Copies the rest of the document
    {
        newObj << nextLine << std::endl;
    }

    newObj.close();
    baseObj.close();
}

bool Map::inBounds(Vector3f pos)
{
    if(pos.x >= 0 && pos.x <= mapDimensions.x)
    {
        if(pos.z >= 0 && pos.z <= mapDimensions.z)
        {
            if(pos.y >= 0 && pos.y <= mapDimensions.y)
            {
                return true;
            }
        }
    }
    return false;
}

//! This is where the main collision math gets donea
Hitbox::hbType Map::checkCollisions(Vector3f pos, std::vector<Hitbox*> boxes)
{
    //int i(0);   
    for(auto ring : rings)
    {
        //i++;
        if((ring->getMeshCentroid() - pos).length() < 10.0f)
        {
            auto badCounter = ring->hitboxes.begin();
            for(auto ringHB : ring->hitboxes)
            {
                for(auto otherHB : boxes)
                {
                    if(otherHB->doCollsions(*ringHB))
                    {
                        if(ringHB->Type == Hitbox::Target)
                        {
                            ring->hitboxes.erase(badCounter);
                            std::cout <<"Git good" << std::endl;
                        }
                        return ringHB->Type;
                    }
                }
            badCounter++;
            }
        }
        
    }
    if(pos.y < 5)
    {
        for(auto groundHB : ground.hitboxes)
        {
            for(auto otherHB : boxes)
            {
                if(otherHB->doCollsions(*groundHB))
                {
                    return groundHB->Type;
                }
            }
        }
    }
    //
    return Hitbox::NONE;
}