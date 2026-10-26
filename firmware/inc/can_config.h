#ifndef CAN_CONFIG_H
#define CAN_CONFIG_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @file can_config.h
 * @brief Configuration parameters for STM32 + MCP2515 CAN 2.0B Industrial Node
 * @author Aasatha Singh
 */

/* CAN Baud Rate Prescalers (for 16 MHz MCP2515 Crystal) */
#define CAN_BAUD_1000K_CNF1   0x00
#define CAN_BAUD_1000K_CNF2   0x80
#define CAN_BAUD_1000K_CNF3   0x00

#define CAN_BAUD_500K_CNF1    0x00
#define CAN_BAUD_500K_CNF2    0x90
#define CAN_BAUD_500K_CNF3    0x02

#define CAN_BAUD_250K_CNF1    0x41
#define CAN_BAUD_250K_CNF2    0xF1
#define CAN_BAUD_250K_CNF3    0x85

#define CAN_BAUD_125K_CNF1    0x03
#define CAN_BAUD_125K_CNF2    0xAC
#define CAN_BAUD_125K_CNF3    0x03

/* Default Network Configuration */
#define DEFAULT_NODE_ID       0x01
#define CAN_TIMEOUT_MS        100
#define MAX_RETRY_COUNT       3
#define HEARTBEAT_INTERVAL_MS 1000

/* Hardware SPI Pinout mapping (STM32F103 SPI1) */
#define MCP_CS_PIN            GPIO_PIN_4
#define MCP_CS_PORT           GPIOA
#define MCP_INT_PIN           GPIO_PIN_0
#define MCP_INT_PORT          GPIOB

/* RS485 Direction Control Pin */
#define RS485_DE_RE_PIN       GPIO_PIN_1
#define RS485_DE_RE_PORT      GPIOA

#endif /* CAN_CONFIG_H */
