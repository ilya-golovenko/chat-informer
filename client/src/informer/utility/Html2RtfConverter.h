//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once

// MISSIO headers
#include <missio/rtf/document.hpp>


//TODO: convert::to_string - use boost::spirit::karma
//      convert::from_string - use boost::spirit::qi

// create separate RTF library (missio_rtf)

//TODO: reuse rtf_font_t and fonts_!
//TODO: use boost::spirit or boost::lex for parsing of HTML data

//TODO: http://www.codeproject.com/KB/HTML/XHTML2RTF.aspx
//TODO: http://interglacial.com/rtf/
//TODO: http://search.cpan.org/~sburke/RTF-Writer/lib/RTF/Cookbook.pod
//TODO: http://www.codeproject.com/KB/library/rtflib.aspx

//TODO: \u1576 - this is a single Unicode word (UCS-2, signed int16: -32768..+32767)

enum html_tag_types
{
    tag_b,
    tag_i,
    tag_em,
    tag_u,
    tag_strike,
    tag_br,
    tag_div,
    tag_sub,
    tag_sup,
    tag_p,
    tag_ul,
    tag_li,
    tag_body,
    tag_a,
    tag_span,
    tag_style,

    tag_unknown
};

struct rtf_range_t
{
    std::size_t begin;
    std::size_t end;

    bool contains(std::size_t pos) const
    {
        return pos >= begin && pos <= end;
    }

    bool operator==(rtf_range_t const& other) const
    {
        return begin == other.begin && end == other.end;
    }
};

struct rtf_link_t
{
    std::string url;
    rtf_range_t range;

    bool operator==(rtf_link_t const& other) const
    {
        return url == other.url && range == other.range;
    }
};

typedef boost::optional<rtf_link_t> optional_rtf_link_t;

struct rtf_font_t
{
    std::string name;
    std::size_t charset;

    bool operator==(rtf_font_t const& other) const
    {
        return name == other.name && charset == other.charset;
    }
};

struct rtf_color_t
{
    unsigned char red;
    unsigned char green;
    unsigned char blue;

    bool operator==(rtf_color_t const& other) const
    {
        return red == other.red && green == other.green && blue == other.blue;
    }
};

struct rtf_context_t
{
    std::size_t font_size;
    std::size_t fore_color;
    std::size_t back_color;
};

struct html_tag_info_t
{
    html_tag_types tag;
    bool is_closing;
};

struct html_tag_style_t
{
    std::string tag_name;
    std::string css_data;
    std::size_t font_size;
    std::size_t fore_color;
    std::size_t back_color;
};

typedef boost::optional<html_tag_style_t> optional_html_tag_style_t;

class Html2RtfConverter
{
public:
    Html2RtfConverter();
    ~Html2RtfConverter();

    Html2RtfConverter(Html2RtfConverter const&) = delete;
    Html2RtfConverter& operator=(Html2RtfConverter const&) = delete;

    std::string convert(std::string const& html);

    std::size_t get_link_count() const;
    rtf_link_t const& get_link(std::size_t index) const;
    optional_rtf_link_t find_link(std::size_t begin, std::size_t end) const;

    rtf_color_t get_background_color() const;
    rtf_color_t get_hyperlink_color(bool hovered) const;

private:
    std::string get_html_tag(std::string const& string, std::size_t& offset);
    std::string get_plain_text(std::string const& string, std::size_t& offset, bool update_pos);

    html_tag_info_t get_tag_info(std::string const& html_tag) const;
    std::string get_tag_attr(std::string const& html_tag, std::string const& name) const;

    std::string get_tag_name(std::string const& tag, bool is_css) const;
    std::string get_style_attr(std::string const& tag, std::string const& name, bool is_css) const;

    std::string get_css_tag(std::string const& css, std::size_t& offset) const;
    optional_html_tag_style_t get_tag_style(std::string const& tag_name) const;

    std::size_t skip_whitespace(std::string const& str, std::size_t offset) const;
    std::size_t find_whitespace(std::string const& str, std::size_t offset) const;

    //void write_list_item_symbol(std::ostream& os) const;
    std::string get_list_item_symbol() const;

    std::size_t get_font_size(std::string const& value);
    std::size_t get_color_index(std::string const& value);

    std::string parse_html_data(std::string const& html);

    void parse_css_data(std::string const& css);

    void write_rtf_header(std::ostream& os) const;
    void write_font_table(std::ostream& os) const;
    void write_color_table(std::ostream& os) const;

private:
    bool is_newline_;
    std::size_t char_position_;
    std::vector<rtf_link_t> links_;
    std::vector<rtf_font_t> fonts_;
    std::vector<rtf_color_t> colors_;
    std::vector<html_tag_style_t> styles_;
};
