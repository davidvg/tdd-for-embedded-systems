

all:
	make -C 01-LedDriver
	make -C 02-LightScheduler

clean:
	make -C 01-LedDriver clean
	make -C 02-LightScheduler clean
