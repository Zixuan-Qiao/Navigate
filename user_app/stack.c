#include "def.h"

Status InitStack(Stack *S){
	S->base = (Vertex *)malloc(STACK_INIT_SIZE * sizeof(Vertex));
	if(!S->base) exit(OVERFLOW);
	S->top = S->base;
	S->stacksize = STACK_INIT_SIZE;
	return TRUE;
}

Status GetTop(Stack *S, Vertex *v){
	if(S->top == S->base) return ERROR;
	*v = *(S->top - 1);
	return TRUE;
}

Status Push(Stack *S, Vertex v){
	if(S->top - S->base >= S->stacksize) {
		S->base = (Vertex *)realloc(S->base,
				(S->stacksize + STACKINCREMENT) * sizeof(Vertex));
		if(!S->base) exit(OVERFLOW);
		S->stacksize += STACKINCREMENT;	
	}
	*(S->top) = v;
	(S->top)++;
	return TRUE;
}

Status Pop(Stack *S, Vertex *v){
	if(S->top == S->base) return ERROR;
	
	*v = *--(S->top);
	return TRUE;
}

Status StackEmpty(Stack *S){

	if(S->base == S->top) return TRUE;
	return FALSE;
}

Status CopyStack(Stack *S1, Stack *S2){

	S1->top = S1->base;
	Vertex *resume;
	resume = S2->top;
	S2->top = S2->base;
	while(S2->top != resume){
		*(S1->top) = *(S2->top);
		(S1->top)++;
		(S2->top)++;
	}

	return TRUE;
}

Status ClearStack(Stack *S){

	S->top = S->base;
	return TRUE;

}
