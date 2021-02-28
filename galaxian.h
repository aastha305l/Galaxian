//galaxian.h

#ifndef GALAXIAN_H
#define GALAXIAN_H

#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <sstream>

#include "Includes.h"
#include "Constants.h"
#include "compgeom.h"
#include "Surface.h"
#include "Event.h"

class Bullets
{
public:
    Bullets(int x = 0, int y = 0, int w = 5, int h = 10, int dy = 0, int dx = 0, int power = 5, int amount = 1, int speed = 5)
        :x_(x), y_(y), w_(w), h_(h), dy_(dy), dx_(dx), power_(power), amount_(amount), speed_(speed)
    {}
    //if it is rectangle or circle kind of bullets
    void initBullets(bool shape);
    void moveBullets(Surface & surface, int r, int g, int b);
    void setX(int x);
    void setY(int y);
    void setW(int w);
    void setH(int h);
    void setP(int p);
    void setA(int a);
    void setS(int s);
    int getX();
    int getY();
    int getW();
    int getH();
    int getDY();
private:
    //if it is rectangle or circle kind of bullets
    int power_;//how much health will one bullet decrease
    int amount_;//if it will shoot only one buttet at a time or multiple
    int speed_;//the speed with which it will attack
    int dx_;
    int dy_;
    int w_;
    int h_;
    int x_;
    int y_;
};

class Aliens
{
public:
    Aliens(int numOfAliens = 40, int breakAfter = 10, bool alone = true, int speed = 5)
        :numOfAliens_(numOfAliens), breakAfter_(breakAfter), alone_(alone), speed_(speed)
    {}

    void createTheGameScreen(Surface & surface, Image & image, int & changeInXForMovingAliens);
    void print(Image & image, Surface & surface);
    void makeAliens(Surface & surface, Image & image, int x, int y);
    void initializeAlienRect(Image & image, int x, int y);
    void setAlienRect(int index, int separate);
    void randomMoveAliens(Surface & surface, int changeInX, Bullets & bullets, int max, int min);
    void userMoveAliens(Surface & surface, KeyPressed & keypressed, int changeX, Bullets & bullets);
    void fallRandomAliens(Surface & surface, Bullets & bullets, int changeInX);
    void moveToTop(int index, int changeInX, int changeY);
    bool moveFromTop(int index, int changeInX, int changeY);
    void setAlienAlive(int i, bool alive);
    void decreaseLives(int i);
    void initOtherVariables();
    void setIsFalling(int i, bool fall);
    bool getAlive(int i);
    int getNOAliens();
    int getLives(int i);
    bool getIsFalling(int i);
    Rect getRect(int i);
    Rect maxMinRect(bool check);
    Rect findMaxRect();
    Rect findMinRect();
    static bool foundInFA(int i);
    static std::vector<int> & getFA();
    static int getCountFA(int i);
    static void decreaseCountFA(int i);
    static int getTotalFA();
private:
    bool alone_; //if the aliens drop alone or not
    int speed_; //the speed of the alien
    int numOfAliens_;//total number of aliens of a particular type
    int breakAfter_;//break after having these many aliens in one line
    Rect alienImageRect[50];//max amount of images that we can have
    bool alienIsAlive[50];//to keep track of if the aliens are alive or not
    bool alienIsFalling_[50];
    int initXPosition_[50];
    int initYPosition_[50];
    int lives_[50];
    static std::vector<int> fallingAlien_;
    static int countFallingAlien_[50];
};

int maxminfinder(Aliens & alien1, Aliens & alien2, Aliens & alien3, bool check);
void checkCollisionWAliens(Surface & surface, Aliens & aliens);
void checkCollisionWSpaceS(Surface & surface, Aliens & spaceship);
void checkCollisionWspaces(Surface & surface, Aliens & aliens, Aliens & spaceship);
void printCollision(Surface & surface, Rect alienRect);
void printStars(Surface & surface);
Rect findAliveAlien(Aliens & aliens, int & a);

static std::vector<Bullets> firedBullets_;
static std::vector<Bullets> ssBullets_;

#endif
