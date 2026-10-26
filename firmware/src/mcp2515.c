#include "mcp2515.h"
#include "can_config.h"

/**
 * @file mcp2515.c
 * @brief MCP2515 CAN controller SPI driver implementation
 * @author Aasatha Singh
 */

/* Hardware SPI abstract wrappers */
static void SPI_Select(void) {
    /* PA4 Low (CS active) */
}

static void SPI_Deselect(void) {
    /* PA4 High (CS idle) */
}

static uint8_t SPI_TransferByte(uint8_t data) {
    /* Mocked hardware SPI transfer for compile independence */
    return data;
}

void MCP2515_Reset(void) {
    SPI_Select();
    SPI_TransferByte(MCP_RESET);
    SPI_Deselect();
}

static uint8_t MCP2515_ReadRegister(uint8_t address) {
    SPI_Select();
    SPI_TransferByte(MCP_READ);
    SPI_TransferByte(address);
    uint8_t result = SPI_TransferByte(0x00);
    SPI_Deselect();
    return result;
}

static void MCP2515_WriteRegister(uint8_t address, uint8_t value) {
    SPI_Select();
    SPI_TransferByte(MCP_WRITE);
    SPI_TransferByte(address);
    SPI_TransferByte(value);
    SPI_Deselect();
}

static void MCP2515_BitModify(uint8_t address, uint8_t mask, uint8_t value) {
    SPI_Select();
    SPI_TransferByte(MCP_BIT_MODIFY);
    SPI_TransferByte(address);
    SPI_TransferByte(mask);
    SPI_TransferByte(value);
    SPI_Deselect();
}

bool MCP2515_SetMode(CAN_MODE_t mode) {
    MCP2515_BitModify(CANCTRL, 0xE0, (uint8_t)mode);
    uint8_t status = MCP2515_ReadRegister(CANSTAT);
    return ((status & 0xE0) == (uint8_t)mode);
}

bool MCP2515_Init(uint8_t speed_cnf1, uint8_t speed_cnf2, uint8_t speed_cnf3) {
    MCP2515_Reset();

    /* Put into configuration mode */
    if (!MCP2515_SetMode(CAN_MODE_CONFIG)) {
        return false;
    }

    /* Configure bit timing registers */
    MCP2515_WriteRegister(CNF1, speed_cnf1);
    MCP2515_WriteRegister(CNF2, speed_cnf2);
    MCP2515_WriteRegister(CNF3, speed_cnf3);

    /* Enable RX interrupts for RXB0 and RXB1 */
    MCP2515_WriteRegister(CANINTE, 0x03);

    /* Accept all valid frames into RXB0 */
    MCP2515_WriteRegister(RXB0CTRL, 0x60);

    /* Switch to Normal operational mode */
    return MCP2515_SetMode(CAN_MODE_NORMAL);
}

bool MCP2515_Transmit(const CAN_Frame_t *frame) {
    if (!frame || frame->dlc > 8) return false;

    /* Check if TXB0 is busy */
    uint8_t ctrl = MCP2515_ReadRegister(TXB0CTRL);
    if (ctrl & 0x08) {
        return false; /* Buffer is currently pending transmission */
    }

    /* Load Standard ID */
    MCP2515_WriteRegister(TXB0SIDH, (uint8_t)(frame->id >> 3));
    MCP2515_WriteRegister(TXB0SIDL, (uint8_t)((frame->id & 0x07) << 5));

    /* Data Length Code */
    MCP2515_WriteRegister(TXB0DLC, frame->dlc);

    /* Load payload bytes */
    for (uint8_t i = 0; i < frame->dlc; i++) {
        MCP2515_WriteRegister(TXB0D0 + i, frame->data[i]);
    }

    /* Request-to-Send on Buffer 0 */
    SPI_Select();
    SPI_TransferByte(MCP_RTS_TXB0);
    SPI_Deselect();

    return true;
}

bool MCP2515_CheckReceive(void) {
    uint8_t status = MCP2515_ReadRegister(CANINTF);
    return (status & 0x01); /* RX0IF set */
}

bool MCP2515_Receive(CAN_Frame_t *frame) {
    if (!frame) return false;
    if (!MCP2515_CheckReceive()) return false;

    /* Read RXB0 identifier */
    uint8_t sidh = MCP2515_ReadRegister(RXB0SIDH);
    uint8_t sidl = MCP2515_ReadRegister(RXB0SIDL);
    frame->id = ((uint32_t)sidh << 3) | (sidl >> 5);
    frame->is_extended = false;

    /* Read DLC */
    frame->dlc = MCP2515_ReadRegister(RXB0DLC) & 0x0F;
    if (frame->dlc > 8) frame->dlc = 8;

    /* Read payload data */
    for (uint8_t i = 0; i < frame->dlc; i++) {
        frame->data[i] = MCP2515_ReadRegister(RXB0D0 + i);
    }

    /* Clear RX0 interrupt flag */
    MCP2515_BitModify(CANINTF, 0x01, 0x00);
    return true;
}

uint8_t MCP2515_GetErrorFlags(void) {
    return MCP2515_ReadRegister(EFLG);
}

uint8_t MCP2515_GetTEC(void) {
    return MCP2515_ReadRegister(TEC);
}

uint8_t MCP2515_GetREC(void) {
    return MCP2515_ReadRegister(REC);
}
