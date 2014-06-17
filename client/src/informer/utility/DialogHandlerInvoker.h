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
    static void Invoke(Function function)
    {
        if(boost::shared_ptr<Dialog> dialog = FindDialog())
        {
            try
            {
                ((*dialog).*function)();
            }
            catch(std::exception const& e)
            {
                LOG_ERROR("caught exception: {0}", e.what());
            }
        }
    }

    template <typename Arg0>
    static void Invoke(Function function, Arg0 const& arg0)
    {
        if(boost::shared_ptr<Dialog> dialog = FindDialog())
        {
            try
            {
                ((*dialog).*function)(arg0);
            }
            catch(std::exception const& e)
            {
                LOG_ERROR("caught exception: {0}", e.what());
            }
        }
    }

    template <typename Arg0, typename Arg1>
    static void Invoke(Function function, Arg0 const& arg0, Arg1 const& arg1)
    {
        if(boost::shared_ptr<Dialog> dialog = FindDialog())
        {
            try
            {
                ((*dialog).*function)(arg0, arg1);
            }
            catch(std::exception const& e)
            {
                LOG_ERROR("caught exception: {0}", e.what());
            }
        }
    }

private:
    static boost::shared_ptr<Dialog> FindDialog()
    {
        return CManagerFactory::Get<CDialogManager>().Find<Dialog>();
    }
};
