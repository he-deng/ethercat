#include "cf1106_hw.h"
#include "wk_spi.h"
#include "wk_gpio.h"

/* 假设 CF1106 的 SPI CS 引脚连接在 PA4 (请根据你的硬件原理图修改) */
#define ESC_CS_LOW()    gpio_bits_reset(GPIOA, GPIO_PINS_4)
#define ESC_CS_HIGH()   gpio_bits_set(GPIOA, GPIO_PINS_4)

/* 基础的 SPI 单字节收发函数封装 */
static uint8_t SPI_ReadWriteByte(uint8_t txData)
{
    while(spi_i2s_flag_get(SPI2, SPI_I2S_TDBE_FLAG) == RESET);
    spi_i2s_data_transmit(SPI2, txData);
    
    while(spi_i2s_flag_get(SPI2, SPI_I2S_RDBF_FLAG) == RESET);
    return spi_i2s_data_receive(SPI2);
}

/**
  * @brief  硬件初始化
  */
void HW_Init(void)
{
    /* 所有的外设初始化已经在 main.c 的 wk_xxx_init() 中完成了 
       这里只需确保 CS 引脚初始处于拉高状态 */
    ESC_CS_HIGH();
}

/**
  * @brief  硬件释放
  */
void HW_Release(void)
{
    // 通常为空
}

/**
  * @brief  向 EtherCAT 从站控制器(ESC)读取数据
  * @param  pData: 数据读取缓冲区的指针
  * @param  Address: ESC 内部寄存器/DPRAM 物理地址
  * @param  Len: 读取的字节长度
  */
void HW_EscRead(MEM_ADDR *pData, uint16_t Address, uint16_t Len)
{
    uint8_t *pBuf = (uint8_t *)pData;
    
    ESC_CS_LOW();
    
    /* 经典的 ESC SPI 读时序 (2字节地址 + 1字节命令码) 
       注意：具体命令码请参考 CF1106A 芯片手册，这里以标准 ET1100 兼容格式为例 */
    SPI_ReadWriteByte((Address >> 8) & 0xFF);     // Address High
    SPI_ReadWriteByte(Address & 0xFF);            // Address Low
    SPI_ReadWriteByte(0x02); // 0x02通常表示 Read Command (带Wait State请看手册)

    /* 连续读取数据 */
    while (Len > 0)
    {
        *pBuf++ = SPI_ReadWriteByte(0xFF); // 发送 Dummy Byte 获取数据
        Len--;
    }
    
    ESC_CS_HIGH();
}

/**
  * @brief  向 EtherCAT 从站控制器(ESC)写入数据
  * @param  pData: 待写入数据的指针
  * @param  Address: ESC 内部寄存器/DPRAM 物理地址
  * @param  Len: 写入的字节长度
  */
void HW_EscWrite(MEM_ADDR *pData, uint16_t Address, uint16_t Len)
{
    uint8_t *pBuf = (uint8_t *)pData;
    
    ESC_CS_LOW();
    
    /* 发送地址和写命令 */
    SPI_ReadWriteByte((Address >> 8) & 0xFF);     // Address High
    SPI_ReadWriteByte(Address & 0xFF);            // Address Low
    SPI_ReadWriteByte(0x04); // 0x04通常表示 Write Command

    /* 连续写入数据 */
    while (Len > 0)
    {
        SPI_ReadWriteByte(*pBuf++);
        Len--;
    }
    
    ESC_CS_HIGH();
}