import board
import digitalio
import analogio
import time
import busio
from adafruit_ili9341 import ILI9341
from adafruit_xpt2046 import XPT2046

def test_display():
    spi = busio.SPI(board.GP18, board.GP19, board.GP16)
    display_cs = digitalio.DigitalInOut(board.GP17)
    display_dc = digitalio.DigitalInOut(board.GP15)
    display = ILI9341(spi, cs=display_cs, dc=display_dc)

    # Test display
    display.fill(0xFF0000) # Red
    time.sleep(1)
    display.fill(0x00FF00) # Green
    time.sleep(1)
    display.fill(0x0000FF) # Blue
    time.sleep(1)

def test_touch():
    spi = busio.SPI(board.GP18, board.GP19, board.GP16)
    touch_cs = digitalio.DigitalInOut(board.GP13)
    touch = XPT2046(spi, touch_cs)

    print("Touch the screen...")
    for _ in range(10):
        if touch.touched:
            x, y = touch.read_data()
            print(f"Touch at ({x}, {y})")
        time.sleep(0.1)

def test_keys():
    keys = []
    for pin in range(8):  # GP0 to GP7
        key = digitalio.DigitalInOut(getattr(board, f'GP{pin}'))
        key.direction = digitalio.Direction.INPUT
        key.pull = digitalio.Pull.UP
        keys.append(key)

    print("Press each key...")
    for _ in range(20):
        for i, key in enumerate(keys):
            if not key.value:  # Active LOW
                print(f"Key {i} pressed")
        time.sleep(0.1)

def test_joysticks():
    joy_pins = [
        analogio.AnalogIn(getattr(board, f'GP{pin}'))
        for pin in [26, 27, 28, 29]
    ]

    print("Move joysticks...")
    for _ in range(20):
        values = [pin.value for pin in joy_pins]
        print(f"Joy1: ({values[0]}, {values[1]}) Joy2: ({values[2]}, {values[3]})")
        time.sleep(0.1)

if __name__ == "__main__":
    print("Starting diagnostic tests...")
    try:
        test_display()
        test_touch()
        test_keys()
        test_joysticks()
        print("All tests completed")
    except Exception as e:
        print(f"Test failed: {e}")
