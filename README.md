# STM32 Debug & Test Helper

[![Build](https://img.shields.io/badge/build-manual-lightgrey)](https://github.com/arttynedaiko7-br32/STM32-Debug-Test-Helper/blob/main/README.md)
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://github.com/arttynedaiko7-br32/STM32-Debug-Test-Helper/blob/main/LICENSE)
[![MCU](https://img.shields.io/badge/MCU-STM32F103C8T6-green)](https://www.st.com/resource/en/reference_manual/rm0008-stm32f101xx-stm32f102xx-stm32f103xx-stm32f105xx-and-stm32f107xx-advanced-arm-based-32-bit-mcus-stmicroelectronics.pdf)
[![IDE](https://img.shields.io/badge/IDE-STM32CubeIDE-blue)](https://www.st.com/en/development-tools/stm32cubeide.html)
[![Framework](https://img.shields.io/badge/Framework-STM32_HAL-orange)]( https://www.st.com/en/embedded-software/stm32cubef1.html)

A toolkit for debugging, unit testing, and execution time measurement
in embedded projects based on STM32 microcontrollers.

The project is designed to simplify logic verification and performance
analysis directly on the microcontroller, without requiring complex
external infrastructure.

Originally developed for STM32F103C8T6, but can be adapted
to other STM32 devices.

---

## Motivation

Debugging embedded software is often complicated by:

- lack of convenient unit testing tools
- difficulty measuring function execution time
- strong dependency on debuggers and external tools
- limited runtime visibility

The goal of this project is to provide a lightweight, extensible,
and embeddable toolkit that helps test and profile firmware directly
in an embedded environment.

---

## Features

- Unit testing of application logic inside an embedded project
- Measurement of execution time for functions and code sections
- Debugging and profiling utilities
- Easy integration into STM32Cube-based projects
- Minimal dependency on a specific microcontroller

---

## Target Platform

- MCU: STM32F103C8T6
- IDE: STM32CubeIDE
- Framework: STM32 HAL

The project follows the standard STM32Cube project structure
and can be opened directly via the `.ioc` file.

---

## Prerequisites: STM32 SWO / SWV Debugging

This project relies on SWO (Serial Wire Output / SWV) for runtime output,
printf redirection, tracing, and timing diagnostics without using UART.
All debug output is transmitted over SWD via ST-LINK.

Both the MCU and ST-LINK must support SWO.

### Requirements

- STM32 microcontroller with SWO support
- ST-LINK debugger (Nucleo / Discovery onboard ST-LINK or external)
- STM32CubeIDE

### Hardware Connection

- SWDIO
- SWCLK
- SWO (trace pin)

Ensure that SWO is correctly routed on your board.

### CubeMX Configuration

1. Open the `.ioc` file
2. Go to:
```

System Core → SYS

```
3. Set:
```

Debug → Trace Asynchronous SW

```
4. Generate code

### Debug Configuration (CubeIDE)

1. Open:
```

Run → Debug Configurations

```
2. Select:
```

STM32 Application → Debugger

```
3. Enable:
```

Enable SWV

```
4. Set:
```

Core Clock = SystemCoreClock

```
5. Start Debug session

### Viewing SWO / SWV Output

1. Switch to Debug Perspective
2. Open:
```

Window → Show View → SWV → SWV ITM Data Console

```
3. Configure trace:
- ITM Port: 0
- Core Clock: same as SystemCoreClock
- Prescaler: start with 1024
4. Click Start Trace

### Capabilities Provided by SWO

- printf output without UART
- Variable data tracing
- Function timing and profiling
- Minimal CPU overhead
- Does not consume USART peripherals

### Notes

- Ensure the Core Clock value is correct
- If no output is visible, try different Prescaler values
- SWO must be supported by both MCU and ST-LINK hardware

---

## Build & Run

1. Open the `.ioc` file in STM32CubeIDE
2. Generate code if required
3. Press Build
4. Flash the microcontroller using standard IDE tools

The firmware is built locally using STM32CubeIDE.

---

## Porting to Other STM32

The project can be adapted to other STM32 microcontrollers:

1. Change the target MCU in the `.ioc` file
2. Review timers or time sources used for measurements
3. Rebuild the project

The architecture is not tightly coupled to STM32F103.

---

## Project Status

The project is under active development.
Public APIs and internal implementation may change.

---

## Contributing

Contributions are welcome and appreciated:

- ideas and feature proposals
- bug reports (issues)
- pull requests
- architectural improvements
- extensions for unit testing and profiling capabilities

Feel free to open an issue or submit a pull request.

---

## Feedback & Contact

If you have questions, ideas, or feedback:

Email: daineko.a32@mail.ru

---

## License

This project is licensed under the
GNU General Public License v3.0 (GPL-3.0).

### License Notice

If you use this code in firmware distributed on a device,
you are required to make the corresponding source code available
under the terms of the GPL v3.
