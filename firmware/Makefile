CC = arm-none-eabi-gcc
CFLAGS = -Wall -Wextra -O2 -mcpu=cortex-m3 -mthumb -I./inc
SRC = src/main.c src/mcp2515.c src/can_protocol.c src/rs485.c src/sensor_node.c

all:
	@echo "Building CAN Industrial Node Firmware..."
	@echo "To compile for ARM Cortex-M: arm-none-eabi-gcc $(CFLAGS) $(SRC)"

clean:
	rm -rf *.o build/
