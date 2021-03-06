#include "bourse.h" 
#include "functions.h"
#include "math.h"


void achat(int *cp, int *reward, int *Naction, int st, int **Env){ 
	int Vach = floor(*cp/Env[st][0]);
    Vach = min(Env[st][1], Vach);
	*cp=*cp-Env[st][0]*Vach;
	*Naction = *Naction + Vach;
	*reward=(Env[st+1][0] - Env[st][0])*Vach;
   }



void vendre(int *cp, int *reward, int *Naction, int st, int **Env){ 
	int Vvend = floor(min(*Naction, Env[st][1]));
	*cp=*cp+Env[st][0]*Vvend;
	*Naction = *Naction - Vvend;
	*reward=(Env[st][0] - Env[st+1][0])*Vvend;
   }


void garde(int *cp, int *reward, int *Naction, int st, int **Env){ 
	*reward=(Env[st+1][0] - Env[st][0])*(*Naction);
	}



void alloc_Env(){
     Env = malloc(rows * sizeof(char*));
     for(int i=0; i<rows; i++) {
         Env[i] = malloc(2 * sizeof(char*));
     }
}

void Env_make(char* file_name){

     FILE* file = fopen(file_name, "r");
     if (file) {
         /* lire la premiere ligne avant EOF */
         
        fscanf(file, "%d",&rows);

     alloc_Env();

     for (int i=0; i<rows; i++){
         fscanf(file, "%d %d", Env[i], (Env[i]+1));
         }
     }
     else{
        printf("erreur d'ouverture\n");
     }

     fclose(file);
}

void Env_reset(){
     goal_row=rows;
}


void alloc_Qtab1(void)
{       
        Q1 = malloc(rows * sizeof(float*));
        int i;
        for (i = 0; i < rows; ++i){
                Q1[i] = malloc(number_actions * sizeof(float*));
        }

        int j;
        for (i = 0; i < rows; ++i) {
                for (j = 0; j < number_actions ; ++j) {
                    Q1[i][j] = 0;
                }
        }
}


void alloc_Qtab2(void)
{       
        Q2 = malloc(rows * sizeof(float*));
        int i;
        for (i = 0; i < rows; ++i){
                Q2[i] = malloc(number_actions * sizeof(float*));
        }

        int j;
        for (i = 0; i < rows; ++i) {
                for (j = 0; j < number_actions ; ++j) {
                    Q2[i][j] = 0;
                }
        }
}


void alloc_Qtab(void)
{       
        Q = malloc(rows * sizeof(float*));
        int i;
        for (i = 0; i < rows; ++i){
                Q[i] = malloc(number_actions * sizeof(float*));
        }

        int j;
        for (i = 0; i < rows; ++i) {
                for (j = 0; j < number_actions ; ++j) {
                    Q[i][j] = 0;
                }
        }
}


int gen_nbr_alea(){

   return (rand() % 2);
   }
   

int make_action(action a, int *cp, int *reward, int *Naction, int st, int **Env ){
    

    int done = 0;
    if(st==goal_row-1){
        done=1;
        return done;
    }


    if(*cp<0.5 && *Naction < 1){
       done   = 1;
       return done;
    }

    if (a==sell){
       vendre(cp, reward, Naction, st, Env);
    }else if (a==buy){
       achat(cp, reward, Naction, st, Env);
    }else if (a==no_action){
       garde(cp, reward, Naction, st, Env);
    }

    return done;
}


int find_max (int state, float ** Q) {
    int best_reward_location =0;
    int i=1;
    while (i<number_actions){
        if (Q[state][best_reward_location] <= Q[state][i] ){
            best_reward_location=i;
        }
        i++;
    }
    return best_reward_location;
}
//* choose action by policy  **//

action env_action_Qpolicy(int st, float **Q, float epsi){

  action at;
  if( ((rand() % 100)/100) < epsi )
    return (enum action)(rand() % number_actions);


  int a =find_max(st,Q);
 
  if (a==0){
    at=sell;
  }else if(a==1){
    at=buy;
  }else if(a==2){
    at=no_action;
  }

  return at;
}

// variable

void print_action(action a){

  if (a==0){
    printf("sell\n");;
  }else if(a==1){
    printf("buy\n");
  }else if(a==2){
    printf("hold\n");
  }
}



void road_draw(int Capital){

//// affiche route
   
        int st=0;
        action at;
        Env_reset();
        int Naction = 0;
        int Sdone=0;
        int cp=Capital;
        int reward =0;
        int gain =0;
        do{
            printf("Jours %d \n",st+1 );
            at=env_action_Qpolicy(st,Q,0.0);
            if(at==sell && Naction==0){
                print_action(no_action);
            }else{
                print_action(at);
            }
            Sdone = make_action(at, &cp, &reward, &Naction, st, Env);
            gain=cp-Capital;
            printf(" Result %d actions Gain actuel %d\n",Naction, gain);
            
            st = st + 1;
        }while(st!=goal_row-1);
        
        printf("Jours %d jour de sortie du march??\n",st+1 );
        if (Naction==0)
        {
            print_action(no_action);
        }else{
            print_action(sell);
        }
        Sdone = make_action(sell, &cp, &reward, &Naction, st, Env);
         
        gain=cp-Capital + Env[goal_row-1][0]*Naction;
        printf("%d gain final\n", gain);
        //printf("\n\n\n\n");
        
    if(Sdone!=1){

    }
    else{
        printf("best road founded\n");
    }
}

void sum_Q1_Q2(float **Q, float **Q1, float **Q2){
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < number_actions ; ++j)
        {
            Q[i][j]=Q1[i][j] + Q2[i][j];
        }
    }
}

void Double_Q_learning(float alpha, float gamma, int Capital){


    int stp1, st;  
    action at,atp1;
    int Sdone=0;
    int reward=0;
    int cp=Capital;
    int Naction=0;
    int k=0;
    int nbr_alea=0;

    float epsi=0.1;
    int max_step=100000;
    int I_max=1000;
    alloc_Qtab1();
    alloc_Qtab2();
    alloc_Qtab();

    for(int i=0;i<I_max;i++){
        st=0;
        Env_reset();
        Naction = 0;
        k=0;
        Sdone=0;
        cp=Capital;
        do{
            //
            sum_Q1_Q2(Q,Q1,Q2);
            at=env_action_Qpolicy(st,Q,epsi);
            Sdone = make_action(at, &cp, &reward, &Naction, st, Env);
            stp1 = st + 1;
            //
            nbr_alea=gen_nbr_alea();
            if (nbr_alea==0){
                atp1 = env_action_Qpolicy(stp1, Q1, 0.0);
                Q1[st][at] += alpha*(reward + gamma*Q2[stp1][atp1] - Q1[st][at]);
            }
            else {
                atp1 = env_action_Qpolicy(stp1, Q2, 0.0);
                Q2[st][at] += alpha*(reward + gamma*Q1[stp1][atp1] - Q2[st][at]);
            
            }
            //
            st=stp1;
            //
            k++;
            if (k>max_step){
                printf("out of range\n");
                break;
            }
            if(st==goal_row-1 || cp <1 ){
                Sdone=1;
            }
        }while(Sdone!=1 );
        
    }

    printf("road_draw by Double_Q_learning\n");

    // affichez les valeurs de Q
   
    for(int k=0;k<rows;k++){
             printf(" %d\n\n",k);
            printf("%f   %f   %f  \n",Q[k][0],Q[k][1],Q[k][2]);
    }


        road_draw(Capital);


}
