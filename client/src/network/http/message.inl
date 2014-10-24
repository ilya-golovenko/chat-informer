//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------


template <typename T> void message::add_header(std::string const& name, T const& value)
{
    add_header(name, boost::lexical_cast<std::string>(value));
}

template <typename T> void message::set_header(std::string const& name, T const& value)
{
    set_header(name, boost::lexical_cast<std::string>(value));
}

template <typename T> boost::optional<T> message::get_header(std::string const& name) const
{
    boost::optional<std::string> value = get_header(name);

    if(value)
        return boost::lexical_cast<T>(*value);

    return boost::optional<T>();
}

template <typename Iterator> void message::set_content(Iterator first, Iterator last)
{
    content_.assign(first, last);
}

template <typename Iterator> void message::add_content(Iterator first, Iterator last)
{
    content_.append(first, last);
}
