import machine
import json
import os
from ili9341 import Display
from xpt2046 import Touch
import time

class KeyboardConfig:
    def __init__(self):
        # Initialize display
        spi = machine.SPI(0,
                         baudrate=40000000,
                         polarity=0,
                         phase=0,
                         sck=machine.Pin(18),
                         mosi=machine.Pin(19),
                         miso=machine.Pin(16))

        self.display = Display(spi,
                             dc=machine.Pin(15),
                             cs=machine.Pin(17),
                             rst=machine.Pin(14),
                             width=240,
                             height=320)

        # Initialize touch
        self.touch = Touch(spi,
                          cs=machine.Pin(13),
                          int_pin=machine.Pin(12, machine.Pin.IN))

        self.config_file = "keymap.json"
        self.load_config()

    def load_config(self):
        try:
            with open(self.config_file, 'r') as f:
                self.keymap = json.load(f)
        except:
            # Default keymap if file doesn't exist
            self.keymap = {
                "combos": {
                    "A1": "KC_ESC",
                    "A2": "KC_TAB",
                    # ... Add more default combos
                }
            }
            self.save_config()

    def save_config(self):
        with open(self.config_file, 'w') as f:
            json.dump(self.keymap, f)

    def draw_menu(self):
        self.display.fill(0)  # Clear screen
        self.display.text("KiBoard Configuration", 10, 10, 0xFFFF)
        # Draw combo buttons
        y = 40
        for combo, keycode in self.keymap["combos"].items():
            self.display.text(f"{combo}: {keycode}", 10, y, 0xFFFF)
            y += 20

    def handle_touch(self, x, y):
        # Convert touch coordinates to menu selection
        selected_combo = y // 20 - 1
        if selected_combo >= 0 and selected_combo < len(self.keymap["combos"]):
            self.edit_combo(list(self.keymap["combos"].keys())[selected_combo])

    def edit_combo(self, combo):
        # Show keycode selection screen
        self.display.fill(0)
        self.display.text(f"Select keycode for {combo}", 10, 10, 0xFFFF)
        # Draw keycode options
        # Handle selection
        # Update config
        self.save_config()

    def run(self):
        while True:
            self.draw_menu()
            if self.touch.touched():
                x, y = self.touch.get_touch()
                self.handle_touch(x, y)
            time.sleep_ms(100)

if __name__ == "__main__":
    config = KeyboardConfig()
    config.run()
