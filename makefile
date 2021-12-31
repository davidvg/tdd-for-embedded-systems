BUILD_UNITY := false

all:
	make -C 01-LedDriver
ifeq ($(BUILD_UNITY), true)
	make -C 01-LedDriver-unity
endif
	make -C 02-LightScheduler

clean:
	make -C 01-LedDriver clean
	make -C 01-LedDriver-unity clean
	make -C 02-LightScheduler clean
