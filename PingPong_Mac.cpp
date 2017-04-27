/*
Learnt From
https://www.youtube.com/watch?v=y8QL62SDlcQ
*/

#include <iostream>
#include <cstdlib>
#include <time.h>
#include <curses.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

using namespace std;
/* Since kbhit is a Function available in windows (conio.h) ,Searched online for Alternative on
Mac Link:https://cboard.cprogramming.com/c-programming/63166-kbhit-linux.html */
int kbhit(void)
{
  struct termios oldt, newt;
  int ch;
  int oldf;

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

  ch = getchar();

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);

  if(ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }

  return 0;
}


enum dir { STOP=0 , LEFT = 1 , UPLEFT = 2 , DOWNLEFT = 3 , RIGHT = 4 , UPRIGHT = 5 , DOWNRIGHT = 6  };

/* Functionality of the Ball*/

class Ball
{
private:
    int x,y;
    int originalX,originalY;
    dir direction;
public:
    Ball( int posX , int posY)
    {
        originalX = posX;
        originalY = posY;
        x = posX; y = posY;
        direction = STOP;
    }

    void reset() {
        x = originalX;
        y = originalY;
        direction = STOP;
    }

    void changeDirection(dir d)
    {
        direction = d;
    }

    void randomDirection()
    {
        direction = (dir)((rand()%6)+1);
    }
    inline void IncY() { y+=2; }; //Change Reflection from pad a bit.
    inline void DecY() { y-=2; };
    inline int getX() { return x; }
    inline int getY() { return y; }
    inline dir getDirection() { return direction; }

    void Move()
    {
        switch (direction)
        {
        case STOP:
            break;

        case LEFT: x--;
            break;

        case UPLEFT: x--; y--;
            break;

        case DOWNLEFT: x--; y++;
            break;

        case RIGHT: x++;
            break;

        case UPRIGHT: x++; y--;
            break;

        case DOWNRIGHT: x++; y++;
            break;

        default:
            break;
        }
    }

    friend ostream& operator<<(ostream& o , Ball b)
    {
        o<<"Ball ["<<b.x<<"]["<<b.y<<"]"<<"Dir:"<<b.direction<<endl;
        return o;
    }
};

/* Functionality of the Pads Controlled by the players*/
class Pads
{
private:
    int x,y;
    int originalX,originalY;

public:
    Pads()
    {
        x = y = 0;
    }
    Pads(int posX , int posY)
    {
        originalX = posX;
        originalY = posY;
        x = posX;
        y = posY;
    }

    inline void reset() { x = originalX; y = originalY; }
    inline int getX() { return x; }
    inline int getY() { return y; }
    inline void moveUp() { y--; }
    inline void moveDown() {y++; }

    friend ostream& operator<<(ostream& o , Pads p)
    {
        o<<"Pad ["<<p.x<<"]["<<p.y<<"]"<<"Dir:"<<endl;
        return o;
    }
};

/* Game Managar will manage Score And Other Aspects of the game*/

class GameManager
{
private:
    int width,height;
    int score1,score2;
    char up1,down1,up2,down2; //keys used to control the pads
    bool quit;

    Ball *ping;
    Pads *player1 , *player2;

public:

    GameManager(int w,int h)
    {
        srand(time(NULL));
        quit = false;
        up1 = 'w';
        up2 = 'i';
        down1 = 's';
        down2 = 'k';
        score1 = score2 = 0;
        width = w;
        height = h;
        ping =  new Ball(w/2,h/2);
        player1 = new Pads(1,h/2-3);
        player2 = new Pads(w-2,h/2-3);

    }

    ~GameManager()
    {
        delete ping,player1,player2;
    }

    void scoreUp(Pads *player)
    {
        if(player1 == player)
            score1++;
        if(player2 == player)
            score2++;

        ping->reset();
        player1->reset();
        player2->reset();
    }

    void Draw()
    {
        system("clear");
        for (int i = 0; i < width+2; ++i)
            cout<<"🀠";
        cout<<endl;

        for (int i = 0; i < height; ++i)
        {
            for (int j = 0; j < width; ++j)
            {
                int ballx = ping->getX();
                int bally = ping->getY();
                int player1x = player1->getX();
                int player1y = player1->getY();
                int player2x = player2->getX();
                int player2y = player2->getY();

                if(j == 0 )
                    cout<<"🀠";

                if(ballx == j && bally == i)
                {
                    cout<<"☻";//ball
                }
                else if (player1x == j && player1y ==i)
                    cout<<"🀫";//player 1
                else if (player2x == j && player2y ==i)
                    cout<<"🀫";//player 2

                else if (player1x == j && player1y+1 ==i)
                        cout<<"🀫";//player 1
                else if (player1x == j && player1y+2 ==i)
                        cout<<"🀫";//player 1
                else if (player1x == j && player1y+3 ==i)
                        cout<<"🀫";//player 1


                else if (player2x == j && player2y+1 ==i)
                        cout<<"🀫";//player 1
                else if (player2x == j && player2y+2 ==i)
                        cout<<"🀫";//player 1
                else if (player2x == j && player2y+3 ==i)
                        cout<<"🀫";//player 1
                else
                    cout<<" ";

                if(j==width-1)
                    cout<<"🀠";

            }
            cout<<endl;
        }

        for (int i = 0; i < width+2; ++i)
            cout<<"🀠";
        cout<<endl;

        cout<<"PLAYER1 : "<<score1 <<" "<<"PLAYER2 : "<<score2<<endl;
    }


    void Input()
    {
        ping->Move();

        int ballx = ping->getX();
        int bally = ping->getY();
        int player1x = player1->getX();
        int player1y = player1->getY();
        int player2x = player2->getX();
        int player2y = player2->getY();


        if(kbhit())
        {
            char current = getchar();
            if( current == up1)
            {
                if(player1y > 0)
                    player1->moveUp();
            }
            if( current == up2)
            {
                if(player2y > 0)
                    player2->moveUp();
            }
            if(current == down1)
            {
                if(player1y+4 < height)
                {
                    player1->moveDown();
                }
            }
            if(current == down2)
            {
                if(player2y+4 < height)
                {
                    player2->moveDown();
                }
            }

            if(ping->getDirection()==STOP)
            {
                ping->randomDirection();
            }
            if(current=='q')
            {
                quit = true;
            }
        }
    }

    void Logic()
    {
        int ballx = ping->getX();
        int bally = ping->getY();
        int player1x = player1->getX();
        int player1y = player1->getY();
        int player2x = player2->getX();
        int player2y = player2->getY();

        //left pad
        for (int i = 0; i < 4; ++i)
        {
            if(ballx == player1x+1)
            {
                if(bally == player1y+i)
                {
                        ping->changeDirection((dir)((rand()%3)+4));
                        if(rand()%2)
                            ping->IncY();
                        else
                            ping->DecY();
                }


            }

        }
        //right pad
        for (int i = 0; i < 4; ++i)
        {
            if(ballx == player2x-1)
                if(bally == player2y+i)
                {
                    ping->changeDirection((dir)((rand()%3)+1));
                    if(rand()%2)
                        ping->IncY();
                    else
                        ping->DecY();
                }

        }

        //bottom wall
        if(bally == height-1)
            ping->changeDirection(ping->getDirection()==DOWNRIGHT?UPRIGHT:UPLEFT);
        //TOPWALL
        if(bally == 0)
            ping->changeDirection(ping->getDirection()==UPRIGHT?DOWNRIGHT:DOWNLEFT);
        //right wall
        if(ballx == width-1)
            scoreUp(player1);

        //right wall
        if(ballx == 0)
            scoreUp(player2);


    }

    void Run()
    {
        while (!quit)
        {
            system("clear");
            Draw();
            Input();
            Logic();
            usleep(10000);
        }
    }


};



int main(int argc, char const *argv[]) {
  srand(time(0));
  GameManager g(80,20);
  g.Run();
  return 0;
}
