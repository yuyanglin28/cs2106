/*************************************
 * Lab 5 Ex3
 * Name:
 * Student No:
 * Lab Group:
 *************************************
 Warning: Make sure your code works on
 lab machine (Linux on x86)
 *************************************/

#include "my_stdio.h"

size_t my_fwrite(const void *p tr, size_t size, size_t nmemb, MY_FILE *stream) {
	size_t to_write = size * nmemb;
	size_t bytes_written;
	if (to_write < 4096) {
		bytes_written = write(stream->fd, ptr, to_write);
	} else {
		bytes_written = write(stream->fd, ptr, to_write);
	}
	//printf("to_write%d\n", to_write);
	//printf("write%d\n", bytes_written);
	if (bytes_written == -1) {
		return -1;
	} else if (bytes_written == 0){
		return 0;
	} else {
		return bytes_written / size;
	}
}
