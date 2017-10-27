/**
 * \file serial.h
 * \author Ryan - David Reyes
 */
#ifndef SERIAL_H_
#define SERIAL_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/** \brief serial port handle */
typedef struct sr_port_s* sr_port_t;

/**
 * \brief Open a port and return a handle to the serial port
 *
 * \param serial_addr string of address to serial port
 */
sr_port_t sr_OpenPort(char *serial_addr); //returns an object or null

/**
 * \brief Initializes port with stop/startbits and 9600 baud
 *
 * \param port serial port handle
 */
bool sr_InitPort(sr_port_t port, int32_t baud_rate);

/**
 * \brief OS agnostic read into byte array
 *
 * \param port serial port object
 * \param buffer pointer to data
 * \param num number of bytes databuff holds
 * \return number of bytes read
 */
size_t sr_ReadPort(sr_port_t port, uint8_t *buffer, size_t num);

/**
 * \brief OS agnostic write from byte array
 *
 * \param port serial port object
 * \param buffer pointer to data
 * \param num number of bytes databuff holds
 * \return number of bytes read
 */
size_t sr_WritePort(sr_port_t port, uint8_t *buffer, size_t num);

/**
 * \brief Closes and deallocates serial port
 *
 * \param port serial port handle
 */
void sr_ClosePort(sr_port_t port);

#endif
