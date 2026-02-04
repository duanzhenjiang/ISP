#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

/*
 * Day1-Task A: I2C 寄存器表执行器
 *
 * 学习目的：
 * 1) 建立“寄存器表 + 延时控制”的肌肉记忆
 * 2) 明白为什么需要 delay（PLL lock / stream 切换 / group hold）
 * 3) 理解不同 sensor/ISP 的寄存器/数值位宽可能不同（8/16 bit）
 *
 * 面试话术要点：
 * - 表驱动能保证初始化可维护，易于调试与版本管理
 * - 延时必须与硬件时序匹配，防止数据通路不稳定
 * - 支持 8/16 bit reg/val 是驱动通用化的关键
 */

typedef struct {
    uint16_t reg;
    uint16_t val;
    uint16_t delay_ms;
} regval_t;

/* 真实项目中替换为 i2c_smbus_write 或底层 HAL。这里仅打印示例。 */
static int32_t i2c_write16(uint8_t dev_addr, uint16_t reg, uint16_t val)
{
    printf("[i2c] dev=0x%02x reg=0x%04x val=0x%04x\n", dev_addr, reg, val);
    return 0;
}

/* 真实项目中替换为 msleep/usleep。 */
static void msleep(uint16_t ms)
{
    /* 简化演示：仅打印，不实际延时 */
    printf("[delay] %u ms\n", ms);
}

int32_t apply_reg_table(uint8_t dev_addr, const regval_t *tbl, size_t n)
{
    size_t i = 0;
    int32_t ret = 0;

    if ((NULL == tbl) || (0U == n)) {
        return -1;
    }

    for (i = 0; i < n; i++) {
        ret = i2c_write16(dev_addr, tbl[i].reg, tbl[i].val);
        if (0 != ret) {
            return -2;
        }
        if (0U != tbl[i].delay_ms) {
            msleep(tbl[i].delay_ms);
        }
    }
    return 0;
}

/* 简单示例：模拟 sensor init + stream on */
int main(void)
{
    const regval_t init_tbl[] = {
        {0x0103, 0x0001, 5},   /* software reset */
        {0x0300, 0x0004, 0},   /* PLL setup */
        {0x0100, 0x0001, 10},  /* stream on */
    };

    int32_t ret = apply_reg_table(0x36, init_tbl, sizeof(init_tbl) / sizeof(init_tbl[0]));
    printf("apply_reg_table ret=%d\n", ret);
    return ret;
}
