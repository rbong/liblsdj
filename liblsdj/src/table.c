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

#include "table.h"

#include <assert.h>

#define ALLOCATION_TABLE_OFFSET (0x2020)
#define ALLOCATION_TABLE_LENGTH (0x32)

#define ENVELOPES_OFFSET (0x1690)
#define TRANSPOSITION_OFFSET (0x3480)
#define COMMAND1_OFFSET (0x3680)
#define COMMAND1_VALUE_OFFSET (0x3880)
#define COMMAND2_OFFSET (0x3a80)
#define COMMAND2_VALUE_OFFSET (0x3c80)

#define CONTENT_LENGTH (512)

#define TABLE_SETTER(OFFSET, LENGTH, VALUE) \
const size_t index = table * LSDJ_TABLE_LENGTH + step; \
assert(index <= LENGTH); \
song->bytes[OFFSET + index] = VALUE;

#define TABLE_GETTER(OFFSET, LENGTH) \
const size_t index = table * LSDJ_TABLE_LENGTH + step; \
assert(index <= LENGTH); \
return song->bytes[OFFSET + index];

bool lsdj_table_is_allocated(const lsdj_song_t* song, uint8_t table)
{
    const size_t index = ALLOCATION_TABLE_OFFSET + table;
    assert(index <= ALLOCATION_TABLE_OFFSET + ALLOCATION_TABLE_LENGTH);

    return song->bytes[index];
}

void lsdj_table_set_envelope(lsdj_song_t* song, uint8_t table, uint8_t step, uint8_t value)
{
    TABLE_SETTER(ENVELOPES_OFFSET, CONTENT_LENGTH, value);
}

uint8_t lsdj_table_get_envelope(const lsdj_song_t* song, uint8_t table, uint8_t step)
{
	TABLE_GETTER(ENVELOPES_OFFSET, CONTENT_LENGTH);
}

void lsdj_table_set_transposition(lsdj_song_t* song, uint8_t table, uint8_t step, uint8_t value)
{
	TABLE_SETTER(TRANSPOSITION_OFFSET, CONTENT_LENGTH, value);
}

uint8_t lsdj_table_get_transposition(const lsdj_song_t* song, uint8_t table, uint8_t step)
{
	TABLE_GETTER(TRANSPOSITION_OFFSET, CONTENT_LENGTH);
}

bool lsdj_table_set_command1(lsdj_song_t* song, uint8_t table, uint8_t step, lsdj_command command)
{
    if (lsdj_song_get_format_version(song) >= 8)
    {
        uint8_t byte = 0;
        if (command == LSDJ_COMMAND_B)
            byte = 1;
        else if (command > 1)
            byte = ((uint8_t)command) + 1;
        else
            byte = (uint8_t)command;
            
        const size_t index = table * LSDJ_TABLE_LENGTH + step;
        assert(index <= CONTENT_LENGTH);
        song->bytes[COMMAND1_OFFSET + index] = byte;
    } else {
        if (command == LSDJ_COMMAND_B)
            return false;
        
        TABLE_SETTER(COMMAND1_OFFSET, CONTENT_LENGTH, (uint8_t)command)
    }
    
    return true;
}

lsdj_command lsdj_table_get_command1(const lsdj_song_t* song, uint8_t table, uint8_t step)
{
	if (lsdj_song_get_format_version(song) >= 8)
    {
        const size_t index = table * LSDJ_TABLE_LENGTH + step;
        assert(index <= COMMAND1_OFFSET);
        
        const uint8_t byte = song->bytes[COMMAND2_OFFSET + index];
        
        if (byte > 1)
            return (lsdj_command)(byte - 1);
        else if (byte == 1)
            return LSDJ_COMMAND_B;
        else
            return (lsdj_command)byte;
    } else {
        TABLE_GETTER(COMMAND1_OFFSET, COMMAND2_OFFSET)
    }
}

void lsdj_table_set_command1_value(lsdj_song_t* song, uint8_t table, uint8_t step, uint8_t value)
{
	TABLE_SETTER(COMMAND1_VALUE_OFFSET, CONTENT_LENGTH, value);
}

uint8_t lsdj_table_get_command1_value(const lsdj_song_t* song, uint8_t table, uint8_t step)
{
	TABLE_GETTER(COMMAND1_VALUE_OFFSET, CONTENT_LENGTH);
}

bool lsdj_table_set_command2(lsdj_song_t* song, uint8_t table, uint8_t step, lsdj_command command)
{
    if (lsdj_song_get_format_version(song) >= 8)
    {
        uint8_t byte = 0;
        if (command == LSDJ_COMMAND_B)
            byte = 1;
        else if (command > 1)
            byte = ((uint8_t)command) + 1;
        else
            byte = (uint8_t)command;
            
        const size_t index = table * LSDJ_TABLE_LENGTH + step;
        assert(index <= CONTENT_LENGTH);
        song->bytes[COMMAND2_OFFSET + index] = byte;
    } else {
        if (command == LSDJ_COMMAND_B)
            return false;
        
        TABLE_SETTER(COMMAND2_OFFSET, CONTENT_LENGTH, (uint8_t)command)
    }
    
    return true;
}

lsdj_command lsdj_table_get_command2(const lsdj_song_t* song, uint8_t table, uint8_t step)
{
    if (lsdj_song_get_format_version(song) >= 8)
    {
        const size_t index = table * LSDJ_TABLE_LENGTH + step;
        assert(index <= COMMAND2_OFFSET);
        
        const uint8_t byte = song->bytes[COMMAND2_OFFSET + index];
        
        if (byte > 1)
            return (lsdj_command)(byte - 1);
        else if (byte == 1)
            return LSDJ_COMMAND_B;
        else
            return (lsdj_command)byte;
    } else {
        TABLE_GETTER(COMMAND2_OFFSET, COMMAND2_OFFSET)
    }
}

void lsdj_table_set_command2_value(lsdj_song_t* song, uint8_t table, uint8_t step, uint8_t value)
{
	TABLE_SETTER(COMMAND2_VALUE_OFFSET, CONTENT_LENGTH, value);
}

uint8_t lsdj_table_get_command2_value(const lsdj_song_t* song, uint8_t table, uint8_t step)
{
	TABLE_GETTER(COMMAND2_VALUE_OFFSET, CONTENT_LENGTH);
}