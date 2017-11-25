#include <Arduino.h>
#include <string.h>
//Wifi Defines
#define AT_RESET        "AT+RST\r\n"
#define AT_STATION      "AT+CWMODE=1\r\n"
#define AT_AP_MODE      "AT+CWMODE=2\r\n"
#define AT_DUAL_MODE    "AT+CWMODE=3\r\n"
#define AT_FIRMWARE     "AT+GMR\r\n"
#define AT_SCAN         "AT+CWLAP\r\n"
#define AT_GET_IP       "AT+CIFSR\r\n"
#define AT_CREAT_SERVER "AT+CIPSERVER=1,5555\r\n"
#define AT_MULTI_CONN   "AT+CIPMUX=1\r\n"
#define AT_CREAT_AP     "AT+CWSAP=\"Drone\",\"kami\",2,0\r\n"
#define AT_QUERY_AP     "AT+CWSAP?\r\n"
#define AT_ECHO_OFF     "ATE0\r\n"
#define AT_ECHO_ON      "ATE1\r\n"
#define AT_GET_CLIENTS  "AT+CWLIF\r\n"
#define AT_STATUS       "AT+CIPSTATUS\r\n"
#define AT_QUERY_TIMEOUT     "AT+CIPSTO?\r\n"
#define AT_SET_TIMEOUT  "AT+CIPSTO=3000\r\n"

inline void wifi_sendCommand(HardwareSerial *hwserial, char *command) {
  (*hwserial).write((uint8_t *)command, strlen(command));
}
