#ifndef CAN_PROTOCOL_H
#define CAN_PROTOCOL_H

#include <stdint.h>
#include <stdbool.h>
#include "mcp2515.h"

/**
 * @file can_protocol.h
 * @brief Application Layer Message Formats and Packet Serialization
 * @author Aasatha Singh
 */

/* CAN Message ID Definitions (Priority Based 11-bit IDs) */
#define MSG_ID_EMERGENCY_SHUTDOWN  0x010  /* Priority 0: Critical fault / E-Stop */
#define MSG_ID_HEARTBEAT_BASE      0x100  /* Priority 1: Node heartbeat (0x100 + NodeID) */
#define MSG_ID_TELEMETRY_TEMP      0x200  /* Priority 2: Temperature data (0x200 + NodeID) */
#define MSG_ID_TELEMETRY_CURRENT   0x210  /* Priority 3: Current & Voltage (0x210 + NodeID) */
#define MSG_ID_ALERT_WARNING       0x300  /* Priority 4: Threshold exceeded warning */
#define MSG_ID_CONFIG_COMMAND      0x400  /* Priority 5: Remote parameter reconfiguration */

/* Packed Telemetry Data Structures */
typedef struct __attribute__((packed)) {
    uint8_t  node_id;
    int16_t  temperature_centi_c; /* Value in 0.01 deg C (-4000 to +15000) */
    uint16_t pressure_mbar;       /* Value in mbar */
    uint8_t  sensor_status;       /* Status flag bitfield */
    uint16_t checksum;            /* CRC-16-CCITT */
} Telemetry_TempPayload_t;

typedef struct __attribute__((packed)) {
    uint8_t  node_id;
    int16_t  current_ma;          /* Current in mA (+/- 30000) */
    uint16_t bus_voltage_mv;      /* Voltage in mV */
    uint8_t  alert_flags;
    uint16_t checksum;
} Telemetry_PowerPayload_t;

/* Protocol Helper Functions */
void Pack_Telemetry_Temp(CAN_Frame_t *frame, uint8_t node_id, float temp_c, float pressure);
void Pack_Telemetry_Power(CAN_Frame_t *frame, uint8_t node_id, float current_a, float voltage_v);
bool Unpack_Telemetry_Temp(const CAN_Frame_t *frame, float *out_temp_c, float *out_pressure);
uint16_t Calculate_CRC16(const uint8_t *data, uint8_t len);

#endif /* CAN_PROTOCOL_H */
