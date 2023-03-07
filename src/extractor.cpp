#include "extractor.hpp"

#include <iostream>
#include <boost/filesystem.hpp>


void LP1CExtractor::read_file_entry(FileEntry &entry)
{
    ifs.read((char*)&entry.filename_length, sizeof(entry.filename_length));
    entry.filename.resize(entry.filename_length);
    ifs.read((char*)entry.filename.data(), entry.filename_length);
    ifs.read((char*)&entry.size, sizeof(entry.size));
    ifs.read((char*)&entry.offset, sizeof(entry.offset));
    ifs.read((char*)entry.uid, sizeof(entry.uid));
}

void LP1CExtractor::read_dir_entry(DirEntry &entry)
{
    ifs.read((char*)&entry.dirname_length, sizeof(entry.dirname_length));
    entry.dirname.resize(entry.dirname_length);
    ifs.read((char*)entry.dirname.data(), entry.dirname_length);
    ifs.read((char*)entry.uid, sizeof(entry.uid));
    ifs.read((char*)&entry.num_files, sizeof(entry.num_files));
}

void LP1CExtractor::read_header(Header &header)
{
    ifs.read((char*)header.magic, sizeof(header.magic));
    ifs.read((char*)&header.num_subdirs, sizeof(header.num_subdirs));
    ifs.read((char*)&header.num_files, sizeof(header.num_files));
}

LP1CExtractor::LP1CExtractor(const std::string &filename)
{
    this->filename = filename;
    this->outdir = filename.substr(0, filename.find_last_of('.'));

    ifs.open(filename, std::ios::binary);
    if (!ifs.is_open())
    {
        std::cout << "Failed to open " << filename << std::endl;
        return;
    }

    read_header(header);

    // create root dir
    filemap["."] = std::vector<FileEntry>();

    // read file entries
    for (unsigned int i = 0; i < header.num_files; i++)
    {
        FileEntry entry;
        read_file_entry(entry);

        // add to filemap
        filemap["."].push_back(entry);
    }

    // read dir entries
    for (unsigned int i = 0; i < header.num_subdirs; i++)
    {
        DirEntry entry;
        read_dir_entry(entry);

        // create dir
        filemap[entry.dirname] = std::vector<FileEntry>();

        // read file entries
        for (unsigned int j = 0; j < entry.num_files; j++)
        {
            FileEntry file_entry;
            read_file_entry(file_entry);

            // add to filemap
            filemap[entry.dirname].push_back(file_entry);
        }
    }
}

LP1CExtractor::~LP1CExtractor()
{
    ifs.close();
}

void LP1CExtractor::extract_file(const FileEntry &entry, const std::string &dirname)
{
    std::string outpath = dirname + "/" + entry.filename;
    std::cout << "Extracting " << entry.filename << " to " << outpath << std::endl;

    // create dir
    boost::filesystem::create_directories(dirname);

    // open file
    std::ofstream ofs(outpath, std::ios::binary);
    if (!ofs.is_open())
    {
        std::cout << "Failed to open " << outpath << std::endl;
        return;
    }

    // seek to offset
    ifs.seekg(entry.offset);

    // read data
    char *data = new char[entry.size];
    ifs.read(data, entry.size);

    // write data
    ofs.write(data, entry.size);

    // close file
    ofs.close();

    // delete data
    delete[] data;
}


void LP1CExtractor::extract_all()
{
    for (auto &pair : filemap)
    {
        std::string dirname = outdir + "/" + pair.first;
        for (auto &entry : pair.second)
        {
            extract_file(entry, dirname);
        }
    }
}

