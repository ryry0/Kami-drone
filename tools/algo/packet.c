#include <packet.h>
#include <string.h>
#include <cobs.h>

#define PACKET_HEADER 0x00
#define HEADER_INDEX 0
#define COBS_INDEX 1
#define LENGTH_INDEX 2
#define TYPE_INDEX 3
//#define PRINTF_DEBUG

void pkt_init(pkt_generic_t *packet) {
  memset(packet, 0, sizeof(pkt_generic_t));
}


/* maybe only have it clear total_length bytes? */
void pkt_clear(pkt_generic_t *packet) {
  memset(packet, 0, sizeof(pkt_generic_t));
}

/* piecewise read returns true when finished reading */
bool pkt_readByte(pkt_generic_t *packet, uint8_t input) {
#ifdef PRINTF_DEBUG
  printf("%d:%x ", packet->index, input);
#endif

  if ((packet->index == 0) && (input != PACKET_HEADER))
    return false;

  packet->data[packet->index] = input;
  packet->index++;

  if ((packet->index > LENGTH_INDEX) &&
      (packet->index >= packet->type_payload_length + PKT_HEADER_LENGTH)) {
    packet->index = 0;
    packet->total_length = packet->type_payload_length + PKT_HEADER_LENGTH;

#ifdef PRINTF_DEBUG
    printf("\n");
#endif
    return true;
  }
  return false;
}

uint8_t *pkt_encodeBuffer(pkt_generic_t *packet) {
  cobs_encodeInPlace(packet->data, packet->total_length);
  return packet->data;
}

void pkt_decodePacket(pkt_generic_t *packet) {
  cobs_decodeInPlace(packet->data, packet->total_length);
}

bool pkt_decodeByteHandler(
    pkt_generic_t *packet,
    uint8_t input,
    pkt_handler_func_t callback) {

  bool retval = pkt_readByte(packet, input);
  if (retval) {
    pkt_decodePacket(packet);
    (*callback)(packet);
    pkt_clear(packet);
  }
  return retval;
}

/* piecewise read returns true when finished reading */
/*
static bool pkt_readInputBe(pkt_generic_t *packet, sr_port_t port) {
  uint8_t input = 0;

  sr_ReadPort(port, &input, sizeof(input));
  if ((packet->index == 0) && (input != PACKET_HEADER))
    return false;

  packet->data[MAX_PACKET_LENGTH - 1 - packet->index] = input;
  packet->index++;
  if (packet->index >= MAX_PACKET_LENGTH) {
    packet->index = 0;
    return true;
  }
  return false;
}
*/

void pkt_print(pkt_generic_t *packet) {
#ifdef PRINTF_DEBUG
  printf("header: %x\n", packet->header_byte);
  printf("imu_ax: %d\n", packet->imu_ax);
  printf("imu_ay: %d\n", packet->imu_ay);
  printf("imu_az: %d\n", packet->imu_az);
  printf("imu_gx: %d\n", packet->imu_gx);
  printf("imu_gy: %d\n", packet->imu_gy);
  printf("imu_gz: %d\n", packet->imu_gz);
#endif
}

