//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <core/common.hpp>
#include <core/download.hpp>

// BOOST headers
#include <boost/filesystem/operations.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/make_shared.hpp>
#include <boost/asio.hpp>

// STL headers
#include <iterator>


namespace missio
{

download::pointer download::create(net::http::uri const& uri, boost::filesystem::path const& filename)
{
    return boost::make_shared<download>(uri, filename);
}

download::download(net::http::uri const& uri, boost::filesystem::path const& filename) :
    net::http::download(uri),
    total_size_(0),
    current_size_(0),
    percent_downloaded_(0),
    filename_(filename)
{
    //request_.set_user_agent(missio::informer_name);
}

download::~download()
{
}

void download::handle_response(net::http::response const& response)
{
    boost::optional<boost::uint64_t> content_length;
    content_length = response.get_content_length();

    if(content_length)
        total_size_ = *content_length;

    if(boost::filesystem::exists(filename_))
        boost::filesystem::remove(filename_);

    if(!boost::filesystem::exists(filename_.parent_path()))
        boost::filesystem::create_directories(filename_.parent_path());

    file_.open(filename_, std::ios::out | std::ios::binary);

    if(!file_.is_open())
        abort(boost::asio::error::bad_descriptor);
}

void download::handle_content(net::http::content_range const& content)
{
    boost::copy(content, std::ostream_iterator<char>(file_));

    if(file_.good())
    {
        //if(update_percent_downloaded(content.size()))
        //    progress_handler_(shared_from_this(), percent_downloaded_);
    }
    else
    {
        abort(boost::asio::error::bad_descriptor);
    }
}

void download::handle_completed(boost::system::error_code const& error)
{
    file_.close();

    if(error)
    {
        if(boost::filesystem::exists(filename_))
            boost::filesystem::remove(filename_);
    }

    //if(error != boost::asio::error::operation_aborted)
    //    complete_signal_(shared_from_this(), error);
}

bool download::update_percent_downloaded(std::size_t size)
{
    if(total_size_ > 0)
    {
        current_size_ += size;

        boost::uint64_t ratio = current_size_ / total_size_;
        unsigned int percent = 100 * static_cast<int>(ratio);

        if(percent > percent_downloaded_)
        {
            //percent_completed_ = percent;
            return true;
        }
    }

    return false;
}

}    // namespace missio
