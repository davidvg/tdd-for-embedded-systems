# Home Automation: The Light Scheduler

This file describes the steps for creating the  `HomeAutomation` example.

## List of Test

```txt
[X] Lights are not changed at initialization.
[ ] Day is wrong, time is wrong, no lights are changed.
[ ] Day is right, time is wrong, no lights are changed.
[ ] Day is wrong, time is right, no lights are changed.
[ ] Day is right, time is right, the right light is turned on.
[ ] Day is right, time is right, the right light is turned off.
[ ] Schedule every day.
[ ] Schedule a specific day.
[ ] Schedule all weekdays.
[ ] Schedule weekend days.
[ ] Remove scheduled event.
[ ] Remove non-existent event.
[ ] Multiple scheduled events at the same time.
[ ] Multiple scheduled events for the same light.
[ ] Remove non scheduled light schedule.
[ ] Schedule the masimum supported number of events (128)
[ ] Schedule too many events.
```

## Doubles and mocks

The production headers, `LightController.h` and `TimeService.h`, are used as interfaces. Their corresponding `.c` files are not needed, as they are substituted by the test doubles, `LightControllerSpy` and `FakeTimeService`. For these doubles, a `.h` file includes the header fot the substituted component and this header is implemented in its `.c` file:

```txt
LightController.h --> LightControllerSpy.h --> LightControllerSpy.c
```


## Tests and Interfaces

1. `LightScheduler: NoChangeToLightsDuringInitialization`
1. `LightControllerSpy: Create`
1. `LightControllerSpy: RememberTheLastLightIdControlled`