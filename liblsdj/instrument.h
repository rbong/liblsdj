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

#ifndef LSDJ_INSTRUMENT_H
#define LSDJ_INSTRUMENT_H

#include <stdbool.h>

#include "panning.h"
#include "song.h"

#ifdef __cplusplus
extern "C" {
#endif

//! The amount of instruments in a song
#define LSDJ_INSTRUMENT_COUNT (0x40)

//! The amount of bytes an instrument takes
#define LSDJ_INSTRUMENT_BYTE_COUNT (16)

//! The amount of bytes an instrument name takes
#define LSDJ_INSTRUMENT_NAME_LENGTH (5)

//! The value of an infinite pulse length
#define LSDJ_INSTRUMENT_PULSE_LENGTH_INFINITE (0x40)

//! The kind of instrument types that exist
typedef enum
{
	LSDJ_INSTRUMENT_TYPE_PULSE = 0,
	LSDJ_INSTRUMENT_TYPE_WAVE,
	LSDJ_INSTRUMENT_TYPE_KIT,
	LSDJ_INSTRUMENT_TYPE_NOISE,
} lsdj_instrument_type;

typedef enum
{
	LSDJ_INSTRUMENT_WAVE_VOLUME_0 = 0x00,
	LSDJ_INSTRUMENT_WAVE_VOLUME_1 = 0x60,
	LSDJ_INSTRUMENT_WAVE_VOLUME_2 = 0x40,
	LSDJ_INSTRUMENT_WAVE_VOLUME_3 = 0xA8
} lsdj_instrument_wave_volume;

typedef enum
{
	LSDJ_INSTRUMENT_PULSE_WIDTH_125 = 0,
	LSDJ_INSTRUMENT_PULSE_WIDTH_25,
	LSDJ_INSTRUMENT_PULSE_WIDTH_50,
	LSDJ_INSTRUMENT_PULSE_WIDTH_75
} lsdj_instrument_pulse_width;

typedef enum
{
	LSDJ_INSTRUMENT_VIBRATO_TRIANGLE = 0,
	LSDJ_INSTRUMENT_VIBRATO_SAWTOOTH,
	LSDJ_INSTRUMENT_VIBRATO_SQUARE
} lsdj_vibrato_shape;

typedef enum
{
	LSDJ_INSTRUMENT_VIBRATO_DOWN = 0,
	LSDJ_INSTRUMENT_VIBRATO_UP
} lsdj_vibrato_direction;

typedef enum
{
	LSDJ_INSTRUMENT_PLV_FAST,
	LSDJ_INSTRUMENT_PLV_TICK,
	LSDJ_INSTRUMENT_PLV_STEP,
	LSDJ_INSTRUMENT_PLV_DRUM,
} lsdj_plv_speed;

typedef enum
{
	LSDJ_INSTRUMENT_WAVE_PLAY_ONCE = 0,
	LSDJ_INSTRUMENT_WAVE_PLAY_LOOP,
	LSDJ_INSTRUMENT_WAVE_PLAY_PING_PONG,
	LSDJ_INSTRUMENT_WAVE_PLAY_MANUAL,
} lsdj_wave_play_mode;

//! Returns whether an instrument is in use
/*! @param song The song that contains the instrument
	@param instrument The index of the instrument (< LSDJ_INSTRUMENT_COUNT)
	@return True if the instrument is in use */
bool lsdj_instrument_is_allocated(const lsdj_song_t* song, uint8_t instrument);

//! Change the name of an instrument
/*! @param song The song that contains the instrument
	@param instrument The index of the instrument (< LSDJ_INSTRUMENT_COUNT)
	@param name The name to set, null-terminated or LSDJ_INSTRUMENT_NAME_LENGTH at max */
void lsdj_instrument_set_name(lsdj_song_t* song, uint8_t instrument, const char* name);

//! Retrieve the name of an instrument
/*! @param song The song that contains the instrument
	@param instrument The index of the instrument (< LSDJ_INSTRUMENT_COUNT)
	@return The name of the instrument, at maximum LSDJ_INSTRUMENT_NAME_LENGTH (may not be null-terminated) */
const char* lsdj_instrument_get_name(const lsdj_song_t* song, uint8_t instrument);

//! Change the type of an instrument
/*! @param song The song that contains the instrument
	@param instrument The index of the instrument (< LSDJ_INSTRUMENT_COUNT)
	@param type The type to set */
void lsdj_instrument_set_type(lsdj_song_t* song, uint8_t instrument, lsdj_instrument_type type);

//! Retrieve the type of an instrument
/*! @param song The song that contains the instrument
	@param instrument The index of the instrument (< LSDJ_INSTRUMENT_COUNT)
	@return The type of the instrument */
lsdj_instrument_type lsdj_instrument_get_type(const lsdj_song_t* song, uint8_t instrument);

//! Change the envelope of an instrument
/*! @param song The song that contains the instrument
	@param instrument The index of the instrument (< LSDJ_INSTRUMENT_COUNT)
	@param envelope The envelope value to set
	@note Wave and kit instruments only supports 3 volume levels, which are defined in lsdj_instrument_wave_volume */
void lsdj_instrument_set_envelope(lsdj_song_t* song, uint8_t instrument, uint8_t envelope);

//! Retrieve the envelope of an instrument
/*! @param song The song that contains the instrument
	@param instrument The index of the instrument (< LSDJ_INSTRUMENT_COUNT)
	@return The envelope of the instrument
	@note Wave and kit instruments only supports 3 volume levels, which are defined in lsdj_instrument_wave_volume */
uint8_t lsdj_instrument_get_envelope(const lsdj_song_t* song, uint8_t instrument);

//! Change the panning of an instrument
/*! @param song The song that contains the instrument
	@param instrument The index of the instrument (< LSDJ_INSTRUMENT_COUNT)
	@param panning The panning value to set */
void lsdj_instrument_set_panning(lsdj_song_t* song, uint8_t instrument, lsdj_panning panning);

//! Retrieve the panning of an instrument
/*! @param song The song that contains the instrument
	@param instrument The index of the instrument (< LSDJ_INSTRUMENT_COUNT)
	@return The panning of the instrument */
lsdj_panning lsdj_instrument_get_panning(const lsdj_song_t* song, uint8_t instrument);

//! Change the transpose of an instrument
/*! @param song The song that contains the instrument
	@param instrument The index of the instrument (< LSDJ_INSTRUMENT_COUNT)
	@param transpose Whether the instrument transposes
	@note This only makes sense for pulse and wave instruments */
void lsdj_instrument_set_transpose(lsdj_song_t* song, uint8_t instrument, bool transpose);

//! Retrieve the transpose of an instrument
/*! @param song The song that contains the instrument
	@param instrument The index of the instrument (< LSDJ_INSTRUMENT_COUNT)
	@return Whether the instrument transposes
	@note This only makes sense for pulse and wave instruments*/
bool lsdj_instrument_get_transpose(const lsdj_song_t* song, uint8_t instrument);

//! Enable or disable the table of an instrument
/*! @param song The song that contains the instrument
	@param instrument The index of the instrument (< LSDJ_INSTRUMENT_COUNT)
	@param enabled Whether the table is enabled */
void lsdj_instrument_enable_table(lsdj_song_t* song, uint8_t instrument, bool enabled);

//! Ask whether the table field of an instrument is enabled
/*! @param song The song that contains the instrument
	@param instrument The index of the instrument (< LSDJ_INSTRUMENT_COUNT)
	@return Whether the table is enabled */
bool lsdj_instrument_is_table_enabled(const lsdj_song_t* song, uint8_t instrument);

//! Change the table of an instrument
/*! @param song The song that contains the instrument
	@param instrument The index of the instrument (< LSDJ_INSTRUMENT_COUNT)
	@param table The table value to set or LSDJ_INSTRUMENT_NO_TABLE
	@note You should also call lsdj_instrument_enable_table() to enable/disable it */
void lsdj_instrument_set_table(lsdj_song_t* song, uint8_t instrument, uint8_t table);

//! Retrieve the table of an instrument
/*! @param song The song that contains the instrument
	@param instrument The index of the instrument (< LSDJ_INSTRUMENT_COUNT)
	@note You should also call lsdj_instrument_enable_table() to find out if the table is enabled in the first place
	@return The table of the instrument or LSDJ_INSTRUMENT_NO_TABLE */
uint8_t lsdj_instrument_get_table(const lsdj_song_t* song, uint8_t instrument);

//! Change the table automation of an instrument
/*! @param song The song that contains the instrument
	@param instrument The index of the instrument (< LSDJ_INSTRUMENT_COUNT)
	@param automate Whether the table automates */
void lsdj_instrument_automate_table(lsdj_song_t* song, uint8_t instrument, bool automate);

//! Retrieve the table automation of an instrument
/*! @param song The song that contains the instrument
	@param instrument The index of the instrument (< LSDJ_INSTRUMENT_COUNT)
	@return Whether the table automates */
uint8_t lsdj_instrument_is_table_automated(const lsdj_song_t* song, uint8_t instrument);

//! Change the vibrato direction of an instrument
/*! @param song The song that contains the instrument
	@param instrument The index of the instrument (< LSDJ_INSTRUMENT_COUNT)
	@param direction The direction of the vibrato to write
	@note This only makes sense for pulse and wave instruments */
void lsdj_instrument_set_vibrato_direction(lsdj_song_t* song, uint8_t instrument, lsdj_vibrato_direction direction);

//! Retrieve the vibrato direction of an instrument
/*! @param song The song that contains the instrument
	@param instrument The index of the instrument (< LSDJ_INSTRUMENT_COUNT)
	@return The direction of the vibrato
	@note This only makes sense for pulse and wave instruments*/
lsdj_vibrato_direction lsdj_instrument_get_vibrato_direction(const lsdj_song_t* song, uint8_t instrument);

//! Change the vibrato shape of an instrument
/*! @param song The song that contains the instrument
	@param instrument The index of the instrument (< LSDJ_INSTRUMENT_COUNT)
	@param shape The shape of the vibrato to write
	@note This only makes sense for pulse and wave instruments */
// void lsdj_instrument_set_vibrato_shape(lsdj_song_t* song, uint8_t instrument, lsdj_vibrato_shape shape);

//! Retrieve the vibrato shape of an instrument
/*! @param song The song that contains the instrument
	@param instrument The index of the instrument (< LSDJ_INSTRUMENT_COUNT)
	@return The shape of the vibrato
	@note This only makes sense for pulse and wave instruments*/
lsdj_vibrato_shape lsdj_instrument_get_vibrato_shape(const lsdj_song_t* song, uint8_t instrument);

//! Retrieve the P/L/V speed setting of an instrument
/*! @param song The song that contains the instrument
	@param instrument The index of the instrument (< LSDJ_INSTRUMENT_COUNT)
	@return The speed setting of P/L/V commands
	@note This only makes sense for pulse and wave instruments*/
lsdj_plv_speed lsdj_instrument_get_plv_speed(const lsdj_song_t* song, uint8_t instrument);

//! @todo Drum mode


// --- Pulse --- //

//! Change the pulse width of an instrument
/*! @param song The song that contains the instrument
	@param instrument The index of the instrument (< LSDJ_INSTRUMENT_COUNT)
	@param pulse width The pulse width to set
	@note Calling this only makes sense if the instrument is a pulse instrument */
void lsdj_instrument_set_pulse_width(lsdj_song_t* song, uint8_t instrument, lsdj_instrument_pulse_width pulseWidth);

//! Retrieve the pulse width of an instrument
/*! @param song The song that contains the instrument
	@param instrument The index of the instrument (< LSDJ_INSTRUMENT_COUNT)
	@return The pulse width of the instrument
	@note Calling this only makes sense if the instrument is a pulse instrument */
lsdj_instrument_pulse_width lsdj_instrument_get_pulse_width(const lsdj_song_t* song, uint8_t instrument);

//! Change the length of a pulse instrument
/*! @param song The song that contains the instrument
	@param instrument The index of the instrument (< LSDJ_INSTRUMENT_COUNT)
	@param length The length value to set */
void lsdj_instrument_set_pulse_length(lsdj_song_t* song, uint8_t instrument, uint8_t length);

//! Retrieve the length of a pulse instrument
/*! @param song The song that contains the instrument
	@param instrument The index of the instrument (< LSDJ_INSTRUMENT_COUNT)
	@return The length of the instrument */
uint8_t lsdj_instrument_get_pulse_length(const lsdj_song_t* song, uint8_t instrument);

//! Change the sweep of a pulse instrument
/*! @param song The song that contains the instrument
	@param instrument The index of the instrument (< LSDJ_INSTRUMENT_COUNT)
	@param sweep The sweep value to set */
void lsdj_instrument_set_pulse_sweep(lsdj_song_t* song, uint8_t instrument, uint8_t sweep);

//! Retrieve the sweep of a pulse instrument
/*! @param song The song that contains the instrument
	@param instrument The index of the instrument (< LSDJ_INSTRUMENT_COUNT)
	@return The sweep of the instrument */
uint8_t lsdj_instrument_get_pulse_sweep(const lsdj_song_t* song, uint8_t instrument);

//! Change the pulse2 tune of a pulse instrument
/*! @param song The song that contains the instrument
	@param instrument The index of the instrument (< LSDJ_INSTRUMENT_COUNT)
	@param tune The tune value to set */
void lsdj_instrument_set_pulse2_tune(lsdj_song_t* song, uint8_t instrument, uint8_t tune);

//! Retrieve the pulse2 tune of a pulse instrument
/*! @param song The song that contains the instrument
	@param instrument The index of the instrument (< LSDJ_INSTRUMENT_COUNT)
	@return The pulse2 tune of the instrument */
uint8_t lsdj_instrument_get_pulse2_tune(const lsdj_song_t* song, uint8_t instrument);

//! Change the finetune of a pulse instrument
/*! @param song The song that contains the instrument
	@param instrument The index of the instrument (< LSDJ_INSTRUMENT_COUNT)
	@param finetune The finetune value to set */
void lsdj_instrument_set_pulse_finetune(lsdj_song_t* song, uint8_t instrument, bool finetune);

//! Retrieve the finetune of a pulse instrument
/*! @param song The song that contains the instrument
	@param instrument The index of the instrument (< LSDJ_INSTRUMENT_COUNT)
	@return The finetune of the instrument */
bool lsdj_instrument_get_pulse_finetune(const lsdj_song_t* song, uint8_t instrument);


// --- Wave --- //

//! Change the synth index that a wave instrument uses
/*! @param song The song that contains the instrument
	@param instrument The index of the instrument (< LSDJ_INSTRUMENT_COUNT)
	@param synth The synth the instrument uses (0 - F) */
void lsdj_instrument_wave_set_synth(lsdj_song_t* song, uint8_t instrument, uint8_t synth);

//! Retrieve the synth index that a wave instrument uses
/*! @param song The song that contains the instrument
	@param instrument The index of the instrument (< LSDJ_INSTRUMENT_COUNT)
	@return The synth the instrument uses (0 - F) */
uint8_t lsdj_instrument_wave_get_synth(const lsdj_song_t* song, uint8_t instrument);

//! Change the play mode for a wave instrument
/*! @param song The song that contains the instrument
	@param instrument The index of the instrument (< LSDJ_INSTRUMENT_COUNT)
	@param play_mode The play mode */
void lsdj_instrument_wave_set_play_mode(lsdj_song_t* song, uint8_t instrument, lsdj_wave_play_mode mode);

//! Retrieve the play_mode index that a wave instrument uses
/*! @param song The song that contains the instrument
	@param instrument The index of the instrument (< LSDJ_INSTRUMENT_COUNT)
	@return The play mode */
lsdj_wave_play_mode lsdj_instrument_wave_get_play_mode(const lsdj_song_t* song, uint8_t instrument);

//! Change the length value for a wave instrument
/*! @param song The song that contains the instrument
	@param instrument The index of the instrument (< LSDJ_INSTRUMENT_COUNT)
	@param length The length value (0 - F) */
void lsdj_instrument_wave_set_length(lsdj_song_t* song, uint8_t instrument, uint8_t length);

//! Retrieve the length index that a wave instrument uses
/*! @param song The song that contains the instrument
	@param instrument The index of the instrument (< LSDJ_INSTRUMENT_COUNT)
	@return The length value (0 - F) */
uint8_t lsdj_instrument_wave_get_length(const lsdj_song_t* song, uint8_t instrument);

//! Change the repeat value for a wave instrument
/*! @param song The song that contains the instrument
	@param instrument The index of the instrument (< LSDJ_INSTRUMENT_COUNT)
	@param repeat The repeat value (0 - F) */
void lsdj_instrument_wave_set_repeat(lsdj_song_t* song, uint8_t instrument, uint8_t repeat);

//! Retrieve the repeat index that a wave instrument uses
/*! @param song The song that contains the instrument
	@param instrument The index of the instrument (< LSDJ_INSTRUMENT_COUNT)
	@return The repeat value (0 - F) */
uint8_t lsdj_instrument_wave_get_repeat(const lsdj_song_t* song, uint8_t instrument);

//! Change the speed value for a wave instrument
/*! @param song The song that contains the instrument
	@param instrument The index of the instrument (< LSDJ_INSTRUMENT_COUNT)
	@param speed The speed value (0 - F) */
void lsdj_instrument_wave_set_speed(lsdj_song_t* song, uint8_t instrument, uint8_t speed);

//! Retrieve the speed index that a wave instrument uses
/*! @param song The song that contains the instrument
	@param instrument The index of the instrument (< LSDJ_INSTRUMENT_COUNT)
	@return The speed value (0 - F) */
uint8_t lsdj_instrument_wave_get_speed(const lsdj_song_t* song, uint8_t instrument);
    
#ifdef __cplusplus
}
#endif

#endif
