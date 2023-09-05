SKETCH = arduino.ino
PORT = /dev/ttyACM0


compile:
	arduino-cli compile --fqbn esp32:esp32:XIAO_ESP32C3 $(SKETCH)

upload: compile
	arduino-cli upload -p $(PORT) --fqbn esp32:esp32:XIAO_ESP32C3 $(SKETCH)
