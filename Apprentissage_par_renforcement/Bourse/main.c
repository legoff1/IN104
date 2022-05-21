#include "bourse.h"
#include <time.h>




int main(){

   //int C0;
   srand( time(NULL) );
   Env_make("bour");
   
   printf("on %d days\n", rows);
   printf("number of actions :  %d \n", number_actions); 
   
   printf("choose your initial balance \n");
   //scanf("%d",&C0);
   
   Double_Q_learning(0.1,0.9);

   return 0;
}

