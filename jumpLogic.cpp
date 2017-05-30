#include <iostream>
#include <time.h>
#include <thread>
//for kbhit
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

using namespace std;

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



int mapHeight=20,mapWidth=50;


class sprite{
public:
    int x;
    int y;
public:
    sprite(int a, int b)
    {
        this->x = a;
        this->y = b;
    }
};

void initializeMap(char **map, sprite &ball)
{
    for (int i = 0; i < mapHeight; ++i)
    {
        for (int j = 0; j < mapWidth; ++j)
        {
            map[i][j] = ' ';
        }
    }
    for (int i = 0; i < mapWidth; ++i)
    {
        map[0][i] = '#';
        map[mapHeight-1][i] = '#';
    }
    for(int i=0;i < mapHeight ; i++)
    {
        map[i][0] = '#';
        map[i][mapWidth-1] = '#';
    }
    map[ball.x][ball.y]='O';
}

void draw(char **map, sprite &ball)
{
    for (int i = 0; i < mapHeight; ++i)
    {
        for (int j = 0; j < mapWidth; ++j)
        {
            cout<<map[i][j];
        }
        cout<<endl;
    }
}

void logic(char **map , sprite &ball)
{
    if(map[ball.x+1][ball.y]!='#')
    {
        ball.x++;
    }
}

void input(char **map,sprite &ball)
{
    if(kbhit())
    {
        char keyPressed = getchar();

        if(keyPressed == ' ')
        {
            int current = ball.x;
            while(ball.x > current-6)
            {
                initializeMap(map, ball);
                input(map, ball);
                draw(map, ball);
                usleep(8000);
                system("clear");
                ball.x--;
            }
        }

        if(keyPressed == 'a')
        {
            if(ball.y > 1)
                ball.y--;
        }

        if(keyPressed == 'd')
        {
            if(ball.y < mapWidth-2)
                ball.y++;
        }

        if(keyPressed == 'q')
            exit(0);
    }
}

/*void jumpLogic(char map[], sprite ball)
{
    if(kbhit())
    {
        char keyPressed = getchar();

        if(keyPressed == ' ')
        {
            int current = ball.x;
            while(ball.x > current-6)
            {
                initializeMap(map, ball);
                draw(map, ball);
                usleep(8000);
                system("clear");
                ball.x--;
            }
        }
    }

}*/

int main(int argc, char* argv[])
{
    char **map;
    map = new char*[mapHeight];
    for (int i = 0; i < mapWidth; ++i)
    {
        map[i] = new char[mapWidth];
    }

    sprite ball(mapHeight-2,mapWidth/2);

    //thread j(jumpLogic,map,ball);
    while(true)
    {
        initializeMap(map, ball);
        input(map,ball);
        draw(map,ball);
        logic(map,ball);
        usleep(30000);
        system("clear");
    }
    //j.join();



    return 0;
}
