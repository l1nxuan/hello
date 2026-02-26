#include "stm32f4xx_hal.h"

DMA_HandleTypeDef hdma1; // DMA1_Stream5, Channel 7 -> DAC channel 1

DMA_HandleTypeDef hdma2; // DMA2_Stream0, Channel 0 -> ADC1 channel 13
DMA_HandleTypeDef hdma3; // DMA2_Stream1, Channel 2 -> ADC3 channel 10
