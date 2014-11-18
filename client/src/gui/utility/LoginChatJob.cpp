//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <informer/utility/LoginChatJob.h>
#include <informer/ie_utils/InternetExplorer.h>
#include <informer/ie_utils/HtmlDocument.h>
#include <informer/ie_utils/HtmlForm.h>
#include <informer/ie_utils/HtmlElement.h>
#include <informer/common/Common.h>


CLoginChatJob::Pointer CLoginChatJob::Create()
{
    return std::make_shared<CLoginChatJob>();
}

void CLoginChatJob::Run()
{
    if(m_informerManager.HasCredentials() &&
        !m_informerManager.IsBadCredentials())
    {
        if(SUCCEEDED(::CoInitialize(nullptr)))
        {
            CInternetExplorer browser;

            try
            {
                browser.Start();
                browser.Navigate(ChatLoginUri);

                browser.SetVisible(TRUE);
                browser.WaitReady(10000);

                CHtmlDocument document = browser.GetDocument();
                CString documentDomain = document.GetDomain();

                if(0 == documentDomain.CompareNoCase(ChatHostName))
                {
                    std::wstring const nickname = m_informerManager.GetNickName();
                    std::wstring const password = m_informerManager.GetPassword();

                    CHtmlForm form = document.GetForm(ChatLoginForm);

                    form.GetElement(ChatNickField).SetText(nickname.c_str());
                    form.GetElement(ChatPassField).SetText(password.c_str());

                    form.Submit();
                }
            }
            catch(...)
            {
                try
                {
                    if(!browser.IsVisible())
                        browser.Quit();
                }
                catch(...)
                {
                }

                ::CoUninitialize();

                throw;
            }
        }

        ::CoUninitialize();
    }
}
