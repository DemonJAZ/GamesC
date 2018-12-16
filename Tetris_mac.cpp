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

struct Cell{ 
    int x;
    int y;
};

class Block
{
    vector<Cell> block_outline;
    blockType block_Type;
public:
    void setBlock(blockType block_Type,int spawn){
        this->block_Type = block_Type;
        Cell cell;
        switch (block_Type)      // build block array block_outline
        {
            /*set quardinates of the Cell of each block as per the outline*/
            case BOX:
                for(int i = 0; i < 2; i++)
                {

                    for(int j = 0; j < 2; j++)
                    {
                        
                        cell.x = i+1;
                        cell.y = spawn+j;
                        block_outline.push_back(cell);
                    }

                }
                break;
            case BAR:
                Cell cell;
                cell.x = 1;
                for (int i = 0; i < 4; ++i)
                {
                    cell.y = spawn+i;
                    block_outline.push_back(cell);
                }
                break;
            
            case FOUR:
                cell.x = 1;
                cell.y = spawn;
                block_outline.push_back(cell);
                cell.x+=1;
                block_outline.push_back(cell);
                cell.y+=1;
                block_outline.push_back(cell);
                cell.x+=1;
                block_outline.push_back(cell);
                break;

            default:
                break;
        }
    }
    /* movement of block*/
    void moveLeft(){
        for(int i = 0; i < block_outline.size(); i++)
        {
            block_outline[i].y--;
        }            
    }

    void moveRight(){
        for(int i = 0; i < block_outline.size(); i++)
        {
            block_outline[i].y++;
        }  
    }

    void drawBlock(char **playground){
        for (int i = 0; i < block_outline.size(); ++i)
        {
            playground[block_outline[i].x][block_outline[i].y] = 'X';
        }
    }

    bool moveDown(char **playground){      // move down the block if there is space else return false
        bool can_move = true;
        cout<<block_outline.size()<<endl;
        /* check if space below each Cell of block is clear so it can move*/
        switch (block_Type)
        {
            case BOX:
                for(int i = 2; i < block_outline.size() ; i++)
                {
                    if(playground[(block_outline[i].x)+1][block_outline[i].y] != ' '){ 
                        can_move = false;
                        break;
                    }            
                }
                break;
            
            case BAR:
                for(int i = 0; i < block_outline.size() ; i++)
                {
                    if(playground[(block_outline[i].x)+1][block_outline[i].y] != ' '){ 
                        can_move = false;
                        break;
                    }            
                }
                break;
            
            case FOUR:
                    if(playground[(block_outline[3].x)+1][block_outline[3].y] != ' ' || playground[(block_outline[1].x)+1][block_outline[1].y] != ' '){ 
                        can_move = false;
                        break;
                    }            
                break;
            default:
                break;
        }
        /*update quadrinates of a block and empty */
        if(can_move){
            for (int i = 0; i < block_outline.size(); ++i)
            {
                playground[block_outline[i].x][block_outline[i].y] = ' '; //empty previous quadinates.
                block_outline[i].x++;
            }
            return true;
        }else{
            cout<<block_outline[3].x<<" "<<block_outline[3].y<<endl;
            block_outline.clear();
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

void input(char **playground,bool &onRun,Block &block){
        if(kbhit()){
            char inputreceived = getchar();
            switch (inputreceived)
            {
                case 'a':
                    block.moveLeft();
                    break;
                case'd':
                    block.moveRight();
                    break;
                case 'q': exit(0);
                    break;
                default:
                    break;
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
            int num = rand()%3; // generate random number to set block type
            blockType a = (blockType)num;
            int spawn = (WIDTH/2) - 1; // genereate random spawn position
            block.setBlock(a,spawn);    // create outline of the block
            onRun = true;
        }
        draw(playground,onRun,block);
        logic(playground,onRun,block);
        input(playground,onRun,block);
        usleep(200000);
        system("clear");
    }
    return 0;
}
