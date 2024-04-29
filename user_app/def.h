#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// map
#define SIZE 9

#define TX 0
#define TY 6
#define SX 8
#define SY 4

#define VIABLE 0
#define BLOCKED 1
#define LEFT 2
#define RIGHT 3
#define UP 4
#define DOWN 5
#define TARGET 6
#define NEUTRAL 8

// analog input
#define LENGTH 5

// stack
#define TRUE 1
#define FALSE 0
#define ERROR 0
#define OVERFLOW -2
#define STACK_INIT_SIZE 50
#define STACKINCREMENT 10

typedef int Status;

typedef struct Vertex{
	int x, y;
	int pre_x, pre_y;
}Vertex;

typedef struct{
	Vertex *base;
	Vertex *top;
	int stacksize;
}Stack;

Status InitStack(Stack *S);
Status StackEmpty(Stack *S);
Status GetTop(Stack *S, Vertex *v);
Status Push(Stack *S, Vertex v);
Status Pop(Stack *S, Vertex *v);
Status CopyStack(Stack *S1, Stack *S2);
Status ClearStack(Stack *S);

// auto generation
#define MAX 50
#define INCIDENCE 1
void auto_gen(int m[][SIZE]);

// hadlock
#define VISITED 10
int p_or_n(Vertex u, Vertex v);
int Hadlock(int m[][SIZE]);


