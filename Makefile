# .c, .h, and .o files
SRC_DIR := ./src
INC_DIR := ./include
BUILD_DIR := ./build

TARGET_EXEC := $(BUILD_DIR)/rubot

# Get list of all .c files
SRCS := $(shell find $(SRC_DIR) -name '*.c')
# Get list of .o files from .c files
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Add include directory to compiler flags
INC_FLAGS := $(addprefix -I,$(INC_DIR))
CPPFLAGS := $(INC_FLAGS)

# Build target executable from .o files
$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# Build .o files from .c files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)

.PHONY: run
run: $(TARGET_EXEC)
	$(TARGET_EXEC)