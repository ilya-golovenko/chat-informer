//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <informer/common/Application.h>
#include <informer/managers/ManagerFactory.h>
#include <informer/resources/resource.h>
#include <informer/common/Config.h>
#include <informer/common/Logging.h>
#include <informer/dialogs/MainDlg.h>
#include <network/util/base64.hpp>
#include <crypto/common.hpp>
#include <core/common.hpp>
#include <core/factory.hpp>

// MISSIO headers
#include <missio/logging/common.hpp>
#include <missio/logging/file_writer.hpp>
#include <missio/unicode/convert.hpp>
#include <missio/config/common.hpp>

// BOOST headers
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/uuid/uuid.hpp>


WTL::CAppModule _Module;

CApplication::CApplication()
{
    missio::logging::initialize();
}

CApplication::~CApplication()
{
    missio::logging::shutdown();
}

BOOL CApplication::IsPreviousInstanceRunning()
{
    return m_singleInstance.IsPreviousInstanceRunning();
}

int CApplication::ActivatePreviousInstance()
{
    if(!m_singleInstance.ActivatePreviousInstance())
    {
        ShowErrorMessage(IDS_APP_PREV_ERROR);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int CApplication::Run(HINSTANCE hInstance, LPWSTR /*lpszCmdLine*/, int /*nCmdShow*/)
{
    LOG_COMP_TRACE_FUNCTION(CApplication);

    LOG_COMP_NOTICE(CApplication, "starting");

    int nRet = EXIT_FAILURE;

    if(Initialize(hInstance))
    {
        LoadConfiguration();

        try
        {
            Configure();

            WTL::CMessageLoop messageLoop;
            _Module.AddMessageLoop(&messageLoop);

            ATL::CWindow wndMain = CreateMainDialog();
            m_singleInstance.TrackFirstInstanceRunning(wndMain);

            nRet = messageLoop.Run();
            _Module.RemoveMessageLoop();
        }
        catch(std::exception const& e)
        {
            LOG_COMP_FAILURE(CApplication, "caught exception: ", e);
            ShowErrorMessage(IDS_APP_RUN_ERROR);
        }
        catch(...)
        {
            LOG_COMP_FAILURE(CApplication, "caught unexpected exception");
            ShowErrorMessage(IDS_APP_RUN_ERROR);
        }

        SaveConfiguration();
    }

    Finalize();

    LOG_COMP_NOTICE(CApplication, "stopped");

    return nRet;
}

BOOL CApplication::Initialize(HINSTANCE hInstance)
{
    LOG_COMP_TRACE_FUNCTION(CApplication);

    LOG_COMP_NOTICE(CApplication, "initializing");

    HRESULT hRes = _Module.Init(NULL, hInstance);
    if(FAILED(hRes))
    {
        LOG_COMP_FAILURE(CApplication, "failed to initialize application, error code: ", LOG_HEX(hRes));
        ShowErrorMessage(IDS_APP_INIT_ERROR);
        return FALSE;
    }

    // Install unhandled exception filter
    m_exceptionFilter.Install();

    try
    {
        // Initialize global instances
        chat::factory::create_instances();

        // Create application managers
        CManagerFactory::CreateManagers();
    }
    catch(std::exception const& e)
    {
        LOG_COMP_FAILURE(CApplication, "caught exception: ", e);
        ShowErrorMessage(IDS_APP_INIT_ERROR);
        return FALSE;
    }

    return TRUE;
}

void CApplication::Configure()
{
    LOG_COMP_TRACE_FUNCTION(CApplication);

    LOG_COMP_NOTICE(CApplication, "configuring");

    SetupLogger();
    SetupInstanceID();
    SetupProxySettings();

    // Initialize application managers
    CManagerFactory::InitializeManagers();

    chat::factory::storage().set_alarms(cfg::chat::alarms);
}

void CApplication::Finalize()
{
    LOG_COMP_TRACE_FUNCTION(CApplication);

    LOG_COMP_NOTICE(CApplication, "finalizing");

    try
    {
        // Finalize application managers
        CManagerFactory::FinalizeManagers();

        // Destroy application managers
        CManagerFactory::DestroyManagers();

        // Destroy global instances
        chat::factory::destroy_instances();
    }
    catch(std::exception const& e)
    {
        LOG_COMP_FAILURE(CApplication, "caught exception: ", e);
        ShowErrorMessage(IDS_APP_TERM_ERROR);
    }

    // Remove unhandled exception filter
    m_exceptionFilter.Uninstall();

    _Module.Term();
}

void CApplication::SetupLogger()
{
    LOG_COMP_TRACE_FUNCTION(CApplication);

    LOG_COMP_INFO(CApplication, "configuring logger");

    //TODO: fix me!
    /*
    missio::logging::file_options(missio::logging::from_string(cfg::logging::severity)).
        missio::logging::file_writer::create(
            cfg::logging::filename,
            cfg::logging::format,
            cfg::logging::max_size,
            cfg::logging::max_index
        )
    );
    */

    missio::logging::start();
}

void CApplication::SetupInstanceID()
{
    LOG_COMP_TRACE_FUNCTION(CApplication);

    LOG_COMP_INFO(CApplication, "retrieving instance ID");

    ATL::CRegKey reg_key;
    boost::uuids::uuid value;

    CString strPath(reinterpret_cast<LPCWSTR>(IDS_REG_PATH));

    if(ERROR_SUCCESS == reg_key.Open(HKEY_CURRENT_USER, strPath))
    {
        DWORD size = value.size();

        if(ERROR_SUCCESS != reg_key.QueryBinaryValue(L"ID", &value, &size))
            throw std::runtime_error("cannot load instance ID");
    }
    else
    {
        value = boost::uuids::random_generator()();

        if(ERROR_SUCCESS != reg_key.Create(HKEY_CURRENT_USER, strPath))
            throw std::runtime_error("cannot create instance ID");

        if(ERROR_SUCCESS != reg_key.SetBinaryValue(L"ID", &value, value.size()))
            throw std::runtime_error("cannot save instance ID");
    }

    chat::factory::informer().set_instance_id(chat::instance_id(value));
}

void CApplication::SetupProxySettings()
{
    LOG_COMP_TRACE_FUNCTION(CApplication);

    LOG_COMP_INFO(CApplication, "configuring proxy settings");

    if(cfg::connection::use_proxy)
    {
        net::http::proxy_settings proxy_settings;

        if(cfg::connection::proxy::use_system)
            proxy_settings = GetSystemProxySettings();
        else
            proxy_settings = GetUserProxySettings();

        chat::factory::informer().set_proxy_settings(proxy_settings);
        chat::factory::downloader().set_proxy_settings(proxy_settings);
    }
}

ATL::CWindow CApplication::CreateMainDialog()
{
    LOG_COMP_TRACE_FUNCTION(CApplication);

    LOG_COMP_INFO(CApplication, "creating main dialog");

    return CManagerFactory::Get<CDialogManager>().CreateMainDialog();
}

void CApplication::LoadConfiguration()
{
    LOG_COMP_TRACE_FUNCTION(CApplication);

    LOG_COMP_INFO(CApplication, "loading configuration");

    if(boost::filesystem::exists("logging.conf"))
    {
        try
        {
            LOG_COMP_INFO(CApplication, "loading configuration file: logging.conf");
            chat::config::load_from_file("logging.conf", true);
        }
        catch(std::exception const& e)
        {
            LOG_COMP_ERROR(CApplication, "caught exception: ", e);
        }
    }

    if(boost::filesystem::exists("chat_informer.conf"))
    {
        try
        {
            LOG_COMP_INFO(CApplication, "loading configuration file: chat_informer.conf");
            missio::config::load_from_file("chat_informer.conf", false);
        }
        catch(std::exception const& e)
        {
            LOG_COMP_ERROR(CApplication, "caught exception: ", e);
        }
    }
}

void CApplication::SaveConfiguration()
{
    LOG_COMP_TRACE_FUNCTION(CApplication);

    LOG_COMP_INFO(CApplication, "saving configuration");

    cfg::chat::alarms = chat::factory::storage().get_alarms();

    try
    {
        LOG_COMP_INFO(CApplication, "saving configuration file: chat_informer.conf");
        missio::config::save_to_file("chat_informer.conf");
    }
    catch(std::exception const& e)
    {
        LOG_COMP_ERROR(CApplication, "caught exception: ", e);
    }
}

void CApplication::ShowErrorMessage(UINT stringID)
{
    LOG_COMP_TRACE_FUNCTION(CApplication);

    WTL::AtlMessageBox(NULL, stringID, IDS_APP_NAME, MB_OK | MB_ICONERROR);
}

net::http::proxy_settings CApplication::GetUserProxySettings()
{
    LOG_COMP_TRACE_FUNCTION(CApplication);

    net::http::proxy_settings proxy_settings;

    std::string server = cfg::connection::proxy::server;
    unsigned short port = cfg::connection::proxy::port;

    proxy_settings.set_proxy_server(server, port);

    if(cfg::connection::proxy::authenticate)
    {
        std::string username = cfg::connection::proxy::username;
        std::string password = cfg::connection::proxy::password;

        password = net::util::base64::decode(password);
        password = crypto::decrypt(password, chat::crypto_key);

        proxy_settings.set_credentials(username, password);
    }

    return proxy_settings;
}

net::http::proxy_settings CApplication::GetSystemProxySettings()
{
    LOG_COMP_TRACE_FUNCTION(CApplication);

    ATL::CRegKey settings_reg_key;
    net::http::proxy_settings proxy_settings;

    if(ERROR_SUCCESS == settings_reg_key.Open(HKEY_CURRENT_USER,
        L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Internet Settings"))
    {
        DWORD proxy_enable = 0L;

        if(ERROR_SUCCESS == settings_reg_key.QueryDWORDValue(L"ProxyEnable", proxy_enable))
        {
            if(0L < proxy_enable)
            {
                ULONG size = MAX_PATH;
                WCHAR value[MAX_PATH];

                // For some strange reason sometimes this value is not null-terminated
                LONG result = settings_reg_key.QueryStringValue(L"ProxyServer", value, &size);

                if(ERROR_SUCCESS == result || ERROR_INVALID_DATA == result)
                {
                    std::vector<std::wstring> proxy_list;
                    std::wstring proxy_server(value, size);

                    boost::algorithm::split(
                        proxy_list, proxy_server,
                        boost::algorithm::is_any_of(";"));

                    if(proxy_list.size() > 1)
                    {
                        for(std::size_t i = 0; i < proxy_list.size(); ++i)
                        {
                            if(boost::algorithm::istarts_with(proxy_list[i], L"http="))
                            {
                                proxy_server = boost::algorithm::ierase_first_copy(proxy_list[i], L"http=");
                                break;
                            }
                        }
                    }
                    else if(!proxy_list.empty())
                    {
                        proxy_server = proxy_list.front();
                    }
                    else
                    {
                        proxy_server.clear();
                    }

                    if(!proxy_server.empty())
                    {
                        std::size_t pos = proxy_server.find_first_of(L':');

                        std::wstring hostname = proxy_server.substr(0, pos);
                        unsigned short port = net::http::http_port_number;

                        if(std::string::npos != pos)
                            port = std::stoul(proxy_server.substr(++pos));

                        proxy_settings.set_proxy_server(missio::unicode::to_utf8_string(hostname), port);
                    }
                }
            }
        }
    }

    return proxy_settings;
}
