#include "StudentWorld.h"
#include <string>
#include "Level.h"
#include "Actor.h"
#include <iostream>
#include <sstream>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
StudentWorld::StudentWorld(std::string assetDir)
: GameWorld(assetDir)
{
    m_bonus = 1000;
    m_numJewels = 0;
    m_levelOver = false;
}

//return objects that only block players. takes care of the bug that when a robot is on a jewel/goodie,
//the robot does not block player

Actor* StudentWorld::getActorThatBlocksPlayerAt(int x, int y, Actor* act)
{
    if (act != m_player && m_player != nullptr)
    {
        if (m_player->isDead() == false && m_player->getX()== x && m_player->getY() == y)
        {
            return m_player;
        }
    }
    for (int i = m_actors.size()-1; i >= 0 ; i--)
    {
        if (m_actors[i]->isDead() == false && m_actors[i] != act)
        {
            if (m_actors[i]->getX() ==  x && m_actors[i]->getY() == y)
            {
                if (m_actors[i]->blocksPlayer(m_player->getDirection()))
                    return m_actors[i];
            }
        }
    }
    return nullptr;
}

//returns only goodies at the location. takes care of the bug of when a goodie is on a jewel after
//getting dropped by a kleptobot, the player doesn't recognize that the goodie is there.

Actor* StudentWorld::getGoodieAt(int x, int y, Actor* act)
{
    for (int i = m_actors.size()-1; i >= 0 ; i--)
    {
        if (m_actors[i]->isDead() == false && m_actors[i] != act)
        {
            if (m_actors[i]->getX() ==  x && m_actors[i]->getY() == y)
            {
                string name = m_actors[i]->whoAmI();
                if (name == "ExtraLife" || name == "Health" || name == "Ammunition")
                    return m_actors[i];
            }
        }
    }
    return nullptr;
}


void StudentWorld::setLevelOver()
{
    m_levelOver = true;
}

void StudentWorld::decJewels()
{
    m_numJewels--;
}

int StudentWorld::numJewels()
{
    return m_numJewels;
}

Player* StudentWorld::getPlayer()
{
    return m_player;
}

//changes the level name to a string that can help load the level
string StudentWorld::getLevelName()
{
    string number ="";
    if (getLevel() > 9)
        number = to_string(getLevel());
    if (getLevel() < 10)
        number = "0" + to_string(getLevel());
    return number;
}

//gets actors only damaged by bullets. used by the robot class to determine if it can shoot

Actor* StudentWorld::getActorDamagedByBulletAt(int x, int y, Actor* act)
{
    if (act != m_player && m_player != nullptr)
    {
        if (m_player->isDead() == false && m_player->getX()== x && m_player->getY() == y)
        {
            return m_player;
        }
    }
    for (int i = m_actors.size()-1; i >= 0 ; i--)
    {
        if (m_actors[i]->isDead() == false && m_actors[i] != act)
        {
            string name = m_actors[i]->whoAmI();
            if (m_actors[i]->getX() ==  x && m_actors[i]->getY() == y)
            {
                if (name == "Wall" || name == "VerticalBot" || name == "Boulder" || name == "HorizontalBot" || name == "KleptoBot Factory" || name == "Angry KleptoBot" || name == "KleptoBot")
                        return m_actors[i];
            }
        }
    }
    return nullptr;

}


//gets only actors that block robots. gets rid of bug of multiple robots moving onto a space with goodies/jewels
Actor* StudentWorld::getActorThatBlocksRobotAt(int x, int y, Actor* act)
{
    if (act != m_player && m_player != nullptr)
    {
        if (m_player->isDead() == false && m_player->getX()== x && m_player->getY() == y)
        {
            return m_player;
        }
    }
    for (int i = m_actors.size()-1; i >= 0 ; i--)
    {
        if (m_actors[i]->isDead() == false && m_actors[i] != act)
        {
            if (m_actors[i]->getX() ==  x && m_actors[i]->getY() == y)
            {
                if (m_actors[i]->blocksRobot())
                    return m_actors[i];
            }
        }
    }
    return nullptr;
}


//simply gets the first actor in the array at the position specified by the function

Actor* StudentWorld::getActorAt(int x, int y, Actor* act)
{
    if (act != m_player && m_player != nullptr)
    {
        if (m_player->isDead() == false && m_player->getX()== x && m_player->getY() == y)
        {
            return m_player;
        }
    }
    for (int i = m_actors.size()-1; i >= 0 ; i--)
    {
        if (m_actors[i]->isDead() == false && m_actors[i] != act)
        {
            if (m_actors[i]->getX() ==  x && m_actors[i]->getY() == y)
            {
                return m_actors[i];
            }
        }
    }
    return nullptr;
}

void StudentWorld::addActor(Actor* add)
{
    m_actors.push_back(add);
}



int StudentWorld::init()
{
        Level lev(assetDirectory());
        string level = "level"+getLevelName()+".dat";
        Level::LoadResult result = lev.loadLevel(level);
        if (result == Level::load_fail_file_not_found)
        {
            //ends the game if the next level cannot be found and says that the player won the game
            
            if (getLevel() == 0)
            {
                cerr << "Could not find level00.dat data file\n";
                return GWSTATUS_LEVEL_ERROR;
            }
            else
                return GWSTATUS_PLAYER_WON;
        }
        else if (result == Level::load_fail_bad_format)
        {
            cerr << "Your level was improperly formatted\n";
            return GWSTATUS_LEVEL_ERROR;
        }
        else if (result == Level::load_success)
        {
            cerr << "Successfully loaded level\n";
            
            //adds actors to the correct space in the maze depending on their position in loaded level,
            // and places them in the correct place, and pushes them onto the vector
            
            for(int x = 0; x < 15; x++)
            {
                for (int y = 0; y < 15; y++)
                {
                    cerr << m_actors.size() << endl;
                    Level::MazeEntry ge = lev.getContentsOf(x,y); // x=5, y=10
                    switch (ge)
                    {
                        case Level::empty:
                            break;
                        case Level::player:
                            m_player = new Player (IID_PLAYER, x, y, this);
                            break;
                        case Level::wall:
                            m_actors.push_back(new Wall(IID_WALL,x,y,this));
                            break;
                        case Level::boulder:
                            m_actors.push_back(new Boulder(IID_BOULDER, x, y,this));
                            break;
                        case Level::hole:
                            m_actors.push_back(new Hole(IID_HOLE, x, y, this));
                            break;
                        case Level::jewel:
                            m_actors.push_back(new Jewel(IID_JEWEL, x, y, this));
                            m_numJewels++;
                            break;
                        case Level::extra_life:
                            m_actors.push_back(new ExtraLife(IID_EXTRA_LIFE, x, y,this));
                            break;
                        case Level::restore_health:
                            m_actors.push_back(new Health(IID_RESTORE_HEALTH, x, y,this));
                            break;
                        case Level::ammo:
                            m_actors.push_back(new Ammunition(IID_AMMO, x, y, this));
                            break;
                        case Level::exit:
                            m_actors.push_back(new Exit(IID_EXIT, x, y, this));
                            break;
                        case Level::horiz_snarlbot:
                            m_actors.push_back(new HorizontalBot(IID_SNARLBOT, x, y,getLevel(), this));
                            break;
                        case Level::vert_snarlbot:
                            m_actors.push_back(new VerticalBot(IID_SNARLBOT, x, y, getLevel(), this));
                            break;
                        case Level::angry_kleptobot_factory:
                            m_actors.push_back(new KleptoBotFactory(IID_ROBOT_FACTORY, x, y, "Angry KleptoBot", this));
                            break;
                        case Level::kleptobot_factory:
                            m_actors.push_back(new KleptoBotFactory(IID_ROBOT_FACTORY, x, y, "KleptoBot", this));
                            break;
                        default:
                            break;
                    }
                }
            }
        }

    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    //asks the player to do something
    if (m_player != nullptr && m_player->isDead() == false)
        m_player->doSomething();
    
    //checks to see if the player is dead after its turn
    if (m_player->isDead())
    {
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    
    //checks to see if the level is over after the player has moved
    if (m_levelOver)
    {
        increaseScore(m_bonus);
        return GWSTATUS_FINISHED_LEVEL;
    }
    
    //for each actor in the vector, check to see if the player died, if the player won the level,
    //and then tells the actor to do something if the actor is not dead
    for (int i = 0; i < m_actors.size(); i++)
    {
        if (m_player->isDead())
        {
            decLives();
            return GWSTATUS_PLAYER_DIED;
        }
        if (m_levelOver)
        {
            increaseScore(m_bonus);
            return GWSTATUS_FINISHED_LEVEL;
        }
        if (m_actors[i]->isDead() == false)
            m_actors[i]->doSomething();
    }
    
    //deletes all actors that were set to dead
    for (int j = 0; j < m_actors.size(); j++)
    {
        if (m_actors[j]->isDead())
        {
            delete m_actors[j];
            m_actors.erase(m_actors.begin() + j);
            j--;
        }
    }
    
    if (m_levelOver)
    {
        increaseScore(m_bonus);
        return GWSTATUS_FINISHED_LEVEL;
    }
    if (m_player->isDead())
    {
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    setDisplayText();
    if (m_bonus>0)
        m_bonus--;
    return GWSTATUS_CONTINUE_GAME;
    
}

void StudentWorld::setDisplayText()
{
    int score = getScore();
    int lives = getLives();
    int level = getLevel();
    string header = "";
    ostringstream oss;
    oss<< "Score: ";
    oss.fill('0');
    oss << setw(7) << score;
    oss.fill('0');
    oss <<"  "<< "Level: " << setw(2) << level;
    oss << "  " << "Lives: " << setw(1) << lives;
    oss.fill(' ');
    oss <<"  "<< "Health: " << setw(3) << m_player->getHealth() << "%";
    oss<< "  "<< "Ammo: " << setw(3) << m_player-> getAmmo();
    oss<< "  " << "Bonus: " << setw(4) << m_bonus;
    header = oss.str();
    setGameStatText(header);
    
    
    
}

void StudentWorld::cleanUp()
{
    //deletes all the allocated memories and clears the vector
    delete m_player;
    for (int i = 0; i < m_actors.size(); i++)
    {
        delete m_actors[i];
    }
    m_actors.clear();
    m_levelOver = false;
}

StudentWorld::~StudentWorld()
{
    // does the same thing as cleanUp
    delete m_player;
    for (int i = 0; i < m_actors.size(); i++)
    {
        delete m_actors[i];
    }
    m_actors.clear();
}

