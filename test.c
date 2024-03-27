#include <stdio.h>

int main() {

	FILE *f_lr;
	char lr_c[10];
	
	for(int i = 0; i < 5; i++) {
		f_lr = fopen("./read", "r");
		fgets(lr_c, 10, f_lr);
		fclose(f_lr);
		printf("%s\n", lr_c);
		sleep(5);
	}
	
}


