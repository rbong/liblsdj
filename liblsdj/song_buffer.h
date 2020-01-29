/*
 
 This file is a part of liblsdj, a C library for managing everything
 that has to do with LSDJ, software for writing music (chiptune) with
 your gameboy. For more information, see:
 
 * https://github.com/stijnfrishert/liblsdj
 * http://www.littlesounddj.com
 
 --------------------------------------------------------------------------------
 
 MIT License
 
 Copyright (c) 2018 - 2020 Stijn Frishert
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 
 */

#ifndef LSDJ_SONG_BUFFER_H
#define LSDJ_SONG_BUFFER_H

/* Song buffers are the representation of an LSDJ song uncompressed in memory.
   It hasn't been parsed yet into meaningful data. This is raw song data, and
   for example how the song in working memory within your LSDJ sav is repre-
   sented.

   From here, you can either compress song buffers into memory blocks, which is
   how all other projects in LSDJ are represented. You can also go the other
   way around, and parse them into an lsdj_song_t structure, providing you with
   detailed information about elements within your song. */

#ifdef __cplusplus
extern "C" {
#endif

#include "error.h"

//! The size of a decompressed song in memory
#define LSDJ_SONG_BUFFER_BYTES_COUNT (0x8000)

//! A structure that can hold one entire decompressed song in memory
typedef struct
{
	unsigned char bytes[LSDJ_SONG_BUFFER_BYTES_COUNT];
} lsdj_song_buffer_t;
    
#ifdef __cplusplus
}
#endif

#endif