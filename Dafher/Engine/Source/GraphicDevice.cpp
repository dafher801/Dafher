#include "GraphicDevice.h"

bool GraphicDevice::Init(HWND hWnd) noexcept
{
    RECT rect;
    GetClientRect(hWnd, &rect);
    UINT width = rect.right - rect.left;
    UINT height = rect.bottom - rect.top;

    _aspectRatio = static_cast<float>(width) / static_cast<float>(height);

    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Width = width;
    swapChainDesc.BufferDesc.Height = height;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = hWnd;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.Windowed = TRUE;

    D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };
    D3D_FEATURE_LEVEL featureLevel;

    ASSERT_HR(D3D11CreateDeviceAndSwapChain(nullptr, 
        D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, featureLevels, 1, 
        D3D11_SDK_VERSION, &swapChainDesc, &_swapChain, &_device, &featureLevel, &_context));

    ComPtr<ID3D11Texture2D> backBuffer = nullptr;
    ASSERT_HR(_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
        reinterpret_cast<void**>(backBuffer.GetAddressOf())));
    ASSERT_HR(_device->CreateRenderTargetView(backBuffer.Get(), 
        nullptr, _renderTargetView.GetAddressOf()));
    _context->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), nullptr);

    D3D11_VIEWPORT viewport;
    viewport.Width = static_cast<FLOAT>(width);
    viewport.Height = static_cast<FLOAT>(height);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    _context->RSSetViewports(1, &viewport);

    return true;
}

void GraphicDevice::BeginFrame() noexcept
{
	const float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

    _context->ClearRenderTargetView(_renderTargetView.Get(), clearColor);
}

void GraphicDevice::EndFrame() noexcept
{
    _swapChain->Present(0, 0);
}

void GraphicDevice::Clear() noexcept
{
    _context->ClearState();
}