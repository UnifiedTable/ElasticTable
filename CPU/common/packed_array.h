#pragma once
// Get the value of the idx-th element of arr (bits indicates element size).
#define ACCESS(arr, bits, totalbits, idx)        \
    ((((arr)[(idx) / (totalbits / (bits))]) >>   \
      (totalbits - (1 + (idx) % (totalbits / (bits))) * (bits))) & \
     ((1 << (bits)) - 1))

// Set the idx-th element of arr to 0 (bits indicates element size).
#define CLEAR(arr, bits, totalbits, idx)   \
    (arr)[(idx) / (totalbits / (bits))] &= \
        ~(((1 << (bits)) - 1) << (totalbits - (1 + (idx) % (totalbits / (bits))) * (bits)))

// Set the idx-th element of arr to val (bits indicates element size).
#define WRITE(arr, bits, totalbits, idx, val) \
    CLEAR((arr), (bits), (totalbits), (idx)); \
    (arr)[(idx) / (totalbits / (bits))] |=    \
        ((val) % (1 << (bits))) << (totalbits - (1 + (idx) % (totalbits / (bits))) * (bits))

// Shortcuts to manage 32-bit and 8-bit packed arrays.
#define ACCESS32(arr, bits, idx) ACCESS(arr, bits, 32, idx)
#define CLEAR32(arr, bits, idx) CLEAR(arr, bits, 32, idx)
#define WRITE32(arr, bits, idx, val) WRITE(arr, bits, 32, idx, val)
#define ACCESS8(arr, bits, idx) ACCESS(arr, bits, 8, idx)
#define CLEAR8(arr, bits, idx) CLEAR(arr, bits, 8, idx)
#define WRITE8(arr, bits, idx, val) WRITE(arr, bits, 8, idx, val)

// Conversion between unsigned and signed integers.
#define U2INT(x, bits) (((int32_t)(x) << (32 - (bits))) >> (32 - (bits)))
#define INT2U(x, bits) ((x) & ((1 << (bits)) - 1))