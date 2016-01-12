#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
class Actor;
class Player;
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);

    virtual int init();

    virtual int move();

    virtual void cleanUp();
    
    Player* getPlayer();
    
    Actor* getActorAt(int x, int y, Actor* actor);
    
    Actor* getActorDamagedByBulletAt(int x, int y, Actor* actor);
    
    Actor* getActorThatBlocksRobotAt(int x, int y, Actor* act);
    
    Actor* getActorThatBlocksPlayerAt(int x, int y, Actor* act);
    
    Actor* getGoodieAt(int x, int y, Actor* act);
    
    void addActor(Actor* add);
    
    virtual ~StudentWorld();
    
    void decJewels();
    int numJewels();
    void setLevelOver();

private:
    void setDisplayText();
    std::string getLevelName();
    std::vector<Actor*> m_actors;
    Player* m_player;
    int m_bonus;
    int m_numJewels;
    bool m_levelOver;
};

#endif // STUDENTWORLD_H_
