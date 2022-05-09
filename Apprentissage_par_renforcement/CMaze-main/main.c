#include "Double_Q_learning.h"



int main(){
   maze_make("maze.txt");
   
   printf("%d, %d \n", rows, cols);
   printf("number of actions :  %d \n", number_actions); 
   maze_render();
   
   Double_Q_learning(0.1, 0.9);
    
   maze_render();
   return 0;
}

