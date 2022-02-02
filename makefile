BUILD_UNITY := false

all:
ifeq ($(BUILD_UNITY), true)
	make -C 01-LedDriver-unity
endif
	make -C 01-LedDriver
	make -C 02-LightScheduler-v1
	make -C 03-FormatOutput
	make -C 04-CircularBuffer
	make -C 05-IO
	make -C 05-IO-CppUMock
	make -C 06-LightScheduler-v2
	make -C 07-LightScheduler-v3

clean:
	make -C 01-LedDriver clean
	make -C 01-LedDriver-unity clean
	make -C 02-LightScheduler-v1 clean
	make -C 03-FormatOutput clean
	make -C 04-CircularBuffer clean
	make -C 05-IO clean
	make -C 05-IO-CppUMock clean
	make -C 06-LightScheduler-v2 clean
	make -C 07-LightScheduler-v3 clean
