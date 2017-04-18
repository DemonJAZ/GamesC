#include <iostream>
#include <curses.h>
#include <termios.h>
#include <ncurses.h>
#include <stdio.h>
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

struct snakeLength
{
    int x;
    int y;
    struct snakeLength *next;

    snakeLength(int a,int b)
    {
        x=a;
        y=b;
    }
};

enum Dir { STOP=0 , UP=1 , DOWN = 2 , LEFT = 3 , RIGHT= 4 };

class Snake
{
public:
    struct snakeLength *sl;
    int x,y;
    Dir SnakeDir;
public:
    Snake(int h,int w)
    {
        this->x = w/2;
        this->y = h/2;
        sl = new snakeLength(x,y);
        sl->next=NULL;
        struct snakeLength *trav;
        trav=sl;
        for (int i = 1; i <=3; ++i)
        {
            trav->next=new snakeLength(x,y-i);
            trav = trav->next;
        }
        trav->next=NULL;
        SnakeDir = STOP;
    }
};


void initializeMap(char **map,int mapWidth,int mapHeight)
{
    system("clear");
    for (int i = 0; i < 40; ++i)
    {
        cout<<' ';
    }
    cout<<"SNAKES"<<endl;
    for (int i = 0; i < mapHeight; ++i)
    {
        for (int j = 0; j < mapWidth; ++j)
        {
                map[i][j]=' ';
        }
    }
    for (int i = 0; i < mapWidth; ++i)
    {
        map[0][i]='#';
        map[mapHeight-1][i]='#';
    }
    for (int i = 0; i < mapHeight; ++i)
    {
        map[i][0]='#';
        map[i][mapWidth-1]='#';
    }
}



void draw(Snake viper,char **map,int mapWidth , int mapHeight)
{
    struct snakeLength *trav;
    trav = viper.sl;
    while(trav!=NULL)
    {
        map[trav->x][trav->y]='@';
        trav=trav->next;
    }
    for (int i = 0; i < mapHeight; ++i)
    {
        for (int j = 0; j < mapWidth; ++j)
        {
            cout<<map[i][j];
        }
        cout<<endl;
    }
}


void input(Snake &viper)
{
    if(kbhit())
    {
        char keyPressed = getchar();
        if(keyPressed=='w')
        {
            if(viper.SnakeDir!=DOWN && viper.SnakeDir!=UP)
            {
                viper.x--;
                viper.SnakeDir=UP;
            }
        }
        if(keyPressed=='s')
        {
            if(viper.SnakeDir!=UP && viper.SnakeDir!=DOWN)
            {
                viper.x++;
                viper.SnakeDir=DOWN;
            }
        }
        if(keyPressed=='a')
        {
            if(viper.SnakeDir!=RIGHT && viper.SnakeDir!=LEFT)
            {
                viper.y--;
                viper.SnakeDir=LEFT;
            }
        }

        if(keyPressed=='d')
        {
            if(viper.SnakeDir!=LEFT && viper.SnakeDir!=RIGHT)
            {
                viper.y++;
                viper.SnakeDir=RIGHT;
            }
        }
        if(keyPressed=='q')
            exit(0);
    }
}

void Logic(Snake &viper,int mapWidth,int mapHeight)
{
    if(viper.x<=0 || viper.x>= mapHeight-1 || viper.y<=0 || viper.y>= mapWidth)
    {
        system("clear");
        cout<<"*************** GAME OVER *****************"<<endl;
        exit(0);
    }
    switch (viper.SnakeDir)
    {
    case UP:
        viper.x--;
        break;
    case DOWN:
        viper.x++;
        break;
    case LEFT:
        viper.y--;
        break;
    case RIGHT:
        viper.y++;
        break;
    default:
        break;
    }
}

void UpdateSnake(Snake &viper)
{
    struct snakeLength *head,*trav,*del;
    head = new snakeLength(viper.x,viper.y);
    head->next = viper.sl;
    viper.sl = head;
    trav=viper.sl;

    while(trav->next->next!=NULL)
    {
        trav=trav->next;
    }
    del=trav->next;
    trav->next=NULL;
    delete(del);
}

int main(int argc, char* argv[])
{
    int mapWidth=100,mapHeight=30;
    char **map;
    map = new char*[mapHeight];
    for (int i = 0; i < mapHeight; ++i)
    {
        map[i]=new char[mapWidth];
    }
    Snake viper(mapWidth,mapHeight);

    while(1){
        system("clear");
        initializeMap(map,mapWidth,mapHeight);
        draw(viper,map,mapWidth,mapHeight);
        input(viper);
        Logic(viper,mapWidth,mapHeight);
        UpdateSnake(viper);
        usleep(50000);

    }



    return 0;
}
