default:
	arduino-cli compile --fqbn=rp2040:rp2040:rpipico .

install_platform:
	arduino-cli config init --overwrite
	arduino-cli core update-index
	arduino-cli core install rp2040:rp2040

deps:
	arduino-cli lib install "TinyGPSPlus"
	arduino-cli lib install "RTClib"
	arduino-cli lib install "Time"
	arduino-cli lib install "Etherkit Si5351"

install_arduino_cli:
	curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | BINDIR=~/.local/bin sh

format:
	astyle --options=formatter.conf firmware_v0.01.ino core2.cpp ft817.cpp credits.h pdx_common.h
