#pragma once

#include <cstdint>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#include "LP1C.hpp"

class LP1CExtractor
{
public:
    LP1CExtractor(const std::string& filename);
    ~LP1CExtractor();


    void extract_file(const FileEntry& entry, const std::string& dirname);
    void extract_all();


private:

    void read_header(Header& header);
    void read_file_entry(FileEntry& entry);
    void read_dir_entry(DirEntry& entry);

    std::ifstream ifs;
    std::string filename;
    std::string outdir;

    Header header;
    std::map<std::string, std::vector<FileEntry>> filemap; // dirname -> file entries
    std::vector<DirEntry> subdirs; // dir entries
};
