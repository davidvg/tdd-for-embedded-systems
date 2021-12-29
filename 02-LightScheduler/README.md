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
[X] Schedule every day, time is wrong, no changes.
[X] Schedule every day, right time, light turns on.
[ ] Schedule a specific day.
[ ] Schedule all weekdays.
[ ] Schedule weekend days.
[ ] Remove scheduled event.
[ ] Remove non-existent event.
[ ] Multiple scheduled events at the same time.
[ ] Multiple scheduled events for the same light.
[ ] Remove non scheduled light schedule.
[ ] Schedule the maximum supported number of events (128)
[ ] Schedule too many events.
[X] No lights are scheduled and none turns on during wakeup.
```

## Doubles and mocks

The production headers, `LightController.h` and `TimeService.h`, are used as interfaces. Their corresponding `.c` files are not needed, as they are substituted by the test doubles, `LightControllerSpy` and `FakeTimeService`. For these doubles, a `.h` file includes the header fot the substituted component and this header is implemented in its `.c` file:

```txt
LightController.h --> LightControllerSpy.h --> LightControllerSpy.c
```


## Tests and Interfaces

The needded steps to pass the test is described in each test.

1. `LightScheduler: NoChangeToLightsDuringInitialization`
1. `LightControllerSpy: Create`
1. `LightControllerSpy: RememberTheLastLightIdControlled`
1. `FakeTimeService: Create`
1. `FakeTimeService: Set`
1. `LightScheduler: NoScheduleNothingHappens`
1. `LightScheduler: ScheduleOnEverydayNotTimeYet`
1. `LightScheduler: ScheduleOnEverydayItsTime`