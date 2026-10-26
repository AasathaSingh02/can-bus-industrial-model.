#ifndef RS485_H
#define RS485_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @file rs485.h
 * @brief Half-Duplex RS-485 Communication Driver (MAX485 Transceiver)
 * @author Aasatha Singh
 */

typedef enum {
    RS485_MODE_RX = 0,
    RS485_MODE_TX = 1
} RS485_Mode_t;

void RS485_Init(uint32_t baudrate);
void RS485_SetDirection(RS485_Mode_t mode);
void RS485_Transmit(const uint8_t *buffer, uint16_t length);
bool RS485_Receive(uint8_t *buffer, uint16_t max_len, uint16_t *actual_len, uint32_t timeout_ms);

#endif /* RS485_H */
