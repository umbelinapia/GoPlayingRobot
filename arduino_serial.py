import time
import serial

from config import ARDUINO_PLUGGED, ARDUINO_PORT, BAUD_RATE


class ArduinoSerial:
    def __init__(self):
        self.arduino = None
        self.connect()

    def connect(self):
        if not ARDUINO_PLUGGED:
            print("Arduino disabled in config.")
            return

        try:
            self.arduino = serial.Serial(ARDUINO_PORT, BAUD_RATE, timeout=1)
            time.sleep(2)
            print("Arduino connected.")
        except Exception as e:
            print(f"Could not connect to Arduino: {e}")
            self.arduino = None

    def send(self, message):
        if not ARDUINO_PLUGGED:
            return

        if self.arduino and self.arduino.is_open:
            self.arduino.write((message + "\n").encode("utf-8"))
            print(f"Sent: {message}")