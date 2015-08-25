#include <ui/Window.hpp>
#include <ui/Graphics.hpp>

using namespace ui;

Graphics::Graphics()
    : m_factoriesCreated(false)
    , m_deviceCreated   (false)
    , m_pDirect2dFactory(nullptr)
    , m_pRenderTarget   (nullptr)
{
}

Graphics::~Graphics()
{
}

void Graphics::initialize()
{
    HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if((S_OK == hr) || (S_FALSE == hr)) {
        if(createFactories()) {
            if(createDevice()) {
                return;
            }
            destroyFactories();
        }
        CoUninitialize();
    }
}

void Graphics::uninitialize()
{
    destroyDevice();
    destroyFactories();
    CoUninitialize();
}

bool Graphics::createFactories()
{
    if(m_factoriesCreated)
        return true;

    bool r =
        S_OK == D2D1CreateFactory(
            D2D1_FACTORY_TYPE_MULTI_THREADED,
            &m_pDirect2dFactory
            );
    if(!r)
        return false;

    r =
        S_OK == CoCreateInstance(
                        CLSID_WICImagingFactory,
                        NULL,
                        CLSCTX_INPROC_SERVER,
                        IID_PPV_ARGS(&m_pIWICFactory)
                        );
    if(!r) {
        SAFE_RELEASE(m_pDirect2dFactory);
        return false;
    }

    m_factoriesCreated = true;
    return true;
}

bool Graphics::createDevice()
{
    if(!m_factoriesCreated)
        return false;

    if(m_deviceCreated)
        return true;

    RECT rc;

    GetClientRect(m_hwnd, &rc);

    m_width  = rc.right  - rc.left;
    m_height = rc.bottom - rc.top;

    D2D1_SIZE_U size = D2D1::SizeU(m_width, m_height);

    if(S_OK != m_pDirect2dFactory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(m_hwnd, size),
        &m_pRenderTarget
        )
      ) {
          return false;
    }
    m_deviceCreated = true;
    return true;
}

void Graphics::destroyFactories()
{
    SAFE_RELEASE(m_pDirect2dFactory);
    SAFE_RELEASE(m_pIWICFactory);
    m_factoriesCreated = false;
}

void Graphics::destroyDevice()
{
    SAFE_RELEASE(m_pRenderTarget);
    m_deviceCreated = false;
}

bool Graphics::loadBitmapsFromFile(
    wstring       fileName,
    ID2D1Bitmap** ppBitmaps, 
    unsigned int  nBitmaps,
    unsigned int& nLoadedBitmaps
    )
{
    if(!m_factoriesCreated)
        return false;

    IWICBitmapDecoder     *pDecoder   = NULL;
    IWICBitmapFrameDecode *pSource    = NULL;
    IWICFormatConverter   *pConverter = NULL;

    HRESULT hr = m_pIWICFactory->CreateDecoderFromFilename(
        fileName.c_str(),
        NULL,
        GENERIC_READ,
        WICDecodeMetadataCacheOnLoad,
        &pDecoder
        );

    if(S_OK != hr)
        return false;

    bool result = false; 
    UINT nFrames;
    pDecoder->GetFrameCount(&nFrames);

    if(S_OK == hr) {
        unsigned int nFramesUsed = (nFrames < nBitmaps)? nFrames : nBitmaps;
        unsigned int i;
        for(i = 0; i < nFramesUsed; i++) {
            hr = pDecoder->GetFrame(i, &pSource);
            if(S_OK != hr)
                break;
            hr = m_pIWICFactory->CreateFormatConverter(&pConverter);
            if(S_OK == hr) {
                hr = pConverter->Initialize(
                    pSource,
                    GUID_WICPixelFormat32bppPBGRA,
                    WICBitmapDitherTypeNone,
                    NULL,
                    0.f,
                    WICBitmapPaletteTypeMedianCut
                    );
                if(S_OK == hr) {
                    hr = m_pRenderTarget->CreateBitmapFromWicBitmap(
                        pConverter,
                        NULL,
                        ppBitmaps + i
                        );
                    result = true;
                }
                SAFE_RELEASE(pConverter);
            }
            SAFE_RELEASE(pSource);
        }
        nLoadedBitmaps = i;
    }

    SAFE_RELEASE(pDecoder);
    return result;
}