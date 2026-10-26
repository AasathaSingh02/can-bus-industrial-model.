#ifndef SENSOR_NODE_H
#define SENSOR_NODE_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @file sensor_node.h
 * @brief Node Management, Sensor Sampling, and Health Diagnostics
 * @author Aasatha Singh
 */

typedef struct {
    uint8_t  node_id;
    float    current_temperature_c;
    float    current_pressure_bar;
    float    bus_current_a;
    float    bus_voltage_v;
    uint32_t packets_sent;
    uint32_t packets_received;
    uint32_t error_count;
    uint32_t uptime_seconds;
} Node_State_t;

void Node_Init(Node_State_t *node, uint8_t id);
void Node_SampleSensors(Node_State_t *node);
void Node_ProcessHeartbeat(Node_State_t *node);
void Node_CheckThresholds(Node_State_t *node);

#endif /* SENSOR_NODE_H */
