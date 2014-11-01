/******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <termios.h>
#include <fcntl.h>
using namespace std;
int read_keybord();

/******************************************************************************/
#define C 50
#define L 30
#define B 10
char screen[L][C];

int ba = L/2;
/******************************************************************************/
// Vous utilisez le bouton a pour alle a gauche et d pour aller a droite.
/* Pour le plan du jeu ********************************************************/
void display () 
{
  for (int i = 0 ; i < L ; i++)
    for (int j = 0 ; j < C ; j++)
      { if (i == 0 || i == L - 1 || j == 0 || j == C - 1)
	  screen[i][j] = '*';
	else
	  screen[i][j] = ' '; 
      }
}

void clear_screen()
{
  system("clear");
  for (int i = 0 ; i < L ; i++)
    { for (int j = 0 ; j < C ; j++)
	cout << screen[i][j]; 
      cout << endl; }
}
/* Pour le brique en bas ******************************************************/
void bas (int key){
  if (key == 'a' && ba > 1)
    ba -= 1;
  if (key == 'd' && (ba+B+2)<L+21)
    ba += 1;
  for (int a=0;a<B;a++)
    screen[L-2][ba+a] = '=';
}
/*Pour la balle ***************************************************************/
int x=1, y=1;
bool gd=1, hb=1; //pour tester si la balle qui va de gauche-droite ou de haut en bas 
                 //gd et hb = 0 pour terter di la balle qui va de droite-gauche ou de bas en 

void balle()
{  
  if (gd==1) //balle: gauche a droite
    { 
      if (hb==1) //balle: haut a bas
	{
	  screen[x][y]='o'; x++; y++; 
	}
      
      if (screen[x][y]=='=' || screen[x][y-1]=='=') //toucher la barre
	{
	  hb=0; x--; y--;
	}      
   
      if (y==C-1 && hb==1) //toucher frontier a droite et aller au frontier au dessous
	{
	  gd=0; y--; x++;
	}

      if (x==L+1)
	{
	  cout << "GAME OVER" << endl;
	  exit(0);
	}
      
      if (hb==0) //balle: bas a haut
	{
	  screen[x][y]='o'; x--; y++;
	}
      
      if (x==0) //toucher frontier au-dessus
	{
	  hb=1; x++;
	}
	
      if (y==C-1 && hb==0) //toucher frontier a droite et aller au frontier au dessus
	{
	  gd=0; y--; x--;
	}
    }
  
  if (gd==0) //balle: droite a gauche
    {
      if (hb==1) //balle: haut a bas
	{
	  screen[x][y]='o'; y--; x++;
	}
      
      if (screen[x][y]=='=' || screen[x][y+1]=='=') //toucher la barre
	{
	  hb=0; x--; y++;
	}
        
       
      if (y==0 && hb==1) //toucher frontier a gauche et aller au frontier au dessous
	{
	  gd=1; y++;
	}

      
      if (x==L+1) //fin
	{
	  cout << "GAME " << endl;
	  exit(0);
	}

      if (hb==0) //balle: bas a haut
	{
	  screen[x][y]='o'; y--; x--;
	}

      if (x==0) //toucher frontier au dessus
	{
	  hb=1; x++;
	}
     
      if (y==0 && hb==0) //toucher frontier a gauche et aller au frontier au dessus
	{
	  gd=1; y++;
	}
    }
}
/******************************************************************************/

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

int read_keybord() {
  int key;
  if (kbhit())
    key = getc(stdin);
  else
    key = 0;
  return key;
}
/******************************************************************************/
int main() {
  int key;
  do {
    key = read_keybord();   
    clear_screen();
    display();  
    bas(key);
    balle();
    
    usleep(100 * 1000);
  } while (key != 'q');
  
}

/******************************************************************************/
