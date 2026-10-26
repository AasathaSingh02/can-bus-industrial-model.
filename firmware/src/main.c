#include <stdio.h>
#include "can_config.h"
#include "mcp2515.h"
#include "can_protocol.h"
#include "rs485.h"
#include "sensor_node.h"

/**
 * @file main.c
 * @brief Industrial CAN Node Main Control Loop
 * @author Aasatha Singh
 */

int main(void) {
    Node_State_t node;
    Node_Init(&node, DEFAULT_NODE_ID);

    /* Initialize CAN controller at 250 kbps */
    bool can_ok = MCP2515_Init(CAN_BAUD_250K_CNF1, CAN_BAUD_250K_CNF2, CAN_BAUD_250K_CNF3);
    RS485_Init(115200);

    if (!can_ok) {
        /* Trap in error state if CAN init failed */
        while (1) {}
    }

    /* Main Operational Loop */
    while (1) {
        /* 1. Periodic Sensor Acquisition */
        Node_SampleSensors(&node);

        /* 2. Format & Transmit Temperature Telemetry */
        CAN_Frame_t tx_frame;
        Pack_Telemetry_Temp(&tx_frame, node.node_id, node.current_temperature_c, node.current_pressure_bar * 1000.0f);
        if (MCP2515_Transmit(&tx_frame)) {
            node.packets_sent++;
        } else {
            node.error_count++;
        }

        /* 3. Handle Inbound CAN Messages */
        if (MCP2515_CheckReceive()) {
            CAN_Frame_t rx_frame;
            if (MCP2515_Receive(&rx_frame)) {
                node.packets_received++;
                /* Forward critical alarms over RS485 for legacy PLCs */
                if (rx_frame.id == MSG_ID_EMERGENCY_SHUTDOWN) {
                    RS485_Transmit(rx_frame.data, rx_frame.dlc);
                }
            }
        }

        /* 4. Fault Diagnostics */
        uint8_t eflg = MCP2515_GetErrorFlags();
        if (eflg & 0x20) { /* Bus-off state */
            MCP2515_Init(CAN_BAUD_250K_CNF1, CAN_BAUD_250K_CNF2, CAN_BAUD_250K_CNF3);
        }

        /* Delay loop iteration */
        for (volatile uint32_t i = 0; i < 50000; i++) {}
    }

    return 0;
}
