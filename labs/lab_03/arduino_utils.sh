USB="/dev/ttyUSB0"
BOARD="arduino:avr:uno"
SKETCH=${1:-"./supervisor"}
LIBRARIES=${2:-"./libraries"}

arduino-cli compile -v $SKETCH --libraries $LIBRARIES -b ${BOARD} &&
arduino-cli upload -v $SKETCH -p ${USB} -b ${BOARD} # &&
# arduino-cli monitor -p ${USB} --config 115200