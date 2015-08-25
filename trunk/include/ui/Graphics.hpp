#pragma once

#include <string>

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "windowscodecs.lib")

#include <common/macro.h>

using namespace std;

namespace ui
{
    class Window;

    class Graphics
    {
        friend class Window;

        HWND m_hwnd;
        bool m_factoriesCreated;
        bool m_deviceCreated;

        unsigned int m_width;
        unsigned int m_height;

    public:

        GET_ACCESSOR_CONST(HWND, hwnd);
        GET_ACCESSOR_CONST(bool, deviceCreated);

        GET_ACCESSOR_CONST(unsigned int, width);
        GET_ACCESSOR_CONST(unsigned int, height);

    private:

        ID2D1Factory*          m_pDirect2dFactory;
        IWICImagingFactory*    m_pIWICFactory;
        ID2D1HwndRenderTarget* m_pRenderTarget;

        explicit Graphics();
        ~Graphics();

        void initialize();
        void uninitialize();

        bool createFactories();
        void destroyFactories();

        bool createDevice();
        void destroyDevice();

        bool loadBitmapsFromFile(
            wstring       fileName,
            ID2D1Bitmap** ppBitmaps, 
            unsigned int  nBitmaps,
            unsigned int& nLoadedBitmaps
            );

    public:

        bool present();
    };
}