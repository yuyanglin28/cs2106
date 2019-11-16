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

	size_t write_size = write(stream->fd, stream->buffer, stream->offset);
	for (int i = 0; i < 4096; i++) {
		stream->buffer[i] = '\0';
	}
	stream->offset = 0;
	stream->first_read = 1;
	if (write_size != -1) {
		return 0;
	} else {
		return MY_EOF;
	}
}

int my_fseek(MY_FILE *stream, long offset, int whence) {
	if (my_fflush(stream) != 0) {
		return -1;
	}
	if (whence == SEEK_SET) {
	  return offset;
	} else if (whence == SEEK_END) {
		long pre = lseek(stream->fd, 0, SEEK_END);
		return pre + offset;
	} else if (whence == SEEK_CUR){
		long pre = lseek(stream->fd, 0, SEEK_CUR);
		return pre + offset;
	} else {
		return -1;
	}
}
