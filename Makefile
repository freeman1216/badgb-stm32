CC = arm-none-eabi-gcc
CFLAGS = -ggdb -O2 -flto -DBAD_HANDLERS_USE_UART -Wall -fjump-tables -mcpu=cortex-m4 -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard
LDFLAGS = -Tstm32f411ceu6.ld -nolibc --specs=nosys.specs -nostartfiles
INCLUDES = -Iinc/ -Iinc/app -Iinc/driver/

SRC_DIR = src
BUILD_DIR = build
SOURCES = \
	$(SRC_DIR)/startup_stm32f411ceu6.c\
	$(SRC_DIR)/main.c\
	$(SRC_DIR)/handlers.c\
	$(SRC_DIR)/app/cpu.c\
	$(SRC_DIR)/app/ppu.c\
	$(SRC_DIR)/app/timer.c\
	$(SRC_DIR)/app/mem.c\
	$(SRC_DIR)/app/badstate.c
BIN = $(BUILD_DIR)/badgb-stm32.elf

all: $(BUILD_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(INCLUDES) $(SOURCES) -o $(BIN)

clean:
	rm -f $(BUILD_DIR)/*.elf

###############
# Build dir   #
###############
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
