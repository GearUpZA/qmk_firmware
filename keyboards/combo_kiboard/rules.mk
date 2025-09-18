# MCU name
MCU = RP2040
BOOTLOADER = rp2040

# Build Options
#   change yes to no to disable
#
BOOTMAGIC_ENABLE = yes      # Enable Bootmagic Lite
MOUSEKEY_ENABLE = yes       # Mouse keys
EXTRAKEY_ENABLE = yes       # Audio control and System control
CONSOLE_ENABLE = no         # Console for debug
COMMAND_ENABLE = no         # Commands for debug and configuration
NKRO_ENABLE = yes           # Enable N-Key Rollover
BACKLIGHT_ENABLE = no       # Enable keyboard backlight functionality
RGBLIGHT_ENABLE = no        # Enable keyboard RGB underglow
AUDIO_ENABLE = no           # Audio output
UNICODE_ENABLE = no         # Unicode
BLUETOOTH_ENABLE = no       # Enable Bluetooth
WIRELESS_ENABLE = no        # Enable wireless
SPLIT_KEYBOARD = no         # Enable split keyboard support

# Custom features
COMBO_ENABLE = yes          # Enable combo functionality
JOYSTICK_ENABLE = yes       # Enable joystick functionality
POINTING_DEVICE_ENABLE = yes # Enable mouse/pointing device
ANALOG_DRIVER_REQUIRED = yes # Required for analog joystick inputs

# Custom source files
SRC += lib/profile_manager.c \
       lib/touch_interface.c

# Include paths for lib directory
VPATH += keyboards/combo_kiboard/lib

# Enable SPI for touchscreen
SPI_DRIVER_REQUIRED = yes

# Joystick configuration
JOYSTICK_DRIVER = analog

# Optimize for size due to feature complexity
OPT_DEFS += -DHAL_USE_ADC=TRUE
OPT_DEFS += -DHAL_USE_SPI=TRUE

# Link time optimization
LTO_ENABLE = yes

# Reduce memory usage
GRAVE_ESC_ENABLE = no
MAGIC_ENABLE = no
SPACE_CADET_ENABLE = no
