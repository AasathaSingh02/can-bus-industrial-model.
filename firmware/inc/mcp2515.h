#ifndef MCP2515_H
#define MCP2515_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @file mcp2515.h
 * @brief Standalone SPI Driver for Microchip MCP2515 CAN Controller
 * @author Aasatha Singh
 */

/* MCP2515 SPI Instructions */
#define MCP_RESET             0xC0
#define MCP_READ              0x03
#define MCP_WRITE             0x02
#define MCP_RTS_TXB0          0x81
#define MCP_RTS_TXB1          0x82
#define MCP_RTS_TXB2          0x84
#define MCP_READ_STATUS       0xA0
#define MCP_RX_STATUS         0xB0
#define MCP_BIT_MODIFY        0x05

/* MCP2515 Registers */
#define CANCTRL               0x0F
#define CANSTAT               0x0E
#define CNF1                  0x2A
#define CNF2                  0x29
#define CNF3                  0x28
#define CANINTE               0x2B
#define CANINTF               0x2C
#define EFLG                  0x2D
#define TEC                   0x1C
#define REC                   0x1D

/* TX Buffer 0 Registers */
#define TXB0CTRL              0x30
#define TXB0SIDH              0x31
#define TXB0SIDL              0x32
#define TXB0EID8              0x33
#define TXB0EID0              0x34
#define TXB0DLC               0x35
#define TXB0D0                0x36

/* RX Buffer 0 Registers */
#define RXB0CTRL              0x60
#define RXB0SIDH              0x61
#define RXB0SIDL              0x62
#define RXB0EID8              0x63
#define RXB0EID0              0x64
#define RXB0DLC               0x65
#define RXB0D0                0x66

/* Operational Modes */
typedef enum {
    CAN_MODE_NORMAL     = 0x00,
    CAN_MODE_SLEEP      = 0x20,
    CAN_MODE_LOOPBACK   = 0x40,
    CAN_MODE_LISTENONLY = 0x60,
    CAN_MODE_CONFIG     = 0x80
} CAN_MODE_t;

/* CAN Frame Structure */
typedef struct {
    uint32_t id;          /* Standard (11-bit) or Extended (29-bit) ID */
    bool is_extended;     /* true if 29-bit ID */
    bool is_rtr;          /* Remote Transmission Request flag */
    uint8_t dlc;          /* Data Length Code (0-8) */
    uint8_t data[8];      /* Payload bytes */
} CAN_Frame_t;

/* Function Prototypes */
bool MCP2515_Init(uint8_t speed_cnf1, uint8_t speed_cnf2, uint8_t speed_cnf3);
bool MCP2515_SetMode(CAN_MODE_t mode);
bool MCP2515_Transmit(const CAN_Frame_t *frame);
bool MCP2515_Receive(CAN_Frame_t *frame);
bool MCP2515_CheckReceive(void);
uint8_t MCP2515_GetErrorFlags(void);
uint8_t MCP2515_GetTEC(void);
uint8_t MCP2515_GetREC(void);
void MCP2515_SetFilterMask(uint8_t mask_reg, uint32_t mask, bool ext);
void MCP2515_Reset(void);

#endif /* MCP2515_H */
