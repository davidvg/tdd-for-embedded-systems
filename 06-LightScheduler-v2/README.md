# Light Scheduler: Multiple Controllers

## Tests

The module `t1/mocks/LightDriverSpy` is equivalent to `LightControllerSpy`.

### Tests in `t1`

```
t1
- tests
    - LightSchedulerTest.cpp
    - LightControllerTest.cpp
    - LightSchedulerRandomizeTest.cpp
    - RandomMinuteTest.cpp
    - util
        - TimeServiceHelpersTest.cpp
    - devices
        - MemMappedLightDriverTest.cpp
        - AcmeWirelessLightDriverTest.cpp
        - X10LightDriverTest.cpp
- mocks
    - LightDriverSpyTest.cpp
    - FakeTimeServiceTest.cpp
```

### Tests in `t2`

```
t2
- tests
    - LightSchedulerTest.cpp
    - LightControllerTest.cpp
    - LightSchedulerRandomizeTest.cpp
    - RandomMinuteTest.cpp
    - util
        - TimeTest.cpp
    - devices
        - MemMappedLightDriverTest.cpp
        - LightDriverTest.cpp
        - AcmeWirelessLightDriverTest.cpp
        - X10LightDriverTest.cpp
- mocks
    - LightDriverSpyTest.cpp
    - FakeTimeServiceTest.cpp
```

### Test comparison




## ToDo

```txt
[ ] Test 02/LightDriverSpy using t2/LightDriverSpyTest
[X] Inside LightControllerSpy substitute LightController by LightDriver
[X] Rename LightControllerSpy to LightDriverSpy
```

## 1. Testing `LightDriverSpy`

We use the tests file `t1/mocks/LightDriverSpyTest.cpp`.

- Rename `LightControllerSpy` to `LightDriverSpy`.
- Inside the renamed `LightDriverSpy` substitute `LightController` by `LightDriver`.
- Create `include/devices/LightDriver.h`
- Inside the existing `LightController.h` update the interface from page 196, creating `LightController_Add()`. Rename the `_On()` and `_Off()` methods to `_TurnOn()` and `_TurnOff()`.
- Inside `include/devices/LightDriver.h` create the structs and enum from page 196.
- Declare `LightDriverSpy_Reset()`, `LightDriverSpy_Create()`, `LightController_Destroy()` inside `LightDriverSpy.h`.
- Include `LightController.h`
- Inside `LightController.h` include `LightDriver.h`.
