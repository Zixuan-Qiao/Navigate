#include "def.h"

int Hadlock(int m[][SIZE]){

	Stack *nstack, *pstack, *visit;
	nstack = (Stack *)malloc(sizeof(Stack));
	pstack = (Stack *)malloc(sizeof(Stack));
	visit = (Stack *)malloc(sizeof(Stack));
	InitStack(nstack);
	InitStack(pstack);
	InitStack(visit);

	Vertex s = {SX, SY, -1, -1}; 
	Push(pstack, s);
	int detour = 0;

	Vertex u,v;

	while(1){			// items in the stack were available 
		Pop(pstack, &u);	// when they are pushed into the stack
		if(m[u.x][u.y] != VIABLE){	// but they can be visited afterwards
			if(StackEmpty(pstack)){
				if(StackEmpty(nstack)){
					return 0;
				} else {
					CopyStack(pstack, nstack);
					ClearStack(nstack);
					detour++;
				}
			}
			continue;
		}

		m[u.x][u.y] = VISITED;	// visited
		Push(visit, u);
		if(u.x == TX && u.y == TY){
			return 1;
		}
		if(u.y + 1 < SIZE) {
			if(m[u.x][u.y + 1] == VIABLE){	// available
				v.x = u.x;
				v.y = u.y + 1;
				v.pre_x = u.x;
				v.pre_y = u.y;
				if(p_or_n(u, v)){
					Push(pstack, v);
				} else {
					Push(nstack, v);
				}
			}
		}
		if(u.x - 1 >= 0) {
			if(m[u.x - 1][u.y] == VIABLE){
				v.x = u.x - 1;
				v.y = u.y;
				v.pre_x = u.x;
				v.pre_y = u.y;
				if(p_or_n(u, v)){
					Push(pstack, v);
				} else {
					Push(nstack, v);
				}
			}
		}
		if(u.y - 1 >= 0) {
			if(m[u.x][u.y - 1] == VIABLE){
				v.x = u.x;
				v.y = u.y - 1;
				v.pre_x = u.x;
				v.pre_y = u.y;
				if(p_or_n(u, v)){
					Push(pstack, v);
				} else {
					Push(nstack, v);
				}
			}
		}
		if(u.x + 1 < SIZE) {
			if(m[u.x + 1][u.y] == VIABLE){
				v.x = u.x + 1;
				v.y = u.y;
				v.pre_x = u.x;
				v.pre_y = u.y;
				if(p_or_n(u, v)){
					Push(pstack, v);
				} else {
					Push(nstack, v);
				}
			}
		}
		if(StackEmpty(pstack)){
			if(StackEmpty(nstack)){
				return 0;
			} else {
				CopyStack(pstack, nstack);
				ClearStack(nstack);
				detour++;
			}
		}
	}
	
}

int p_or_n(Vertex u, Vertex v){
	int du, dv;
	du = abs(u.x - TX) + abs(u.y - TY);
	dv = abs(v.x - TX) + abs(v.y - TY);
	if(du > dv) return 1;
	return 0;
}


