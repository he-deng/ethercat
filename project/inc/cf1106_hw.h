#ifndef _CF1106_HW_H_
#define _CF1106_HW_H_

#include "at32f435_437.h"

/* ========================================================================
   1. 宏定义拦截：防止 ecat_def.h 与 AT32 标准库的 typedef 发生冲突 
   ======================================================================== */
#define TRUE   TRUE
#define FALSE  FALSE
#define INT32  INT32
#define INT16  INT16
#define INT8   INT8
#define UINT32 UINT32
#define UINT16 UINT16
#define UINT8  UINT8
#define BOOL   uint8_t

/* ========================================================================
   2. 基础 SPI 读写接口声明
   ======================================================================== */
void HW_Init(void);
void HW_Release(void);
void HW_EscRead(MEM_ADDR *pData, uint16_t Address, uint16_t Len);
void HW_EscWrite(MEM_ADDR *pData, uint16_t Address, uint16_t Len);

/* ========================================================================
   3. SSC 协议栈要求的底层数据访问宏
   ======================================================================== */
/* 8/16/32 位读取宏 */
#define HW_EscReadByte(WordValue, Address)       HW_EscRead((MEM_ADDR *)&(WordValue), (Address), 1)
#define HW_EscReadWord(WordValue, Address)       HW_EscRead((MEM_ADDR *)&(WordValue), (Address), 2)
#define HW_EscReadDWord(DWordValue, Address)     HW_EscRead((MEM_ADDR *)&(DWordValue), (Address), 4)
#define HW_EscReadMbxMem(pData, Address, Len)    HW_EscRead((MEM_ADDR *)(pData), (Address), (Len))

#define HW_EscReadByteIsr(WordValue, Address)    HW_EscRead((MEM_ADDR *)&(WordValue), (Address), 1)
#define HW_EscReadWordIsr(WordValue, Address)    HW_EscRead((MEM_ADDR *)&(WordValue), (Address), 2)
#define HW_EscReadDWordIsr(DWordValue, Address)  HW_EscRead((MEM_ADDR *)&(DWordValue), (Address), 4)

/* 8/16/32 位写入宏 */
#define HW_EscWriteByte(WordValue, Address)      HW_EscWrite((MEM_ADDR *)&(WordValue), (Address), 1)
#define HW_EscWriteWord(WordValue, Address)      HW_EscWrite((MEM_ADDR *)&(WordValue), (Address), 2)
#define HW_EscWriteDWord(DWordValue, Address)    HW_EscWrite((MEM_ADDR *)&(DWordValue), (Address), 4)
#define HW_EscWriteMbxMem(pData, Address, Len)   HW_EscWrite((MEM_ADDR *)(pData), (Address), (Len))

#define HW_EscWriteByteIsr(WordValue, Address)   HW_EscWrite((MEM_ADDR *)&(WordValue), (Address), 1)
#define HW_EscWriteWordIsr(WordValue, Address)   HW_EscWrite((MEM_ADDR *)&(WordValue), (Address), 2)
#define HW_EscWriteDWordIsr(DWordValue, Address) HW_EscWrite((MEM_ADDR *)&(DWordValue), (Address), 4)

/* ========================================================================
   4. 中断与事件寄存器宏
   ======================================================================== */
/* 根据你实际使用的 PDI 中断引脚修改 EXINT_LINE_x (此处假设为 Line 9) */
#define DISABLE_ESC_INT()                        exint_interrupt_enable(EXINT_LINE_9, FALSE)
#define ENABLE_ESC_INT()                         exint_interrupt_enable(EXINT_LINE_9, TRUE)

/* 读取 AL Event 寄存器 (地址 0x0220) */
static inline UINT16 HW_GetALEventRegister(void) {
    UINT16 alEvent = 0;
    HW_EscReadWord(alEvent, 0x0220);
    return alEvent;
}
#define HW_GetALEventRegister_Isr()              HW_GetALEventRegister()

/* ========================================================================
   5. 定时器相关宏 (供 ecatappl.c 使用)
   ======================================================================== */
extern uint32_t ecat_timer_inc_p_ms;
#define HW_GetTimer()                            (ecat_timer_inc_p_ms)
#define HW_ClearTimer()                          (ecat_timer_inc_p_ms = 0)
#define ECAT_TIMER_INC_P_MS                      1  /* 定时器 1 个 Tick 为 1ms */

#endif /* _CF1106_HW_H_ */
// （注意：确保最后这里有一行空行，消除 "last line of file ends without a newline" 警告）