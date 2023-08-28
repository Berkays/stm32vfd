#ifndef __PERIPHERALS_H
#define __PERIPHERALS_H

#include "hal_driver.h"

#ifdef __cplusplus
extern "C"
{
#endif
    extern ADC_HandleTypeDef hadc1;
    extern TIM_HandleTypeDef htim1;
    extern TIM_HandleTypeDef htim2;

    void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
#ifdef __cplusplus
}
#endif

void InitPeripherals(void);
void Error_Handler(void);

void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_TIM1_Init(void);
void MX_TIM2_Init(void);
void MX_ADC1_Init(void);
#endif