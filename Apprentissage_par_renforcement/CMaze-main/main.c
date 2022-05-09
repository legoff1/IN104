#include "Q_learning.h"
#include <time.h>




int main(){

   int choice;
   srand( time(NULL) );
   maze_make("maze.txt");
   
   printf("%d, %d \n", rows, cols);
   printf("number of actions :  %d \n", number_actions); 
   maze_render();
   
   printf("please choose Q_learning(1) or Double_Q_learning(0) \n");
   scanf("%d",&choice);

   if (choice == 0)
      Double_Q_learning(0.1,0.9);
   else
      Q_learning(0.1, 0.9);
    
   maze_render();
   return 0;
}

