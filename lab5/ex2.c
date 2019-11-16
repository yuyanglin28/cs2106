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

int load_buffer(MY_FILE *stream) {
	size_t read_size = read(stream->fd, stream->buffer, 4096);
	if (read_size == -1) {
		return -1;
	}
	for (int i = read_size; i < 4096; i++) {
		stream->buffer[i] = '\0';
	}
  stream->offset = 0;
	return 1;
}

size_t my_fread(void *ptr, size_t size, size_t nmemb, MY_FILE *stream) {
	char* ptrChar;
	ptrChar = ptr;
	if (stream->first_read) {
	  if (load_buffer(stream) == -1)
		  return -1;
		stream->first_read = 0;
	}
	int count = 0;
	for (int i = 0; i < nmemb; i++) {
		for (int j = 0; j < size; j++) {
			ptrChar[j + i * size] = stream->buffer[stream->offset];
			stream->offset ++;
			if (stream->offset >= 4096) {
				if (load_buffer(stream) == -1)
				  return -1;
			}
			if (stream->buffer[stream->offset] == '\0'){
				return count;
			}
		}
		count++;
	}
	return count;
}
