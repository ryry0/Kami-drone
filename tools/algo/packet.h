/** \file
 * \author Ryan-David Reyes
 *
 * \brief This is an implementation of a binary communication protocol with
 * packet framing.
 */
#ifndef PACKET_H_
#define PACKET_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

//@{
/** \brief General use constants */
#define MAX_PAYLOAD_LENGTH 251
#define PKT_HEADER_LENGTH 3
#define PKT_TYPE_LENGTH 1
//@}

//@{
/** \brief Payload interpretation macros
 *
 * Use these to easily cast the payload into a particular structure.
 * */
#define pkt_interp(type, packet) ((type *) (packet.payload))
#define pkt_interpPtr(type, packet) ((type *) (packet->payload))
//@}

/** \brief General packet structure */
typedef struct {
  union {
    struct {
      uint8_t header;
      uint8_t cobs_byte;
      uint8_t type_payload_length; //includes the type, so always nonzero
      uint8_t type;
      uint8_t payload[MAX_PAYLOAD_LENGTH];
    };
    uint8_t data[255];
  };
  size_t index;
  uint8_t total_length;
} pkt_generic_t;

/** \brief Callback function type */
typedef void (*pkt_handler_func_t)(pkt_generic_t *);

/**
 * \brief Initialize a generic packet structure.
 *
 * \param packet The packet structure.
 */
void pkt_init(pkt_generic_t *packet);


/**
 * \brief Read one byte from a stream and put it into the packet structure.
 *
 * \param packet The packet structure.
 * \param input The incoming byte from the stream.
 *
 * \return Returns true when the whole packet has been received.
 */
bool pkt_readByte(pkt_generic_t *packet, uint8_t input);

/**
 * \brief Decode the packet. Must call after readByte finishes to interpret the
 * packet correctly.
 *
 * \param packet The packet structure.
 */
void pkt_decodePacket(pkt_generic_t *packet);

/**
 * \brief Encode buffer performs encoding of the packet so that it is ready to send.
 *
 * Assumes total length field has been set.
 *
 * \param packet The packet structure.
 * \return a byte pointer to the packet structure for serialization.
 */
uint8_t *pkt_encodeBuffer(pkt_generic_t *packet);

/**
 * \brief Print the contents of the packet.
 *
 * \param packet The packet structure.
 */
void pkt_print(pkt_generic_t *packet);

/**
 * \brief Clear the contents of the packet structure to zero.
 *
 * \param packet The packet structure.
 */
void pkt_clear(pkt_generic_t *packet);

/**
 * \brief Function abstracting most common use case.
 *
 * Pass in a packet, and a byte from the byte stream. When the packet has
 * arrived, it automatically decodes the packet and calls the callback function.
 *
 * \param packet The packet structure.
 * \param input The incoming byte from the stream.
 * \param callback Function to call upon completion.
 *
 * \return Returns true when whole packet has been received.
 */
bool pkt_decodeByteHandler(pkt_generic_t *packet, uint8_t input,
    pkt_handler_func_t callback);

/** \brief Returns the current index of the packet. */
inline size_t pkt_getIndex(const pkt_generic_t *packet) { return packet->index; }

/** \brief Returns the total length of the packet. */
inline size_t pkt_getTotalLength(const pkt_generic_t *packet)
{ return packet->total_length; }

/** \brief Sets the total length of the packet, used when sending. */
inline void pkt_setTotalLength(pkt_generic_t *packet, size_t total_length)
{ packet->total_length = total_length; }

/**
 * \brief Convenience function to set the header of a generic packet.
 *
 * \param packet The packet structure.
 * \param type The type of message
 * \param length The length of the payload and the type byte.
 */
inline void pkt_setHeader(pkt_generic_t *packet, uint8_t type, size_t payload_length) {
  packet->header = 0x00;
  packet->cobs_byte = 0x00;
  packet->type = type;
  packet->type_payload_length = PKT_TYPE_LENGTH + payload_length;
  packet->total_length = PKT_HEADER_LENGTH + packet->type_payload_length;
}

/**
 * \brief Convenience function to set a type-only no-payload packet.
 *
 * \param packet The packet structure.
 * \param type The type of message
 */
inline void pkt_setHeaderTypeOnly(pkt_generic_t *packet, uint8_t type) {
  pkt_setHeader(packet, type, 0);
}

#endif
