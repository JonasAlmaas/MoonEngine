#pragma once

#define BIT_MASK(n) (((unsigned long long)0x1) << (n))
#define BYTE_MASK(n) (((unsigned long long)0xff) << ((n) * 8))
#define BIT_MASK_RANGE(start, end) ((BIT_MASK((end) - (start) + 1) - 1) << (start))
#define BYTE_MASK_RANGE(start, end) (BIT_MASK_RANGE((start) * 8, (end + 1) * 8 - 1))

#define BIT_VALUE(x, n) (((x) & BIT_MASK(n)) >> (n))
#define BYTE_VALUE(x, n) ((uint8_t)(((x) & BYTE_MASK(n)) >> ((n) * 8)))
#define BIT_VALUE_RANGE(x, start, end) (((x) & BIT_MASK_RANGE(start, end)) >> (start))
#define BYTE_VALUE_RANGE(x, start, end) (((x) & BYTE_MASK_RANGE(start, end)) >> (start * 8))
