# Questions

## What's `stdint.h`?

A header file that bmp.h needs.

## What's the point of using `uint8_t`, `uint32_t`, `int32_t`, and `uint16_t` in a program?

To precisely use a certain number of bits for each data type.

## How many bytes is a `BYTE`, a `DWORD`, a `LONG`, and a `WORD`, respectively?

1, 4, 4, 2.

## What (in ASCII, decimal, or hexadecimal) must the first two bytes of any BMP file be? Leading bytes used to identify file formats (with high probability) are generally called "magic numbers."

0x424d.

## What's the difference between `bfSize` and `biSize`?

bfSize is the size of the whole file. biSize is the size of BITMAPINFOHEADER. Both in bytes.

## What does it mean if `biHeight` is negative?

The start is the upper left corner and works its way down.

## What field in `BITMAPINFOHEADER` specifies the BMP's color depth (i.e., bits per pixel)?

biBitCount.

## Why might `fopen` return `NULL` in lines 24 and 32 of `copy.c`?

It might return NULL if there is no memory available.

## Why is the third argument to `fread` always `1` in our code?

Because there is only 1 of each type of header.

## What value does line 65 of `copy.c` assign to `padding` if `bi.biWidth` is `3`?

3.

## What does `fseek` do?

It moves the position of where you are in the file.

## What is `SEEK_CUR`?

It is the current position of the cursor in the file.

## Whodunit?

It was Professor Plum with the candlestick in the library.
