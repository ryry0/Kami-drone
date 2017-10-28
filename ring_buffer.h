/** \file
 * \author Ryan-David Reyes
 *
 * \brief This file contains an implementation of a ring buffer, usable on
 * microcontrollers.
 */
#ifndef RING_BUFFER_H_
#define RING_BUFFER_H_

#include <stdint.h>
#include <stdlib.h>

/** \brief This is the structure that represents the ring buffer */
struct ring_buffer_s {
  float *buffer;
  size_t buffer_size; //full allocated size
  size_t head;
  size_t tail;
  size_t length; //number of items stored
};

/** \brief Opaque handle for the ring buffer */
typedef struct ring_buffer_s *ring_buffer_t;

/**
 * \brief Allocate a ring buffer handle of size size.
 *
 * \param size The maximum size of the ring buffer
 * \return An opaque handle to the ring buffer.
 */
ring_buffer_t rb_create(size_t size);

/**
 * \brief Initialize the buffer and set maximum size.
 *
 * Assumes the internal buffer is either set or already allocated.
 *
 * \param r_buffer The ring buffer
 * \param size The maximum size of the ring buffer
 */
void rb_init(ring_buffer_t r_buffer, size_t size);

/**
 * \brief This function pops all values off except for the tail.
 *
 * \param r_buffer The ring buffer
 * \param data_buff Pointer where popped values are copied.
 */
void rb_popAllButBack(ring_buffer_t r_buffer, float *data_buff);

/**
 * \brief This function pushes a value to the back of the ring buffer.
 *
 * Since this buffer is circular, when the buffer is full, the front value gets
 * popped off automatically.
 *
 * \param r_buffer The ring buffer
 * \param data Data to push onto the buffer.
 */
void rb_pushBack(ring_buffer_t r_buffer, float data);

/**
 * \brief This function prints the contents of the buffer.
 *
 * \param r_buffer The ring buffer
 */
void rb_print(ring_buffer_t r_buffer);

/**
 * \brief This function deallocates the buffer.
 *
 * \param r_buffer The ring buffer
 */
void rb_destroy(ring_buffer_t r_buffer);

/**
 * \brief Reads the ring buffer into a traditional array, in order
 *
 * \param r_buffer The ring buffer
 * \param buffer Destination buffer
 */
void rb_read(ring_buffer_t r_buffer, float *buffer);

/**
 * \brief Returns the value at the index.
 *
 * \param r_buffer The ring buffer
 * \return The value at the index
 */
float rb_get(const ring_buffer_t r_buffer, size_t index);

/**
 * \brief Returns the number of elements in the buffer
 *
 * \param r_buffer The ring buffer
 * \return The size of the buffer
 */
size_t rb_getLength(const ring_buffer_t r_buffer);

/**
 * \brief Set the internally used buffer to use an external array
 *
 * \param r_buffer The ring buffer
 * \param buffer The external array
 */
void rb_setBuffer(ring_buffer_t r_buffer, float *buffer);

/**
 * \brief Push a value to the front of the buffer
 *
 * \param r_buffer The ring buffer
 * \param data Data to push onto the buffer.
 */
void rb_pushFront(ring_buffer_t r_buffer, float data);

#endif
