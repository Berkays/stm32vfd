# Summary

Variable frequency SVPWM driver(VFD) prototype for STM32 MCUs. Default values and drivers are designed to run on a blue pill module with STM32F103C8T6. Since the project uses CubeMx HAL drivers, It can easily be ported to other STM32 microprocessors. Default lookup tables are constructed using **0x7FFFh(32.767)** as peak value and **k=0.15** for third harmonic coefficent.

| Value         | Default Value | Description                                                                                                                                                                                                                     |
| ------------- | ------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| TIM1 Clock    | 72Mhz         |                                                                                                                                                                                                                                 |
| TIM2 Clock    | 72Mhz         |                                                                                                                                                                                                                                 |
| Prescaler     | 2 - 1         | Used for TIM1 and TIM2                                                                                                                                                                                                          |
| TIM1 Period   | 32767         | Lookup tables need to be adjusted accordingly.<br>PWM frequency needs to be tweaked for your driver circuit.                                                                                                                    |
| PWM Frequency | ~1KHz         | 72MHz / (2 \* 32767)                                                                                                                                                                                                            |
| TIM2 Period   | N/A           | Dynamically adjusted using potentiometer.                                                                                                                                                                                       |
| LUT_STEP      | 1             | Defines how much the pointer will step in single interrupt.<br>Higher LUT step will result in higher frequencies without touching TIM2 period.<br>Depending on the value, it can skip some values entirely wasting flash space. |

## Peripherals

- **Timer1** with complementary pwm outputs and deadtime support.
- **Timer2** Simple timer for sinewave lookup table stepping.
- **ADC** Reading frequency input from a potentiometer

## CubeMx Script

CubeMx generated folder structure can automatically be imported using the [import.sh script](./scripts/import.sh).

Generated folder root path must be provided as an argument:

`./scripts/import.sh ~/CubeMxWorkSpace/VFD`
