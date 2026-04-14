/* add user code begin Header */
/**
  **************************************************************************
  * @file     main.c
  * @brief    main program
  **************************************************************************
  */
/* add user code end Header */

/* Includes ------------------------------------------------------------------*/
#include "at32f435_437_wk_config.h"
#include "wk_exint.h"
#include "wk_spi.h"
#include "wk_tmr.h"
#include "wk_gpio.h"
#include "wk_system.h"

/* private includes ----------------------------------------------------------*/
/* add user code begin private includes */
#include "ecat_def.h"        // SSC 协议栈全局定义
#include "ecatslv.h"         // SSC 从站核心头文件
#include "applInterface.h"   // SSC 应用接口
#include "cf1106_hw.h"       // 你的 CF1106 硬件抽象层头文件 (见下文)
/* add user code end private includes */

/* private typedef -----------------------------------------------------------*/
/* add user code begin private typedef */
/* add user code end private typedef */

/* private define ------------------------------------------------------------*/
/* add user code begin private define */
/* add user code end private define */

/* private macro -------------------------------------------------------------*/
/* add user code begin private macro */
/* add user code end private macro */

/* private variables ---------------------------------------------------------*/
/* add user code begin private variables */
/* add user code end private variables */

/* private function prototypes --------------------------------------------*/
/* add user code begin function prototypes */
/* add user code end function prototypes */

/* private user code ---------------------------------------------------------*/
/* add user code begin 0 */
/* add user code end 0 */

/**
  * @brief main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  /* add user code begin 1 */
  /* add user code end 1 */

  /* system clock config. */
  wk_system_clock_config();

  /* config periph clock. */
  wk_periph_clock_config();

  /* nvic config. */
  wk_nvic_config();

  /* timebase config for delay */
  wk_timebase_init();

  /* init gpio function. */
  wk_gpio_config();

  /* init spi2 function. */
  wk_spi2_init();

  /* init exint function. */
  wk_exint_config();

  /* init tmr7 function. */
  wk_tmr7_init();

  /* add user code begin 2 */
  
  /* 1. 初始化 EtherCAT 硬件接口 (SPI, 中断引脚等) */
  HW_Init(); 
  
  /* 2. 初始化 EtherCAT 协议栈 */
  MainInit();
  
  /* 3. 标记应用程序已启动，允许 SSC 运行 */
  bRunApplication = TRUE;

  /* add user code end 2 */

  while(1)
  {
    /* add user code begin 3 */
    
    /* 4. EtherCAT 协议栈主循环 */
    MainLoop();
    
    /* add user code end 3 */
  }
}