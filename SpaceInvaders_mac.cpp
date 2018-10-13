/*Code Written By Jaspreet Singh Mahal*/
#include <iostream>
#include <time.h>
#include <deque>
//for kbhit
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
using namespace std;

const int HEIGHT = 29;
const int WIDTH = 120;
const int INVADERSCOUNT =50;
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

class ship{

    int x;
    int y;

public:
    ship(int x,int y){
        this->x = x;
        this->y = y;
    }
    inline int getX(){ return this->x;}
    inline int getY(){ return this->y;}
    inline void setX(int x){ this->y = x;}
    inline void setY(int y){ this->y = y;}
    inline void moveLeft(){this->y--;}
    inline void moveRight(){this->y++;}
};

class invader{
    int x;
    int y;
    
public:
    bool destroyed;
    inline int getx(){return this->x;}
    inline int gety(){return this->y;}
    inline void setX(int x){ this->x = x;}
    inline void setY(int y){ this->y = y;}
};

struct blast{
    int x;
    int y;
};

enum Direction{LEFT=1,RIGHT=2};
void spaceDraw(ship *deathstar,char **space,invader *invaders,deque<blast>*fireballs){
    
    for(int i = 0; i < HEIGHT; i++)
    {
        for(int j = 0; j < WIDTH; j++)
        {
            space[i][j]=' ';            
        }
        
    }    
    for(int i = 0; i < HEIGHT; i++)
    {
        space[i][0] = space[i][WIDTH-1] = '*';
    }
    
    for(int i = 0; i < WIDTH; i++)
    {
        space[0][i] = space[HEIGHT-1][i] = '*';
    }
    space[deathstar->getX()][deathstar->getY()] = 'A';
    
    for(int i=0;i<INVADERSCOUNT;i++){
        if(!invaders[i].destroyed)
            space[invaders[i].getx()][invaders[i].gety()] = 'U';
    }

    for(auto itr = fireballs->begin() ; itr!=fireballs->end();itr++){
        space[(*itr).x][(*itr).y] = '@';
    }    

    for(int i = 0; i < HEIGHT; i++)
    {
        
        for(int j = 0; j < WIDTH; j++)
        {
            cout<<space[i][j];
        }
        cout<<endl;
    }
    
}

void input(ship *deathstar,deque<blast>*fireballs){
    if(kbhit()){
        char inputreceived = getchar();
        
        switch (inputreceived)
        {
            case 'a':
                deathstar->moveLeft();
                break;
            case 'd':
                deathstar->moveRight();
                break;
            case ' ':
                blast b;
                b.x = deathstar->getX()-1;
                b.y = deathstar->getY();
                fireballs->push_back(b); 
                break;   
            case 'q':
                exit(0);   
            default:
                break;
        }
    }
}

void logic(invader *invaders,Direction *dir,deque<blast>*fireballs){
    if(invaders[0].gety()>=(WIDTH - 40) || invaders[0].gety()<=0){
        for(int i=0;i<INVADERSCOUNT;i++){
            invaders[i].setX( invaders[i].getx()+1 );
        }
         if(*dir == LEFT){
            *dir = RIGHT;
        }else{
            *dir = LEFT;
        }
    }
    for(int i = 0;i<INVADERSCOUNT ; i++){
        if(*dir == LEFT){
            invaders[i].setY( invaders[i].gety()-1 );
        }else{
            invaders[i].setY( invaders[i].gety()+1 );
        }
    }
    auto itr = fireballs->begin();
    while(itr!=fireballs->end()){
        (*itr).x--;
        if((*itr).x < 1){
            fireballs->pop_front();
        }
        itr++;
    }
      
}

int main(int argc, char const *argv[])
{
    deque<blast> fireballs;
    ship *deathstar;
    deathstar = new ship(HEIGHT-2,WIDTH/2);
    char **space;
    space = new char*[HEIGHT];
    for(int i = 0; i < WIDTH; i++)
    {
        space[i] = new char[WIDTH];
    }
    invader *invaders;
    invaders = new invader[INVADERSCOUNT];
    int sx = 1;
    int sy = 1;   
    int k=0;
    for(int i = 1; i <= 5; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            invaders[k].setX(sx);
            invaders[k].setY(sy);
            invaders[k].destroyed = false;
            k++;
            sy+=3;
        }
        sx = i*3;
        sy = 1;
    }
    Direction dir  = RIGHT;
    int timer = 0;
    while(1){
        system("clear");
        input(deathstar,&fireballs);
        spaceDraw(deathstar,space,invaders,&fireballs);
        if(timer == 40)
        {
            logic(invaders,&dir,&fireballs);
            timer = 0;
        }
        timer++;
        usleep(1000);
    }

    
    return 0;
}
