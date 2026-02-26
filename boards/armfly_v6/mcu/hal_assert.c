#include "stm32f4xx_hal.h"

#ifdef USE_FULL_ASSERT
#include <stdint.h>
#include "SEGGER_RTT.h"

/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line)
{
    const char* filename = __builtin_strrchr((const char*)file, '/') ? __builtin_strrchr((const char*)file, '/') + 1
                                                                     : (const char*)file;
    (void)SEGGER_RTT_printf(0, "[%s:%3d] assert_param error.\n", filename, line);
}
#endif
