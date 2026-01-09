#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

/*
 * Day1-Task C: 最小 BPC（坏点修复）
 *
 * 学习目的：
 * 1) 理解坏点检测的阈值意义（避免真实边缘被误判）
 * 2) 了解 Bayer 上应按通道处理（同色邻域）
 * 3) 形成“检测 + 修复 + 截断”的完整流程
 */

static uint16_t u16_abs_diff(uint16_t a, uint16_t b)
{
    return (a > b) ? (uint16_t)(a - b) : (uint16_t)(b - a);
}

static uint16_t clamp_u16(uint32_t v, uint16_t maxv)
{
    return (v > (uint32_t)maxv) ? maxv : (uint16_t)v;
}

/* 对单通道平面做 BPC：用左右上下的平均替代异常点 */
int32_t bpc_2d_u16(uint16_t *img, int32_t w, int32_t h, uint16_t thr, uint16_t maxv)
{
    int32_t x = 0;
    int32_t y = 0;

    if ((NULL == img) || (w <= 2) || (h <= 2)) {
        return -1;
    }

    for (y = 1; y < (h - 1); y++) {
        for (x = 1; x < (w - 1); x++) {
            uint16_t c = img[y * w + x];
            uint16_t l = img[y * w + (x - 1)];
            uint16_t r = img[y * w + (x + 1)];
            uint16_t u = img[(y - 1) * w + x];
            uint16_t d = img[(y + 1) * w + x];

            uint16_t diff = u16_abs_diff(c, (uint16_t)((l + r + u + d) >> 2));
            if (diff > thr) {
                uint32_t rep = (uint32_t)l + (uint32_t)r + (uint32_t)u + (uint32_t)d;
                img[y * w + x] = clamp_u16(rep >> 2, maxv);
            }
        }
    }
    return 0;
}

/* 简单自测：构造一个 5x5 图，注入坏点后修复 */
int main(void)
{
    const int32_t w = 5;
    const int32_t h = 5;
    uint16_t img[w * h];
    int32_t i = 0;

    for (i = 0; i < (w * h); i++) {
        img[i] = 100;
    }

    /* 注入一个坏点 */
    img[2 * w + 2] = 1000;

    printf("before: center=%u\n", img[2 * w + 2]);
    (void)bpc_2d_u16(img, w, h, 200, 1023);
    printf("after:  center=%u\n", img[2 * w + 2]);

    return 0;
}
