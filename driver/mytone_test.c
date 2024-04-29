#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    char val[3];
    val[0] = 10;
    val[1] = 40;
    val[2] = '\0';

    int fd = open("/dev/tone0", O_RDWR);
    printf("Open\n");

    write(fd, &val, sizeof(val));
    printf("Write\n");
    
    close(fd);
    printf("Closed\n");
    
    return 0;
}
