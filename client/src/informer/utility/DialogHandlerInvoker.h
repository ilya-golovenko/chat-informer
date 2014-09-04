//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once

// Application headers
#include <informer/managers/ManagerFactory.h>
#include <informer/managers/DialogManager.h>
#include <informer/common/Logging.h>

// BOOST headers
#include <boost/shared_ptr.hpp>

// STL headers
#include <exception>


template
<
    typename Dialog,
    typename Function
>
class DialogHandlerInvoker
{
public:
    template <typename ...Args>
    static void Invoke(Function function, Args const& ...args)
    {
        if(boost::shared_ptr<Dialog> dialog = FindDialog())
        {
            try
            {
                ((*dialog).*function)(args...);
            }
            catch(std::exception const& e)
            {
                LOG_ERROR("caught exception: ", e);
            }
        }
    }

private:
    static boost::shared_ptr<Dialog> FindDialog()
    {
        return CManagerFactory::Get<CDialogManager>().Find<Dialog>();
    }
};
