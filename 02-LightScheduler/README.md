# Home Automation: The Light Scheduler

This file describes the steps for creating the  `HomeAutomation` example.

## List of Test

```txt
[X] Lights are not changed at initialization.
[X] Day is wrong, time is wrong, no lights are changed.
[X] Day is right, time is wrong, no lights are changed.
[X] Day is wrong, time is right, no lights are changed.
[X] Day is right, time is right, the right light is turned on.
[X] Day is right, time is right, the right light is turned off.
[X] Schedule every day, time is wrong, no changes.
[X] Schedule every day, right time, light turns on.
[X] Schedule a specific day.
[X] Schedule all weekdays.
[X] Schedule weekend days.
[X] Remove scheduled event.
[ ] Remove non-existent event.
[X] Multiple scheduled events at the same time.
[X] Multiple scheduled events for the same light.
[X] Remove non scheduled light schedule.
[X] Schedule the maximum supported number of events (128)
[X] Schedule too many events.
[X] No lights are scheduled and none turns on during wakeup.
```

## `LightScheduler`, `LightController` and `TimeService`

### Doubles and mocks

The production headers, `LightController.h` and `TimeService.h`, are used as interfaces. Their corresponding `.c` files are not needed, as they are substituted by the test doubles, `LightControllerSpy` and `FakeTimeService`. For these doubles, a `.h` file includes the header fot the substituted component and this header is implemented in its `.c` file:

```txt
LightController.h --> LightControllerSpy.h --> LightControllerSpy.c
```


### Tests and Interfaces

The needded steps to pass the test is described in each test.

1. `LightScheduler: NoChangeToLightsDuringInitialization`
1. `LightControllerSpy: Create`
1. `LightControllerSpy: RememberTheLastLightIdControlled`
1. `FakeTimeService: Create`
1. `FakeTimeService: Set`
1. `LightScheduler: NoScheduleNothingHappens`
1. `LightScheduler: ScheduleOnEverydayNotTimeYet`
1. `LightScheduler: ScheduleOnEverydayItsTime`
1. `LightScheduler: ScheduleOffEverydayItsTime`
1. `LightScheduler: ScheduleTuesdayButItsMonday`
1. `LightScheduler: ScheduleTuesdayAndItsTuesday`
1. `LightScheduler: ScheduleWeekEndItsFriday`
1. `LightScheduler: ScheduleWeekEndItsSaturday`
1. `LightScheduler: ScheduleWeekEndItsSunday`
1. `LightScheduler: ScheduleWeekEndItsMonday`
1. `LightScheduler: ScheduleWeekdayItsSunday`
1. `LightScheduler: ScheduleWeekdayItsMonday`
1. `LightScheduler: ScheduleWeekdayItsFriday`
1. `LightSchedulerInitAndCleanup: CreateStartsOneMinuteAlarm`
1. `LightSchedulerInitAndCleanup: DestroyCancelsOneMinuteAlarm`
1. `LightScheduler: RememberAllLightStates`
1. `LightScheduler: ScheduleTwoEventsAtTheSameTime`
1. `LightScheduler: RejectsTooManyEvents`
1. `LightScheduler: RemoveRecyclesScheduleSlot`
1. `LightScheduler: RemoveMultipleScheduledEvent`
1. `LightScheduler: AcceptsValidLightIds`
1. `LightScheduler: RejectsInvalidLights`


## `RandomMinute`

`RandomMinute_Get()` is used to return a random number in the range `[-BOUND, BOUND]`.

### Tests

1. `RandomMinute: GetIsInRange`
1. `RandomMinute: AllValuesPossible`