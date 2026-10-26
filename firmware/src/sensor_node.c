#include "sensor_node.h"

/**
 * @file sensor_node.c
 * @brief Telemetry state tracking and anomaly threshold monitoring
 * @author Aasatha Singh
 */

void Node_Init(Node_State_t *node, uint8_t id) {
    if (!node) return;
    node->node_id = id;
    node->current_temperature_c = 25.0f;
    node->current_pressure_bar = 1.013f;
    node->bus_current_a = 0.45f;
    node->bus_voltage_v = 12.0f;
    node->packets_sent = 0;
    node->packets_received = 0;
    node->error_count = 0;
    node->uptime_seconds = 0;
}

void Node_SampleSensors(Node_State_t *node) {
    if (!node) return;
    /* Simulated sampling fluctuation for hardware emulation */
    node->current_temperature_c = 25.4f;
    node->current_pressure_bar = 1.025f;
    node->bus_current_a = 0.48f;
    node->bus_voltage_v = 11.98f;
}

void Node_ProcessHeartbeat(Node_State_t *node) {
    if (!node) return;
    node->uptime_seconds++;
}

void Node_CheckThresholds(Node_State_t *node) {
    if (!node) return;
    if (node->current_temperature_c > 85.0f) {
        node->error_count++;
    }
}
