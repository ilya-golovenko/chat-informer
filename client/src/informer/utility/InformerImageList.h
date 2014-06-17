//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once

// ATL/WTL headers
#include <atlbase.h>
#include <atlapp.h>
#include <atlctrls.h>

// STL headers
#include <list>
#include <map>


template <typename Key>
class CInformerImageList
{
public:
    ~CInformerImageList()
    {
        m_images.Destroy();
    }

    void Create(int cx, int cy)
    {
        ATLASSERT(m_images.IsNull());
        ATLASSERT(cx > 0 && cy > 0);

        m_images.Create(cx, cy, ILC_COLOR32, 0, 8);
    }

    bool IsNull() const
    {
        return m_images.IsNull();
    }

    int GetImageCount() const
    {
        ATLASSERT(!m_images.IsNull());
        return m_indicesMap.size();
    }

    void Add(Key const& key, HBITMAP hBitmap)
    {
        ATLASSERT(hBitmap != NULL);
        ATLASSERT(!m_images.IsNull());

        int index = GetFreeImageIndex();

        if(index < 0)
            index = m_images.Add(hBitmap);
        else
            m_images.Replace(index, hBitmap, NULL);

        m_indicesMap.insert(std::make_pair(key, index));
    }

    bool Remove(Key const& key)
    {
        ATLASSERT(!m_images.IsNull());

        index_map::iterator it = m_indicesMap.find(key);

        if(it != m_indicesMap.end())
        {
            m_freeImages.push_back(it->second);
            m_indicesMap.erase(it);
            return true;
        }

        return false;
    }

    bool Contains(Key const& key) const
    {
        return (m_indicesMap.find(key) != m_indicesMap.end());
    }

    void Clear()
    {
        ATLASSERT(!m_images.IsNull());

        m_images.RemoveAll();
        m_freeImages.clear();
        m_indicesMap.clear();
    }

    void Draw(HDC hDC, Key const& key, POINT pt)
    {
        ATLASSERT(!m_images.IsNull());

        index_map::iterator it = m_indicesMap.find(key);

        if(it != m_indicesMap.end())
            m_images.Draw(hDC, it->second, pt, ILD_NORMAL);
    }

private:
    int GetFreeImageIndex()
    {
        if(!m_freeImages.empty())
        {
            int index = m_freeImages.front();
            m_freeImages.pop_front();
            return index;
        }

        return -1;
    }

private:
    typedef std::map<Key, int> index_map;

private:
    WTL::CImageList m_images;
    index_map m_indicesMap;
    std::list<int> m_freeImages;
};
