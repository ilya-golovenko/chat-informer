//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once

// Windows headers
#include <windows.h>

// STL headers
#include <string>


std::wstring FormatErrorMessage(HRESULT errorCode);
std::wstring FormatErrorMessage(DWORD errorCode = ::GetLastError());
