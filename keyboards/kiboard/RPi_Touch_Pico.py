from machine import Pin,I2C,SPI,PWM,Timer
import framebuf
import time
import json
import os

LCD_Width  = 320
LCD_Height = 240


#Pin definition
I2C_SDA = 6
I2C_SCL = 7
I2C_IRQ = 17
I2C_RST = 16

DC = 14
CS = 9
SCK = 10
MOSI = 11
MISO = 12
RST = 13
BL = 15

CST328_LCD_TOUCH_MAX_POINTS = 5

# Define CST328_Touch structure
class CST328_Touch:
    def __init__(self):
        self.points = 0
        self.coords = [{"x": 0, "y": 0, "strength": 0} for _ in range(CST328_LCD_TOUCH_MAX_POINTS)]
        self.state = 0

class Touch_Data:
    def __init__(self):
        self.x = 0
        self.y = 0
        self.points = 0

cst328_data  =  CST328_Touch()
touch_data  =  CST328_Touch()
Touch_Data_Old  =  Touch_Data()

#LCD Driver
class LCD_1inch69(framebuf.FrameBuffer):
    def __init__(self):
        self.width = LCD_Width
        self.height = LCD_Height

        self.cs = Pin(CS,Pin.OUT)
        self.rst = Pin(RST,Pin.OUT)

        self.cs(1)
        self.spi = SPI(1,100_000_000,polarity=0, phase=0,bits= 8,sck=Pin(SCK),mosi=Pin(MOSI),miso=None)
        self.dc = Pin(DC,Pin.OUT)
        self.dc(1)
        self.buffer = bytearray(self.height * self.width * 2)
        super().__init__(self.buffer, self.width, self.height, framebuf.RGB565)
        self.init_display()

        #Define colors
        self.red   =   0x07E0
        self.green =   0x001f
        self.blue  =   0xf800
        self.white =   0xffff
        self.black =   0x0000
        self.brown =   0X8430
        self.gray  =   0x8410
        self.yellow =  0x07FF

        self.fill(self.white)
        self.show()

        self.pwm = PWM(Pin(BL))
        self.pwm.freq(5000)

    def write_cmd(self, cmd):
        self.cs(1)
        self.dc(0)
        self.cs(0)
        self.spi.write(bytearray([cmd]))
        self.cs(1)

    def write_data(self, buf):
        self.cs(1)
        self.dc(1)
        self.cs(0)
        self.spi.write(bytearray([buf]))
        self.cs(1)

    def set_bl_pwm(self,duty):
        self.pwm.duty_u16(duty)

    def init_display(self):
        """Initialize dispaly"""
        self.rst(1)
        time.sleep(0.01)
        self.rst(0)
        time.sleep(0.01)
        self.rst(1)
        time.sleep(0.01)

        self.write_cmd(0x36)
        self.write_data(0x70)  # Change this value to 0x70 for 90-degree rotation

        self.write_cmd(0x3A)
        self.write_data(0x05)

        self.write_cmd(0xB2)
        self.write_data(0x0B)
        self.write_data(0x0B)
        self.write_data(0x00)
        self.write_data(0x33)
        self.write_data(0x35)

        self.write_cmd(0xB7)
        self.write_data(0x11)

        self.write_cmd(0xBB)
        self.write_data(0x35)

        self.write_cmd(0xC0)
        self.write_data(0x2C)

        self.write_cmd(0xC2)
        self.write_data(0x01)

        self.write_cmd(0xC3)
        self.write_data(0x0D)

        self.write_cmd(0xC4)
        self.write_data(0x20)

        self.write_cmd(0xC6)
        self.write_data(0x13)

        self.write_cmd(0xD0)
        self.write_data(0xA4)
        self.write_data(0xA1)

        self.write_cmd(0xD6)
        self.write_data(0xA1)

        self.write_cmd(0xE0)
        self.write_data(0xF0)
        self.write_data(0x06)
        self.write_data(0x0B)
        self.write_data(0x0A)
        self.write_data(0x09)
        self.write_data(0x26)
        self.write_data(0x29)
        self.write_data(0x33)
        self.write_data(0x41)
        self.write_data(0x18)
        self.write_data(0x16)
        self.write_data(0x15)
        self.write_data(0x29)
        self.write_data(0x2D)

        self.write_cmd(0xE1)
        self.write_data(0xF0)
        self.write_data(0x04)
        self.write_data(0x08)
        self.write_data(0x08)
        self.write_data(0x07)
        self.write_data(0x03)
        self.write_data(0x28)
        self.write_data(0x32)
        self.write_data(0x40)
        self.write_data(0x3B)
        self.write_data(0x19)
        self.write_data(0x18)
        self.write_data(0x2A)
        self.write_data(0x2E)

        self.write_cmd(0x21)
        self.write_cmd(0x11)
        time.sleep(0.12)
        self.write_cmd(0x29)

    def setWindows(self,Xstart,Ystart,Xend,Yend):
        self.write_cmd(0x2A)
        self.write_data(Xstart >> 8)
        self.write_data(Xstart)
        self.write_data((Xend-1) >> 8)
        self.write_data(Xend-1)

        self.write_cmd(0x2B)
        self.write_data((Ystart) >> 8)
        self.write_data(Ystart)
        self.write_data(((Ystart)-1) >> 8)
        self.write_data((Ystart)-1)

        self.write_cmd(0x2C)

    def show(self):
        self.setWindows(0,0,self.width,self.height)

        self.cs(1)
        self.dc(1)
        self.cs(0)
        self.spi.write(self.buffer)
        self.cs(1)

    def Windows_show(self,Xstart,Ystart,Xend,Yend):
        if Xstart > Xend:
            data = Xstart
            Xstart = Xend
            Xend = data

        if (Ystart > Yend):
            data = Ystart
            Ystart = Yend
            Yend = data

        if (Xend < 239):
            Xend = Xend + 1
        if (Yend < 319):
            Yend = Yend + 1

        self.setWindows(Xstart,Ystart,Xend,Yend)
        self.cs(1)
        self.dc(1)
        self.cs(0)
        for i in range (Ystart,Yend):
            Addr = (Xstart * 2) + (i * 240 * 2)
            self.spi.write(self.buffer[Addr : Addr+((Xend-Xstart)*2)])
        self.cs(1)

    def write_text(self,text,x,y,size,color):
        ''' Method to write Text on OLED/LCD Displays with a variable font size '''
        background = self.pixel(x,y)
        info = []
        self.text(text,x,y,color)
        for i in range(x,x+(8*len(text))):
            for j in range(y,y+8):
                px_color = self.pixel(i,j)
                info.append((i,j,px_color)) if px_color == color else None
        self.text(text,x,y,background)
        for px_info in info:
            self.fill_rect(size*px_info[0] - (size-1)*x , size*px_info[1] - (size-1)*y, size, size, px_info[2])

#Touch drive
class Touch_CST328(object):
    def __init__(self,address=0x1A,mode=0,i2c_num=1,i2c_sda=I2C_SDA,i2c_scl=I2C_SCL,irq_pin=I2C_IRQ,rst_pin=I2C_RST,LCD=None):
        self._bus = I2C(id=i2c_num,scl=Pin(i2c_scl),sda=Pin(i2c_sda),freq=400_000)
        self._address = address
        self.int=Pin(irq_pin,Pin.IN, Pin.PULL_UP)
        self.rst=Pin(rst_pin,Pin.OUT)
        self.Reset()
        bRet=self.CST328_Read_cfg()
        if bRet :
            print("Success:Detected CST328.")
        else    :
            print("Error: Not Detected CST328.")
            return None
        self.int.irq(handler=self.Int_Callback,trigger=Pin.IRQ_FALLING)

    def _read_nbyte(self, reg, length):
        reg_high = (reg >> 8) & 0xFF
        reg_low = reg & 0xFF
        self._bus.writeto_mem(int(self._address), reg_high, bytes([reg_low]))
        rec = self._bus.readfrom(int(self._address), length)
        return rec

    def _write_null(self, cmd):
        cmd_high = (cmd >> 8) & 0xFF
        cmd_low = cmd & 0xFF
        self._bus.writeto_mem(int(self._address), cmd_high, bytes([cmd_low]))

    def _write_nbyte(self, cmd, val):
        cmd_high = (cmd >> 8) & 0xFF
        cmd_low = cmd & 0xFF
        self._bus.writeto_mem(int(self._address), cmd_high, bytes([cmd_low, val]))

    def CST328_Read_cfg(self):
        buf = bytearray(24)
        HYN_REG_MUT_DEBUG_INFO_MODE         = 0xD101
        HYN_REG_MUT_NORMAL_MODE             = 0xD109
        HYN_REG_MUT_DEBUG_INFO_TP_NTX       = 0xD1F4
        HYN_REG_MUT_DEBUG_INFO_RES_X        = 0xD1F8
        HYN_REG_MUT_DEBUG_INFO_BOOT_TIME    = 0xD1FC

        self._write_null(HYN_REG_MUT_DEBUG_INFO_MODE)
        buf = self._read_nbyte(HYN_REG_MUT_DEBUG_INFO_BOOT_TIME, 4)
        print("TouchPad_ID:" + hex(buf[0]) + "," + hex(buf[1]) + "," + hex(buf[2]) + "," + hex(buf[3]))
        buf = self._read_nbyte(HYN_REG_MUT_DEBUG_INFO_RES_X, 4)
        print("TouchPad_X_MAX:" + str(buf[1]*256+buf[0]) + "    TouchPad_Y_MAX:" + str(buf[3]*256+buf[2]) + "\r\n")

        buf = self._read_nbyte(HYN_REG_MUT_DEBUG_INFO_TP_NTX, 24)
        print("D1F4:" + hex(buf[0]) + "," + hex(buf[1]) + "," + hex(buf[2]) + "," + hex(buf[3]))
        print("D1F8:" + hex(buf[4]) + "," + hex(buf[5]) + "," + hex(buf[6]) + "," + hex(buf[7]))
        print("D1FC:" + hex(buf[8]) + "," + hex(buf[9]) + "," + hex(buf[10]) + "," + hex(buf[11]))
        print("D200:" + hex(buf[12]) + "," + hex(buf[13]) + "," + hex(buf[14]) + "," + hex(buf[15]))
        print("D204:" + hex(buf[16]) + "," + hex(buf[17]) + "," + hex(buf[18]) + "," + hex(buf[19]))
        print("D208:" + hex(buf[20]) + "," + hex(buf[21]) + "," + hex(buf[22]) + "," + hex(buf[23]))
        print("CACA Read:" + hex((buf[11]<< 8) | buf[10]))

        self._write_null(HYN_REG_MUT_NORMAL_MODE)
        if (((buf[11] << 8) | buf[10]) != 0xCACA):
            return False
        return True

    def Int_Callback(self,pin):
        self.Touch_Read_Data()
        self.Touch_Get_XY()

    def Reset(self):
        self.rst(0)
        time.sleep_ms(1)
        self.rst(1)
        time.sleep_ms(50)

    def Touch_Read_Data(self):
        buf = bytearray(41)
        ESP_LCD_TOUCH_CST328_READ_Number_REG    = 0xD005
        ESP_LCD_TOUCH_CST328_READ_XY_REG        = 0xD000
        clear = 0
        touch_cnt = 0
        buf = self._read_nbyte( ESP_LCD_TOUCH_CST328_READ_Number_REG, 1)
        if ((buf[0] & 0x0F) == 0x00):
           self._write_nbyte(ESP_LCD_TOUCH_CST328_READ_Number_REG,clear)
        else :
            touch_cnt = buf[0] & 0x0F
            if (touch_cnt > CST328_LCD_TOUCH_MAX_POINTS or touch_cnt == 0) :
                self._write_nbyte(ESP_LCD_TOUCH_CST328_READ_Number_REG, clear)
                return True

            buf = self._read_nbyte( ESP_LCD_TOUCH_CST328_READ_XY_REG, 27)
            self._write_nbyte(ESP_LCD_TOUCH_CST328_READ_Number_REG, clear)

            if(touch_cnt > CST328_LCD_TOUCH_MAX_POINTS):
                touch_cnt = CST328_LCD_TOUCH_MAX_POINTS
            cst328_data.points = touch_cnt
            num = 0
            for i in range(touch_cnt):
                if(i>0):
                    num = 2
                cst328_data.coords[i]["x"] = ((buf[(i * 5) + 1 + num] << 4) + ((buf[(i * 5) + 3 + num] & 0xF0)>> 4))
                cst328_data.coords[i]["y"] = ((buf[(i * 5) + 2 + num] << 4) + ( buf[(i * 5) + 3 + num] & 0x0F))
                cst328_data.coords[i]["strength"] = (buf[(i * 5) + 4 + num])

    def Touch_Get_XY(self):
        if cst328_data.points:
            for i in range(cst328_data.points):
                # Adjust coordinates for 90-degree rotation
                raw_x = cst328_data.coords[i]["x"]
                raw_y = cst328_data.coords[i]["y"]
                touch_data.coords[i]["x"] = raw_y
                touch_data.coords[i]["y"] = LCD_Height - raw_x
                touch_data.coords[i]["strength"] = cst328_data.coords[i]["strength"]
            touch_data.state = 1
        else:
            touch_data.state = 0
        touch_data.points = cst328_data.points
        cst328_data.points = 0

# Keybind Configuration Application
class KeybindConfig:
    def __init__(self, lcd, touch):
        self.lcd = lcd
        self.touch = touch

        # Grid configuration
        self.columns = ["1", "12", "13", "14", "2", "23", "24", "3", "34", "4", " "]
        self.rows = ["A", "AB", "AC", "AD", "B", "BC", "BD", "C", "CD", "D", " "]

        # Cell dimensions and positions (reduced width to fit all columns)
        self.header_height = 20
        self.cell_width = 24
        self.cell_height = 16
        self.start_x = 40
        self.start_y = self.header_height + 5

        # Available keys (standard keyboard layout)
        self.available_keys = [
            'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
            'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
            '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
            'F1', 'F2', 'F3', 'F4', 'F5', 'F6', 'F7', 'F8', 'F9', 'F10', 'F11', 'F12',
            'ESC', 'TAB', 'CAPS', 'SHIFT', 'CTRL', 'ALT', 'SPACE', 'ENTER', 'BKSP', 'DEL',
            'UP', 'DOWN', 'LEFT', 'RIGHT', 'HOME', 'END', 'PGUP', 'PGDN', 'INS',
            '`', '-', '=', '[', ']', '\\', ';', "'", ',', '.', '/', '~', '!', '@',
            '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '{', '}', '|',
            ':', '"', '<', '>', '?'
        ]

        # Grid assignments (row, col) -> assigned key
        self.assignments = {}

        # Current state
        self.current_mode = "main"  # "main" or "keyboard"
        self.selected_cell = None
        self.last_touch_time = 0

        # EEPROM simulation file
        self.config_file = "keybind_config.json"

        # Load existing configuration
        self.load_config()

    def clear_config(self):
        """Clear all assignments and reset available keys"""
        # Reset available keys to original list
        self.available_keys = [
            'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
            'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
            '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
            'F1', 'F2', 'F3', 'F4', 'F5', 'F6', 'F7', 'F8', 'F9', 'F10', 'F11', 'F12',
            'ESC', 'TAB', 'CAPS', 'SHIFT', 'CTRL', 'ALT', 'SPACE', 'ENTER', 'BKSP', 'DEL',
            'UP', 'DOWN', 'LEFT', 'RIGHT', 'HOME', 'END', 'PGUP', 'PGDN', 'INS',
            '`', '-', '=', '[', ']', '\\', ';', "'", ',', '.', '/', '~', '!', '@',
            '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '{', '}', '|',
            ':', '"', '<', '>', '?'
        ]

        # Clear all assignments
        self.assignments.clear()

        # Show confirmation message
        self.lcd.fill_rect(110, 80, 100, 40, self.lcd.red)
        self.lcd.rect(110, 80, 100, 40, self.lcd.black)
        self.lcd.text("CLEARED!", 130, 95, self.lcd.white)
        self.lcd.show()
        time.sleep(1)

        # Redraw main grid
        self.draw_main_grid()

    def draw_main_grid(self):
        self.lcd.fill(self.lcd.white)

        # Draw title
        self.lcd.text("Keybind Config", 5, 2, self.lcd.black)

        # Draw column headers
        for col_idx, col_label in enumerate(self.columns):
            x = self.start_x + col_idx * self.cell_width + 8
            # Handle empty space label
            display_label = col_label if col_label.strip() else " "
            self.lcd.text(display_label, x, self.header_height - 10, self.lcd.black)

        # Draw row headers and grid
        for row_idx, row_label in enumerate(self.rows):
            y = self.start_y + row_idx * self.cell_height

            # Draw row header (handle empty space label)
            if row_label.strip():
                display_label = row_label if len(row_label) <= 4 else row_label[:4]
            else:
                display_label = " "
            self.lcd.text(display_label, 8, y + 4, self.lcd.black)

            # Draw row cells
            for col_idx in range(len(self.columns)):
                x = self.start_x + col_idx * self.cell_width

                # Check if this cell has an assignment
                cell_key = (row_idx, col_idx)
                if cell_key in self.assignments:
                    # Cell is assigned - draw in green
                    self.lcd.fill_rect(x, y, self.cell_width-1, self.cell_height-1, self.lcd.green)
                    self.lcd.rect(x, y, self.cell_width-1, self.cell_height-1, self.lcd.black)
                    # Draw assigned key (truncated if too long)
                    key_text = self.assignments[cell_key]
                    if len(key_text) > 3:
                        key_text = key_text[:3]
                    text_x = x + (self.cell_width - len(key_text) * 8) // 2
                    self.lcd.text(key_text, text_x, y + 4, self.lcd.black)
                else:
                    # Cell is empty - draw in gray
                    self.lcd.fill_rect(x, y, self.cell_width-1, self.cell_height-1, self.lcd.gray)
                    self.lcd.rect(x, y, self.cell_width-1, self.cell_height-1, self.lcd.black)

        # Draw instructions and buttons (adjusted positioning)
        self.lcd.text("Tap cell to assign", 5, 210, self.lcd.black)

        # Draw save/load/clear buttons side by side
        self.lcd.fill_rect(180, 205, 45, 15, self.lcd.blue)    # Save button
        self.lcd.fill_rect(230, 205, 45, 15, self.lcd.green)   # Load button
        self.lcd.fill_rect(280, 205, 45, 15, self.lcd.red)     # Clear button
        self.lcd.rect(180, 205, 45, 15, self.lcd.black)        # Save border
        self.lcd.rect(230, 205, 45, 15, self.lcd.black)        # Load border
        self.lcd.rect(280, 205, 45, 15, self.lcd.black)        # Clear border
        self.lcd.text("SAVE", 190, 210, self.lcd.white)
        self.lcd.text("LOAD", 240, 210, self.lcd.white)
        self.lcd.text("CLR", 292, 210, self.lcd.white)

        self.lcd.show()

    def draw_keyboard_selection(self):
        self.lcd.fill(self.lcd.white)

        # Draw title
        self.lcd.text("Select Key", 5, 2, self.lcd.black)

        # Calculate layout for keyboard keys (adjusted for rotated display)
        keys_per_row = 12
        key_width = 25
        key_height = 18
        start_x = 5
        start_y = 20

        row = 0
        col = 0

        for i, key in enumerate(self.available_keys):
            x = start_x + col * key_width
            y = start_y + row * key_height

            # Don't draw if it would go off screen
            if y + key_height > 190:
                break

            # Draw key button
            self.lcd.fill_rect(x, y, key_width-2, key_height-2, self.lcd.blue)
            self.lcd.rect(x, y, key_width-2, key_height-2, self.lcd.black)

            # Draw key text (truncated if needed)
            key_text = key
            if len(key_text) > 4:
                key_text = key_text[:4]
            text_x = x + (key_width - len(key_text) * 8) // 2
            self.lcd.text(key_text, text_x, y + 5, self.lcd.white)

            col += 1
            if col >= keys_per_row:
                col = 0
                row += 1

        # Draw back button (adjusted for 180 degree rotation)
        self.lcd.fill_rect(5, 205, 50, 20, self.lcd.red)
        self.lcd.rect(5, 205, 50, 20, self.lcd.black)
        self.lcd.text("BACK", 15, 212, self.lcd.white)

        self.lcd.show()

    def save_config(self):
        """Save current keybind configuration to file (simulated EEPROM)"""
        try:
            # Convert assignments to a more readable format
            config_data = {
                "version": 1,
                "grid_size": {"rows": len(self.rows), "cols": len(self.columns)},
                "row_labels": self.rows,
                "col_labels": self.columns,
                "assignments": {}
            }

            # Convert tuple keys to string format for JSON serialization
            for (row, col), key in self.assignments.items():
                coord_str = f"{row},{col}"
                config_data["assignments"][coord_str] = key

            # Write to file
            with open(self.config_file, 'w') as f:
                json.dump(config_data, f)

            print(f"Configuration saved to {self.config_file}")

            # Show save confirmation (adjusted for 180 degree rotation)
            self.lcd.fill_rect(110, 80, 100, 40, self.lcd.green)
            self.lcd.rect(110, 80, 100, 40, self.lcd.black)
            self.lcd.text("SAVED!", 140, 95, self.lcd.white)
            self.lcd.show()
            time.sleep(1)
            self.draw_main_grid()

            return True
        except Exception as e:
            print(f"Save error: {e}")

            # Show error message (adjusted for 180 degree rotation)
            self.lcd.fill_rect(110, 80, 100, 40, self.lcd.red)
            self.lcd.rect(110, 80, 100, 40, self.lcd.black)
            self.lcd.text("SAVE ERROR!", 125, 95, self.lcd.white)
            self.lcd.show()
            time.sleep(1)
            self.draw_main_grid()

            return False

    def load_config(self):
        """Load keybind configuration from file (simulated EEPROM)"""
        try:
            if not self.config_file in os.listdir():
                print("No saved configuration found")
                return False

            with open(self.config_file, 'r') as f:
                config_data = json.load(f)

            # Clear current assignments
            self.assignments.clear()

            # Restore available keys list
            assigned_keys = []

            # Load assignments
            for coord_str, key in config_data["assignments"].items():
                row, col = map(int, coord_str.split(','))
                self.assignments[(row, col)] = key
                assigned_keys.append(key)

            # Remove assigned keys from available keys
            for key in assigned_keys:
                if key in self.available_keys:
                    self.available_keys.remove(key)

            print(f"Configuration loaded from {self.config_file}")
            print(f"Loaded {len(self.assignments)} key assignments")

            return True

        except Exception as e:
            print(f"Load error: {e}")
            return False

    def export_for_keyboard(self, filename="keyboard_config.json"):
        """Export configuration in a format optimized for keyboard firmware"""
        try:
            # Create a simple mapping for keyboard firmware
            keyboard_config = {
                "version": 1,
                "timestamp": time.time(),
                "grid_map": {},
                "key_positions": []
            }

            # Create position-based mapping
            for (row, col), key in self.assignments.items():
                position = f"{self.rows[row]}{self.columns[col]}"
                keyboard_config["grid_map"][position] = key

                # Also create a linear array for easier firmware access
                keyboard_config["key_positions"].append({
                    "row": row,
                    "col": col,
                    "row_label": self.rows[row],
                    "col_label": self.columns[col],
                    "position": position,
                    "key": key
                })

            # Sort by position for consistent ordering
            keyboard_config["key_positions"].sort(key=lambda x: (x["row"], x["col"]))

            with open(filename, 'w') as f:
                json.dump(keyboard_config, f, indent=2)

            print(f"Keyboard configuration exported to {filename}")
            return True

        except Exception as e:
            print(f"Export error: {e}")
            return False
    def get_touched_cell(self, x, y):
        """Determine which grid cell was touched"""
        if self.current_mode != "main":
            return None

        # Check for button touches
        if 180 <= x <= 225 and 205 <= y <= 220:
            return "SAVE"
        elif 230 <= x <= 275 and 205 <= y <= 220:
            return "LOAD"
        elif 280 <= x <= 325 and 205 <= y <= 220:
            return "CLEAR"

        # Check if touch is within grid area
        grid_left = self.start_x
        grid_right = self.start_x + len(self.columns) * self.cell_width
        grid_top = self.start_y
        grid_bottom = self.start_y + len(self.rows) * self.cell_height

        if (x < grid_left or x > grid_right or y < grid_top or y > grid_bottom):
            return None

        # Calculate which cell - fix the calculation
        col = int((x - self.start_x) / self.cell_width)
        row = int((y - self.start_y) / self.cell_height)

        # Ensure we're within bounds
        if 0 <= row < len(self.rows) and 0 <= col < len(self.columns):
            print(f"Touch at {x},{y} -> Grid cell ({row},{col}) = {self.rows[row]}{self.columns[col]}")
            return (row, col)
        return None

    def get_touched_key(self, x, y):
        """Determine which key was touched in keyboard mode"""
        if self.current_mode != "keyboard":
            return None

        # Check back button (adjusted for 180 degree rotation)
        if 5 <= x <= 55 and 205 <= y <= 225:
            return "BACK"

        # Calculate keyboard key layout (adjusted for 180 degree rotation)
        keys_per_row = 12
        key_width = 25
        key_height = 18
        start_x = 5
        start_y = 20

        if (x < start_x or y < start_y or
            x > start_x + keys_per_row * key_width or
            y > 190):
            return None

        col = int((x - start_x) / key_width)
        row = int((y - start_y) / key_height)

        key_index = row * keys_per_row + col

        if 0 <= key_index < len(self.available_keys):
            return self.available_keys[key_index]
        return None

    def handle_touch(self):
        """Handle touch events"""
        current_time = time.ticks_ms()

        if touch_data.state:
            touch_data.state = 0

            # Debounce touches
            if current_time - self.last_touch_time < 300:
                return
            self.last_touch_time = current_time

            x = touch_data.coords[0]["x"]
            y = touch_data.coords[0]["y"]

            print(f"Touch at {x}, {y} in mode {self.current_mode}")

            if self.current_mode == "main":
                # Handle main grid touch
                cell = self.get_touched_cell(x, y)
                if cell:
                    print(f"Touched {cell}")
                    if cell == "SAVE":
                        self.save_config()
                        self.export_for_keyboard()
                    elif cell == "LOAD":
                        if self.load_config():
                            self.lcd.fill_rect(110, 80, 100, 40, self.lcd.blue)
                            self.lcd.rect(110, 80, 100, 40, self.lcd.black)
                            self.lcd.text("LOADED!", 135, 95, self.lcd.white)
                            self.lcd.show()
                            time.sleep(1)
                        self.draw_main_grid()
                    elif cell == "CLEAR":
                        self.clear_config()
                    else:
                        # Regular cell selection
                        self.selected_cell = cell
                        self.current_mode = "keyboard"
                        self.draw_keyboard_selection()

            elif self.current_mode == "keyboard":
                # Handle keyboard selection
                key = self.get_touched_key(x, y)
                if key:
                    print(f"Touched key {key}")
                    if key == "BACK":
                        self.current_mode = "main"
                        self.draw_main_grid()
                    elif key in self.available_keys and self.selected_cell:
                        # Check if cell already has an assignment and return that key to available list
                        if self.selected_cell in self.assignments:
                            old_key = self.assignments[self.selected_cell]
                            if old_key not in self.available_keys:
                                self.available_keys.append(old_key)
                                self.available_keys.sort()  # Keep list sorted

                        # Assign key to selected cell
                        self.assignments[self.selected_cell] = key
                        self.available_keys.remove(key)
                        print(f"Assigned {key} to cell {self.selected_cell}")
                        self.current_mode = "main"
                        self.selected_cell = None
                        self.draw_main_grid()

    def run(self):
        """Main application loop"""
        print("Starting Keybind Configuration")
        self.draw_main_grid()

        try:
            while True:
                self.handle_touch()
                time.sleep_ms(50)  # Small delay to prevent excessive CPU usage

        except KeyboardInterrupt:
            print("Keybind configuration stopped")
            pass

# Main execution
if __name__=='__main__':
    LCD = LCD_1inch69()
    LCD.set_bl_pwm(65535)

    Touch = Touch_CST328()

    # Create and run the keybind configuration app
    app = KeybindConfig(LCD, Touch)
    app.run()

