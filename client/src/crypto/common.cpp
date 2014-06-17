//---------------------------------------------------------------------------
//
//    This file is part of missio project
//    Copyright (C) 2011 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <crypto/common.hpp>
#include <crypto/manager.hpp>
#include <crypto/md5/computer.hpp>


namespace crypto
{

std::string encrypt(std::string const& data, std::string const& pass)
{
    crypto::manager crypto_manager(pass);
    return encrypt(data, crypto_manager);
}

std::string decrypt(std::string const& data, std::string const& pass)
{
    crypto::manager crypto_manager(pass);
    return decrypt(data, crypto_manager);
}

std::string encrypt(std::string const& data, boost::uuids::uuid const& key)
{
    crypto::manager crypto_manager(key);
    return encrypt(data, crypto_manager);
}

std::string decrypt(std::string const& data, boost::uuids::uuid const& key)
{
    crypto::manager crypto_manager(key);
    return decrypt(data, crypto_manager);
}

std::string encrypt(std::string const& data, crypto::manager& crypto_manager)
{
    crypto::manager::buffer_type input;
    crypto::manager::buffer_type output;

    input.assign(data.data(), data.size());
    crypto_manager.encrypt(input, output);

    return std::string(output.begin(), output.end());
}

std::string decrypt(std::string const& data, crypto::manager& crypto_manager)
{
    crypto::manager::buffer_type input;
    crypto::manager::buffer_type output;

    input.assign(data.data(), data.size());
    crypto_manager.decrypt(input, output);

    return std::string(output.begin(), output.end());
}

std::string calculate_md5(std::string const& data)
{
    md5::computer md5_computer(data.data(), data.size());
    return md5_computer.get_digest().to_string();
}

}   // namespace crypto
