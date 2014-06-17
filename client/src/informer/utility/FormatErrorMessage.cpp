//---------------------------------------------------------------------------////    This file is part of Chat Informer project//    Copyright (C) 2011, 2013 Ilya Golovenko////---------------------------------------------------------------------------// Application headers#include <informer/utility/FormatErrorMessage.h>//TODO: use %0 escape sequence to get rid of the terminating '\n'.//TODO: see how _com_error class formats string from HRESULT argument//TODO: reuse WTL::CString::FormatMessage here, return CString instead of std::wstringstd::wstring FormatErrorMessage(HRESULT errorCode){    return FormatErrorMessage(static_cast<DWORD>(HRESULT_CODE(errorCode)));}std::wstring FormatErrorMessage(DWORD errorCode){    LPVOID messageBuffer = 0;    DWORD nRet = ::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,        0, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPWSTR>(&messageBuffer), 0, 0);    if(!nRet || NULL == messageBuffer)        return std::wstring(L"Unknown error");    std::wstring errorMessage(static_cast<LPWSTR>(messageBuffer));    boost::algorithm::trim_right_if(errorMessage, boost::is_any_of(L"\r\n."));    ::LocalFree(messageBuffer);    return errorMessage;}