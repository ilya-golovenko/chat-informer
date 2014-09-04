//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_core_download_hpp
#define _chat_core_download_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200) 

// Application headers
#include <net/http/download.hpp>

// BOOST headers
#include <boost/system/error_code.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/signals2.hpp>
#include <boost/cstdint.hpp>

// STL headers
#include <string>


namespace chat
{

class download :
    public net::http::download
{
public:
    static pointer create(net::http::uri const& uri, boost::filesystem::path const& filename);

public:
    download(net::http::uri const& uri, boost::filesystem::path const& filename);
    virtual ~download();

    boost::filesystem::path const& filename() const;

    template <typename Handler>
    void on_progress(Handler const& handler)
    {
        progress_signal_.connect(handler);
    }

    template <typename Handler>
    void on_completion(Handler const& handler)
    {
        complete_signal_.connect(handler);
    }

protected:
    virtual void handle_response(net::http::response const& response);
    virtual void handle_content(net::http::content_range const& content);
    virtual void handle_completed(boost::system::error_code const& error);

private:
    bool update_percent_downloaded(std::size_t size);

private:
    typedef boost::signals2::signal<bool (pointer, unsigned int)> progress_signal_type;
    typedef boost::signals2::signal<void (pointer, boost::system::error_code const&)> complete_signal_type;

private:
    boost::uint64_t total_size_;
    boost::uint64_t current_size_;

    unsigned int percent_downloaded_;

    boost::filesystem::ofstream file_;
    boost::filesystem::path filename_;

    progress_signal_type progress_signal_;
    complete_signal_type completion_signal_;
};

}   // namespace chat

#endif  // _chat_core_download_hpp
