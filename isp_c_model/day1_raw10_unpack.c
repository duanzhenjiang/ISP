#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

/*
 * Day1-Task B: RAW10 unpack
 *
 * 学习目的：
 * 1) 牢记 RAW10 打包：每 5 字节 = 4 像素（10bit）
 * 2) unpack 后用 16bit 存储，便于后续 ISP 处理与定点化
 * 3) 理解 stride / 对齐 / DMA 行尾 padding 的影响
 */

int32_t raw10_unpack_to_u16(const uint8_t *src, size_t src_len,
                            uint16_t *dst, size_t dst_pixels)
{
    size_t si = 0;
    size_t di = 0;

    if ((NULL == src) || (NULL == dst)) {
        return -1;
    }

    while ((si + 4U) < src_len && (di + 3U) < dst_pixels) {
        uint8_t b0 = src[si + 0U];
        uint8_t b1 = src[si + 1U];
        uint8_t b2 = src[si + 2U];
        uint8_t b3 = src[si + 3U];
        uint8_t b4 = src[si + 4U];

        dst[di + 0U] = (uint16_t)((((uint16_t)b0) << 2) | ((b4 >> 0) & 0x3));
        dst[di + 1U] = (uint16_t)((((uint16_t)b1) << 2) | ((b4 >> 2) & 0x3));
        dst[di + 2U] = (uint16_t)((((uint16_t)b2) << 2) | ((b4 >> 4) & 0x3));
        dst[di + 3U] = (uint16_t)((((uint16_t)b3) << 2) | ((b4 >> 6) & 0x3));

        si += 5U;
        di += 4U;
    }

    return 0;
}

/* 简单自测：构造 4 个像素，打包到 5 字节后再解包 */
int main(void)
{
    uint8_t packed[5] = {0};
    uint16_t unpacked[4] = {0};

    /* 假设 10bit 像素值 */
    uint16_t p0 = 0x12A; /* 298 */
    uint16_t p1 = 0x155; /* 341 */
    uint16_t p2 = 0x2AA; /* 682 */
    uint16_t p3 = 0x3FF; /* 1023 */

    /* 手动打包 RAW10: 高 8bit 放在 b0..b3，低 2bit 放在 b4 */
    packed[0] = (uint8_t)(p0 >> 2);
    packed[1] = (uint8_t)(p1 >> 2);
    packed[2] = (uint8_t)(p2 >> 2);
    packed[3] = (uint8_t)(p3 >> 2);
    packed[4] = (uint8_t)(((p0 & 0x3) << 0)
                        | ((p1 & 0x3) << 2)
                        | ((p2 & 0x3) << 4)
                        | ((p3 & 0x3) << 6));

    (void)raw10_unpack_to_u16(packed, sizeof(packed), unpacked, 4);

    printf("unpacked: %u %u %u %u\n",
           unpacked[0], unpacked[1], unpacked[2], unpacked[3]);
    return 0;
}
