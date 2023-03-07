#pragma once

#include <cstdint>
#include <fstream>

struct Header
{
    char magic[4]; // always LP1C
    uint32_t num_subdirs; // number of subdirectories
    uint32_t num_files; // number of files in the root directory (not incl. subdirectories)
};

struct FileEntry
{
    uint8_t filename_length;
    std::string filename;
    
    uint32_t size;   // size in bytes of the file data
    uint32_t offset; // byte offset of the file data
    uint8_t uid[8];  // unused and unknown
};

struct DirEntry
{
    uint8_t dirname_length;
    std::string dirname;

    uint8_t uid[4];     // unused and unknown 
    uint32_t num_files; // number of files in the subdirectory
};


