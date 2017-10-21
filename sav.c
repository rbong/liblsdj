#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "compression.h"
#include "sav.h"

//! The memory place of the header
static const unsigned int HEADER_START = SONG_DECOMPRESSED_SIZE;
static const unsigned int BLOCK_COUNT = 191;
static const unsigned int BLOCK_SIZE = 0x200;

typedef struct
{
	char project_names[SAV_PROJECT_COUNT * 8];
	unsigned char versions[SAV_PROJECT_COUNT * 1];
	unsigned char empty[30];
	char init[2];
	unsigned char active_project;
} header_t;

// Read compressed project data from memory sav file
void read_compressed_blocks(lsdj_vio_read_t read, lsdj_vio_seek_t seek, void* user_data, lsdj_project_t* projects, lsdj_error_t** error)
{
    // Read the block allocation table
    unsigned char blocks_alloc_table[BLOCK_COUNT];
    read(blocks_alloc_table, sizeof(blocks_alloc_table), user_data);
    
    // Read the blocks
    unsigned char blocks[BLOCK_COUNT][BLOCK_SIZE];
    read(blocks, sizeof(blocks), user_data);
    
    // Pointers for storing decompressed song data
    // Handle decompression
    for (unsigned char i = 0; i < BLOCK_COUNT; ++i)
    {
        unsigned char project = blocks_alloc_table[i];
        if (projects[project].song)
            continue;
        
        unsigned char data[SONG_DECOMPRESSED_SIZE];
        lsdj_decompress(&blocks[0][0], i, BLOCK_SIZE, data);
        
        // Read the song from memory
        projects[project].song = malloc(sizeof(lsdj_song_t));
        lsdj_read_song_from_memory(data, projects[project].song, error);
        if (error)
            return;
    }
}

void lsdj_read_sav(lsdj_vio_read_t read, lsdj_vio_seek_t seek, void* user_data, lsdj_sav_t* sav, lsdj_error_t** error)
{
    // Check for incorrect input
    if (read == NULL)
        return lsdj_create_error(error, "read is NULL");
    
    if (seek == NULL)
        return lsdj_create_error(error, "seek is NULL");
    
    if (sav == NULL)
        return lsdj_create_error(error, "sav is NULL");
    
    // Skip memory representing the working song (we'll get to that)
    seek(HEADER_START, SEEK_SET, user_data);
    
    // Read the header block, before we start processing each song
    header_t header;
    read(&header, sizeof(header), user_data);
    
    // Check the initialization characters. If they're not 'jk', we're
    // probably not dealing with an actual LSDJ sav format file.
    if (header.init[0] != 'j' || header.init[1] != 'k')
        return lsdj_create_error(error, "SRAM initialization check wasn't 'jk'");
    
    // Allocate data for all the projects and store their names
    for (int i = 0; i < SAV_PROJECT_COUNT; ++i)
    {
        memcpy(sav->projects[i].name, &header.project_names[i * 8], 8);
        sav->projects[i].version = header.versions[i];
    }
    
    // Store the active project index
    sav->active_project = header.active_project;
    
    // Read the compressed projects
    read_compressed_blocks(read, seek, user_data, sav->projects, error);
    if (error)
        return;
    
    // Read the working song
    seek(0, SEEK_SET, user_data);
    unsigned char song_data[SONG_DECOMPRESSED_SIZE];
    read(song_data, sizeof(song_data), user_data);
    lsdj_read_song_from_memory(song_data, &sav->song, error);
}

static size_t lsdj_fread(void* ptr, size_t count, void* user_data)
{
    return fread(ptr, count, 1, (FILE*)user_data);
}

static int lsdj_fseek(long offset, int whence, void* user_data)
{
    return fseek((FILE*)user_data, offset, whence);
}

void lsdj_read_sav_from_file(const char* path, lsdj_sav_t* sav, lsdj_error_t** error)
{
    FILE* file = fopen("/Users/stijnfrishert/Desktop/LSDj/4ntler/Chipwrecked Set.sav", "r");
    if (file == NULL)
        return lsdj_create_error(error, "could not open file");

    lsdj_read_sav(lsdj_fread, lsdj_fseek, file, sav, error);
    
    fclose(file);
}

typedef struct
{
    const unsigned char* begin;
    const unsigned char* cur;
    size_t size;
} lsdj_memory_data_t;

static size_t lsdj_mread(void* ptr, size_t count, void* user_data)
{
    lsdj_memory_data_t* mem = (lsdj_memory_data_t*)user_data;
    
    memcpy(ptr, mem->cur, count);
    mem->cur += count;
    
    return count;
}

static int lsdj_mseek(long offset, int whence, void* user_data)
{
    lsdj_memory_data_t* mem = (lsdj_memory_data_t*)user_data;
    
    switch (whence)
    {
        case SEEK_SET: mem->cur = mem->begin + offset; break;
        case SEEK_CUR: mem->cur = mem->cur + offset; break;
        case SEEK_END: mem->cur = mem->begin + mem->size + offset; break;
    }
    
    return 0;
}

void lsdj_read_sav_from_memory(const unsigned char* data, size_t size, lsdj_sav_t* sav, lsdj_error_t** error)
{
    if (data == NULL)
        return lsdj_create_error(error, "data is NULL");

    lsdj_memory_data_t mem;
    mem.begin = data;
    mem.cur = mem.begin;
    mem.size = size;
    
    lsdj_read_sav(lsdj_mread, lsdj_mseek, &mem, sav, error);
}

void lsdj_write_sav(const lsdj_sav_t* sav, const char* path, lsdj_error_t** error)
{
    if (sav == NULL)
        return lsdj_create_error(error, "sav is NULL");
    
    FILE* file = fopen(path, "w");
    if (file == NULL)
        return lsdj_create_error(error, "could not open file for writing");
    
    // Write the working project
    unsigned char song_data[SONG_DECOMPRESSED_SIZE];
    lsdj_write_song_to_memory(&sav->song, song_data);
    fwrite(song_data, sizeof(song_data), 1, file);
    
    // Create the header for writing
    header_t header;
    memset(&header, 0, sizeof(header));
    header.init[0] = 'j';
    header.init[1] = 'k';
    header.active_project = sav->active_project;
    
    // Create the block allocation table for writing
    unsigned char block_alloc_table[BLOCK_COUNT];
    memset(&block_alloc_table, 0xFF, sizeof(block_alloc_table));
    unsigned char* table_ptr = block_alloc_table;
    
    // Write project specific data
    unsigned char blocks[BLOCK_SIZE][BLOCK_COUNT];
    unsigned char current_block = 0;
    memset(blocks, 0, sizeof(blocks));
    for (int i = 0; i < SAV_PROJECT_COUNT; ++i)
    {
        // Write project name
        memcpy(&header.project_names[i * 8], sav->projects[i].name, 8);
        
        // Write project version
        header.versions[i] = sav->projects[i].version;
        
        if (sav->projects[i].song)
        {
            // Compress the song to memory
            unsigned char song_data[SONG_DECOMPRESSED_SIZE];
            lsdj_write_song_to_memory(sav->projects[i].song, song_data);
            unsigned int written_block_count = lsdj_compress(song_data, &blocks[0][0], BLOCK_SIZE, current_block, BLOCK_COUNT);
            
            current_block += written_block_count;
            for (int j = 0; j < written_block_count; ++j)
                *table_ptr++ = (unsigned char)i;
        }
    }
    
    // Write the header and blocks
    fwrite(&header, sizeof(header), 1, file);
    fwrite(&block_alloc_table, sizeof(block_alloc_table), 1, file);
    fwrite(blocks, sizeof(blocks), 1, file);
    
    // Close the file
    fclose(file);
}

void lsdj_clear_sav(lsdj_sav_t* sav)
{
    for (int i = 0; i < SAV_PROJECT_COUNT; ++i)
        lsdj_clear_project(&sav->projects[i]);
    
    sav->active_project = 0;
}
