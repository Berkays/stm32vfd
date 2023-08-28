#include "main.h"

#include <math.h>
#include "rttserial.h" // Comment `#define SERIAL_OUTPUT` to disable RTT output

// Sinewave lookup tables
#include "tim1.h"
#include "tim2.h"
#include "tim3.h"

static_assert(sizeof(tim1) == sizeof(tim2), "LUT size mismatch between phase-1 and phase-2");
static_assert(sizeof(tim1) == sizeof(tim3), "LUT size mismatch between phase-1 and phase-3");
static_assert(sizeof(tim2) == sizeof(tim3), "LUT size mismatch between phase-2 and phase-3");

constexpr uint16_t POINT_COUNT = sizeof(tim1) / sizeof(uint16_t);

// Configurations
#define MAX_ADC_VALUE 4096 // 12 bit
#define ADC_HYSTERESIS 500 // Prevent potentiometer zero ohm state fluctuations
#define LUT_STEP 1
#define MAX_AMPLITUDE_DIVIDER 10
#define MIN_AMPLITUDE_DIVIDER 1
#define MAX_FREQUENCY 120
#define MIN_FREQUENCY 5

uint16_t amplitudeDivider = 1;
uint32_t frequency = MIN_FREQUENCY;

// LUT pointer
int index = 0;

// PWM CCR Variables
uint16_t s1 = 0;
uint16_t s2 = 0;
uint16_t s3 = 0;

// Get sine stepper autoreload value given desired output sine frequency.
// Adjust CLK and Prescaler to match with TIM2.
constexpr uint16_t GET_DESIRED_FREQ_ARR(const uint16_t DesiredFrequency)
{
    const uint32_t CLK = 72000000;
    const uint32_t PRESCALER = 2 - 1;
    const uint32_t SampleFrequency = DesiredFrequency * POINT_COUNT;
    return (uint16_t)(CLK / ((PRESCALER + 1) * SampleFrequency));
}

// ADC variables
uint32_t adcVal = 0;
uint32_t adcTemp = MIN_FREQUENCY;

// ADC Potentiometer step sizes
constexpr uint16_t AMPLITUDE_STEP = MAX_ADC_VALUE / MAX_AMPLITUDE_DIVIDER;

// Integer map function
uint16_t map(uint16_t x, uint16_t from_min, uint16_t from_max, uint16_t to_min, uint16_t to_max)
{
    return (x - from_min) * (to_max - to_min) / (from_max - from_min) + to_min;
}

int main(void)
{
    InitPeripherals();
    HAL_Delay(1000);

    // Set lowest frequency for startup
    TIM2->ARR = GET_DESIRED_FREQ_ARR(MIN_FREQUENCY);

    // Start pwm timers
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
    HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
    HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);

    // Start sine stepper timer
    HAL_TIM_Base_Start_IT(&htim2);

    // Configure ADC. Better if DMA is utilized for higher output frequencies.
    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = ADC_CHANNEL_3;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
    HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    while (1)
    {
        HAL_ADC_Start(&hadc1);
        if (HAL_ADC_PollForConversion(&hadc1, 5) == HAL_OK)
        {
            adcVal = HAL_ADC_GetValue(&hadc1);

            /* Amplitude modulation
            amplitudeDivider = (int)(adcVal / AMPLITUDE_STEP) + 1;
            if (amplitudeDivider < MIN_AMPLITUDE_DIVIDER)
                amplitudeDivider = MIN_AMPLITUDE_DIVIDER;
            else if (amplitudeDivider > MAX_AMPLITUDE_DIVIDER)
                amplitudeDivider = MAX_AMPLITUDE_DIVIDER;
             */

            /* Frequency Modulation */
            if (adcVal <= ADC_HYSTERESIS)
                adcTemp = MIN_FREQUENCY;
            else
                adcTemp = map(adcVal, ADC_HYSTERESIS, MAX_ADC_VALUE, MIN_FREQUENCY, MAX_FREQUENCY);

            // Update sine stepper frequency
            frequency = adcTemp;
            TIM2->ARR = GET_DESIRED_FREQ_ARR(frequency * LUT_STEP);
        }

        HAL_ADC_Stop(&hadc1);
        sendInt("FREQ:" RTT_CTRL_TEXT_BRIGHT_GREEN "%d\n", frequency * LUT_STEP);
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    // Assign temporary values from LUT
    s1 = tim1[index];
    s2 = tim2[index];
    s3 = tim3[index];

    // Scale values if using amplitude divider
    if (amplitudeDivider > MIN_AMPLITUDE_DIVIDER)
    {
        s1 /= amplitudeDivider;
        s2 /= amplitudeDivider;
        s3 /= amplitudeDivider;
    }

    // Set pwm duty cycle
    TIM1->CCR1 = s1;
    TIM1->CCR2 = s2;
    TIM1->CCR3 = s3;

    // Increment LUT pointer
    index += LUT_STEP;
    if (index >= POINT_COUNT)
        index = 0;
}