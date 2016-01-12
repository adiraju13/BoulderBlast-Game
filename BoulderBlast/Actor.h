#ifndef ACTOR_H_
#define ACTOR_H_
#include "GraphObject.h"
#include <string>

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor: public GraphObject
{
public:
    Actor(int imageID, int startX, int startY, Direction dir, std::string name, StudentWorld* world);
    virtual bool isDead() const;
    virtual void setDead();
    StudentWorld* getWorld() const;
    virtual ~Actor();
    virtual void doSomething() = 0;
    virtual bool blocksPlayer(Direction dir)= 0;
    virtual bool damagedByBullet() = 0;
    virtual bool blocksBoulder() const;
    virtual bool blocksRobot() const;
    std::string whoAmI() const;
    void fireBullet();
    
    
private:
    bool m_isDead;
    StudentWorld* m_world;
    std::string m_name;

};

class Player: public Actor
{
public:
    Player(int imageID, int startX, int startY, StudentWorld* world);
    virtual ~Player();
    void decreaseHitPoints();
    void decreaseAmmunition();
    const int getAmmunition();
    void setAmmunition();
    void setHitPoints();
    int getAmmo() const;
    virtual bool blocksPlayer(Direction dir);
    virtual void doSomething();
    int getHealth() const;
    bool damagedByBullet();

private:
    int m_hitPoints;
    int m_ammunition;
};

class Wall: public Actor
{
public:
    Wall(int imageID, int startX, int startY,  StudentWorld* world);
    virtual void doSomething();
    virtual bool blocksPlayer(Direction dir);
    bool damagedByBullet ();
    virtual ~Wall();
};


class Bullet: public Actor
{
public:
    Bullet(int imageID, int startX, int startY, Direction dir, StudentWorld* world);
    virtual void doSomething();
    virtual bool blocksPlayer(Direction dir);
    bool damagedByBullet();
    bool blocksRobot() const;
    virtual ~Bullet();
    
};

class Boulder: public Actor
{
public:
    Boulder(int imageID, int startX, int startY, StudentWorld* world);
    virtual void doSomething();
    virtual bool blocksPlayer(Direction dir);
    void push(Direction dir);
    bool damagedByBullet();
    virtual ~Boulder();

    
private:
    int m_health;
    
};

class Hole: public Actor
{
public:
    Hole(int imageID, int startX, int startY, StudentWorld* world);
    virtual void doSomething();
    virtual bool blocksPlayer(Direction dir);
    void push(Direction dir);
    virtual bool blocksBoulder() const;
    bool damagedByBullet();
    virtual ~Hole();
    
private:
    
};

class Goodie: public Actor
{
public:
    Goodie(int imageID, int startX, int startY, std::string name, StudentWorld* world);
    virtual bool blocksPlayer(Direction dir);
    virtual bool damagedByBullet();
    virtual bool isPlayerOnTopOfMe();
    virtual bool blocksRobot() const;
    virtual ~Goodie();
    
};


class Jewel: public Goodie
{
public:
    Jewel(int imageID, int startX, int startY, StudentWorld* world);
    virtual void doSomething();
    virtual ~Jewel();
};

class ExtraLife: public Goodie
{
public:
    ExtraLife(int imageID, int startX, int startY, StudentWorld* world);
    virtual void doSomething();
    virtual ~ExtraLife();
};

class Health: public Goodie
{
public:
    Health(int imageID, int startX, int startY, StudentWorld* world);
    virtual void doSomething();
    virtual ~Health();
};


class Ammunition: public Goodie
{
public:
    Ammunition(int imageID, int startX, int startY, StudentWorld* world);
    virtual void doSomething();
    virtual ~Ammunition();
};

class Exit: public Goodie
{
public:
    Exit(int imageID, int startX, int startY, StudentWorld* world);
    virtual void doSomething();
    virtual bool blocksBoulder() const;
    virtual ~Exit();
private:
    bool m_playedSound;
};

class Bot: public Actor
{
public:
    Bot(int imageID, int startX, int startY, Direction dir, int health, std::string name, int level, StudentWorld* world);
    virtual bool blocksPlayer(Direction dir);
    virtual bool damagedByBullet();
    virtual bool canDoSomething();
    virtual bool isBlocked(Direction dir);
    virtual bool canShoot(Direction dir);
    virtual void addScore() = 0;
    virtual ~Bot();
private:
    int m_ticksMax;
    int m_health;
    int m_tickCounter;
};

class SnarlBot: public Bot
{
public:
    SnarlBot(int imageID, int startX, int startY, Direction dir, std::string name, int level, StudentWorld* world);
    virtual void switchDirection(Direction dir);
    virtual void addScore();
    virtual ~SnarlBot();
};

class HorizontalBot: public SnarlBot
{
public:
    HorizontalBot(int imageID, int startX, int startY, int level, StudentWorld* world);
    virtual void doSomething();
    virtual ~HorizontalBot();
};

class VerticalBot: public SnarlBot
{
public:
    VerticalBot(int imageID, int startX, int startY, int level, StudentWorld* world);
    virtual void doSomething();
    virtual ~VerticalBot();
};

class KleptoBotFactory: public Actor
{
public:
    KleptoBotFactory(int imageID, int startX, int startY, std::string whatToProduce, StudentWorld *world);
    virtual void doSomething();
    virtual bool blocksPlayer(Direction dir);
    virtual bool damagedByBullet();
    virtual int canIProduce();
    virtual ~KleptoBotFactory();
private:
    std::string m_produce;
    bool produced;
};

class KleptoBot: public Bot
{
public:
    KleptoBot(int imageID, int startX, int startY, int level, int healthOfBot,std::string name, StudentWorld* world);
    virtual void addScore();
    virtual void doSomething();
    virtual void setDead();
    virtual ~KleptoBot();
private:
    void determineDirection(Direction dir);
    void changeDirection();
    int m_health;
    bool m_hasPickedUpItem;
    int m_distanceToMove;
    std::string whatAmIHolding;
    
};

class AngryKleptoBot: public KleptoBot
{
public:
    AngryKleptoBot(int imageID, int startX, int startY, int level,int healthOfBot, std::string name, StudentWorld* world);
    virtual void addScore();
    virtual ~AngryKleptoBot();
};


#endif // ACTOR_H_
