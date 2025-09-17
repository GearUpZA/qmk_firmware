# KiBoard - Quick Start Guide

Get your KiBoard up and running in minutes!

## What You Need

### Hardware Checklist
- ✅ Assembled KiBoard with all components connected
- ✅ USB-C cable
- ✅ Computer with USB port

### Software Requirements
- **QMK Toolbox** (for easy flashing) OR **QMK CLI** (for command line)
- **Web browser** (for testing)

## 5-Minute Setup

### Step 1: Download Firmware (2 minutes)
1. **Download** the latest `kiboard_default.uf2` firmware file
2. **Save** it to your desktop or downloads folder

### Step 2: Flash Firmware (2 minutes)
1. **Disconnect** KiBoard from computer
2. **Hold BOOTSEL button** on the Raspberry Pi Pico
3. **Connect USB cable** while holding BOOTSEL
4. **Release BOOTSEL** - computer shows "RPI-RP2" drive
5. **Drag and drop** `kiboard_default.uf2` to RPI-RP2 drive
6. **Wait for automatic reboot** - KiBoard is now ready!

### Step 3: Test Basic Functions (1 minute)
1. **Open** a text editor or [keyboard-test.com](https://keyboard-test.com)
2. **Press each key** (A, B, C, D, 1, 2, 3, 4) - should type letters/numbers
3. **Move joysticks** - should move mouse cursor
4. **Try a combo** - press A+1 together for spacebar

## Basic Usage

### Key Layout
```
Physical:   [A] [B] [C] [D]    [1] [2] [3] [4]
Default:    [A] [B] [C] [D]    [1] [2] [3] [4]
```

### Essential Combos (Press Together)
| Combo | Result | Use Case |
|-------|--------|----------|
| A + 1 | Space | Most common combo |
| A + 2 | Enter | Confirm actions |
| A + 3 | Tab | Navigate fields |
| A + 4 | Escape | Cancel/close |
| B + 1 | Shift | Modifier key |
| C + 1 | Ctrl+C | Copy |
| C + 2 | Ctrl+V | Paste |

### Joystick Modes
- **Default**: Digital mode (4-direction buttons per joystick)
- **Profile Cycling**: Hold A+B+C+D for 3 seconds to enter config mode

## Quick Configuration

### Enter Configuration Mode
1. **Hold** A+B+C+D keys together
2. **Count to 3** slowly
3. **Release** keys - touchscreen activates
4. **Use touch interface** to modify settings

### Change Joystick Profiles
1. **In config mode**: Touch "Joystick Profiles"
2. **Select profile**:
   - Profile 0: Both digital (default)
   - Profile 1: Left analog, right digital
   - Profile 2: Left digital, right analog
   - Profile 3: Both analog
3. **Touch "Apply"** to save changes

## Common Applications

### Gaming Setup
```
Profile: Left joystick = analog (camera)
         Right joystick = digital (UI navigation)

Keys: A=W, B=A, C=S, D=D (WASD movement)
      1=Space, 2=Shift, 3=Ctrl, 4=Tab
```

### Office/Productivity
```
Profile: Both joysticks = digital

Common combos:
- A+1 = Space (typing)
- C+1 = Ctrl+C (copy)
- C+2 = Ctrl+V (paste)
- B+3 = Alt+Tab (switch windows)
```

### Media Control
```
Keys: A=Previous, B=Play/Pause, C=Next, D=Stop
      1=Vol Up, 2=Vol Down, 3=Mute, 4=Eject

Joysticks: Navigation in media apps
```

## Troubleshooting

### Not Working?
| Problem | Quick Fix |
|---------|-----------|
| No response | Check USB connection, try different port |
| Wrong characters | Re-flash firmware, check keyboard layout |
| Joystick not moving | Verify joystick profile (try config mode) |
| Combos not working | Check timing - press keys simultaneously |
| Can't enter config mode | Ensure all 4 keys (A+B+C+D) pressed together |

### Reset to Defaults
1. **Enter BOOTSEL mode** (hold button while connecting USB)
2. **Re-flash** default firmware
3. **Test** basic functionality

## Next Steps

### Learn More
- **[Build Guide](BUILD_GUIDE.md)** - Hardware assembly details
- **[Configuration Guide](CONFIGURATION_GUIDE.md)** - Advanced settings
- **[Keymap Guide](KEYMAP_GUIDE.md)** - Custom layouts and combos
- **[Joystick Profiles](JOYSTICK_PROFILES.md)** - Detailed joystick options

### Customize Your KiBoard
1. **Try different profiles** for your specific needs
2. **Create custom combos** for your workflow
3. **Experiment with layers** for different applications
4. **Share your configurations** with the community

### Get Help
- **Check documentation** for detailed guides
- **Test systematically** if issues arise
- **Join community forums** for support and ideas

## Pro Tips

### Efficiency Tips
- **Learn 4-5 essential combos** first
- **Use joystick profiles** for different tasks
- **Practice muscle memory** for common combos
- **Customize for your workflow** rather than general use

### Advanced Features
- **Layer switching** for different app contexts
- **Macro recording** for repetitive tasks
- **Profile automation** based on active application
- **Touch interface** for real-time adjustments

---

**Congratulations!** Your KiBoard is ready to use. Start with basic combos and gradually explore advanced features as you become comfortable with the system.

**Need Help?** Check the [Troubleshooting Guide](TROUBLESHOOTING.md) or community resources.
