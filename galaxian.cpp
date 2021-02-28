//galaxian.cpp
#include "galaxian.h"

//if shape is true then circle
//if shape is false then rect

std::vector<int> Aliens::fallingAlien_;
int Aliens::countFallingAlien_[50] = {0};

void Bullets::moveBullets(Surface & surface, int r, int g, int b)
{
    int size = firedBullets_.size();
    
    for (int i = 0; i < size; ++i)
    {
        Bullets tempBullet = firedBullets_[i];
        int x = tempBullet.getX();
        int y = tempBullet.getY();
        int w = tempBullet.getW();
        int h = tempBullet.getH();
        surface.put_circle(x, y, w, r, g, b);
        firedBullets_[i].setY(y + firedBullets_[i].dy_);
    }
   

    size = ssBullets_.size();
    for (int i = 0; i < size; ++i)
    {
        Bullets tempBullet = ssBullets_[i];
        int x = tempBullet.getX();
        int y = tempBullet.getY();
        int w = tempBullet.getW();
        int h = tempBullet.getH();
        surface.put_rect(x, y, w, h, 255, 223, 0);
        ssBullets_[i].setY(y + ssBullets_[i].getDY());

        if (ssBullets_[i].getY() <= 0)
        {
            ssBullets_.erase(ssBullets_.begin() + i);
            --i;
            --size;
        }
    }
    
    return;
}
 

void Bullets::setX(int x) { this->x_ = x; }
void Bullets::setY(int y) { this->y_ = y; }
void Bullets::setW(int w) { this->w_ = w; }
void Bullets::setH(int h) { this->h_ = h; }
void Bullets::setP(int p) { this->power_ = p; }
void Bullets::setA(int a) { this->amount_ = a; }
void Bullets::setS(int s) { this->speed_ = s; }

int Bullets::getX() { return this->x_; }
int Bullets::getY() { return this->y_; }
int Bullets::getW() { return this->w_; }
int Bullets::getH() { return this->h_; }
int Bullets::getDY() { return this->dy_; }


void Aliens::setAlienAlive(int i, bool alive) { this->alienIsAlive[i] = alive; }

Rect Aliens::getRect(int i) { return this->alienImageRect[i]; }
bool Aliens::getAlive(int i) { return this->alienIsAlive[i]; }
bool Aliens::getIsFalling(int i) { return this->alienIsFalling_[i]; }
int Aliens::getNOAliens() { return this->numOfAliens_; }
int Aliens::getLives(int i) { return this->lives_[i]; }
std::vector<int> & Aliens::getFA() { return fallingAlien_; }
int Aliens::getCountFA(int i) { return countFallingAlien_[i]; }
void Aliens::decreaseCountFA(int i) { --countFallingAlien_[i]; }
void Aliens::setIsFalling(int i, bool fall) { this->alienIsFalling_[i] = fall; }


void Aliens::createTheGameScreen(Surface & surface, Image & image, int & changeInXForMovingAliens)
{
    this->print(image, surface);
    return;
}


void Aliens::makeAliens(Surface & surface, Image & image, int x, int y)
{
    this->initializeAlienRect(image, x, y);
    this->initOtherVariables();
    return;
}


void Aliens::initOtherVariables()
{
    for (int i = 0; i < this->numOfAliens_; ++i)
    {
        Rect tempRect = this->alienImageRect[i];
        this->initXPosition_[i] = tempRect.x;
        this->initYPosition_[i] = tempRect.y;
        this->alienIsFalling_[i] = false;
        this->alienIsAlive[i] = true;
        this->lives_[i] = 3;
    }
    return;
}


void Aliens::initializeAlienRect(Image & image_, int x, int y)
{
    Rect tempRect = image_.getRect();
    int separateX = tempRect.w;
    int separateY = tempRect.h;

    for (int i = 0; i < this->numOfAliens_; ++i)
    {
        this->alienImageRect[i] = image_.getRect();
        this->alienImageRect[i].x += x;
        this->alienImageRect[i].y += y;
        if (i != 0 && (i % this->breakAfter_) == 0)
        {
            separateY += tempRect.h;
            separateX = tempRect.w;
        }
        this->setAlienRect(i, separateX);
        this->alienImageRect[i].y += separateY;
        
        separateX += tempRect.w;
    }
    return;
}


void Aliens::decreaseLives(int i)
{
    if (this->lives_[i] == 0) this->alienIsAlive[i] = false;
    else this->lives_[i] -= 1;
    return;
}


void Aliens::setAlienRect(int index, int separate)
{
    this->alienImageRect[index].x = separate;
    return;
}


bool Aliens::foundInFA(int i)
{
    for (int j = 0; j < fallingAlien_.size(); ++j)
    {
        if (fallingAlien_[j] == i) return true;
    }
    return false;
}


int Aliens::getTotalFA()
{
    int total = 0;
    for (int i = 0; i < 50; ++i)
    {
        total += countFallingAlien_[i];
    }
    return total;
}


void Aliens::randomMoveAliens(Surface & surface, int changeInX, Bullets & bullets, int max, int min)
{
    srand((unsigned int) time(NULL));
    int randomAlien = rand() % (this->numOfAliens_ - 1);
    int shouldFall = rand() % 2;
    int changeInY = 10;
    
    for (int i = 0; i < this->numOfAliens_; ++i)
    {
        bool found = foundInFA(i);
        if (!this->alienIsFalling_[i]) this->alienImageRect[i].x += changeInX;

        if (this->alienImageRect[i].y <= this->initYPosition_[i] && (this->alienIsFalling_[i]))
        {
            if (moveFromTop(i, changeInX, changeInY))
            {
                this->alienIsFalling_[i] = false;
            }
        }
        if (randomAlien == i && shouldFall && (!this->alienIsFalling_[i]) && (this->getTotalFA() < 3) && (this->alienIsAlive[i]))
        {
            if (!foundInFA(i)) fallingAlien_.push_back(i);
            //if (!this->alone) makeOtherFall();
            Bullets tempBullet(this->alienImageRect[i].x + 5, this->alienImageRect[i].y, 5, 10, 30);
            firedBullets_.push_back(tempBullet);
            this->alienIsFalling_[i] = true;
            ++countFallingAlien_[i];
        }
    }
    this->fallRandomAliens(surface, bullets, changeInX);
    
    return;
}


void Aliens::userMoveAliens(Surface & surface, KeyPressed & keypressed, int changeX, Bullets & bullets)
{
    if (keypressed[LEFTARROW] && this->alienImageRect[0].x - changeX >= 0) this->alienImageRect[0].x -= changeX;
    
    if (keypressed[RIGHTARROW] && this->alienImageRect[0].x + this->alienImageRect[0].w + changeX <= W) this->alienImageRect[0].x += changeX;
    
    if (keypressed[SPACE] && this->alienIsAlive[0])
    {
        int size = ssBullets_.size();
        Bullets tempBullet(this->alienImageRect[0].x + 13, this->alienImageRect[0].y - 18, 5, 10, -20);
        if (size == 0)
        {
            ssBullets_.push_back(tempBullet);
        }
        else
        {
            Bullets lastBullet = ssBullets_[size - 1];
            if (lastBullet.getY() + lastBullet.getH() < H - 100)
            {
                ssBullets_.push_back(tempBullet);
            }
        }
    }
    return;
}


void Aliens::fallRandomAliens(Surface & surface, Bullets & bullets, int changeInX)
{
    int mult = rand() % 3 - 1;
    int changeY = 10;
    int changeX = 10 * mult;
    int size = fallingAlien_.size();

    for (int j = 0; j < size; ++j)
    {
        //takes the index of the fallingAlien
        int index = fallingAlien_[j];

        if (countFallingAlien_[index] == 0)
        {
            fallingAlien_.erase(fallingAlien_.begin() + j);
            firedBullets_.erase(firedBullets_.begin() + j);
            --j;
            --size;
            //continue;
        }
        else if (this->alienIsFalling_[index])
        {
            //changes the y value of alien
            this->alienImageRect[index].y += changeY;
            //if the x value of alien goes out of bound then it changes the symbol of changeX
            if (this->alienImageRect[index].x + this->alienImageRect[index].w + changeX >= W || this->alienImageRect[index].x + changeX <= 0) changeX *= mult;
            this->alienImageRect[index].x += changeX;
        
            if (this->alienImageRect[index].y >= H)
            {
                moveToTop(index, changeInX, changeY);
                --countFallingAlien_[index];
            }
        }
    }
    return;
}


bool Aliens::moveFromTop(int index, int changeInX, int changeY)
{
    if (this->initYPosition_[index] - this->alienImageRect[index].y < changeY )
    {
        changeY = this->initYPosition_[index] - this->alienImageRect[index].y;
    }
    this->alienImageRect[index].x += changeInX;
    this->alienImageRect[index].y += changeY;
    
    if (changeY <= 0)
    {
        return true;
    }
    return false;
}


Rect findAliveAlien(Aliens & aliens, int & a)
{
    for (int i = 0; i < aliens.getNOAliens(); ++i)
    {
        if (aliens.getAlive(i) && (!aliens.getIsFalling(i)))
        {
            a = i;
            return aliens.getRect(i);
        }
    }
    return -1;
}

void Aliens::moveToTop(int index, int changeInX, int changeY)
{
    int i = 0;
    Rect tempRect = findAliveAlien(*this, i);
    int oriXtempRect = this->initXPosition_[i];
    int diff = oriXtempRect - tempRect.x;
    int positionX = this->initXPosition_[index] - diff;
    this->alienImageRect[index].x = positionX;
    this->alienImageRect[index].y = 0;
    //this->alienIsAlive[index] = true;
    
    return;
}


int maxminfinder(Aliens & alien1, Aliens & alien2, Aliens & alien3, bool check)
//int maxminfinder(Aliens & alien1, Aliens & alien2, bool check)
{
    int x;
    Rect tempRect;
    if (check)
    {
        x = -1;
        tempRect = alien1.findMaxRect();
        if (tempRect.x > x) x = tempRect.x;
        tempRect = alien2.findMaxRect();
        if (tempRect.x > x) x = tempRect.x;
        tempRect = alien3.findMaxRect();
        if (tempRect.x > x) x = tempRect.x;
        //tempRect = alien4.findMaxRect();
        //if (tempRect.x > x) x = tempRect.x;
    }
    else
    {
        x = 9999;
        tempRect = alien1.findMinRect();
        if (tempRect.x < x) x = tempRect.x;
        tempRect = alien2.findMinRect();
        if (tempRect.x < x) x = tempRect.x;
        tempRect = alien3.findMinRect();
        if (tempRect.x < x) x = tempRect.x;
        //tempRect = alien4.findMinRect();
        //if (tempRect.x < x) x = tempRect.x;
    }
    return x;
}

//if check is true then it is max
//if it is false then it is min
Rect Aliens::maxMinRect(bool check)
{
    Rect tempRect;
    int index;
    for (int i = 0; i < this->numOfAliens_; ++i)
    {
        bool found = foundInFA(i);
        if (this->alienIsAlive[i] && !found)
        {
            tempRect = this->alienImageRect[i];
            index = i + 1;
            break;
        }
    }
    for (int i = index; i < this->numOfAliens_; ++i)
    {
        bool found = foundInFA(i);
        if (this->alienIsAlive[i] && !found)
        {
            if (check == true && tempRect.x < this->alienImageRect[i].x)
            {
                tempRect = this->alienImageRect[i];
            }
            if (check == false && tempRect.x > this->alienImageRect[i].x)
            {
                tempRect = this->alienImageRect[i];
            }
        }
    }
    return tempRect;
}


Rect Aliens::findMaxRect() { return this->maxMinRect(true); }
Rect Aliens::findMinRect() { return this->maxMinRect(false); }


void Aliens::print(Image & image, Surface & surface)
{
    for (int i = 0; i < numOfAliens_; ++i)
    {
        if (this->alienIsAlive[i] == true)
        {
            surface.put_image(image, this->alienImageRect[i]);
        }
    }
    return;
}


void printStars(Surface & surface)
{
    int x = rand() % W;
    int y = rand() % H;
    int r = rand() % 256;
    int g = rand() % 256;
    int b = rand() % 256;

    surface.put_pixel(x, y, r, g, b);
    for (int i = 0; i < rand() % 200 + 50; i++)
    {
        surface.put_circle(x, y, 1, 224, 213, 126);
        x = rand() % W;
        y = rand() % H;
    }

    return;
}


//checks the collision of bullets from spaceship with aliens
void checkCollisionWAliens(Surface & surface, Aliens & aliens)
{
    int sizeA = aliens.getNOAliens();
    int sizeSB = ssBullets_.size();
    for (int i = 0; i < sizeSB; ++i)
    {
        Bullets spaceBullet = ssBullets_[i];
        int xS = ssBullets_[i].getX();
        int yS = ssBullets_[i].getY();
        int wS = ssBullets_[i].getW();
        int hS = ssBullets_[i].getH();
        for (int j = 0; j < sizeA; ++j)
        {
            Rect alienRect = aliens.getRect(j);
            int xA = alienRect.x;
            int yA = alienRect.y;
            int wA = alienRect.w;
            int hA = alienRect.h;
            if ((aliens.getAlive(j)) && ((xA <= xS && xA + wA >= xS) || (xA <= xS + wS && xA + wA >= xS + wS))
                && (yA <= yS && yA + hA >= yS))
            {
                printCollision(surface, alienRect);
                aliens.decreaseLives(j);

                
                if ((!aliens.getAlive(j)) && aliens.getIsFalling(j))
                {
                    aliens.setIsFalling(j, false);
                    aliens.decreaseCountFA(j);
                }
                
                
                ssBullets_.erase(ssBullets_.begin() + i);
                --sizeSB;
                --i;
                break;
            }
        }
    }
    return;
}


//checks the collision of bullets from aliens with spaceship
void checkCollisionWSpaceS(Surface & surface, Aliens & spaceship)
{
    int sizeA = spaceship.getNOAliens();
    int sizeSB = firedBullets_.size();
    for (int i = 0; i < sizeSB; ++i)
    {
        Bullets bullet = firedBullets_[i];
        int xS = bullet.getX();
        int yS = bullet.getY();
        int wS = bullet.getW();
        //int hS = bullet[i].getH();
        for (int j = 0; j < sizeA; ++j)
        {
            Rect alienRect = spaceship.getRect(j);
            int xA = alienRect.x;
            int yA = alienRect.y;
            int wA = alienRect.w;
            int hA = alienRect.h;
            if ((spaceship.getAlive(j)) && ((xA <= xS - wS && xA + wA >= xS - wS) || (xA <= xS + wS && xA + wA >= xS + wS)) && (yA <= yS + wS && yA + hA >= yS + wS))
            {
                spaceship.decreaseLives(j);
                printCollision(surface, alienRect);
                firedBullets_.erase(firedBullets_.begin() + i);
                --sizeSB;
                --i;
                break;
            }
        }
    }
    return;
}


//checks the collision of aliens with spaceship
void checkCollisionWspaces(Surface & surface, Aliens & aliens, Aliens & spaceship)
{
    int sizeS = spaceship.getNOAliens();
    int sizeFA = aliens.getFA().size();
    int shouldBreak = false;
    for (int i = 0; i < sizeFA; ++i)
    {
        int index = aliens.getFA()[i];
        if (aliens.getIsFalling(index))
        {
            Rect alien = aliens.getRect(index);
            int xA = alien.x;
            int yA = alien.y;
            int wA = alien.w;
            int hA = alien.h;
            for (int j = 0; j < sizeS; ++j)
            {
                Rect spaceRect = spaceship.getRect(j);
                int xS = spaceRect.x;
                int yS = spaceRect.y;
                int wS = spaceRect.w;
                int hS = spaceRect.h;
                if ((spaceship.getAlive(j)) && aliens.getAlive(i) && ((xS <= xA && xS + wS >= xA) || (xS <= xA + wA && xS + wS >= xA + wA)) && (yS <= yA + hA && yS + hS >= yA + hA))
                {
                    spaceship.decreaseLives(j);
                    aliens.decreaseLives(index);
                    printCollision(surface, spaceRect);
                    shouldBreak = true;

                    
                    if ((!aliens.getAlive(i)) && aliens.getIsFalling(i))
                    {
                        aliens.setIsFalling(i, false);
                        aliens.decreaseCountFA(i);
                    }
                    
                    break;
                }
            }
        }
        if (shouldBreak) break;
    }
    return;
}


void printCollision(Surface & surface, Rect alienRect)
{
    int x = alienRect.x;
    int y = alienRect.y;
    int w = alienRect.w;
    int h = alienRect.h;
    for (int i = 0; i < 50; i++)
    {
        int a[] = {alienRect.x + rand() % alienRect.w,
                   alienRect.x + rand() % alienRect.w,
                   alienRect.x + rand() % alienRect.w,
                   alienRect.x + rand() % alienRect.w,
                   alienRect.x + rand() % alienRect.w,
                   alienRect.x + rand() % alienRect.w};
    
        int b[] = {alienRect.y + rand() % alienRect.h,
                   alienRect.y + rand() % alienRect.h,
                   alienRect.y + rand() % alienRect.h,
                   alienRect.y + rand() % alienRect.h,
                   alienRect.y + rand() % alienRect.h,
                   alienRect.y + rand() % alienRect.h};

        surface.put_polygon(a, b, 6, rand() % 256, rand() % 256, rand() % 256);
    }
    
    return;
}
