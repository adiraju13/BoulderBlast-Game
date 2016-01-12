#include "Actor.h"
#include "StudentWorld.h"
#include <string>
#include <iostream>
using namespace std;
// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp


///////////////////////////////////////
///////ACTOR///////////////////////////
///////////////////////////////////////


Actor::Actor(int imageID, int startX, int startY, Direction dir,string name, StudentWorld* world)
:GraphObject(imageID, startX, startY, dir)
{
    m_isDead = false;
    m_world = world;
    m_name = name;
}
bool Actor::blocksRobot() const
{
    return true;
}

Actor::~Actor()
{
    
}
string Actor::whoAmI() const
{
    return m_name;
}
bool Actor::blocksBoulder() const
{
    return true;
}

void Actor::fireBullet()
{
    //create bullets and set them in the correct direction depending on the actor who fired it
    Actor* nBullet;
    switch (getDirection())
    {
        case right:
            nBullet = new Bullet (IID_BULLET, getX()+1, getY(), getDirection(), getWorld());
            nBullet -> setVisible(true);
            break;
        case left:
             nBullet = new Bullet (IID_BULLET, getX()-1, getY(), getDirection(), getWorld());
            nBullet -> setVisible(true);
            break;
        case up:
             nBullet = new Bullet (IID_BULLET, getX(), getY()+1, getDirection(), getWorld());
            nBullet -> setVisible(true);
            break;
        case down:
            nBullet = new Bullet (IID_BULLET, getX(), getY()-1, getDirection(), getWorld());
            nBullet -> setVisible(true);
            break;
        default:
            nBullet = nullptr;
    }
    if (whoAmI() == "Player")
        getWorld()->playSound(SOUND_PLAYER_FIRE);
    getWorld()->addActor(nBullet);
}


 
bool Actor::isDead() const
{
    return m_isDead;
}

void Actor::setDead()
{
    setVisible(false);
    m_isDead = true;
}

StudentWorld* Actor::getWorld() const
{
    return m_world;
}



///////////////////////////////////////////////
///////////////PLAYER//////////////////////////
///////////////////////////////////////////////





void Player::doSomething()
{
    
    //For each direction key pressed, check if there is something there, and if there is whether it blocks the player
    //Use the world's function to see if there is something blocking the player. If nothing blocks the player
    //change the position of the player depending on what key was pressed.
    
    if (isDead() == false)
    {
        int ch;
        if (getWorld()->getKey(ch))
        {
            if (ch == KEY_PRESS_LEFT)
            {
                setDirection(left);
                if (getWorld()->getActorThatBlocksPlayerAt(getX()-1, getY(), this) == nullptr)
                {
                    moveTo(getX()-1, getY());
                }
            }
            else if (ch == KEY_PRESS_RIGHT)
            {
                setDirection(right);
                if (getWorld()->getActorThatBlocksPlayerAt(getX() + 1, getY(), this) == nullptr)
                {
                    moveTo(getX()+1, getY());
                }
            }
            else if (ch == KEY_PRESS_UP)
            {
                
                setDirection(up);
                if (getWorld()->getActorThatBlocksPlayerAt(getX(), getY() + 1, this) == nullptr)
                {
                    moveTo(getX(), getY() + 1);
                }
            }
            else if (ch == KEY_PRESS_DOWN)
            {
                setDirection(down);
                if (getWorld()->getActorThatBlocksPlayerAt(getX(), getY() - 1, this) == nullptr)
                {
                    moveTo(getX(), getY() - 1);
                }
            }
            else if (ch == KEY_PRESS_ESCAPE)
            {
                setDead();
            }
            else if (ch == KEY_PRESS_SPACE)
            {
                if (m_ammunition > 0)
                {
                    fireBullet();
                    m_ammunition--;
                }
            }
        }
    }
}

Player::Player(int imageID, int startX, int startY, StudentWorld* world)
: Actor(imageID, startX, startY, right, "Player", world)
{
    setVisible(true);
    m_ammunition = 20;
    m_hitPoints = 20;

}

bool Player::blocksPlayer(Direction dir)
{
    return true;
}

//when the player's damaged by bullet's class is called, the player
//loses 2 health and plays the correct sound
bool Player::damagedByBullet()
{
    m_hitPoints -= 2;
    getWorld()->playSound(SOUND_PLAYER_IMPACT);
    if (m_hitPoints <= 0)
    {
        getWorld()->playSound(SOUND_PLAYER_DIE);
        setDead();
    }
    return true;
}

int Player::getHealth() const
{
    return m_hitPoints * 5;
}

int Player::getAmmo() const
{
    return m_ammunition;
}

void Player::setAmmunition()
{
    m_ammunition +=20;
}

void Player::setHitPoints()
{
    m_hitPoints = 20;
}

Player::~Player()
{
    
}



//////////////////////////////////////////////////////
//////////////////WALL////////////////////////////////
//////////////////////////////////////////////////////





bool Wall::damagedByBullet()
{
    return true;
}

Wall::Wall(int imageID, int startX, int startY, StudentWorld* world)
:Actor(imageID, startX, startY, none, "Wall", world )
{
    setVisible(true);
}

void Wall::doSomething()
{
    
}


bool Wall::blocksPlayer(Direction dir)
{
    return true;
}


Wall::~Wall()
{
    
}

//////////////////////////////////////////////////////
//////////////////BULLET//////////////////////////////
//////////////////////////////////////////////////////




Bullet::Bullet(int imageID, int startX, int startY, Direction dir, StudentWorld* world)
:Actor(imageID, startX, startY, dir, "Bullet", world )
{
    setVisible(true);
}

Bullet::~Bullet()
{
    
}

bool Bullet::blocksRobot() const
{
    return false;
}

void Bullet::doSomething()
{
    //checks to see if the bullet is on the same space as an object that can be damaged by a bullet or an object
    //that a bullet is destroyed when it hits, and if it is, calls that objects damagedByBullet function
    //and sets itself to dead
    if (getWorld()->getActorDamagedByBulletAt(getX(), getY(), this) != nullptr && getWorld()->getActorDamagedByBulletAt(getX(), getY(), this)->damagedByBullet())
    {
        setDead();
        return;
    }
    if (isDead() == false)
    {
        Direction dir = getDirection();
        if (dir == right)
        {
            moveTo(getX() + 1, getY());
        }
        else if (dir == left)
        {
            moveTo(getX() - 1, getY());
        }
        else if (dir == up)
        {
            moveTo(getX(), getY() + 1);
        }
        else if (dir == down)
        {
            moveTo(getX(), getY() - 1);
        }
    }
    
    //Does the same thing as the comment at the beginning of Bullet's doSomething, but is repeated after the bullet
    //moves to see if the bullet is on an object after the bullet moves
    
    if (getWorld()->getActorDamagedByBulletAt(getX(), getY(), this) != nullptr && getWorld()->getActorDamagedByBulletAt(getX(), getY(), this)->damagedByBullet())
    {
        setDead();
        return;
    }
}

bool Bullet::damagedByBullet()
{
    return false;
}

bool Bullet::blocksPlayer(Direction dir)
{
    return false;
}


//////////////////////////////////
///////////////////BOULDER////////
//////////////////////////////////

Boulder::Boulder(int imageID, int startX, int startY, StudentWorld* world)
:Actor(imageID, startX, startY, none, "Boulder", world)
{
    m_health = 10;
    setVisible(true);
}
Boulder::~Boulder()
{
    
}

void Boulder::doSomething()
{
    
}
void Boulder::push(Direction dir)
{
    //depending on what direction is passed,  move the boulder in that direction
    
    switch(dir)
    {
        case up:
            moveTo(getX(), getY()+1);
            break;
        case down:
            moveTo(getX(), getY()-1);
            break;
        case left:
            moveTo(getX()-1, getY());
            break;
        case right:
            moveTo(getX()+1, getY());
            break;
        default:
            break;
    }
}
bool Boulder::blocksPlayer(Direction dir)
{

    //checks to see if there is an object in front of a boulder, and if there is, whether that object blocks a boulder
    //if the object does not block the boulder, or if there is no object, then call the Boulder's push function
    
    switch (dir)
    {
        case up:
            if (getWorld()->getActorAt(getX(), getY()+1, this) == nullptr) {
                push(dir);
                return false;
            }
            else if (!getWorld()->getActorAt(getX(), getY()+1, this)->blocksBoulder()) {
                push(dir);
                getWorld()->getActorAt(getX(), getY(), this)->setDead();
                setDead();
                return false;
            }
            break;
        case down:
            if (getWorld()->getActorAt(getX(), getY()-1, this) == nullptr) {
                push(dir);
                return false;
            }
            else if (!getWorld()->getActorAt(getX(), getY()-1, this)->blocksBoulder()) {
                push (dir);
                getWorld()->getActorAt(getX(), getY(), this)->setDead();
                setDead();
                return false;
            }
            break;
        case left:
            if (getWorld()->getActorAt(getX()-1, getY(), this) == nullptr) {
                push(dir);
                return false;
            }
            else if (!getWorld()->getActorAt(getX()-1, getY(), this)->blocksBoulder()) {
                push(dir);
                getWorld()->getActorAt(getX(), getY(), this)->setDead();
                setDead();
                return false;
            }
            break;
        case right:
            if (getWorld()->getActorAt(getX()+1, getY(), this) == nullptr) {
                push(dir);
                return false;
            }
            else if (!getWorld()->getActorAt(getX()+1, getY(), this)->blocksBoulder()) {
                push(dir);
                getWorld()->getActorAt(getX(), getY(), this)->setDead();
                setDead();
                return false;
            }
            break;
        default:
            return true;
    }

    return true;
}

//when the Boulder's damaged by bullet's class is called, the boulder
//loses 2 health
//if the boulder's health is less than or equal to 0, then set the boulder to dead
bool Boulder::damagedByBullet()
{
    m_health -= 2;
    if (m_health <= 0)
    {
        setDead();
    }
    return true;
}

/////////////////////////////////////////
///////////////////HOLE//////////////////
/////////////////////////////////////////


Hole::Hole(int imageID, int startX, int startY, StudentWorld* world)
:Actor(imageID, startX, startY, none, "Hole", world)
{
    setVisible(true);
}
Hole::~Hole()
{
    
}

void Hole::doSomething()
{
    
    
}
bool Hole::blocksPlayer(Direction dir)
{
    return true;
}
bool Hole::damagedByBullet()
{
    return false;
}
bool Hole::blocksBoulder() const
{
    return false;
}

/////////////////////////////////////////////////////////////////
////////////////////GOODIE///////////////////////////////////////
/////////////////////////////////////////////////////////////////
Goodie::Goodie(int imageID, int startX, int startY, std::string name, StudentWorld* world)
:Actor(imageID, startX, startY, none, name, world)
{
    
}

Goodie::~Goodie()
{
    
}

bool Goodie::blocksRobot() const
{
    return false;
}

bool Goodie::isPlayerOnTopOfMe()
{

    //checks to see if the player is alive, and if it is, if its on top of the goodie
    
    if (getWorld()->getPlayer() == nullptr)
        return false;
    if (getWorld()->getPlayer()->isDead())
        return false;
    if (getX()==getWorld()->getPlayer()->getX() && getY()==getWorld()->getPlayer()->getY())
        return true;
    return false;
}

bool Goodie::blocksPlayer(Direction dir)
{
    return false;
}

bool Goodie::damagedByBullet()
{
    return false;
}




/////////////////////////////////////////////////////////////////
////////////////////JEWEL////////////////////////////////////////
/////////////////////////////////////////////////////////////////

Jewel::Jewel(int imageID, int startX, int startY, StudentWorld* world)
:Goodie(imageID, startX, startY, "Jewel", world)
{
    setVisible(true);
}
Jewel::~Jewel()
{
    
}
void Jewel::doSomething()
{
    //if the player is on top of the jewel, make the player pick it up and update count of jewels in the game
    
    if (isDead())
        return;
    if (isPlayerOnTopOfMe())
    {
        setDead();
        getWorld()->increaseScore(50);
        getWorld()->decJewels();
        getWorld()->playSound(SOUND_GOT_GOODIE);
    }
}


/////////////////////////////////////////////////////////////////
////////////////////ExtraLife////////////////////////////////////
/////////////////////////////////////////////////////////////////

ExtraLife::ExtraLife(int imageID, int startX, int startY, StudentWorld* world)
:Goodie(imageID, startX, startY, "ExtraLife", world)
{
    setVisible(true);
}
ExtraLife::~ExtraLife()
{
    
}
void ExtraLife::doSomething()
{
    
    //if the player is on top of the ExtraLife goodie, pick it up and update the player's number of lifes through
    //the student world's incLives function
    
    if (isDead())
        return;
    if (isPlayerOnTopOfMe())
    {
        setDead();
        getWorld()->increaseScore(1000);
        getWorld()->incLives();
        getWorld()->playSound(SOUND_GOT_GOODIE);
    }
}




/////////////////////////////////////////////////////////////////
////////////////////HEALTH///////////////////////////////////////
/////////////////////////////////////////////////////////////////
Health::Health(int imageID, int startX, int startY, StudentWorld* world)
:Goodie(imageID, startX, startY, "Health", world)
{
    setVisible(true);
}
Health::~Health()
{
    
}
void Health::doSomething()
{
    //if the player is on top of health goodie, update the health of the player accordingly
    
    if (isDead())
        return;
    if (isPlayerOnTopOfMe())
    {
        setDead();
        getWorld()->increaseScore(500);
        getWorld()->getPlayer()->setHitPoints();
        getWorld()->playSound(SOUND_GOT_GOODIE);
    }
}





/////////////////////////////////////////////////////////////////
////////////////////AMMO/////////////////////////////////////////
/////////////////////////////////////////////////////////////////

Ammunition::Ammunition(int imageID, int startX, int startY, StudentWorld* world)
:Goodie(imageID, startX, startY, "Ammunition", world)
{
    setVisible(true);
}

Ammunition::~Ammunition()
{
    
}

void Ammunition::doSomething()
{
    
     //if the player is on top of Ammo goodie, update the Ammo of the player accordingly
    
    if (isDead())
        return;
    if (isPlayerOnTopOfMe())
    {
        setDead();
        getWorld()->increaseScore(100);
        getWorld()->getPlayer()->setAmmunition();
        getWorld()->playSound(SOUND_GOT_GOODIE);
    }
}


/////////////////////////////////////////////////////////////////
////////////////////EXIT/////////////////////////////////////////
/////////////////////////////////////////////////////////////////
Exit::Exit(int imageID, int startX, int startY, StudentWorld* world)
:Goodie(imageID, startX, startY, "Exit", world)
{
    setVisible(false);
    m_playedSound = false;
}

Exit::~Exit()
{
    
}

bool Exit::blocksBoulder() const
{
    return false;
}

void Exit::doSomething()
{
    
    if (isDead())
        return;
    
    //every tick check if the number of jewels is equal to zero and if the exit had already been
    //shown. If the exit had not already been revealed and number of jewels is equal to 0,
    //set the exit to visible and play proper sound
    
    if (!m_playedSound && getWorld()->numJewels() == 0)
    {
        setVisible(true);
        getWorld()->playSound(SOUND_REVEAL_EXIT);
        m_playedSound = true;
    }
    
    //if the player is on top of the jewel and if the exit is active (used playedSound == true and synonymous to
    //the exit being activated), set the level to over
    
    if (isPlayerOnTopOfMe() && m_playedSound)
    {
        setDead();
        getWorld()->increaseScore(2000);
        getWorld()->setLevelOver();
        getWorld()->playSound(SOUND_FINISHED_LEVEL);
    }
}

/////////////////////////////////////////////////////////////////
////////////////////BOT//////////////////////////////////////////
/////////////////////////////////////////////////////////////////

Bot::Bot(int imageID, int startX, int startY, Direction dir, int health, string name, int level, StudentWorld* world)
:Actor(imageID, startX, startY, dir, name, world)
{
    setVisible(true);
    m_ticksMax = (28 - level)/4;
    m_tickCounter = 1;
    m_health = health;
}

bool Bot::blocksPlayer(Direction dir)
{
    return true;
}

bool Bot::damagedByBullet()
{
    m_health -= 2;
    getWorld()->playSound(SOUND_ROBOT_IMPACT);
    if (m_health <= 0)
    {
        getWorld()->playSound(SOUND_ROBOT_DIE);
        addScore();
        setDead();
    }
    return true;
}

bool Bot::canDoSomething()
{

    //checks if a robot can do something in its do something method by comparing the number of ticks its waited to
    //the number of ticks it has to wait before it can do something
    //if the number of ticks it has waited equals the number of ticks it has to wait, tell the bot it can do something,
    //and set the tick counter back to 1. Otherwise, increment the number of ticks it has waited by 1
    
    if (m_tickCounter == m_ticksMax)
    {
        m_tickCounter = 1;
        return true;
    }
    else
    {
        m_tickCounter++;
        return false;
    }
}


bool Bot::isBlocked(Direction dir)
{
    //checks to see if there is an object in front of a robot, and if there is, whether that object blocks a robot
    //if the object does not block the boulder, or if there is no object return true. else return false
    
    switch (dir)
    {
        case up:
            if (getWorld()->getActorThatBlocksRobotAt(getX(), getY()+1, this) == nullptr)
            {
                return false;
            }
            else
                return true;
            break;
        case down:
            if (getWorld()->getActorThatBlocksRobotAt(getX(), getY()-1, this) == nullptr)
            {
                return false;
            }
            else
                return true;
            break;
        case left:
            if (getWorld()->getActorThatBlocksRobotAt(getX()-1, getY(), this) == nullptr)
            {
                return false;
            }
            else
                return true;
            break;
        case right:
            if (getWorld()->getActorThatBlocksRobotAt(getX()+1, getY(), this) == nullptr)
            {
                return false;
            }
            else
                return true;
        default:
            return true;
    }
    
    return true;
}

bool Bot::canShoot(Direction dir)
{
    //depending on the direction, check if the first closest object in the direction of the robot can be
    // damaged by a bullet. if there is no such object, return false, that the robot can't shoot
    // If there is an object but it is not the player, also return false.
    // If it is the player, return true, that the player can shoot
    
    if (dir == up)
    {
        for (int y = getY(); y < 15; y++)
        {
            Actor* temp = getWorld()->getActorDamagedByBulletAt(getX(), y, this);
            if (temp != nullptr)
            {
                if (temp->isDead() == false)
                {
                    if (temp->whoAmI() == "Player")
                        return true;
                    else
                        return false;
                }
            }
            
        }
    }
    else if (dir == down)
    {
        for (int y = getY(); y >= 0; y--)
        {
            Actor* temp = getWorld()->getActorDamagedByBulletAt(getX(), y, this);
            if (temp != nullptr)
            {
                if (temp->isDead() == false)
                {
                    if (temp->whoAmI() == "Player")
                        return true;
                    else
                        return false;
                }
            }
            
        }
        
    }
    else if (dir == left)
    {
        for (int x = getX(); x >= 0; x--)
        {
            Actor* temp = getWorld()->getActorDamagedByBulletAt(x, getY(), this);
            if (temp != nullptr)
            {
                if (temp->isDead() == false)
                {
                    if (temp->whoAmI() == "Player")
                    {
                        return true;
                    }
                    else
                        return false;
                }
            }
        }
    }
    else if (dir == right)
    {
        for (int x = getX(); x < 15; x++)
        {
            Actor* temp = getWorld()->getActorDamagedByBulletAt(x, getY(), this);
            if (temp != nullptr)
            {
                if (temp->isDead() == false)
                {
                    if (temp->whoAmI() == "Player")
                        return true;
                    else
                        return false;
                }
            }
        }
    }
    return false;
}



Bot::~Bot()
{
    
}


/////////////////////////////////////////////////////////////////
////////////////////SnarlBot/////////////////////////////////////
/////////////////////////////////////////////////////////////////

SnarlBot::SnarlBot(int imageID, int startX, int startY, Direction dir, string name, int level, StudentWorld* world)
:Bot(imageID, startX, startY, dir, 10 , name, level, world)
{
    
}


SnarlBot::~SnarlBot()
{
    
}

void SnarlBot::switchDirection(Direction dir)
{
    //changes the direction to the opposite direction of what the snarlbot currently faces
    
    switch(dir)
    {
        case up:
            setDirection(down);
            break;
        case down:
            setDirection(up);
            break;
        case left:
            setDirection(right);
            break;
        case right:
            setDirection(left);
            break;
        default:
            return;
        
        
    }
}

void SnarlBot::addScore()
{
    getWorld()->increaseScore(100);
}

/////////////////////////////////////////////////////////////////
////////////////////Horizontal //////////////////////////////////
////////////////////SnarlBot/////////////////////////////////////
/////////////////////////////////////////////////////////////////

HorizontalBot::HorizontalBot(int imageID, int startX, int startY, int level, StudentWorld* world)
:SnarlBot(imageID, startX, startY, right, "HorizontalBot", level, world)
{
    
}

void HorizontalBot::doSomething()
{
    
    // if the robot can do something depending on the tick, see if it can shoot. if it can shoot, shoot and do nothing else
    // if it can't shoot, check if it can move. if it can, move either left or right. if it cant move, change its direction
    // approprietly
    
    if (isDead())
        return;
    if (canDoSomething() == false)
        return;
    if (canShoot(getDirection()))
    {
        fireBullet();
        getWorld()->playSound(SOUND_ENEMY_FIRE);
        return;
    }
    if (isBlocked(getDirection()))
    {
        switchDirection(getDirection());
    }
    else
    {
        if (getDirection() == right)
        {
            moveTo(getX()+1, getY());
        }
        if (getDirection() == left)
        {
            moveTo(getX()-1, getY());
        }
    }
}

HorizontalBot::~HorizontalBot()
{
    
}

/////////////////////////////////////////////////////////////////
////////////////////Vertical ////////////////////////////////////
////////////////////SnarlBot/////////////////////////////////////
/////////////////////////////////////////////////////////////////


VerticalBot::VerticalBot(int imageID, int startX, int startY, int level, StudentWorld* world)
:SnarlBot(imageID, startX, startY, up, "VerticalBot", level, world)
{
    
}

void VerticalBot::doSomething()
{
    
    // if the robot can do something depending on the tick, see if it can shoot. if it can shoot, shoot and do nothing else
    // if it can't shoot, check if it can move. if it can, move either up or down. if it cant move, change its direction
    // approprietly
    
    if (isDead())
        return;
    if (canDoSomething() == false)
        return;
    if (canShoot(getDirection()))
    {
        fireBullet();
        getWorld()->playSound(SOUND_ENEMY_FIRE);
        return;
    }
    if (isBlocked(getDirection()))
    {
        switchDirection(getDirection());
    }
    else
    {
        if (getDirection() == up)
        {
            moveTo(getX(), getY()+1);
        }
        if (getDirection() == down)
        {
            moveTo(getX(), getY()-1);
        }
    }
}

VerticalBot::~VerticalBot()
{
    
}

/////////////////////////////////////////////////////////////////
////////////////////KleptoBot ///////////////////////////////////
////////////////////Factory//////////////////////////////////////
/////////////////////////////////////////////////////////////////

KleptoBotFactory::KleptoBotFactory(int imageID, int startX,  int startY, string whatToProduce, StudentWorld* world)
:Actor(imageID, startX, startY, none, "KleptoBot Factory", world)
{
    setVisible(true);
    m_produce = whatToProduce;
    produced = false;
}

int KleptoBotFactory::canIProduce()
{
    //count number of kleptobots within the 3 by 3 box centered around the factory by using nested for loops
    //use getActorDamagedByBullet, because it returns the correct subset of actors that doesn't check for
    //goodies. This takes care of the problem of if there is robot on goodie at that point in time
    
    int count = 0;
    for (int x = getX() - 3; x <= getX() + 3; x++)
    {
        if (x >= 0 && x < 15)
        {
            for (int y = getY() - 3; y <= getY()+3; y++)
            {
                if (y >= 0 && y < 15)
                {
                    Actor* temp = getWorld()->getActorDamagedByBulletAt(x, y, this);
                    if (temp != nullptr && (temp->whoAmI() == "KleptoBot" || temp->whoAmI() == "Angry KleptoBot"))
                        count++;
                }
            
            }
        }
    }
    return count;
}

void KleptoBotFactory::doSomething()
{
    //if the factory  has less than 3 kleptbots around it, and there is not kleptobot on top of it, create a robot if
    //the random number generated between 1-50 equals 1 (1 in 50 chance the robot is created)
    //Create the robot depending on whether the factory was told to create kleptbots or angry kleptobots
    //when the factory was constructed
    
    if (canIProduce() < 3)
    {
        Actor* temp = getWorld()->getActorDamagedByBulletAt(getX(), getY(), this);
        if (temp != nullptr && (temp->whoAmI() == "KleptoBot" || temp->whoAmI() == "Angry KleptoBot"))
            return;
        int random = rand()%50 + 1;
        if (random == 1)
        {
            getWorld()->playSound(SOUND_ROBOT_BORN);
            if(m_produce == "KleptoBot")
                getWorld()->addActor(new KleptoBot(IID_KLEPTOBOT, getX(), getY(), getWorld()->getLevel(),5,"KleptoBot", getWorld()));
            else if (m_produce == "Angry KleptoBot")
            {
                getWorld()->addActor(new AngryKleptoBot(IID_KLEPTOBOT, getX(), getY(), getWorld()->getLevel(),8,"Angry KleptoBot", getWorld()));
            }
            produced = true;
        }
    }
}

bool KleptoBotFactory::blocksPlayer(Direction dir)
{
    return true;
}

bool KleptoBotFactory::damagedByBullet()
{
    return true;
}

KleptoBotFactory::~KleptoBotFactory()
{
    
}

/////////////////////////////////////////////////////////////////
////////////////////KleptoBot ///////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

KleptoBot::KleptoBot(int imageID, int x, int y, int level, int healthOfBot, string name, StudentWorld* world)
:Bot(imageID, x, y, right, healthOfBot ,name, level, world)
{
    setVisible(true);
    m_hasPickedUpItem = false;
    m_distanceToMove = rand() % 6 + 1;
    whatAmIHolding = "";
}

void KleptoBot::doSomething()
{
    // check to see if it can do something depending on the tick number. if it can, and the kleptobot is a angry
    // kleptobot, shoot if it can. If it can't shoot, check if its on top of a goodie, and if the random number
    // genereated between 1 and 10 equals 1. If these are satisfied, pick up the goodie, set the goodie to dead,
    // and tell the kleptobot to remember what goodie it picked up.
    
    if (isDead())
        return;
    if (canDoSomething() == false)
        return;
    if (whoAmI() == "Angry KleptoBot")
    {
        if (canShoot(getDirection()))
        {
            fireBullet();
            getWorld()->playSound(SOUND_ENEMY_FIRE);
            return;
        }
    }
    Actor* temp = getWorld()->getGoodieAt(getX(), getY(), this);
    if (temp != nullptr)
    {
        string whatIsIt = temp->whoAmI();
        if (m_hasPickedUpItem == false && (whatIsIt == "Health" || whatIsIt == "ExtraLife" || whatIsIt == "Ammunition"))
        {
            int random = rand()%10+1;
            if (random == 1)
            {
                getWorld()->playSound(SOUND_ROBOT_MUNCH);
                m_hasPickedUpItem = true;
                whatAmIHolding = whatIsIt;
                temp->setDead();
            }
        }
    }
    
    //if the robot hasn't moved the distance randomly genreated and it isnt blocked, the robot moves in the direction
    //it is currently facing. If not, find a new random distance to move, change the direction of the robot, and move in
    //that direction if it can
    
    if (m_distanceToMove > 0 && !isBlocked(getDirection()))
    {
        m_distanceToMove--;
        switch (getDirection())
        {
            case up:
                moveTo(getX(), getY()+1);
                break;
            case down:
                moveTo(getX(), getY() - 1);
                break;
            case left:
                moveTo(getX()-1, getY());
                break;
            case right:
                moveTo(getX()+1, getY());
                break;
            default:
                break;
        }
    }
    else
    {
        m_distanceToMove = rand()%6 +1;
        changeDirection();
        if (!isBlocked(getDirection()))
        {
            m_distanceToMove--;
            switch (getDirection())
            {
                case up:
                    moveTo(getX(), getY()+1);
                    break;
                case down:
                    moveTo(getX(), getY() - 1);
                    break;
                case left:
                    moveTo(getX()-1, getY());
                    break;
                case right:
                    moveTo(getX()+1, getY());
                    break;
                default:
                    break;
            }
        }

    }

}

void KleptoBot::changeDirection()
{
    // set the direction to a random direction depending on the random number generated
    int randomDirection = rand()%4 + 1;
    switch (randomDirection)
    {
        case 1:
            determineDirection(up);
            break;
        case 2:
            determineDirection(down);
            break;
        case 3:
            determineDirection(right);
            break;
        case 4:
            determineDirection(left);
            break;
        default:
            cerr<<"random number generation doesn't work" << endl;
            break;
        
    }
}

void KleptoBot::determineDirection(Direction dir)
{
    
    //check if the random direciton generated is blocked. if its not, set the direction to that direction
    //if it is blocked in that direction, but not in another, set to one of the other directions.
    //if all the directions are blocked, set back to the originally determined random direction
    Direction curr = dir;
    if (dir == up)
    {
        if (!isBlocked(up))
        {
            setDirection(up);
        }
        else if (!isBlocked(down))
        {
            setDirection(down);
        }
        else if (!isBlocked(left))
        {
            setDirection(left);
        }
        else if (!isBlocked(right))
        {
            setDirection(right);
        }
        else
            setDirection(curr);
        return;
    }
    else if (dir == down)
    {
        if (!isBlocked(down))
        {
            setDirection(down);
        }
        else if (!isBlocked(up))
        {
            setDirection(up);
        }
        else if (!isBlocked(left))
        {
            setDirection(left);
        }
        else if (!isBlocked(right))
        {
            setDirection(right);
        }
        else
            setDirection(curr);
        return;
    }
    else if (dir == right)
    {
        if (!isBlocked(right))
        {
            setDirection(right);
        }
        else if (!isBlocked(up))
        {
            setDirection(up);
        }
        else if (!isBlocked(left))
        {
            setDirection(left);
        }
        else if (!isBlocked(down))
        {
            setDirection(down);
        }
        else
            setDirection(curr);
        return;

    }
    else if (dir == left)
    {
        if (!isBlocked(left))
        {
            setDirection(left);
        }
        else if (!isBlocked(up))
        {
            setDirection(up);
        }
        else if (!isBlocked(right))
        {
            setDirection(right);
        }
        else if (!isBlocked(down))
        {
            setDirection(down);
        }
        else
            setDirection(curr);
        return;

    }
    else
    {
        cerr << "determineDirection function did not work" << endl;
    }

}

void KleptoBot::setDead()
{
    
// when the robot dies, create a new object where the kleptobot died depending on what was picked up
    
    if(whatAmIHolding == "ExtraLife")
        getWorld()->addActor(new ExtraLife(IID_EXTRA_LIFE, getX(), getY(), getWorld()));
    else if(whatAmIHolding == "Ammunition")
        getWorld()->addActor(new Ammunition(IID_AMMO, getX(), getY(), getWorld()));
    else if(whatAmIHolding == "Health")
        getWorld()->addActor(new Health(IID_RESTORE_HEALTH, getX(), getY(), getWorld()));
    Actor::setDead();
    
}

void KleptoBot::addScore()
{
    getWorld()->increaseScore(10);
}

KleptoBot::~KleptoBot()
{
    
}

/////////////////////////////////////////////////////////////////
////////////////////Angry////////////////////////////////////////
///////////////////////KleptoBot/////////////////////////////////
/////////////////////////////////////////////////////////////////

AngryKleptoBot::AngryKleptoBot(int imageID, int x, int y, int level, int healthOfBot, string name, StudentWorld* world )
:KleptoBot(imageID, x, y, level, healthOfBot, name, world)
{
    
}

void AngryKleptoBot::addScore()
{
    getWorld()->increaseScore(20);
}

AngryKleptoBot::~AngryKleptoBot()
{
    
}
