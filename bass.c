#include "gbd.h"
#include <pigpio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

/*initialise shared memory for use with GBD. Function copied from GBD examples. */
static void *shm_init(const char *filename)
{
	int fd, ret = -1;
	void *lmap = NULL;
	size_t shm_filesize;

	fd = shm_open(filename, O_RDWR | O_CREAT, (mode_t) 0666);
	if(fd < 0){
     fprintf(stderr, "shm_open(3): %s\n", strerror(errno));
     goto exit;
	}

	shm_filesize = sysconf(_SC_PAGE_SIZE);
	ret = ftruncate(fd, shm_filesize);
	if (ret < 0) {
		fprintf(stderr, "ftruncate(2): %sn", strerror(errno));
		goto exit;
	}	
  
	lmap = mmap(0, shm_filesize, PROT_READ | PROT_WRITE, 
			       MAP_SHARED, fd, 0);
	if(lmap	== MAP_FAILED) {
  	fprintf(stderr, "mmap(2): %s\n", strerror(errno));
		goto exit;
	}

	return lmap;
exit:
	close(fd);
	return NULL;
}

int main(void){
	int *beats;
	int servo = 1500;
	int prevcnt[GBD_BEAT_COUNT_BUF_SIZE];
	
	gpioInitialise();
	gpioSetMode(17, PI_OUTPUT);
	gpioServo(17, servo);

	beats = (int *)shm_init(GBD_BEAT_COUNT_FILE);
	if(!beats){
		fprintf(stderr, "Could not open GBD IPC file!\n");
		return -1;
	}

	for(;;) {
		usleep(10000);

		if(beats[BASSLINE] != prevcnt[BASSLINE]){
			prevcnt[BASSLINE] = beats[BASSLINE];
			if (servo == 2300){
				gpioServo(17, 700);
				servo = 700;
			}else {
				gpioServo(17, 2300);
				servo = 2300;
			}
		}	
		
	}	
	return 0;
}	





