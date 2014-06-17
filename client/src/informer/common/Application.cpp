//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <informer/common/Application.h>
#include <informer/managers/ManagerFactory.h>
#include <informer/resources/resource.h>
#include <informer/common/Config.h>
#include <informer/common/Logging.h>
#include <informer/dialogs/MainDlg.h>
#include <net/util/base64.hpp>
#include <config/manager.hpp>
#include <crypto/common.hpp>
#include <core/common.hpp>
#include <core/factory.hpp>

// MISSIO headers
#include <missio/logging/dispatcher.hpp>
#include <missio/logging/file_writer.hpp>
#include <missio/utf8/convert.hpp>


WTL::CAppModule _Module;

CApplication::CApplication()
{
    logging::dispatcher::create();
}

CApplication::~CApplication()
{
    logging::dispatcher::destroy();
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
    LOG_NOTICE("starting");
    int nRet = EXIT_FAILURE;

    if(Initialize(hInstance))
    {
        LoadConfiguration();

        try
        {
            Configure();

            WTL::CMessageLoop theLoop;
            _Module.AddMessageLoop(&theLoop);

            ATL::CWindow wndMain = CreateMainDialog();
            m_singleInstance.TrackFirstInstanceRunning(wndMain);

            nRet = theLoop.Run();
            _Module.RemoveMessageLoop();
        }
        catch(std::exception const& e)
        {
            LOG_FAILURE("caught exception: ", e);
            ShowErrorMessage(IDS_APP_RUN_ERROR);
            ATLASSERT(FALSE);
        }
        catch(...)
        {
            LOG_FAILURE("caught unexpected exception");
            ShowErrorMessage(IDS_APP_RUN_ERROR);
            ATLASSERT(FALSE);
        }

        SaveConfiguration();
    }

    Finalize();

    LOG_NOTICE("stopped");
    return nRet;
}

BOOL CApplication::Initialize(HINSTANCE hInstance)
{
    LOG_TRACE_SCOPE();
    LOG_NOTICE("initializing");

    HRESULT hRes = _Module.Init(NULL, hInstance);
    if(FAILED(hRes))
    {
        LOG_FAILURE("cannot initialize application (error: ", hRes, ")");
        ShowErrorMessage(IDS_APP_INIT_ERROR);
        ATLASSERT(FALSE);
        return FALSE;
    }

    // Install unhandled exception filter
    m_exceptionFilter.Install();

    try
    {
        // Initialize global instances
        missio::factory::create_instances();

        // Create application managers
        CManagerFactory::CreateManagers();
    }
    catch(std::exception const& e)
    {
        LOG_FAILURE("cannot initialize application: ", e);
        ShowErrorMessage(IDS_APP_INIT_ERROR);
        ATLASSERT(FALSE);
        return FALSE;
    }

    return TRUE;
}

void CApplication::Configure()
{
    LOG_TRACE_SCOPE();
    LOG_NOTICE("configuring");

    SetupLogger();
    SetupInstanceID();
    SetupProxySettings();

    // Initialize application managers
    CManagerFactory::InitializeManagers();

    missio::factory::storage().set_alarms(cfg::chat::alarms);
}

void CApplication::Finalize()
{
    LOG_TRACE_SCOPE();
    LOG_NOTICE("finalizing");

    try
    {
        // Finalize application managers
        CManagerFactory::FinalizeManagers();

        // Destroy application managers
        CManagerFactory::DestroyManagers();

        // Destroy global instances
        missio::factory::destroy_instances();
    }
    catch(std::exception const& e)
    {
        LOG_FAILURE("cannot finalize application: ", e);
        ShowErrorMessage(IDS_APP_TERM_ERROR);
        ATLASSERT(FALSE);
    }

    // Remove unhandled exception filter
    m_exceptionFilter.Uninstall();

    _Module.Term();
}

void CApplication::SetupLogger()
{
    LOG_INFO("configuring logger");

    logging::dispatcher::instance().add_writer(
        logging::from_string(
            cfg::logging::severity
        ),
        logging::file_writer::create(
            cfg::logging::filename,
            cfg::logging::format,
            cfg::logging::max_size,
            cfg::logging::max_index
        )
    );

    logging::dispatcher::instance().start();
}

void CApplication::SetupInstanceID()
{
    LOG_INFO("retrieving instance ID");

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

    missio::factory::informer().set_instance_id(missio::instance_id(value));
}

void CApplication::SetupProxySettings()
{
    LOG_INFO("configuring proxy settings");

    if(cfg::connection::use_proxy)
    {
        net::http::proxy_settings proxy_settings;

        if(cfg::connection::proxy::use_system)
            proxy_settings = GetSystemProxySettings();
        else
            proxy_settings = GetUserProxySettings();

        missio::factory::informer().set_proxy_settings(proxy_settings);
        missio::factory::downloader().set_proxy_settings(proxy_settings);
    }
}

ATL::CWindow CApplication::CreateMainDialog()
{
    return CManagerFactory::Get<CDialogManager>().CreateMainDialog();
}

void CApplication::LoadConfiguration()
{
    LOG_INFO("loading configuration");

    if(boost::filesystem::exists("logging.conf"))
    {
        try
        {
            LOG_INFO("loading configuration file: logging.conf");
            config::manager::instance().load_file("logging.conf", true);
        }
        catch(std::exception const& e)
        {
            LOG_ERROR("cannot load configuration file: ", e);
            ATLASSERT(FALSE);
        }
    }

    if(boost::filesystem::exists("chat_informer.conf"))
    {
        try
        {
            LOG_INFO("loading configuration file: chat_informer.conf");
            config::manager::instance().load_file("chat_informer.conf", false);
        }
        catch(std::exception const& e)
        {
            LOG_ERROR("cannot load configuration file: ", e);
            ATLASSERT(FALSE);
        }
    }
}

void CApplication::SaveConfiguration()
{
    LOG_INFO("saving configuration");

    cfg::chat::alarms = missio::factory::storage().get_alarms();

    try
    {
        LOG_INFO("saving configuration file: chat_informer.conf");
        config::manager::instance().save_file("chat_informer.conf");
    }
    catch(std::exception const& e)
    {
        LOG_ERROR("cannot save configuration file: ", e);
        ATLASSERT(FALSE);
    }
}

void CApplication::ShowErrorMessage(UINT stringID)
{
    WTL::AtlMessageBox(NULL, stringID, IDS_APP_NAME, MB_OK | MB_ICONERROR);
}

net::http::proxy_settings CApplication::GetUserProxySettings()
{
    net::http::proxy_settings proxy_settings;

    std::string server = cfg::connection::proxy::server;
    unsigned short port = cfg::connection::proxy::port;

    proxy_settings.set_proxy_server(server, port);

    if(cfg::connection::proxy::authenticate)
    {
        std::string username = cfg::connection::proxy::username;
        std::string password = cfg::connection::proxy::password;

        password = net::util::base64::decode(password);
        password = crypto::decrypt(password, missio::crypto_key);

        proxy_settings.set_credentials(username, password);
    }

    return proxy_settings;
}

net::http::proxy_settings CApplication::GetSystemProxySettings()
{
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
                            port = boost::lexical_cast<unsigned short>(proxy_server.substr(++pos));

                        proxy_settings.set_proxy_server(utf8::convert(hostname), port);
                    }
                }
            }
        }
    }

    return proxy_settings;
}
