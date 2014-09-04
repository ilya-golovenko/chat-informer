//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <informer/utility/ExceptionFilter.h>
#include <informer/common/Logging.h>

// MISSIO headers
#include <missio/logging/factory.hpp>


CExceptionFilter::CExceptionFilter() :
    m_oldExceptionFilter(NULL)
{
}

CExceptionFilter::~CExceptionFilter()
{
    Uninstall();
}

void CExceptionFilter::Install()
{
    if(NULL == m_oldExceptionFilter)
        m_oldExceptionFilter = ::SetUnhandledExceptionFilter(&UnhandledExceptionFilter);
}

void CExceptionFilter::Uninstall()
{
    if(NULL != m_oldExceptionFilter)
    {
        ::SetUnhandledExceptionFilter(m_oldExceptionFilter);
        m_oldExceptionFilter = NULL;
    }
}

LONG WINAPI CExceptionFilter::UnhandledExceptionFilter(EXCEPTION_POINTERS* info)
{
    PEXCEPTION_RECORD exceptionRecord = info->ExceptionRecord;

    DWORD exceptionCode = exceptionRecord->ExceptionCode;
    PVOID exceptionAddress = exceptionRecord->ExceptionAddress;

    std::wstring const module_name = GetModuleName(exceptionAddress);
    wchar_t const* exception_string = GetExceptionString(exceptionCode);

    LOG_FAILURE("unhandled exception ", missio::format::hex(exception_code, 8, true), " (",
        exception_string, ") at location ", exception_address, " in module ", module_name); 

    if(exceptionCode == EXCEPTION_ACCESS_VIOLATION)
    {
        if(exceptionRecord->NumberParameters >= 2)
        {
            PVOID violationAddress = reinterpret_cast<PVOID>(exceptionRecord->ExceptionInformation[1]);

            if(exceptionRecord->ExceptionInformation[0])
                LOG_FAILURE("access violation error while writing to location ", violationAddress);
            else
                LOG_FAILURE("access violation error while reading from location ", violationAddress);
        }
    }

    // wait for dispatcher to finish
    missio::logging::stop();

    // destroy dispatcher
    missio::logging::shutdown();

    //TODO: show error message
    //::MessageBox(NULL, L"", L"", MB_OK | MB_ICONERROR);

    return EXCEPTION_EXECUTE_HANDLER;
}

std::wstring CExceptionFilter::GetModuleName(LPCVOID address)
{
    HMODULE hModule = NULL;

    MEMORY_BASIC_INFORMATION memoryInfo;
    HANDLE hProcess = ::GetCurrentProcess();

    if(::VirtualQueryEx(hProcess, address, &memoryInfo, sizeof(memoryInfo)))
        hModule = reinterpret_cast<HMODULE>(memoryInfo.AllocationBase);

    if(NULL != hModule)
    {
        wchar_t moduleName[MAX_PATH];

        if(::GetModuleFileName(hModule, moduleName, sizeof(moduleName)))
        {
            ::PathStripPath(moduleName);
            return std::wstring(moduleName);
        }
    }

    return std::wstring(L"<unknown>");
}

wchar_t const* CExceptionFilter::GetExceptionString(DWORD exception)
{
    switch(exception)
    {
        case EXCEPTION_ACCESS_VIOLATION:
            return L"Access Violation";

        case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
            return L"Array Bound Exceeded";

        case EXCEPTION_BREAKPOINT:
            return L"Breakpoint";

        case EXCEPTION_DATATYPE_MISALIGNMENT:
            return L"Datatype Misalignment";

        case EXCEPTION_FLT_DENORMAL_OPERAND:
            return L"Float Denormal Operand";

        case EXCEPTION_FLT_DIVIDE_BY_ZERO:
            return L"Float Divide By Zero";

        case EXCEPTION_FLT_INEXACT_RESULT:
            return L"Float Inexact Result";

        case EXCEPTION_FLT_INVALID_OPERATION:
            return L"Float Invalid Operation";

        case EXCEPTION_FLT_OVERFLOW:
            return L"Float Overflow";

        case EXCEPTION_FLT_STACK_CHECK:
            return L"Float Stack Check";

        case EXCEPTION_FLT_UNDERFLOW:
            return L"Float Underflow";

        case EXCEPTION_GUARD_PAGE:
            return L"Guard Page";

        case EXCEPTION_ILLEGAL_INSTRUCTION:
            return L"Illegal Instruction";

        case EXCEPTION_IN_PAGE_ERROR:
            return L"In Page Error";

        case EXCEPTION_INT_DIVIDE_BY_ZERO:
            return L"Integer Divide By Zero";

        case EXCEPTION_INT_OVERFLOW:
            return L"Integer Overflow";

        case EXCEPTION_INVALID_DISPOSITION:
            return L"Invalid Disposition";

        case EXCEPTION_INVALID_HANDLE:
            return L"Invalid Handle";

        case EXCEPTION_NONCONTINUABLE_EXCEPTION:
            return L"Noncontinuable Exception";

        case EXCEPTION_PRIV_INSTRUCTION:
            return L"Privileged Instruction";

        case EXCEPTION_SINGLE_STEP:
            return L"Single Step";

        case EXCEPTION_STACK_OVERFLOW:
            return L"Stack Overflow";
    }

    return L"<unknown>";
}
