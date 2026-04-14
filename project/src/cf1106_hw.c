#include "cf1106_hw.h"
#include "wk_spi.h"
#include "wk_gpio.h"

uint32_t ecat_timer_inc_p_ms = 0;

/* 假设 CF1106 的 SPI CS 引脚连接在 PA4 (请根据你的硬件原理图修改) */
#define ESC_CS_LOW()    gpio_bits_reset(SPI2_CS_GPIO_PORT, SPI2_CS_PIN)
#define ESC_CS_HIGH()   gpio_bits_set(SPI2_CS_GPIO_PORT, SPI2_CS_PIN)

/* 基础的 SPI 单字节收发函数封装 */
static uint8_t spi_read_write_byte(uint8_t txData)
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
    
    spi_read_write_byte((Address >> 5) & 0xFF);
    
    /* 字节2：地址 A[4:0] << 3 拼接 读命令 (010b = 0x02) */
    spi_read_write_byte(((Address & 0x1F) << 3) | 0x02);
	spi_read_write_byte(0xFF);

    /* 连续读取定长数据 */
    while (Len > 0)
    {
        *pBuf++ = spi_read_write_byte(0xFF); // 发送 Dummy 字节获取数据
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
    
    spi_read_write_byte((Address >> 5) & 0xFF);
    
    /* 字节2：地址 A[4:0] << 3 拼接 写命令 (100b = 0x04) */
    spi_read_write_byte(((Address & 0x1F) << 3) | 0x04);

    /* 连续写入定长数据 */
    while (Len > 0)
    {
        spi_read_write_byte(*pBuf++);
        Len--;
    }
    
    ESC_CS_HIGH();
}
