#include "can_protocol.h"
#include <string.h>

/**
 * @file can_protocol.c
 * @brief CAN Application Protocol serialization and CRC verification
 * @author Aasatha Singh
 */

uint16_t Calculate_CRC16(const uint8_t *data, uint8_t len) {
    uint16_t crc = 0xFFFF;
    for (uint8_t i = 0; i < len; i++) {
        crc ^= (uint16_t)data[i] << 8;
        for (uint8_t bit = 0; bit < 8; bit++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ 0x1021; /* CRC-16-CCITT */
            } else {
                crc = crc << 1;
            }
        }
    }
    return crc;
}

void Pack_Telemetry_Temp(CAN_Frame_t *frame, uint8_t node_id, float temp_c, float pressure) {
    if (!frame) return;

    frame->id = MSG_ID_TELEMETRY_TEMP + node_id;
    frame->is_extended = false;
    frame->is_rtr = false;
    frame->dlc = 8;

    int16_t t_int = (int16_t)(temp_c * 100.0f);
    uint16_t p_int = (uint16_t)pressure;

    frame->data[0] = node_id;
    frame->data[1] = (uint8_t)(t_int >> 8);
    frame->data[2] = (uint8_t)(t_int & 0xFF);
    frame->data[3] = (uint8_t)(p_int >> 8);
    frame->data[4] = (uint8_t)(p_int & 0xFF);
    frame->data[5] = 0x01; /* Sensor OK */

    uint16_t crc = Calculate_CRC16(frame->data, 6);
    frame->data[6] = (uint8_t)(crc >> 8);
    frame->data[7] = (uint8_t)(crc & 0xFF);
}

void Pack_Telemetry_Power(CAN_Frame_t *frame, uint8_t node_id, float current_a, float voltage_v) {
    if (!frame) return;

    frame->id = MSG_ID_TELEMETRY_CURRENT + node_id;
    frame->is_extended = false;
    frame->is_rtr = false;
    frame->dlc = 8;

    int16_t i_int = (int16_t)(current_a * 1000.0f);
    uint16_t v_int = (uint16_t)(voltage_v * 1000.0f);

    frame->data[0] = node_id;
    frame->data[1] = (uint8_t)(i_int >> 8);
    frame->data[2] = (uint8_t)(i_int & 0xFF);
    frame->data[3] = (uint8_t)(v_int >> 8);
    frame->data[4] = (uint8_t)(v_int & 0xFF);
    frame->data[5] = 0x00;

    uint16_t crc = Calculate_CRC16(frame->data, 6);
    frame->data[6] = (uint8_t)(crc >> 8);
    frame->data[7] = (uint8_t)(crc & 0xFF);
}

bool Unpack_Telemetry_Temp(const CAN_Frame_t *frame, float *out_temp_c, float *out_pressure) {
    if (!frame || frame->dlc < 8) return false;

    uint16_t received_crc = ((uint16_t)frame->data[6] << 8) | frame->data[7];
    uint16_t computed_crc = Calculate_CRC16(frame->data, 6);

    if (received_crc != computed_crc) {
        return false; /* CRC failure */
    }

    int16_t t_raw = ((int16_t)frame->data[1] << 8) | frame->data[2];
    uint16_t p_raw = ((uint16_t)frame->data[3] << 8) | frame->data[4];

    if (out_temp_c) *out_temp_c = (float)t_raw / 100.0f;
    if (out_pressure) *out_pressure = (float)p_raw;

    return true;
}
