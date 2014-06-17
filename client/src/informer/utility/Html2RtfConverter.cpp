//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <informer/utility/Html2RtfConverter.h>


html2rtf_converter::html2rtf_converter() :
    is_newline_(false),
    char_position_(0)
{
}

html2rtf_converter::~html2rtf_converter()
{
}

std::string html2rtf_converter::convert(std::string const& html)
{
    std::ostringstream output;

    // cleanup

    is_newline_ = true;
    char_position_ = 0;

    links_.clear();
    fonts_.clear();
    colors_.clear();
    styles_.clear();

    // default fonts

    rtf_font_t times_new_roman = { "Times New Roman", 0 };
    rtf_font_t wingdings = { "Wingdings", 2 };
    rtf_font_t symbol = { "Symbol", 2 };

    fonts_.push_back(times_new_roman);
    fonts_.push_back(wingdings);
    fonts_.push_back(symbol);

    // default colors

    rtf_color_t black = { 0x00, 0x00, 0x00 };
    rtf_color_t white = { 0xFF, 0xFF, 0xFF };

    colors_.push_back(black);
    colors_.push_back(white);

    // parse html data
    std::string rtf_data = parse_html_data(html);

    write_rtf_header(output);
    write_font_table(output);
    write_color_table(output);

    output << "\\pard\\fs20\\cf0 ";
    output << rtf_data << "\\par}";

    return output.str();
}

std::size_t html2rtf_converter::get_link_count() const
{
    return links_.size();
}

rtf_link_t const& html2rtf_converter::get_link(std::size_t index) const
{
    return links_[index];
}

optional_rtf_link_t html2rtf_converter::find_link(std::size_t begin, std::size_t end) const
{
    rtf_range_t range = { begin, end };

    for(std::size_t i = 0; i < links_.size(); ++i)
    {
        if(range == links_[i].range)
            return boost::make_optional(links_[i]);
    }

    return optional_rtf_link_t();
}

rtf_color_t html2rtf_converter::get_background_color() const
{
    optional_html_tag_style_t style = get_tag_style("BODY");

    if(style && style->back_color >= 0)
        return colors_[style->back_color];

    return colors_[1];
}

rtf_color_t html2rtf_converter::get_hyperlink_color(bool hovered) const
{
    optional_html_tag_style_t style;

    if(hovered)
        style = get_tag_style("A:HOVER");

    if(!style)
        style = get_tag_style("A");

    if(style && style->fore_color >= 0)
        return colors_[style->fore_color];

    return colors_[0];
}

std::string html2rtf_converter::get_html_tag(std::string const& string, std::size_t& offset)
{
    std::size_t start = string.find('<', offset);

    if(std::string::npos != start)
    {
        offset = string.find('>', start + 1);

        if(std::string::npos != offset)
            return string.substr(start, ++offset - start);
    }

    return std::string();
}

std::string html2rtf_converter::get_plain_text(std::string const& string, std::size_t& offset, bool update_pos)
{
    std::size_t start = offset;
    offset = string.find_first_of('<', start);

    if(offset > start)
    {
        if(update_pos)
        {
            is_newline_ = false;
            char_position_ += offset - start;
        }
    }

    return string.substr(start, offset - start);
}

std::string html2rtf_converter::get_tag_name(std::string const& tag, bool is_css) const
{
    std::size_t start;

    if(!is_css)
        start = tag.find('<');
    else
        start = skip_whitespace(tag, 0);

    if(std::string::npos != start)
    {
        std::size_t end;

        if(!is_css)
        {
            ++start;

            end = find_whitespace(tag, start + 1);

            if(std::string::npos == end)
                end = tag.find('>', start + 1);
        }
        else
        {
            end = std::min(
                tag.find('{', start + 1),
                find_whitespace(tag, start + 1));
        }

        if(std::string::npos != end)
            return tag.substr(start, end - start);
    }

    return std::string();
}

static char const* tag_names[] =
{
    "b",
    "i",
    "em",
    "u",
    "strike",
    "br",
    "div",
    "sub",
    "sup",
    "p",
    "ul",
    "li",
    "body",
    "a",
    "span",
    "style"
};

enum { tag_count = sizeof(tag_names) / sizeof(tag_names[0]) };

html_tag_info_t html2rtf_converter::get_tag_info(std::string const& html_tag) const
{
    html_tag_info_t tag_info;

    tag_info.tag = tag_unknown;
    tag_info.is_closing = false;

    std::string tag_name = get_tag_name(html_tag, false);

    if(!tag_name.empty())
    {
        tag_info.is_closing = '/' == tag_name[0];

        if(tag_info.is_closing)
            tag_name = tag_name.substr(1);

        for(std::size_t i = 0; i < tag_count; ++i)
        {
            if(boost::iequals(tag_name, tag_names[i]))
                tag_info.tag = static_cast<html_tag_types>(i);
        }
    }

    return tag_info;
}

std::string html2rtf_converter::get_tag_attr(std::string const& html_tag, std::string const& attr_name) const
{
    std::size_t pos = html_tag.find(attr_name);

    if(std::string::npos != pos)
    {
        std::size_t start = html_tag.find('=', pos + 1);
        start = skip_whitespace(html_tag, start + 1);

        if(std::string::npos != start)
        {
            char c = html_tag[start];

            if('"' == c || '\'' == c)
                pos = html_tag.find(c, ++start);
            else
                pos = find_whitespace(html_tag, start + 1);

            return html_tag.substr(start, pos - start);
        }
    }

    return std::string();
}

std::string html2rtf_converter::get_style_attr(std::string const& tag, std::string const& name, bool is_css) const
{
	std::string value = tag;

    if(!is_css)
        value = get_tag_attr(tag, "style");

    if(!value.empty())
    {
        std::size_t pos = value.find(name);

        if(std::string::npos != pos)
        {
            std::size_t start = value.find(':', pos + 1);
            start = skip_whitespace(value, start + 1);

            if(std::string::npos != start)
            {
                pos = std::min(
                    value.find(',', start + 1),
                    value.find(';', start + 1));

                if(std::string::npos == pos)
                {
                    if(is_css)
                        pos = value.find('}', start + 1);
                    else
                        pos = value.find('>', start + 1);
                }

                return value.substr(start, pos - start);
            }
        }
    }

    return std::string();
}

std::string html2rtf_converter::get_list_item_symbol() const
{
    optional_html_tag_style_t style = get_tag_style("LI");

    if(style)
    {
        std::string symbol = get_style_attr(
            style->css_data, "list-style-type", true);

        if(boost::algorithm::iequals(symbol, "square"))
            return std::string("\\f1\\'A7");

        if(boost::algorithm::iequals(symbol, "disc"))
            return std::string("\\f2\\'B7");
    }

    return std::string("\\f2\\'B7");
}

std::size_t html2rtf_converter::get_font_size(std::string const& value)
{
    long size = std::strtol(value.c_str(), 0, 10);
    return static_cast<std::size_t>(size * 2);
}

struct color_entry
{
    std::string name;
    rtf_color_t color;
};

static boost::array<color_entry, 16> const colors =
{{
    { "aqua",    { 0x00, 0xFF, 0xFF } },
    { "black",   { 0x00, 0x00, 0x00 } },
    { "blue",    { 0x00, 0x00, 0xFF } },
    { "fuchsia", { 0xFF, 0x00, 0xFF } },
    { "gray",    { 0x80, 0x80, 0x80 } },
    { "green",   { 0x00, 0x80, 0x00 } },
    { "lime",    { 0x00, 0xFF, 0x00 } },
    { "maroon",  { 0x80, 0x00, 0x00 } },
    { "navy",    { 0x00, 0x00, 0x80 } },
    { "olive",   { 0x80, 0x80, 0x00 } },
    { "purple",  { 0x80, 0x00, 0x80 } },
    { "red",     { 0xFF, 0x00, 0x00 } },
    { "silver",  { 0xC0, 0xC0, 0xC0 } },
    { "teal",    { 0x00, 0x80, 0x80 } },
    { "white",   { 0xFF, 0xFF, 0xFF } },
    { "yellow",  { 0xFF, 0xFF, 0x00 } }
}};

std::size_t html2rtf_converter::get_color_index(std::string const& value)
{
    rtf_color_t color = { 0x00, 0x00, 0x00 };

    if('#' == value[0])
    {
        char const* pvalue = value.c_str() + 1;
        long bgr = std::strtol(pvalue, 0, 16);

        color.red = (bgr >> 16) & 0xFF;
        color.green = (bgr >> 8) & 0xFF;
        color.blue = bgr & 0xFF;
    }
    else
    {
        for(std::size_t i = 0; i < colors.size(); ++i)
        {
            if(boost::algorithm::iequals(value, colors[i].name))
            {
                color = colors[i].color;
                break;
            }
        }
    }

    for(std::size_t i = 0; i < colors_.size(); ++i)
    {
        if(color == colors_[i])
            return i;
    }

    colors_.push_back(color);
    return (colors_.size() - 1);
}

std::string html2rtf_converter::parse_html_data(std::string const& html)
{
    rtf_link_t rtf_link;
    rtf_context_t context;
    std::ostringstream output;
    std::stack<rtf_context_t> contexts;

    context.font_size = 20;
    context.fore_color = 0;
    context.back_color = 1;

    std::string input = html;

    boost::erase_all(input, "\r");
    boost::erase_all(input, "\n");

    input = util::unescape_html(input);

    for(std::size_t offset = 0; offset < input.size(); )
    {
        std::ostringstream buffer;

        bool new_context = false;
        bool restore_context = false;
        rtf_context_t old_context = context;

        output << get_plain_text(input, offset, true);

        std::string html_tag = get_html_tag(input, offset);

        if(html_tag.empty())
            break;

        html_tag_info_t tag_info = get_tag_info(html_tag);

        if(!tag_info.is_closing)
        {
            switch(tag_info.tag)
            {
                case tag_b:
                    buffer << "\\b ";
                    break;

                case tag_i:
                    buffer << "\\i ";
                    break;

                case tag_em:
                    buffer << "\\i ";
                    break;

                case tag_u:
                    buffer << "\\ul ";
                    break;

                case tag_strike:
                    buffer << "\\strike ";
                    break;

                case tag_br:
                    buffer << "\\line ";
                    is_newline_ = true;
                    ++char_position_;
                    break;

                case tag_div:
                    if(!is_newline_)
                    {
                        buffer << "\\par ";
                        is_newline_ = true;
                        ++char_position_;
                    }
                    new_context = true;
                    break;

                case tag_sub:
                    buffer << "{\\sub ";
                    break;

                case tag_sup:
                    buffer << "{\\sup ";
                    break;

                case tag_p:
                    buffer << "\\par ";
                    is_newline_ = true;
                    ++char_position_;
                    break;

                case tag_ul:
                    buffer << "\\par\\li360 ";
                    is_newline_ = true;
                    new_context = true;
                    ++char_position_;
                    break;

                case tag_li:
                    if(!is_newline_)
                    {
                        buffer << "\\line ";
                        ++char_position_;
                    }
                    // write_list_item_symbol(buffer);
                    //buffer << get_list_item_symbol();
                    buffer << "\\bullet";
                    buffer << "\\li720\\fi-360 ";
                    ++char_position_;
                    break;

                case tag_body:
                    new_context = true;
                    break;

                case tag_a:
                    new_context = true;
                    rtf_link.range.begin = char_position_;
                    rtf_link.url = get_tag_attr(html_tag, "href");
                    break;

                case tag_span:
                    new_context = true;
                    break;

                case tag_style:
                    parse_css_data(get_plain_text(input, offset, false));
                    break;
            }
        }
        else
        {
            switch(tag_info.tag)
            {
                case tag_b:
                    buffer << "\\b0 ";
                    break;

                case tag_i:
                    buffer << "\\i0 ";
                    break;

                case tag_em:
                    buffer << "\\i0 ";
                    break;

                case tag_u:
                    buffer << "\\ul0 ";
                    break;

                case tag_strike:
                    buffer << "\\strike0 ";
                    break;

                case tag_div:
                    restore_context = true;
                    break;

                case tag_sub:
                    buffer << "}";
                    break;

                case tag_sup:
                    buffer << "}";
                    break;

                case tag_ul:
                    if(!is_newline_)
                    {
                        buffer << "\\par ";
                        is_newline_ = true;
                        ++char_position_;
                    }
                    restore_context = true;
                    break;

                case tag_li:
                    buffer << "\\par ";
                    is_newline_ = true;
                    ++char_position_;
                    break;

                case tag_body:
                    restore_context = true;
                    break;

                case tag_a:
                    rtf_link.range.end = char_position_;
                    links_.push_back(rtf_link);
                    restore_context = true;
                    break;

                case tag_span:
                    restore_context = true;
                    break;
            }
        }

        std::string tag_name = get_tag_name(html_tag, false);
        optional_html_tag_style_t style = get_tag_style(tag_name);

        if(style)
        {
            context.font_size = style->font_size;
            context.fore_color = style->fore_color;
            context.back_color = style->back_color;
        }

        if(new_context)
        {
            contexts.push(old_context);

            std::string value = get_style_attr(html_tag, "color", false);

            if(!value.empty())
                context.fore_color = get_color_index(value);

            value = get_style_attr(html_tag, "background-color", false);

            if(value.empty())
                value = get_style_attr(html_tag, "background", false);

            if(!value.empty())
                context.back_color = get_color_index(value);

            value = get_style_attr(html_tag, "font-size", false);

            if(!value.empty())
                context.font_size = get_font_size(value);
        }
        else if(restore_context)
        {
            old_context = context;

            if(!contexts.empty())
            {
                context = contexts.top();
                contexts.pop();
            }
        }

        if(new_context)
        {
            output << "{";

            if(old_context.fore_color != context.fore_color)
                output << "\\cf" << context.fore_color << " ";

            if(old_context.back_color != context.back_color)
                output << "\\highlight" << context.back_color << " ";

            if(old_context.font_size != context.font_size)
                output << "\\fs" << context.font_size << " ";

            output << buffer.str();
        }
        else if(restore_context)
        {
            output << buffer.str();
            output << "}";
        }
        else
        {
            output << buffer.str();
        }
    }

    return output.str();
}

std::string html2rtf_converter::get_css_tag(std::string const& css, std::size_t& offset) const
{
    std::size_t start = skip_whitespace(css, offset);

    if(std::string::npos != start)
    {
        offset = css.find('}', start + 1);

        if(std::string::npos != offset)
            return css.substr(start, ++offset - start);
    }

    return std::string();
}

optional_html_tag_style_t html2rtf_converter::get_tag_style(std::string const& tag_name) const
{
    for(std::size_t i = 0; i < styles_.size(); ++i)
    {
        if(boost::algorithm::iequals(tag_name, styles_[i].tag_name))
            return optional_html_tag_style_t(styles_[i]);
    }

    return optional_html_tag_style_t();
}

std::size_t html2rtf_converter::skip_whitespace(std::string const& str, std::size_t offset) const
{
    for(; offset < str.size(); ++offset)
    {
        if(!isspace(str[offset]))
            return offset;
    }

    return std::string::npos;
}

std::size_t html2rtf_converter::find_whitespace(std::string const& str, std::size_t offset) const
{
    for(; offset < str.size(); ++offset)
    {
        if(isspace(str[offset]))
            return offset;
    }

    return std::string::npos;
}

void html2rtf_converter::parse_css_data(std::string const& css)
{
    for(std::size_t offset = 0; offset < css.size(); )
    {
        std::string css_tag = get_css_tag(css, offset);

        if(css_tag.empty())
            return;

        std::string tag_name = get_tag_name(css_tag, true);

        if(tag_name.empty())
            return;

        html_tag_style_t tag_style;

        tag_style.back_color = 0;
        tag_style.fore_color = 1;
        tag_style.font_size = 20;

        tag_style.tag_name = tag_name;
        tag_style.css_data = css_tag;

        std::string value = get_style_attr(css_tag, "color", true);

        if(!value.empty())
            tag_style.fore_color = get_color_index(value);

        value = get_style_attr(css_tag, "background", true);

        if(!value.empty())
            tag_style.back_color = get_color_index(value);

        value = get_style_attr(css_tag, "font-size", true);

        if(!value.empty())
            tag_style.font_size = get_font_size(value);

        styles_.push_back(tag_style);
    }
}

void html2rtf_converter::write_rtf_header(std::ostream& os) const
{
    os << "{\\rtf1\\ansi\\uc0\\deff0\\deftab360 ";
}

void html2rtf_converter::write_font_table(std::ostream& os) const
{
    os << "{\\fonttbl";
/*
    for(std::size_t i = 0; i < fonts_.size(); ++i)
    {
        os << "{"
        os << "\\f" << i;
        os << "\\fcharset" << fonts_[i].charset;
        os << " " << fonts_[i].name; << ";";
        os << "}";
    }
*/
    os << "{\\f0\\fswiss\\fcharset204 Arial CYR;}";
    os << "{\\f1\\ftech\\fcharset2 Wingdings;}";
    os << "{\\f2\\ftech\\fcharset2 Symbol;}";
    os << "}";
}

void html2rtf_converter::write_color_table(std::ostream& os) const
{
    os << "{\\colortbl";

    for(std::size_t i = 0; i < colors_.size(); ++i)
    {
        os << "\\red" << colors_[i].red;
        os << "\\green" << colors_[i].green;
        os << "\\blue" << colors_[i].blue;
        os << ";";
    }

    os << "}";
}
