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
    for(auto box : boxes)
    {
        delete box;
    }
}

void Map::Draw(MasterShader shader)
{
    for(auto ring : rings)
    {
        ring->Draw(shader);
    }
    for(auto box : boxes)
    {
        box->Draw(shader);
    }
    for(auto coin : coins)
    {
        coin->Draw(shader);
    }

    ground->Draw(shader);
    
}

void Map::DrawSkybox(Vector3f camPos , MasterShader shader)
{
    sky->Draw(camPos, shader);
}

void Map::DrawHitboxes(MasterShader shader)
{
    //std::cout << "=============================" <<std::endl;
    for(auto ring : rings)
    {   
        //std::cout << "Ring" <<std::endl;
        ring->DrawHitboxes(shader);
    }
    for(auto box : boxes)
    {
        box->DrawHitboxes(shader);
    }
    for(auto coin : coins)
    {
        coin->DrawHitboxes(shader);
    }

    ground->DrawHitboxes(shader);
}


bool Map::Init(std::string mapPath, MasterShader skyboxShader, MasterShader defaultShader, Model ring, Model box, Model coin)
{

    std::fstream newFile;
    newFile.open(mapPath, std::ios::in);
    if(!newFile.good())
	{
		std::cerr<<"Failed to open file at "<<mapPath<<std::endl;
		return false;
	}

    //! Reading Plan POS
    newFile >> planePos.x >> planePos.y >> planePos.z; 

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
    sky = new Skybox;
    sky->Init(skyboxShader.TextureMapUniformLocation, paths);

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
    Model newGround;
    ModelHelper::initTexture(texturePath, texture);
    newGround.loadOBJ("../models/newGen.obj", defaultShader.TextureMapUniformLocation, texture);

    //! Creates hitbox for ground
    Hitbox::OBB newOBB;
    newOBB.centrePoint = Vector3f(mapDimensions.x / 2, 0, mapDimensions.z / 2);
    newOBB.extents[0] = (mapDimensions.x) / 2; 
    newOBB.extents[1] = 0.1f;
    newOBB.extents[2] = (mapDimensions.z) / 2;

    newGround.hitboxes.push_back(new Hitbox(newOBB, Hitbox::Obstacle)); 

    ground = new Model(newGround);

    //================================================//
    //! Ring Loading
    std::cout << std::endl << "//========== Loading Rings ==========\\\\" << std::endl;
    newFile >> ringCount;

    Vector3f location, rotation;
    int scale;

    rings.clear();

    for(int i(0); i < ringCount; i++)
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

    //! Boxes are optional so they may be skipped
    if(newFile.eof())
    {
        newFile.close();
        return true;    
    }

    //================================================//
    //! Box Loading
    std::cout << std::endl << "//========== Loading Boxes ==========\\\\" << std::endl;
    int num;
    newFile >> num;

    Vector3f vecScale; //New Vector for boxes

    boxes.clear();

    for(int i(0); i < num; i++)
    {
        if(newFile.eof())
        {
            newFile.close();
            return false;
        }
            
        newFile >> location.x   >> location.y   >> location.z  
                >> rotation.x   >> rotation.y   >> rotation.z 
                >> vecScale.x   >> vecScale.y   >> vecScale.z;

        if(!inBounds(location))
        {
            std::cout << "Box " << i << " is Out of Bounds"<< std::endl;
        }
        
        boxes.push_back(new Model(box));

        //rings.back()->loadOBJ("../models/torus.obj");
        boxes.back()->setPosition(location);
        boxes.back()->setRotation(rotation);
        boxes.back()->setScale(scale);

        boxes.back()->reloadHitbox();
    }

     //! Boxes are optional so they may be skipped
    if(newFile.eof())
    {
        newFile.close();
        return true;    
    }

    //================================================//
    //! Coin Loading
    std::cout << std::endl << "//========== Loading Coins ==========\\\\" << std::endl;
    newFile >> num;

    coins.clear();

    for(int i(0); i < num; i++)
    {
        if(newFile.eof())
        {
            newFile.close();
            return false;
        }
            
        newFile >> location.x   >> location.y   >> location.z  
                >> rotation.x   >> rotation.y   >> rotation.z;

        if(!inBounds(location))
        {
            std::cout << "Coin " << i << " is Out of Bounds"<< std::endl;
        }
        
        coins.push_back(new Model(coin));

        //rings.back()->loadOBJ("../models/torus.obj");
        coins.back()->setPosition(location);
        coins.back()->setRotation(rotation);
        coins.back()->setScale(1);

        coins.back()->reloadHitbox();
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

//! Resets
void Map::Reset()
{
   for(auto ring : rings)
    {
        ring->reloadHitbox();
        ring->colour = Vector3f();
    } 
    for(auto coin : coins)
    {
        if(coin->getMeshCentroid().y < -1000)
        {
            coin->translate(0,5000,0);
        }
    }
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
Hitbox::hbType Map::checkCollisions(Vector3f pos, std::vector<Hitbox*> otherBoxes)
{
    //! For hitting the floor
    if(pos.y < 5)
    {
        for(auto groundHB : ground->hitboxes)
        {
            for(auto otherHB : otherBoxes)
            {
                if(otherHB->doCollsions(*groundHB))
                {
                    return groundHB->Type;
                }
            }
        }
    }
    
    //! For hitting or passing through
    for(auto ring : rings)
    {
        //i++;
        if((ring->getMeshCentroid() - pos).length() < 10.0f)
        {
            auto badCounter = ring->hitboxes.begin();
            for(auto ringHB : ring->hitboxes)
            {
                for(auto otherHB : otherBoxes)
                {
                    if(otherHB->doCollsions(*ringHB))
                    {
                        if(ringHB->Type == Hitbox::Target)
                        {
                            ring->hitboxes.erase(badCounter);
                            ring->colour = Vector3f(0,0.25f,0);
                        }
                        return ringHB->Type;
                    }
                }
            badCounter++;
            }
        }
        
    }

    //! For hitting or passing through
    for(auto box : boxes)
    {
        //i++;
        if((box->getMeshCentroid() - pos).length() < 10.0f)
        {
            for(auto boxHB : box->hitboxes)
            {
                for(auto otherHB : otherBoxes)
                {
                    if(otherHB->doCollsions(*boxHB))
                    {
                        return boxHB->Type;
                    }
                }
            }
        }
        
    }

    //! For hitting or passing through
    for(auto coin : coins)
    {
        //i++;
        if((coin->getMeshCentroid() - pos).length() < 10.0f)
        {
            for(auto coinHB : coin->hitboxes)
            {
                for(auto otherHB : otherBoxes)
                {
                    if(otherHB->doCollsions(*coinHB))
                    {
                        coin->translate(0,-5000,0);
                        return coinHB->Type;
                    }
                }
            }
        }
        
    }

    
    //
    return Hitbox::NONE;
}