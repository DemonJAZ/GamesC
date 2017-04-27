/* Code Written By Jaspreet Singh Mahal
   #SelfMade
*/
#include <iostream>
#include <curses.h>
using namespace std;

struct Location
{
    int x;
    int y;
};

class Game{
private:
    char grid[3][3];
    bool visited[3][3];
    bool turn; //player 1 turn
    struct Location positions[9];
public:
    Game()
    {
        int k=0;
        for(int i=0;i<3;i++)
        {
            for(int j=0;j<3;j++)
            {
                grid[i][j]=' ';
                visited[i][j]=false;
                positions[k].x=i;
                positions[k].y=j;
                k++;
            }
        }

        turn=true;
    }

    void draw()
    {
        cout<<"HERE IS WHAT YOU NEED TO INPUT:"<<endl;

        int posi = 1;
        for(int i=0;i<3;i++)
        {
                cout<<posi++<<"|"<<posi++<<"|"<<posi++<<endl;
        }

        cout<<"-------------------------CURRENT GRID ------------------------"<<endl;
        int j=0;
        for(int i=0;i<3;i++)
        {
            if(i<2){
                cout<<grid[i][j++]<<" | "<<grid[i][j++]<<" | "<<grid[i][j++]<<endl;
                cout<<"__|___|__"<<endl;
                cout<<"  |   |  "<<endl;
                j=0;
            }
            else
            {
                cout<<grid[i][j++]<<" | "<<grid[i][j++]<<" | "<<grid[i][j++]<<endl;

            }

        }

    }

    void inputAndCheck()
    {
        if(turn)
            cout<<"Player 1 turn!!!!!"<<endl;
        else
            cout<<"Player 2 turn!!!!!"<<endl;

        int PlayerInput;
        cin>>PlayerInput;
        if(turn)
            grid[positions[PlayerInput-1].x][positions[PlayerInput-1].y] = 'X';
        else
            grid[positions[PlayerInput-1].x][positions[PlayerInput-1].y] = 'O';

        bool Winner=checkgrid(PlayerInput-1);
        //bool Winner=false;
        if(Winner && turn)
        {
            cout<<"****************** Player 1 is the winner ************************"<<endl;
            int j=0;
            for(int i=0;i<3;i++)
            {
                if(i<2){
                    cout<<grid[i][j++]<<" | "<<grid[i][j++]<<" | "<<grid[i][j++]<<endl;
                    cout<<"__|___|__"<<endl;
                    cout<<"  |   |  "<<endl;
                    j=0;
                }
                else
                {
                    cout<<grid[i][j++]<<" | "<<grid[i][j++]<<" | "<<grid[i][j++]<<endl;

                }

            }
            exit(0);
        }
        if(Winner)
        {
            cout<<"****************** Player 2 is the winner ************************"<<endl;
            int j=0;
            for(int i=0;i<3;i++)
            {
                if(i<2){
                    cout<<grid[i][j++]<<" | "<<grid[i][j++]<<" | "<<grid[i][j++]<<endl;
                    cout<<"__|___|__"<<endl;
                    cout<<"  |   |  "<<endl;
                    j=0;
                }
                else
                {
                    cout<<grid[i][j++]<<" | "<<grid[i][j++]<<" | "<<grid[i][j++]<<endl;

                }

            }
            exit(0);
        }
        turn=!turn;
    }

    bool checkgrid(int input)
    {
        char currentCheck;
        if(turn)
            currentCheck='X';
        else
            currentCheck='O';

        int x,y;
        x = positions[input].x;
        y = positions[input].y;
        int a,b,c,d;
        a=b=c=d=1;
        if(checkHorizontal(4,x,y,currentCheck,a) || checkVertical(4,x,y,currentCheck,b)
           || checkTLtoTR(4,4,x,y,currentCheck,c) || checkTRtoTL(4,4,x,y,currentCheck,d))
            return true;
        else
            return false;
    }

    //Grid Checking System
    bool checkVertical(int px,int x,int y, char currentCheck,int &c)
    {
        if(c==3)
            return true;

        if(x>0 && grid[x-1][y]==currentCheck && (x-1)!=px )
        {
            px=x;
            c+=1;
            return checkVertical(px,x-1,y,currentCheck,c);
        }

        if(x+1<3 && grid[x+1][y]==currentCheck && (x+1)!=px )
        {
            px = x;
            c+=1;
            return checkVertical(px,x+1,y,currentCheck,c);
        }
        return false;
    }

    bool checkHorizontal(int py,int x,int y, char currentCheck,int &c)
    {
        if(c==3)
            return true;

        if(y>0 && grid[x][y-1]==currentCheck && py!=(y-1))
        {
            c+=1;
            py=y;
            return checkHorizontal(py,x,y-1,currentCheck,c);
        }
        if(y+1<3 && grid[x][y+1]==currentCheck && py!=(y+1))
        {
            py=y;
            c+=1;
            return checkHorizontal(py,x,y+1,currentCheck,c);
        }
        return false;
    }

    bool checkTLtoTR(int px , int py , int x,int y, char currentCheck,int &c)
    {
        if(c==3)
            return true;

        if(y>0 && x>0 && grid[x-1][y-1]==currentCheck && px!=(x-1) && py!=(y-1))
        {
            px=x;
            py=y;
            c+=1;
            return checkTLtoTR(px,py,x-1,y-1,currentCheck,c);
        }
        if(y+1<3 && x+1<3 && grid[x+1][y+1]==currentCheck && px != (x+1) && py!= (y+1) )
        {
            px=x;
            py=y;
            c+=1;
            return checkTLtoTR(px,py,x+1,y+1,currentCheck,c);
        }
        return false;
    }

    bool checkTRtoTL(int px,int py,int x,int y, char currentCheck,int &c)
    {
        if(c==3)
            return true;

        if(y>0 && x+1<30 && grid[x+1][y-1]==currentCheck && px !=(x+1) && py!=(y-1))
        {
            px=x;
            py=y;
            c+=1;
            return checkTRtoTL(px,py,x,y-1,currentCheck,c);
        }
        if(y+1<3 && x>0 && grid[x-1][y+1]==currentCheck && px!=(x-1) && py!=(y+1))
        {
            px=x;
            py=y;
            c+=1;
            return checkTRtoTL(px,py,x,y+1,currentCheck,c);
        }
        return false;
    }
    //Grid System End;

};


int main(int argc, char* argv[])
{
    system("clear");
    cout<<"WELCOME TO THE GAME OF TIC TAC TOE"<<endl;
    cout<<"player1 is X Player 2 is O"<<endl;
    Game t;
    while(1)
    {
        t.draw();
        t.inputAndCheck();
        system("clear");
    }
    return 0;
}
