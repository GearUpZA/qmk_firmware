# Build Options
#   change yes to no to disable
#
# Note: Most features are now configured in keyboard.json
# Only hardware-specific settings remain here

# Hardware drivers and custom source files
SRC += joystick.c config_mode.c

# Joystick functionality is enabled via keyboard.json features section
# and automatically includes the analog driver when needed
