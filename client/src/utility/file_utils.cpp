//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <utility/file_utils.hpp>

// BOOST headers
#include <boost/filesystem/fstream.hpp>

// STL headers
#include <stdexcept>
#include <string>


namespace util
{
namespace file
{

std::string read(boost::filesystem::path const& filename)
{
    boost::filesystem::ifstream file(filename, std::ios::binary);

    if(!file.is_open())
        throw std::runtime_error("cannot open file: " + filename.string());

    char buffer[2048];
    std::string result;

    while(file.good() && !file.eof())
    {
        file.read(buffer, sizeof(buffer));
        std::streamsize count = file.gcount();
        result.append(buffer, std::size_t(count));
    }

    file.close();

    return result;
}

void write(boost::filesystem::path const& filename, std::string const& data)
{
    boost::filesystem::ofstream file(filename, std::ios::trunc | std::ios::binary);

    if(!file.is_open())
        throw std::runtime_error("cannot create file: " + filename.string());

    file.write(data.data(), data.size());
    file.close();
}

void append(boost::filesystem::path const& filename, std::string const& data)
{
    boost::filesystem::ofstream file(filename, std::ios::app | std::ios::binary);

    if(!file.is_open())
        throw std::runtime_error("cannot append file: " + filename.string());

    file.write(data.data(), data.size());
    file.close();
}

}   // namespace file
}   // namespace util
