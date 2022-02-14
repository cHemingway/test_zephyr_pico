Messing around with Zephyr

Attempt to flash an LED on the rpi_pico using led_blink API, not raw GPIO+Delay

### How this works
- The [rpi_pico device tree](https://github.com/zephyrproject-rtos/zephyr/blob/main/boards/arm/rpi_pico/rpi_pico.dts) already contains a GPIO LED instance, but unused.

- Enable GPIO, LED and GPIO_LED Kconfig options in `prj.conf`
```
CONFIG_GPIO=y
CONFIG_LED=y
CONFIG_LED_GPIO=y
```

Hence the LED_GPIO driver will be built in, rather than expose the led as _just_ a GPIO

- Get the _parent_ of the led0 alias and use this to control it.

- Hardcode back that we want to access the first LED (0), as getting this from devicetree is a bit tricky. See https://github.com/zephyrproject-rtos/zephyr/issues/42015

- Try and use led_blink, but this is not implemented for the `led-gpio` driver, it was split into a [separate KConfig option](https://github.com/zephyrproject-rtos/zephyr/pull/21578/commits/cd065bd94b47975a90800827e85a422e3ce36ca6) in an [earlier pull request](https://github.com/zephyrproject-rtos/zephyr/pull/21578) then abandoned.

- Then uses software timers instead, which is easier.

### How to load code
Hold the boot switch and replug USB
copy  `build/zephyr/zephyr.uf2` to your rpi_pico