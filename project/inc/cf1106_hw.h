#ifndef _CF1106_HW_H_
#define _CF1106_HW_H_

#include "at32f435_437.h"
#include "ecat_def.h"

/* 声明 SSC 协议栈需要的底层硬件接口 */
void HW_Init(void);
void HW_Release(void);

/* SPI 读写接口 (16位/32位/8位均由 SSC 宏定义决定，通常是指针+长度) */
void HW_EscRead(MEM_ADDR *pData, uint16_t Address, uint16_t Len);
void HW_EscWrite(MEM_ADDR *pData, uint16_t Address, uint16_t Len);

/* 中断处理函数声明（提供给外设中断调用） */
void HW_EcatTimerIsr(void);
void HW_EcatPdiIsr(void);
void HW_EcatSync0Isr(void);
void HW_EcatSync1Isr(void);

#endif /* _CF1106_HW_H_ */