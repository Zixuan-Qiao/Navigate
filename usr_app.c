#include "def.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(void) {
	
	int m[SIZE][SIZE];
	int i = 1;
	
	// auto generation and verification
	while(1) {
		auto_gen(m);
		if(Hadlock(m))	break;
		i++;
		if(i > MAX) {
			printf("Failed to generate valid map, please lower block incident rate. \n");
			return 0;
		}
	}
	
	m[TX][TY] = TARGET;
	
	int curr_x = SX;
	int curr_y = SY;
	
	m[curr_x][curr_y] = NEUTRAL;
	
	FILE *f_lr;
	FILE *f_ud;
	char lr_c[LENGTH];
	char ud_c[LENGTH];
	int lr_i;
	int ud_i;
	int dir;
	
	char edge[SIZE + 3];
	memset(edge, '*', sizeof(char) * (SIZE + 2));
	edge[SIZE + 2] = '\0';
	
	char val[3];
	val[0] = 0;
	val[1] = 0;
	val[2] = '\0';

	int fd = open("/dev/tone0", O_RDWR);
	
	while(1) {
		// print map
		system("clear");
		printf("\t%s\n", edge);
		for(int i = 0; i < SIZE; i++) {
			printf("\t*");
			for(int j = 0; j < SIZE; j++) {
				switch(m[i][j]) {
					case VIABLE:
						printf(" ");
						break;
					case VISITED:
						printf(" ");
						break;
					case BLOCKED:
						printf("*");
						break;
					case LEFT:
						printf("<");
						break;
					case RIGHT:
						printf(">");
						break;
					case UP:
						printf("^");
						break;
					case DOWN:
						printf("v");
						break;
					case TARGET:
						printf("#");
						break;
					case NEUTRAL:
						printf("x");
						break;
					default:
						printf("Error in map. \n");
				}					
			}
			printf("*\n");
		}
		printf("\t%s\n", edge);
		
		sleep(1);
		
		// get direction
		f_lr = fopen("/sys/bus/iio/devices/iio:device0/in_voltage0_raw", "r");
		f_ud = fopen("/sys/bus/iio/devices/iio:device0/in_voltage1_raw", "r");
		
		fgets(lr_c, LENGTH, f_lr);
		fgets(ud_c, LENGTH, f_ud);
		
		fclose(f_lr);
		fclose(f_ud);
		
		lr_i = atoi(lr_c);
		ud_i = atoi(ud_c);
		
		if(ud_i > 3000)	dir = UP;
		else if(ud_i < 500) dir = DOWN;
		else if(lr_i > 3000) dir = RIGHT;
		else if(lr_i < 500) dir = LEFT;
		else dir = NEUTRAL;

		switch(dir) {
			case LEFT:
				if(curr_y > 0 && m[curr_x][curr_y - 1] != BLOCKED) {
					m[curr_x][curr_y] = 0;
					curr_y--;
				} else {
					val[0] = 10;
					val[1] = 20;
					val[2] = '\0';
					write(fd, &val, sizeof(val));
				}
				m[curr_x][curr_y] = LEFT;
				break;
			case RIGHT:
				if(curr_y < SIZE - 1 && m[curr_x][curr_y + 1] != BLOCKED) {
					m[curr_x][curr_y] = 0;
					curr_y++;
				} else {
					val[0] = 5;
					val[1] = 20;
					val[2] = '\0';
					write(fd, &val, sizeof(val));
				}
				m[curr_x][curr_y] = RIGHT;
				break;
			case UP:
				if(curr_x > 0 && m[curr_x - 1][curr_y] != BLOCKED) {
					m[curr_x][curr_y] = 0;
					curr_x--;
				} else {
					val[0] = 1;
					val[1] = 30;
					val[2] = '\0';
					write(fd, &val, sizeof(val));
				}
				m[curr_x][curr_y] = UP;
				break;
			case DOWN:
				if(curr_x < SIZE - 1 && m[curr_x + 1][curr_y] != BLOCKED) {
					m[curr_x][curr_y] = 0;
					curr_x++;
				} else {
					val[0] = 15;
					val[1] = 15;
					val[2] = '\0';
					write(fd, &val, sizeof(val));
				}
				m[curr_x][curr_y] = DOWN;
				break;
			case NEUTRAL:
				m[curr_x][curr_y] = NEUTRAL;
				break;
			default:
				printf("Error in direction. \n");		
		}
		
		if(curr_x == TX && curr_y == TY) {
			printf("Success! \n");
			return 0;
		}
	}
	
	close(fd);
	return 0;
}
