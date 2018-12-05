#include <iostream>
#include <time.h>
#include <vector>
//for kbhit
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

using namespace std;
int HEIGHT = 30;
int WIDTH = 50;
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

enum blockType {BOX,BAR,FOUR,REV_FOUR,SEVEN,REV_SEVEN,BUTTON}; // determine which block to spawn

struct blockStructure{
    int x;
    int y;
};

class Block
{
    vector<blockStructure> outline;
    blockType b;
public:
    void setBlock(blockType b,int spawn){
        this->b = b;
        switch (b)      // build block array outline
        {
            case BOX:
                for(int i = 0; i < 2; i++)
                {

                    for(int j = 0; j < 2; j++)
                    {
                        blockStructure b;
                        b.x = i+1;
                        b.y = spawn+j;
                        outline.push_back(b);
                    }

                }
                break;
            case BAR:
                blockStructure b;
                b.x = 1;
                for (int i = 0; i < 4; ++i)
                {
                    b.y = spawn+i;
                    outline.push_back(b);
                }
                break;
            default:
                break;
        }
    }

    void drawBlock(char **playground){
        for (int i = 0; i < outline.size(); ++i)
        {
            playground[outline[i].x][outline[i].y] = 'X';
        }
    }

    bool moveDown(char **playground){      // move down the block if there is space else return false
        if(playground[(outline[outline.size()-1].x)+1][outline[outline.size()-1].y] == ' '){
            for (int i = 0; i < outline.size(); ++i)
            {
                playground[outline[i].x][outline[i].y] = ' ';
                outline[i].x++;
            }
            return true;
        }else{
            outline.clear();
            return false;
        }
    }
};






void draw(char **playground,bool &onRun,Block &block){

    block.drawBlock(playground); // draw the block
    for (int i = 0; i < HEIGHT; ++i)
    {
        for (int j = 0; j < WIDTH; ++j)
        {
            cout<<playground[i][j];
        }
        cout<<endl;
    }
}

void logic(char **playground,bool &onRun,Block &block){
    if(onRun){
        bool movedDown = block.moveDown(playground);
        if(!movedDown){
            onRun = false;
        }
    }
}

void input(char **playground,bool &onRun){
    if(onRun){
        if(kbhit()){
            char inputreceived = getchar();
            switch (inputreceived)
            {
            case 'q': exit(0);
                break;
            default:
                break;
            }
        }
    }
}

int main(int argc, char* argv[])
{
    //building up the environment
    char **playground;
    playground = new char*[HEIGHT];
    for(int i=0 ; i<HEIGHT ; i++){
        playground[i] = new char[WIDTH];
    }
    for(int i=0; i < HEIGHT;i++){
        playground[i][0] = '#';
        playground[i][WIDTH-1] = '#';
    }
    for(int i=0; i < WIDTH;i++){
        playground[0][i] = '#';
        playground[HEIGHT-1][i] = '#';
    }
    for (int i = 1; i < HEIGHT-1; ++i)
    {
        for (int j = 1; j < WIDTH-1; ++j)
        {
            playground[i][j] = ' ';
        }
    }
    bool onRun = false;
    // below is the frames
    Block block;
    while(1){
        // reset block if not block is running
        if(!onRun){
            int num = rand()%2; // generate random number to set block type
            blockType a = (blockType)num;
            int spawn = 1+rand()%(WIDTH-3); // genereate random spawn position
            block.setBlock(a,spawn);    // create outline of the block
            onRun = true;
        }
        draw(playground,onRun,block);
        logic(playground,onRun,block);
        input(playground,onRun);
        usleep(10000);
        system("clear");
    }
    return 0;
}
