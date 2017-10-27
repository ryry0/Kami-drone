#include <cobs.h>

/*
   cobs Encode in place does the encoding on the same array passed in, and does
   not require using a different array. This may be preferable for performance
   reasons. Encode in place assumes that the first byte is the header byte and
   the second byte is the code byte.
   */
void cobs_encodeInPlace(uint8_t* buffer, const uint8_t total_length) {
  unsigned int zero_byte_pos = 1;

  for (unsigned int i = 2; i < total_length; ++i) {
    if (buffer[i] == 0) {
      buffer[zero_byte_pos] = i - zero_byte_pos;
      zero_byte_pos = i;
    }
  }
  buffer[zero_byte_pos] = total_length - zero_byte_pos;
}

/*
   For destination, pass in an array that is 2 bytes larger than the source
   length.
   */
void cobs_encode(const uint8_t* source, const uint8_t length, uint8_t* destination) {
  destination [0] = 0;

  //copy source to destination offset by 2 bytes
  memcpy(destination + 2, source, length);
  cobs_encodeInPlace(destination, length + 2);
}

/*
   Decode in place does the decoding on the same array passed in. Decode in place
   assumes that the first byte is the header byte and the second byte is the code
   byte.
   */
void cobs_decodeInPlace(uint8_t* buffer, const uint8_t length) {
  unsigned int next_zero_byte_pos = buffer[1];
  unsigned int current_zero_byte_pos = 1;

  for (unsigned int i = 2; i < length; ++i) {
    if (i - current_zero_byte_pos == next_zero_byte_pos) {
      next_zero_byte_pos = buffer[i];
      current_zero_byte_pos = i;
      buffer[i] = 0;
    }
  }
}

/*
   For destination, pass in an array that is at least as large as the source
   length minus 2.
   Strips the  header and code byte from the data.
   */
void cobs_decode(const uint8_t* source, const uint8_t length, uint8_t* destination) {
  uint8_t* buffer = (uint8_t*) malloc(length);

  memcpy(buffer, source, length);
  cobs_decodeInPlace(buffer, length);
  memcpy(destination,buffer+2, length-2);

  free(buffer);
}
