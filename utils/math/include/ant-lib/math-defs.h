#pragma once

#define ANT_MIN(x, y) (((x) < (y)) ? (x) : (y))

#define ANT_DIV_UP(x, y) ((((x) - 1) / (y)) + 1)

#define ANT_FLIP_BYTES(x, y) { uint8_t _ant_flip_bytes_buffer = (x); (x) = (y); (y) = _ant_flip_bytes_buffer; }