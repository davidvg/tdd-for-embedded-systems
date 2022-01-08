BUILD_UNITY := false

all:
ifeq ($(BUILD_UNITY), true)
	make -C 01-LedDriver-unity
endif
	make -C 01-LedDriver
	make -C 02-LightScheduler
	make -C 03-FormatOutput
	make -C 04-CircularBuffer

clean:
	make -C 01-LedDriver clean
	make -C 01-LedDriver-unity clean
	make -C 02-LightScheduler clean
	make -C 03-FormatOutput clean
	make -C 04-CircularBuffer clean
