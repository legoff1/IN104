#ifndef BOURSE_H
#define BOURSE_H

#include <stdio.h>
#include <stdlib.h>

//int Ci=400;
int** Env;
float **Q;
float **Q1;
float **Q2;
int rows;
int start_row;
int goal_row;

enum action{
     sell,
     buy,
     no_action,
     number_actions 
};
 
typedef enum action action ;
void achat(int *cp, int *reward, int *Naction, int st, int **Env)

void vendre(int *cp, int *reward, int *Naction, int st, int **Env)

void garde(int *cp, int *reward, int *Naction, int st, int **Env)

void alloc_Env();

void Env_make(char* );

void Env_reset();

void alloc_Qtab1(void);

void alloc_Qtab2(void);

void alloc_Qtab(void);

int gen_nbr_alea(void);

int make_action(action a, int *cp, int *reward, int *Naction, int st, int **Env );

int find_max (int state, float ** Q);

action env_action_Qpolicy(int st, float **Q, float epsi);

void print_action(action a);

void road_draw(void);

void sum_Q1_Q2(float **Q, float **Q1, float **Q2);

void Double_Q_learning(float alpha, float gamma);


#endif /* BOURSE_H */
