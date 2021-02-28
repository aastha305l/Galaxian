//main.cpp
//#include "yliow.h"
#include <ctime>
#include "galaxian.h"


void createFirstScreen(Surface & surface, int &yValue)
{
    const int colorConst = 255;
    const int addToY = 5;
    Uint8 r = rand() % 256;
    Uint8 g = rand() % 256;
    Uint8 b = rand() % 256;
    Font font("fonts/FreeSans.ttf", 50);
    Color c = {r, g, b};
    Image image(font.render("Galaxian", c));
    Rect rect = image.getRect();

    if (yValue < H / 2) yValue += addToY;
    rect.x = 200;
    rect.y = yValue;
    
    surface.put_image(image, rect);

    return;
}


void endScreen(Surface & surface)
{
    const int colorConst = 255;
    Uint8 r = rand() % 256;
    Uint8 g = rand() % 256;
    Uint8 b = rand() % 256;
    Font font("fonts/FreeSans.ttf", 50);
    Color c = {r, g, b};
    Image starimage(font.render("*************************", c));
    Image textimage(font.render("        Game Over        ", c));
    Rect starrect = starimage.getRect();
    Rect textrect = textimage.getRect();

    starrect.x = 50;
    starrect.y = 50;
    textrect.x = 50;
    textrect.y = 100;

    surface.put_image(starimage, starrect);
    surface.put_image(textimage, textrect);
    starrect.y = 170;
    surface.put_image(starimage, starrect);
    //delay(10);
}


int main()
{
    int count = 0;
    Surface surface(W, H);
    Sound sound("sounds/laser.wav");
    Music music("sounds/GameLoop.ogg");
    music.play();
    
    bool needFirstScreen = true;
    bool gameOver = false;
    int yValue = -5;
    int changeInXForMovingAliens = 10;

    Aliens blueAliens(40, 10);
    Image blueAliensImage("images/galaxian/GalaxianAquaAlien.gif");
    blueAliens.makeAliens(surface, blueAliensImage, 70, 50);
    Bullets blueAliensBullets;

    Rect tempRect = blueAliensImage.getRect();

    Aliens redAliens(7, 7, false);
    Image redAliensImage("images/galaxian/GalaxianRedAlien.gif");
    redAliens.makeAliens(surface, redAliensImage, 80, 50 - tempRect.h);
    Bullets redAliensBullets;

    Aliens purpleAliens(10, 10);
    Image purpleAliensImage("images/galaxian/GalaxianPurpleAlien.gif");
    purpleAliens.makeAliens(surface, purpleAliensImage, 70, 50 - 2 * tempRect.h);
    Bullets purpleAliensBullets;

    Aliens spaceship(1, 1);
    Image spaceshipImage("images/galaxian/GalaxianGalaxip.gif");
    spaceship.makeAliens(surface, spaceshipImage, 300, 400);
    Bullets spaceshipBullets;

    
    //while (count < 500)//make 1 later
    while(1)
    {
        Event event;
        //++count; //remove it later
        srand((unsigned int) time(NULL));
        int r = rand() % 255;
        int g = rand() % 255;
        int b = rand() % 255;
        surface.lock();
        surface.fill(BLACK);
        printStars(surface);
        if (needFirstScreen) createFirstScreen(surface, yValue);

        
        Font font("fonts/FreeSans.ttf", 20);
        Color c = {255, 223, 0};
        Image scoreTextimage(font.render("Score: ", c));
        Rect scoreTextrect = scoreTextimage.getRect();
        scoreTextrect.x = 0;
        scoreTextrect.y = 0;

        /*
        Image scoreimage(font.render(scorechar, c));
        Rect scorerect = scoreimage.getRect();
        scorerect.x = 10;
        scorerect.y = 0;
        surface.put_image(scoreimage, scorerect);
        */
        
        KeyPressed keypressed = get_keypressed();
        if (event.poll() && event.type() == QUIT)
        {
            break;
        }
        else
        {
        
            if ((!spaceship.getAlive(0)) || gameOver)
            {
                endScreen(surface);
                surface.put_image(scoreTextimage, scoreTextrect);
                gameOver = true;
            }
            else
            {
                for (int i = 1; i <= spaceship.getLives(0); ++i)
                {
                    Rect tempRect = spaceshipImage.getRect();
                    tempRect.x = W - tempRect.w * i;
                    tempRect.y = 0;
                    surface.put_image(spaceshipImage, tempRect);
                }
            }
            if (keypressed[SPACE] && needFirstScreen && (!gameOver))
            {
                needFirstScreen = false;
                surface.fill(BLACK);
            }
            if ((!needFirstScreen) && (!gameOver))
            {
                surface.put_image(scoreTextimage, scoreTextrect);
                blueAliens.createTheGameScreen(surface, blueAliensImage, changeInXForMovingAliens);
                redAliens.createTheGameScreen(surface, redAliensImage, changeInXForMovingAliens);
                purpleAliens.createTheGameScreen(surface, purpleAliensImage, changeInXForMovingAliens);
                spaceship.createTheGameScreen(surface, spaceshipImage, changeInXForMovingAliens);

                int max = maxminfinder(blueAliens, redAliens, purpleAliens, true);
                int min = maxminfinder(blueAliens, redAliens, purpleAliens, false);
                if ((max + blueAliens.getRect(0).w >= W) || min <= 0)
                {
                    changeInXForMovingAliens *= -1;
                }
                blueAliens.randomMoveAliens( surface, changeInXForMovingAliens, blueAliensBullets, max, min);
                redAliens.randomMoveAliens(surface, changeInXForMovingAliens, redAliensBullets, max, min);
                purpleAliens.randomMoveAliens(surface, changeInXForMovingAliens, purpleAliensBullets, max, min);
                spaceship.userMoveAliens(surface, keypressed, 10, spaceshipBullets);
                spaceshipBullets.moveBullets(surface, r, g, b);

                checkCollisionWAliens(surface, blueAliens);
                checkCollisionWAliens(surface, redAliens);
                checkCollisionWAliens(surface, purpleAliens);
                checkCollisionWSpaceS(surface, spaceship);
                checkCollisionWspaces(surface, blueAliens, spaceship);
                checkCollisionWspaces(surface, redAliens, spaceship);
                checkCollisionWspaces(surface, purpleAliens, spaceship);
            }
            surface.unlock();
            surface.flip();
            delay(100);
        }
    }
    
    return 0;
}













/*

const int sizeX1 = 15;
const int numOfRowsBlueAlien = 4;

int laserX[500] = {0};
int laserY[500] = {0};
int laserXYAlive[500] = {0};


struct BlueAlien
{
    int xAxis[sizeX1];
    int yAxis[sizeX1];
    int isAlive[sizeX1];
};


void initAlienisAliveArray(BlueAlien alien[]);
void print_image(Surface & surface, Image & image, Rect & rect);
void print_alien(Surface & surface, Image & alien_image, Rect & alien_rect, int dxAlien, int dyAlien, BlueAlien alien[]);
void print_rect(Surface & surface, Rect & rect);
void print_collision(Surface & surface, Rect & alien_rect);
bool checkIfLaserHitsAlien(Rect & alien_rect, Rect & laser_rect);
bool checkLaserHitsAlien(Rect & alien_rect, Rect & laser_rect, Surface & surface, BlueAlien alien[]);
void print_laser(Surface & surface, Rect & laser_rect, int & indexOfLaser);
void killLaser(int index, int & indexOfLaser);
void increaseYLaser(int & indexOfLaser);
void laserIsAlive(int & indexOfLaser);
void send_laser_rect_for_alien_hits(Rect & alien_rect, Rect & laser_rect, Surface & surface, int & indexOfLaser, BlueAlien alien[]);
void print_stars(Surface & surface);


void initAlienisAliveArray(BlueAlien alien[])
{
    for (int i = 0; i < sizeX1; i++)
    {
        alien[0].isAlive[i] = 1;
        alien[1].isAlive[i] = 1;
        alien[2].isAlive[i] = 1;
        alien[3].isAlive[i] = 1;
    }
    
    return;
}

void print_stars(Surface & surface)
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

void print_image(Surface & surface, Image & image, Rect & rect)
{
    surface.lock();
    surface.put_image(image, rect);
    print_stars(surface);
    surface.unlock();
    surface.flip();
    
    return;
}

void print_alien(Surface & surface, Image & alien_image, Rect & alien_rect, int dxAlien, int dyAlien, BlueAlien alien[])
{
    surface.lock();
    
    Rect temp_rect;
    int dx = 2, dy = 2;
    temp_rect.x = alien_rect.x;
    temp_rect.y = alien_rect.y;
    alien_rect.x = 50 + dxAlien;
    
    for (int i = 0; i < sizeX1; i++)
    {
        alien_rect.x += (alien_rect.w + dx);
        
        alien_rect.y = temp_rect.y;
        alien[0].xAxis[i] = alien_rect.x;
        alien[0].yAxis[i] = alien_rect.y;
        if (alien[0].isAlive[i]) surface.put_image(alien_image, alien_rect);

        alien_rect.y += (alien_rect.h + dy);
        alien[1].xAxis[i] = alien_rect.x;
        alien[1].yAxis[i] = alien_rect.y;
        if (alien[1].isAlive[i]) surface.put_image(alien_image, alien_rect);
        
        alien_rect.y += (alien_rect.h + dy);
        alien[2].xAxis[i] = alien_rect.x;
        alien[2].yAxis[i] = alien_rect.y;
        if (alien[2].isAlive[i]) surface.put_image(alien_image, alien_rect);
        
        alien_rect.y += (alien_rect.h + dy);
        alien[3].xAxis[i] = alien_rect.x;
        alien[3].yAxis[i] = alien_rect.y;
        if (alien[3].isAlive[i]) surface.put_image(alien_image, alien_rect);
    }

    alien_rect.y = temp_rect.y;
    alien_rect.x = temp_rect.x;
    surface.unlock();
    surface.flip();

    return;
}

void print_rect(Surface & surface, Rect & rect)
{
    int r = rand() % 256;
    int g = rand() % 256;
    int b = rand() % 256;
    
    surface.lock();
    surface.put_rect(rect.x, rect.y, rect.w, rect.h, 100, 0, 0);
    surface.unlock();
    surface.flip();
    
    return;
}

void print_collision(Surface & surface, Rect & alien_rect)
{
    surface.lock();
    for (int i = 0; i < 50; i++)
    {
        int x[] = {alien_rect.x + rand() % alien_rect.w,
                   alien_rect.x + rand() % alien_rect.w,
                   alien_rect.x + rand() % alien_rect.w,
                   alien_rect.x + rand() % alien_rect.w,
                   alien_rect.x + rand() % alien_rect.w,
                   alien_rect.x + rand() % alien_rect.w};
    
        int y[] = {alien_rect.y + rand() % alien_rect.h,
                   alien_rect.y + rand() % alien_rect.h,
                   alien_rect.y + rand() % alien_rect.h,
                   alien_rect.y + rand() % alien_rect.h,
                   alien_rect.y + rand() % alien_rect.h,
                   alien_rect.y + rand() % alien_rect.h};

        surface.put_polygon(x, y, 6, rand() % 256, rand() % 256, rand() % 256);
    }
    surface.unlock();
    surface.flip();
    
    return;
}

bool checkIfLaserHitsAlien(Rect & alien_rect, Rect & laser_rect)
{
    if (((alien_rect.x <= laser_rect.x
          && alien_rect.x + alien_rect.w >= laser_rect.x)
        || (alien_rect.x <= laser_rect.x + laser_rect.w
            && alien_rect.x + alien_rect.w >= laser_rect.x + laser_rect.w))
        && (laser_rect.y <= alien_rect.y + alien_rect.h
            && laser_rect.y >= alien_rect.y))
    {
        return true;
    }
    
    return false;
}

void send_laser_rect_for_alien_hits(Rect & alien_rect, Rect & laser_rect, Surface & surface, int & indexOfLaser, BlueAlien alien[])
{
    Rect temp_laser_rect;
    int temp = indexOfLaser;
    temp_laser_rect.w = laser_rect.w;
    temp_laser_rect.h = laser_rect.h;
    
    for (int i = 0; i <= temp; i++)
    {
        temp_laser_rect.x = laserX[i];
        temp_laser_rect.y = laserY[i];
        if (checkLaserHitsAlien(alien_rect, temp_laser_rect, surface, alien))
        {
            killLaser(i, indexOfLaser);
        }
    }

    return;
}

bool checkLaserHitsAlien(Rect & alien_rect, Rect & laser_rect, Surface & surface, BlueAlien alien[])
{
    Rect temp_alien_rect;
    temp_alien_rect.x = 0;
    temp_alien_rect.y = 0;
    temp_alien_rect.w = alien_rect.w;
    temp_alien_rect.h = alien_rect.h;
    
    for (int i = 0; i < sizeX1; i++)
    {
        temp_alien_rect.x = alien[0].xAxis[i];
        temp_alien_rect.y = alien[0].yAxis[i];
        if(alien[0].isAlive[i] && checkIfLaserHitsAlien(temp_alien_rect, laser_rect))
        {
            alien[0].isAlive[i] = 0;
            print_collision(surface, temp_alien_rect);
            return true;
        }

        temp_alien_rect.x = alien[1].xAxis[i];
        temp_alien_rect.y = alien[1].yAxis[i];
        if(alien[1].isAlive[i] && checkIfLaserHitsAlien(temp_alien_rect, laser_rect))
        {
            alien[1].isAlive[i] = 0;
            print_collision(surface, temp_alien_rect);
            return true;
        }

        temp_alien_rect.x = alien[2].xAxis[i];
        temp_alien_rect.y = alien[2].yAxis[i];
        if(alien[2].isAlive[i] && checkIfLaserHitsAlien(temp_alien_rect, laser_rect))
        {
            alien[2].isAlive[i] = 0;
            print_collision(surface, temp_alien_rect);
            return true;
        }

        temp_alien_rect.x = alien[3].xAxis[i];
        temp_alien_rect.y = alien[3].yAxis[i];
        if(alien[3].isAlive[i] && checkIfLaserHitsAlien(temp_alien_rect, laser_rect))
        {
            alien[3].isAlive[i] = 0;
            print_collision(surface, temp_alien_rect);
            return true;
        }
    }

    return false;
}

void laserIsAlive(int & indexOfLaser)
{
    for (int i = 0; i <= indexOfLaser; i++)
    {
        if (laserY[i] <= 0) killLaser(i, indexOfLaser);
    }

    return;
}

void increaseYLaser(int & indexOfLaser)
{
    int dxL = 0;
    int dyL = 25;
    for (int i = 0; i <= indexOfLaser; i++)
    {
        laserX[i] += dxL;
        laserY[i] -= (dyL);
    }

    return;
}

void killLaser(int index, int & indexOfLaser)
{
    for (int i = index; i < indexOfLaser; i++)
    {
        laserX[i] = laserX[i+1];
        laserY[i] = laserY[i+1];
        laserXYAlive[i] = laserXYAlive[i+1];
    }
    indexOfLaser--;
    
    return;
}

void print_laser(Surface & surface, Rect & laser_rect, int & indexOfLaser)
{
    Rect temp_laser_rect;
    temp_laser_rect.w = laser_rect.w;
    temp_laser_rect.h = laser_rect.h;

    for (int i = 0; i <= indexOfLaser; i++)
    {
        temp_laser_rect.x = laserX[i];
        temp_laser_rect.y = laserY[i];
        print_rect(surface, temp_laser_rect);
    }

    return;
}

int main()
{
    Surface surface(W, H);
    Event event;
    Image alien_image("images/galaxian/GalaxianAquaAlien.gif");
    Rect alien_rect = alien_image.getRect();
    Image spaceship_image("images/galaxian/GalaxianGalaxip.gif");
    Rect spaceship_rect = spaceship_image.getRect();
    Rect laser_rect;
    BlueAlien alien[numOfRowsBlueAlien];
    
    int dxSpaceShip = 15;
    int dySpaceShip = 15;
    int dxAlien = 0;
    int dyAlien = 0;
    int changeFactorForDXY = 10;
    int indexOfLaser = -1;

    alien_rect.x = W - 375;
    alien_rect.y = 100;
    spaceship_rect.x = W - 375;
    spaceship_rect.y = W - 200;
    laser_rect.w = alien_rect.w - 30;
    laser_rect.h = alien_rect.h - 12;

    initAlienisAliveArray(alien);
    
    while (1)
    {
         if (event.poll() && event.type() == QUIT) break;
         surface.fill(BLACK);
         print_alien(surface, alien_image, alien_rect, dxAlien, dyAlien, alien);
         print_image(surface, spaceship_image, spaceship_rect);

         KeyPressed keypressed = get_keypressed();
         if (keypressed[LEFTARROW] && spaceship_rect.x - dxSpaceShip >= 0) spaceship_rect.x -= dxSpaceShip;
         if (keypressed[RIGHTARROW] && spaceship_rect.x + spaceship_rect.w + dxSpaceShip <= W) spaceship_rect.x += dxSpaceShip;
         if (keypressed[UPARROW] && spaceship_rect.y - dySpaceShip >= 0) spaceship_rect.y -= dySpaceShip;
         if (keypressed[DOWNARROW] && spaceship_rect.y + spaceship_rect.h + dySpaceShip <= H) spaceship_rect.y += dySpaceShip;
         if (keypressed[SPACE] || indexOfLaser >= 0)
         {
             if (keypressed[SPACE])
             {
                 indexOfLaser++;
                 laserX[indexOfLaser] = spaceship_rect.x + spaceship_rect.w / 2 - 5;
                 laserY[indexOfLaser] = spaceship_rect.y - spaceship_rect.h;
             }
             print_laser(surface, laser_rect, indexOfLaser);
             delay(20);

             increaseYLaser(indexOfLaser);
             laserIsAlive(indexOfLaser);
             send_laser_rect_for_alien_hits(alien_rect, laser_rect, surface, indexOfLaser, alien);
         }

         //check which is alive from the last and use that index in the next if statement
         for (int i = 

         if ((alien[0].xAxis[sizeX1 - 1] + alien_rect.w  >= W) || (alien[0].xAxis[0] <= 0)) changeFactorForDXY *= -1;
         dxAlien += changeFactorForDXY;
         dyAlien += changeFactorForDXY;
         delay(100);
    }
    
        return 0;
}


*/
