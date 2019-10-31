#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <asm/termbits.h>

int main(){
	int fd;
	struct termios2 tios;

	do{
		fd = open("/dev/ttyS4", O_RDONLY);
	} while(fd==0);

	while(ioctl(fd, TCGETS2, &tios) < 0) ;
	tios.c_cflag &= ~CBAUD;
	tios.c_cflag |= BOTHER;
	tios.c_ispeed = 31250;
	tios.c_ospeed = 31250;
	while(ioctl(fd, TCSETS2, &tios) < 0) ;
	close(fd);

	return 0;
}

