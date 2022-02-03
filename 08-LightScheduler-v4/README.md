# LightScheduler: v4

A new feature is added: different types of Light Drivers can be used with the same Light Controller.

To allow this, each driver must contain its specific interface.

The interface is added to `LightDriverStruct` as a `vtable`:

```c
// LightDriverPrivate.h

typedef struct LightDriverStruct
{
    LightDriverInterface vtable;    // A pointer to the interface struct
    const char * type;
    int id;
} LightDriverStruct;
```

Now, each `LightDriver` will call the needed function via its own `vtable`.

A new mock object is used to act as a different driver: `CountingLightDriver`. It has the following structure:

```c
typedef struct CountingLightDriverStruct
{
    LightDriverStruct base;
    int counter;
} CountingLightDriverStruct;
```

The driver will have a function that increments `counter` each time the interface is called.