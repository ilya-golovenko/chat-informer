//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------


template <typename Traits>
basic_uri_builder<Traits>::basic_uri_builder(std::string const& str) :
    impl::uri_builder_base(basic_uri<Traits>(str))
{
}

template <typename Traits>
basic_uri_builder<Traits>::basic_uri_builder(basic_uri<Traits> const& uri) :
    impl::uri_builder_base(uri)
{
}

template <typename Traits>
basic_uri<Traits> basic_uri_builder<Traits>::get_uri() const
{
    return basic_uri<Traits>(components_);
}
