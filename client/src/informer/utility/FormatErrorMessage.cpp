//---------------------------------------------------------------------------////    This file is part of Chat Informer project//    Copyright (C) 2011, 2013 Ilya Golovenko////---------------------------------------------------------------------------// Application headers#include <informer/utility/FormatErrorMessage.h>#include <informer/utility/AutoLocalFree.h>std::wstring FormatErrorMessage(DWORD errorCode){    LPVOID messageBuffer = 0;    DWORD nRet = ::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,        0, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPWSTR>(&messageBuffer), 0, NULL);    CAutoLocalFree autoLocalFree(messageBuffer);    if(0 == nRet)        return std::wstring(L"Unknown error");    std::wstring errorMessage(static_cast<LPWSTR>(messageBuffer));    boost::algorithm::trim(errorMessage, boost::is_any_of(L"\r\n."));    return errorMessage;}