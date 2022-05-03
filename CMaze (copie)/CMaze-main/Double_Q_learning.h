#ifndef DOUBLE_Q_LEARNING_H
#define DOUBLE_Q_LEARNING_H
#include <stdio.h>
#include <stdlib.h>

char** maze;
float **RewardTab;
float **Q;
int rows;
int cols;
int start_row;
int start_col;
int state_row;
int state_col;
int goal_row;
int goal_col;

enum action{
     up,
     down,
     left,
     right,
     number_actions 
};
 
typedef enum action action ;

struct envOutput{
  int new_col;
  int new_row;
  int reward;
  int done;

};

typedef struct envOutput envOutput;

void alloc_maze();

void maze_make(char* );

void maze_render(); 

void maze_reset();

envOutput maze_step(action a); 

void alloc_RewardTab(void);

void alloc_Qtab(void);

void alloc_Qtab1(void);

void alloc_Qtab2(void);

int gen_nbr_alea(void);

void init_RewardTab(void);

int find_max (int state, float ** Q);

action env_action_Qpolicy(envOutput st, float **Q, float epsi);

void init_state(envOutput *stepOut);

void road_draw(void);

void Double_Q_learning(float alpha, float gamma);


#endif /* DOUBLE_Q_LEARNING_H */
