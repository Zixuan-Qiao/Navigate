#include "def.h"

void auto_gen(int m[][SIZE]){

	int ran;
	srand((unsigned)time(NULL));
	for(int i = 0; i < SIZE; i++){
		for(int j = 0; j < SIZE; j++){
			ran = rand() % 10;
			if(ran <= INCIDENCE) m[i][j] = 1;
			else m[i][j] = 0;
		}
	}
}
