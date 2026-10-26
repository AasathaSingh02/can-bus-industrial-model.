# CAN Bus Industrial Monitoring Node

<p align="center">
  <img src="https://img.shields.io/badge/Platform-STM32F103-blue?style=flat-square&logo=stmicroelectronics" />
  <img src="https://img.shields.io/badge/Protocol-CAN%202.0B-brightgreen?style=flat-square" />
  <img src="https://img.shields.io/badge/Protocol-RS485-yellow?style=flat-square" />
  <img src="https://img.shields.io/badge/License-MIT-green?style=flat-square" />
</p>

A multi-node **CAN 2.0B** communication system for real-time industrial sensor monitoring. Each node reads local sensors and broadcasts data on a shared CAN bus with hardware message filtering, automatic error recovery, and a secondary **RS485** interface for legacy equipment compatibility. Validated across a **4-node network** with **99.7% packet delivery** over 10,000 transmissions.

---

## System Architecture

```
 ┌──────────────────────┐     ┌──────────────────────┐
 │      NODE 1          │     │      NODE 2          │
 │  ┌───────┐  ┌──────┐ │     │  ┌───────┐  ┌──────┐ │
 │  │STM32  ├──┤MCP   │ │     │  │STM32  ├──┤MCP   │ │
 │  │F103   │  │2515  │ │     │  │F103   │  │2515  │ │
 │  │       │SPI│      │ │     │  │       │SPI│      │ │
 │  └───┬───┘  └──┬───┘ │     │  └───┬───┘  └──┬───┘ │
 │      │      ┌──┴───┐ │     │      │      ┌──┴───┐ │
 │  Sensors    │TJA   │ │     │  Sensors    │TJA   │ │
 │  (I2C/     │1050  │ │     │  (I2C/     │1050  │ │
 │   ADC)      └──┬───┘ │     │   ADC)      └──┬───┘ │
 └────────────────┼─────┘     └────────────────┼─────┘
                  │                            │
    ──────────────┴────────────────────────────┴──────── CAN_H
    ──────────────┬────────────────────────────┬──────── CAN_L
                  │                            │
  [120Ω]─────────┘                            └────[120Ω]
  Termination                                  Termination
                  │                            │
 ┌────────────────┼─────┐     ┌────────────────┼─────┐
 │      NODE 3    │     │     │      NODE 4    │     │
 │           ┌────┴──┐  │     │           ┌────┴──┐  │
 │           │TJA1050│  │     │           │TJA1050│  │
 │  STM32    └───┬───┘  │     │  STM32    └───┬───┘  │
 │  + MCP2515   SPI     │     │  + MCP2515   SPI     │
 │  + MAX485 (RS485)    │     │  + Sensors          │
 └──────────────────────┘     └──────────────────────┘
         │
         ├─── RS485_A ──────── To legacy serial devices
         └─── RS485_B ────────
```

## Features

- **CAN 2.0B** with 29-bit extended identifiers
- **Hardware message filtering** via MCP2515 acceptance masks — CPU handles only relevant messages
- **Automatic bus-off recovery** with configurable retry logic
- **Configurable baud rates**: 125 kbps, 250 kbps, 500 kbps, 1 Mbps
- **RS485 secondary interface** (half-duplex) for legacy equipment
- **4-node test network** validated with 99.7% packet delivery
- **Error logging** with TEC/REC counters and error frame analysis

## Performance Results

| Metric | Value |
|--------|:-----:|
| Packet delivery rate | 99.7% (9,970 / 10,000) |
| Average latency (end-to-end) | 1.2 ms |
| Maximum latency | 3.8 ms |
| Bus utilization (at 250 kbps) | 34% |
| Error frames observed | 30 (all recovered) |
| Bus-off events | 0 |
| Test duration | 2 hours continuous |

## Directory Structure

```
can-bus-industrial-monitor/
├── firmware/
│   ├── inc/
│   │   ├── can_config.h       # CAN bus configuration & constants
│   │   ├── mcp2515.h          # MCP2515 CAN controller driver
│   │   ├── can_protocol.h     # Application-layer message protocol
│   │   ├── rs485.h            # RS485 transceiver driver (MAX485)
│   │   └── sensor_node.h      # Sensor reading & node management
│   ├── src/
│   │   ├── main.c             # Main application
│   │   ├── mcp2515.c          # MCP2515 SPI driver implementation
│   │   ├── can_protocol.c     # Message packing/unpacking
│   │   ├── rs485.c            # RS485 half-duplex communication
│   │   └── sensor_node.c      # Sensor sampling & data management
│   └── Makefile
├── test/
│   ├── can_test.py            # Python CAN bus test & validation
│   ├── packet_analysis.py     # Packet delivery & latency analysis
│   └── test_results.md        # Detailed test results
├── docs/
│   ├── can_message_format.md  # CAN message ID & data format spec
│   └── wiring_guide.md        # Hardware wiring guide
├── LICENSE
└── README.md
```

## CAN Message Protocol

### Standard Message Format (8 bytes max payload)

| Byte | Field | Description |
|:----:|-------|-------------|
| 0 | MSG_TYPE | Message type (0x01=sensor data, 0x02=status, 0x03=command) |
| 1 | NODE_ID | Source node identifier (0x01–0x04) |
| 2-3 | SENSOR_1 | 16-bit sensor reading (big-endian) |
| 4-5 | SENSOR_2 | 16-bit sensor reading (big-endian) |
| 6 | STATUS | Node status flags |
| 7 | CRC8 | CRC-8 checksum |

### CAN ID Assignment (29-bit Extended)

| ID Range | Priority | Assignment |
|----------|:--------:|------------|
| 0x00000001 – 0x000000FF | Highest | Emergency / fault messages |
| 0x00000100 – 0x000001FF | High | Sensor data (real-time) |
| 0x00000200 – 0x000002FF | Medium | Status & heartbeat |
| 0x00000300 – 0x000003FF | Low | Configuration & commands |

## Hardware Components

| Component | Part Number | Description |
|-----------|-------------|-------------|
| MCU | STM32F103C8T6 | ARM Cortex-M3, 72 MHz |
| CAN Controller | MCP2515-I/SO | Stand-alone CAN 2.0B with SPI |
| CAN Transceiver | TJA1050 | High-speed CAN transceiver |
| RS485 Transceiver | MAX485ESA | Half-duplex RS485 transceiver |
| Crystal | 8 MHz (MCP2515) | CAN bit timing reference |
| Termination | 120Ω (1%) | Bus termination resistor |

## Building & Flashing

```bash
cd firmware/
make clean && make
make flash    # ST-Link required
```

## References
- [MCP2515 Datasheet](https://ww1.microchip.com/downloads/en/devicedoc/20001801h.pdf) — Microchip
- [TJA1050 Datasheet](https://www.nxp.com/docs/en/data-sheet/TJA1050.pdf) — NXP
- [MAX485 Datasheet](https://www.analog.com/media/en/technical-documentation/data-sheets/MAX1487-MAX491.pdf) — Analog Devices
- CAN 2.0B Specification — Robert Bosch GmbH
- TIA/EIA-485 Standard

## License
MIT License — See [LICENSE](LICENSE) for details.
