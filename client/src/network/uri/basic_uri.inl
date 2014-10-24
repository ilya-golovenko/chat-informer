//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------


template <typename Traits>
basic_uri<Traits>::basic_uri(std::string const& str) :
    impl::uri_base(str)
{
    if(!Traits::valid(parts_))
        throw std::invalid_argument("invalid uri string: " + str);
}

template <typename Traits>
basic_uri<Traits>::basic_uri(impl::uri_parts const& parts) :
    impl::uri_base(parts)
{
}

template <typename Traits>
std::string basic_uri<Traits>::scheme() const
{
    return Traits::scheme(parts_);
}

template <typename Traits>
std::string basic_uri<Traits>::user_info() const
{
    return Traits::user_info(parts_);
}

template <typename Traits>
std::string basic_uri<Traits>::host() const
{
    return Traits::host(parts_);
}

template <typename Traits>
std::uint16_t basic_uri<Traits>::port() const
{
    return Traits::port(parts_);
}

template <typename Traits>
std::string basic_uri<Traits>::path() const
{
    return Traits::path(parts_);
}

template <typename Traits>
std::string basic_uri<Traits>::query() const
{
    return Traits::query(parts_);
}

template <typename Traits>
std::string basic_uri<Traits>::fragment() const
{
    return Traits::fragment(parts_);
}

template <typename Traits>
std::string basic_uri<Traits>::host_info() const
{
    std::ostringstream buffer;

    if(!host().empty())
        buffer << encode_host(host());

    if(!Traits::is_default_port(parts_))
        buffer << ':' << port();

    return buffer.str();
}

template <typename Traits>
std::string basic_uri<Traits>::authority() const
{
    std::ostringstream buffer;

    if(!user_info().empty())
        buffer << encode_user_info(user_info()) << '@';

    buffer << host_info();

    return buffer.str();
}

template <typename Traits>
std::string basic_uri<Traits>::relative() const
{
    std::ostringstream buffer;

    buffer << encode_path(path());

    if(!query().empty())
        buffer << '?' << encode_query(query());

    if(!fragment().empty())
        buffer << '#' << encode_fragment(fragment());

    return buffer.str();
}

template <typename Traits>
std::string basic_uri<Traits>::to_string() const
{
    std::ostringstream buffer;

    buffer << scheme() << "://";
    buffer << authority();
    buffer << relative();

    return buffer.str();
}
