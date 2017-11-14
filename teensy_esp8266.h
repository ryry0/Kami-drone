#include <Arduino.h>
#include <string.h>
//Wifi Defines
#define AT_RESET        "AT+RST\r\n"
#define AT_ACCEPT_CONNS "AT+CWMODE=3\r\n"
#define AT_STATION      "AT+CWMODE=1\r\n"
#define AT_FIRMWARE     "AT+GMR\r\n"
#define AT_SCAN         "AT+CWLAP\r\n"
#define AT_GET_IP       "AT+CIFSR\r\n"
#define AT_CREAT_SERVER "AT+CIPSERVER=1,5555\r\n"
#define AT_MULTI_CONN   "AT+CIPMUX=1\r\n"
#define AT_CREAT_AP     "AT+CWSAP=\"Drone\",\"kami\",3,0\r\n"
#define AT_QUERY_AP     "AT+CWSAP?\r\n"
#define AT_ECHO_OFF     "ATE0\r\n"
#define AT_ECHO_ON      "ATE1\r\n"
#define AT_GET_CLIENTS  "AT+CWLIF\r\n"

inline void wifi_sendCommand(HardwareSerial *hwserial, char *command) {
  (*hwserial).write((uint8_t *)command, strlen(command));
}
