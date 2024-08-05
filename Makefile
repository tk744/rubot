TARGET_EXEC = rubot

# .c, .h, and .o files
SRC_DIR := ./src
INC_DIR := ./include
BUILD_DIR := ./build

# Get list of all .c file paths
C_FILES := $(shell find $(SRC_DIR) -name '*.c')
# Get list of .o file paths from .c file paths
O_FILES := $(C_FILES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Add include directory to preprocessor flags
INC_FLAGS := $(addprefix -I,$(INC_DIR))
CPPFLAGS := $(INC_FLAGS)

# Add C standard to compiler flags
CFLAGS := -std=gnu89

# Build target executable from .o files
$(TARGET_EXEC): $(O_FILES)
	$(CC) $(O_FILES) -o $@ $(LDFLAGS)

# Build .o files from .c files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Remove all build and executable files
.PHONY: clean
clean:
	rm -r $(TARGET_EXEC) $(BUILD_DIR)

# Install to Linux system
.SILENT: install
install: $(TARGET_EXEC)
	install -m 755 $(TARGET_EXEC) /usr/bin

# Uninstall from Linux system
.SILENT: uninstall
uninstall: $(TARGET_EXEC)
	rm -f /usr/bin/$(TARGET_EXEC)