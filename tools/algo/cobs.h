/** \file
 *
 * \author Ryan-David Reyes
 *
 * \brief This is an implementation of the COBS packet framing algorithm.
 */
#ifndef COBS_H_
#define COBS_H_

#include <string.h>
#include <stdlib.h>
#include <stdint.h>

/**
 * \brief Performs COBS encoding on the array passed in.
 *
 * Does not require using a different array. This may be preferable for performance
 * reasons. Encode in place assumes that the first byte is the header byte and
 * the second byte is the code byte.
 *
 * \param buffer Array to perform encoding on.
 * \param length Length of the array.
 */
void cobs_encodeInPlace(uint8_t* buffer, const uint8_t total_length);

/**
 * \brief Takes bytes from source, performs COBS encoding, places it in
 * destination.
 *
 * For destination buffer, pass in an array that is 2 bytes larger than the source
 * length.
 *
 * \param source Source array.
 * \param length Length of source array.
 * \param destination Destination array.
 */
void cobs_encode(const uint8_t* source,
    const uint8_t length,
    uint8_t* destination);


/**
 * \brief Decode in place does the decoding on the array passed in.
 *
 * Decode in place assumes that the first byte is the header byte and the second
 * byte is the code byte.
 *
 * \param buffer Array to perform decoding on.
 * \param length Length of the array.
 */
void cobs_decodeInPlace(uint8_t* buffer,
    const uint8_t length);


/**
 * \brief Takes data from source, decodes it, and places it in destination.
 *
 * For destination, pass in an array that is at least as large as the source
 * length minus 2.
 * Strips the  header and code byte from the data.
 *
 * \param source Source array.
 * \param length Length of source array.
 * \param destination Destination array.
 */
void cobs_decode(const uint8_t* source,
    const uint8_t length,
    uint8_t* destination);

#endif
