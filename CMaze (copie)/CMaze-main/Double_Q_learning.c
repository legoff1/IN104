#include "Double_Q_learning.h" 
#include "functions.h"


void alloc_maze(){
     maze = malloc(rows * sizeof(char*));

     for(int i=0; i<rows; i++) {
         maze[i] = malloc(cols * sizeof(char*));
     }
}

void maze_make(char* file_name){
     char c;
     char rows_s[3] ={'\0'};
     char cols_s[3] ={'\0'};
     int rows_i = 0;
     int cols_i = 0;
     int swap = 0;

     FILE* file = fopen(file_name, "r");

     if (file) {
         /* lire la premiere ligne avant EOF */
         while( (c=getc(file)) != EOF) {
               if(c== '\n'){
                      break;
               } else if (c==',') {
                      swap = 1;
               } else if (!swap) {
                      rows_s[rows_i]=c;
                      rows_i++;
               } else {
                      cols_s[cols_i]= c;
                      cols_i++;
               }
         }
     }

     /* convertir le string en nombre */
     rows = atoi(rows_s);
     cols = atoi(cols_s);

     alloc_maze();

     for (int i=0; i<rows; i++){
         for (int j=0; j < cols; j++){
             c = getc(file);

             if (c=='\n'){
                 c = getc(file);
             } else if (c == 's'){
               start_row = i;
               start_col = j;
             } else if ( c == 'g'){
               goal_row = i;
               goal_col = j;
             }

             maze[i][j] = c;
         }
     }

     fclose(file);
}


void maze_render(){
     for (int i=0; i<rows; i++) {
         for (int j=0; j< cols; j++){
             printf("%c ", maze[i][j]);
         }
         printf("\n");
     }
     printf("\n");
}


void maze_reset(){
     state_row = start_row;
     state_col = start_col;
}


void alloc_RewardTab(void)
{
        RewardTab = malloc(rows * sizeof(float*));
        int i;
        for (i = 0; i < rows; ++i){
                RewardTab[i] = malloc(cols * sizeof(float*));
        }
}


void alloc_Qtab1(void)
{       
        float ** Q1 = malloc(cols*rows * sizeof(float*));
        int i;
        for (i = 0; i < cols*rows; ++i){
                Q1[i] = malloc(number_actions * sizeof(float*));
        }

        int j;
        for (i = 0; i < cols*rows; ++i) {
                for (j = 0; j < number_actions ; ++j) {
                    Q1[i][j] = 0;
                }
        }
}


void alloc_Qtab2(void)
{       
        float ** Q2 = malloc(cols*rows * sizeof(float*));
        int i;
        for (i = 0; i < cols*rows; ++i){
                Q2[i] = malloc(number_actions * sizeof(float*));
        }

        int j;
        for (i = 0; i < cols*rows; ++i) {
                for (j = 0; j < number_actions ; ++j) {
                    Q2[i][j] = 0;
                }
        }
}

void alloc_Qtab(void)
{       
        float ** Q = malloc(cols*rows * sizeof(float*));
        int i;
        for (i = 0; i < cols*rows; ++i){
                Q[i] = malloc(number_actions * sizeof(float*));
        }

        int j;
        for (i = 0; i < cols*rows; ++i) {
                for (j = 0; j < number_actions ; ++j) {
                    Q[i][j] = 0;
                }
        }
}


int gen_nbr_alea(){
   time_t t;
   
   /* Intializes random number generator */
   srand((unsigned) time(&t));

   /* Print 5 random numbers from 0 to 49 */
   return (rand() % 2);
   }
   
   


void init_RewardTab(void)
{
        alloc_RewardTab();

        int i, j;
        for (i = 0; i < rows; ++i) {
                for (j = 0; j < cols; ++j) {
                        if (maze[i][j] == '+') {
                                RewardTab[i][j] = -100;
                        } else if (maze[i][j] == 'g') {
                                RewardTab[i][j] = 100;
                        } else {
                                RewardTab[i][j] =-0.04 ;
                        }
                }
        }
}

envOutput maze_step(action a){
    int done = 0;
    envOutput stepOut;

    if (a==up){
       state_row = max(0,state_row -1);
    }else if (a==down){
       state_row = min(rows-1,state_row +1);
    }else if (a==right){
       state_col = min(cols-1,state_col +1);
    }else if (a==left){
       state_col = max(0,state_col -1);
    }
    
    if((state_row == goal_row) && (state_col == goal_col)){
       done   = 1;
    }

    stepOut.reward = RewardTab[state_row][state_col];
    stepOut.done   = done;
    stepOut.new_col = state_col;
    stepOut.new_row = state_row; 

   return stepOut;
}


int find_max (int state, float ** Q) {
    int best_reward_location =0;
    int i=1;
    while (i<4){
        if (Q[state][best_reward_location] < Q[state][i] ){
            best_reward_location=i;
        }
        i++;
    }
    return best_reward_location;
}
//* choose action by policy  **//

action env_action_Qpolicy(envOutput st, float **Q, float epsi){

  action at;
  int state=st.new_row*cols + st.new_col;
  if( ((rand() % 100)/100) < epsi )
    return (enum action)(rand() % number_actions);


  int a =find_max(state,Q);
 
  if (a==0){
    at=up;
  }else if(a==1){
    at=down;
  }else if(a==2){
    at=left;
  }else if(a==3){
    at=right;
  }

  return at;
}

void init_state(envOutput *stepOut){

    stepOut->reward = RewardTab[start_row][start_col];
    stepOut->done   = 0;
    stepOut->new_col = start_col;
    stepOut->new_row = start_row; 
}


// variable


void road_draw(){

//// affiche route
    envOutput st;
    action at;
    init_state(&st);
    maze_reset();
    do{
        at=env_action_Qpolicy(st,Q,0.0);
        st = maze_step(at);
        maze[st.new_row][st.new_col]='.';
    }while(st.done!=1 );
    maze[goal_row][goal_col]='g';
    maze[start_row][start_col]= 's';
}


void Double_Q_learning(float alpha, float gamma){


    envOutput stp1, st;  
    action at,atp1;
    
    int I_max=1000;
    int nbr_alea=0;//
    alloc_Qtab1();//
    alloc_Qtab2();//
    alloc_Qtab();//
    alloc_RewardTab();
    init_RewardTab();
    
    for(int i=0;i<I_max;i++){
        init_state(&st);
        maze_reset();
        
        do{
       
            
            //Q=Q1+Q2
	    for(int k = 0; k < rows; ++k){
	    	for(int l=0; l < cols; ++l){
	        	Q[k][l] = Q1[k][l] + Q2[k][l];
	        }
	    }

            at=env_action_Qpolicy(st,Q,0.1);
            stp1 = maze_step(at);
            
            //
            nbr_alea=gen_nbr_alea();
            if (nbr_alea==0){
            atp1 = env_action_Qpolicy(stp1, Q1, 0.0);
            Q1[st.new_row*cols + st.new_col][at] += alpha*(stp1.reward + gamma*Q2[stp1.new_row*cols + stp1.new_col][atp1] - Q1[st.new_row*cols + st.new_col][at]);
            }
            else {
            atp1 = env_action_Qpolicy(stp1, Q2, 0.0);
            Q2[st.new_row*cols + st.new_col][at] += alpha*(stp1.reward + gamma*Q1[stp1.new_row*cols + stp1.new_col][atp1] - Q2[st.new_row*cols + st.new_col][at]);
            }
            //
            st=stp1;
        }while(st.done!=1 );
        
    }

    // affichez les valeurs de Q
    /*
    for(int k=0;k<rows*cols;k++){
             printf(" %d\n\n",k);
            printf("%f   %f   %f   %f   \n",Q[k][0],Q[k][1],Q[k][2],Q[k][3]);
    }*/
    road_draw();

    
}
