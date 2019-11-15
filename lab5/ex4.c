/*************************************
 * Lab 5 Ex4
 * Name:
 * Student No:
 * Lab Group:
 *************************************
 Warning: Make sure your code works on
 lab machine (Linux on x86)
 *************************************/

#include "my_stdio.h"

int my_fflush(MY_FILE *stream) {
	char ptr = 0;
	long size = lseek(stream->fd, 0, SEEK_END);
	printf("%ld\n", size);
	int buffer_size = 4096;
	size_t items_written = my_fwrite(&ptr, size, 1, stream);
	if (items_written != -1) {
		return 0;
	} else {
		return MY_EOF;
	}
}

int my_fseek(MY_FILE *stream, long offset, int whence) {
	if (whence == SEEK_SET) {
		lseek(stream->fd, offset, whence);
	  return offset;
	} else if (whence == SEEK_END) {
		long pre = lseek(stream->fd, 0, SEEK_END);
		lseek(stream->fd, offset, SEEK_END);
		return pre + offset;
	} else if (whence == SEEK_CUR){
		long pre = lseek(stream->fd, 0, SEEK_CUR);
		lseek(stream->fd, offset, SEEK_CUR);
		return pre + offset;
	} else {
		return -1;
	}
}
