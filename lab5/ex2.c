/*************************************
 * Lab 5 Ex2
 * Name:
 * Student No:
 * Lab Group:
 *************************************
 Warning: Make sure your code works on
 lab machine (Linux on x86)
 *************************************/
#include "my_stdio.h"

size_t my_fread(void *ptr, size_t size, size_t nmemb, MY_FILE *stream) {
	size_t to_read = size * nmemb;
  size_t bytes_read = read(stream->fd, ptr, to_read);
	if (bytes_read == -1) {
		return -1;
	} else if (bytes_read == 0){
		return 0;
	} else {
		return bytes_read / size;
	}

}
