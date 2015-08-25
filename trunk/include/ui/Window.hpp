#pragma once

#include <sdkddkver.h>
#include <windows.h>

#include <common/macro.h>
#include <ui/Idler.hpp>
#include <ui/Graphics.hpp>

namespace ui
{
    class Window
    {
        static Window*        m_pSingleInstance;
        static unsigned int   m_nReferences;
        static const wchar_t* m_pClassName;

        Window();
        ~Window();

    public:

        static Window* instance();

        void free();
        void doLoop(Idler& idler);
        void breakLoop();

    private:

        HINSTANCE m_hinst;
        HWND      m_hwnd;
        Graphics  m_graphics; 
    public:

        GET_ACCESSOR_CONST(HINSTANCE, hinst);
        GET_ACCESSOR_CONST(HWND,      hwnd );
        GET_ACCESSOR(Graphics&, graphics);
    };
}