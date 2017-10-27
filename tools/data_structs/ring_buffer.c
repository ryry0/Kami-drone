#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ring_buffer.h>

//void pointer to buffer? send in datatype size, cast outside? is that safe?

void rb_init(ring_buffer_t r_buffer, size_t size) {
  memset(r_buffer->buffer, 0, size*sizeof(float));

  r_buffer->buffer_size = size;
  r_buffer->head = 0;
  r_buffer->tail = 0;
  r_buffer->length = 0;
}

void rb_setBuffer(ring_buffer_t r_buffer, float *buffer) {
  r_buffer->buffer = buffer;
}

ring_buffer_t rb_create(size_t size) {
  ring_buffer_t r_buffer = malloc(sizeof(struct ring_buffer_s));
  r_buffer->buffer = malloc(sizeof(float)*size);

  rb_init(r_buffer, size);

  return r_buffer;
}

void rb_popAllButBack(ring_buffer_t r_buffer, float *data_buff) {
  for (size_t i = 0; i < r_buffer->length - 1; i++) {
    data_buff[i] = r_buffer->buffer[(r_buffer->head + i) % r_buffer->buffer_size];
  }

  if (r_buffer->tail == 0)
    r_buffer->head = r_buffer->buffer_size -1;

  else
    r_buffer->head = (r_buffer->tail - 1) % r_buffer->buffer_size;

  r_buffer->length =1;
}

void rb_pushBack(ring_buffer_t r_buffer, float data) {

  r_buffer->buffer[r_buffer->tail] = data;

  r_buffer->tail = (r_buffer->tail + 1) % r_buffer->buffer_size;

  if (r_buffer->length == r_buffer->buffer_size)
    r_buffer->head = (r_buffer->head + 1) % r_buffer->buffer_size;
  else
    r_buffer->length++;

} //end void pushBack

void rb_pushFront(ring_buffer_t r_buffer, float data) {

  if (r_buffer->head == 0)
    r_buffer->head = r_buffer->buffer_size -1;
  else
    r_buffer->head = r_buffer->head - 1;

  r_buffer->buffer[r_buffer->head] = data;

  if (r_buffer->length == r_buffer->buffer_size) {
    if (r_buffer->tail == 0)
      r_buffer->tail = r_buffer->buffer_size -1;
    else
      r_buffer->tail = r_buffer->tail - 1;
  }
  else {
    r_buffer->length++;
  }
} //end void pushFront

void rb_print(ring_buffer_t r_buffer) {
  for (size_t i = 0; i < r_buffer->length; i++) {
    printf("%c", r_buffer->buffer[(r_buffer->head + i) % r_buffer->buffer_size]);
  }
  printf("\t length: %ld\n", r_buffer->length);
  printf("\n");
}

void rb_read(ring_buffer_t r_buffer, float *buffer) {
  size_t i = 0;
  for (i = 0; i < r_buffer->length; i++) {
    buffer[i] =
      r_buffer->buffer[ (r_buffer->head + i) %
      r_buffer->buffer_size ];
  }
}

float rb_get(const ring_buffer_t r_buffer, size_t index) {
      return r_buffer->buffer[ (r_buffer->head + index) % r_buffer->buffer_size ];
}

void rb_destroy(ring_buffer_t r_buffer) {
  free(r_buffer->buffer);
}

size_t rb_getLength (const ring_buffer_t r_buffer) {
  return r_buffer->length;
}
