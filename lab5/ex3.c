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

void  clean_buffer(MY_FILE *stream) {
  stream->offset = 0;
	for (int i = 0; i < 4096; i++) {
		stream->buffer[i] = '\0';
	}
}

size_t my_fwrite(const void *ptr, size_t size, size_t nmemb, MY_FILE *stream) {
	/*clean_buffer(stream);
	const char* ptrChar;
	ptrChar = ptr;
	int count = 0;
	for (int i = 0; i < nmemb; i++) {
		for (int j = 0; j < size; j++) {
		  stream->buffer[stream->offset] = ptrChar[j + i * size];
			stream->offset ++;
			if (stream->offset >= 4096) {
				size_t write_size = write(stream->fd, stream->buffer, 4096);
				if (write_size == -1) {
					return -1;
				}
				clean_buffer(stream);
			}
		}
		count++;
	}
	return count;*/

	size_t write_size = write(stream->fd, ptr, size*nmemb);
	return write_size / size;
}
