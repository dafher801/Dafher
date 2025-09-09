#include "GraphicDevice.h"
#include "Engine.h"
#include "Window.h"

bool GraphicDevice::Init() noexcept
{
	HWND hWnd = Engine::GetInstance().GetWindow()->GetHwnd();
    RECT rc;
    GetClientRect(hWnd, &rc);
    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferCount = 1;
    sd.BufferDesc.Width = width;
    sd.BufferDesc.Height = height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;

    D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };
    D3D_FEATURE_LEVEL featureLevel;

    CHECK_HR(D3D11CreateDeviceAndSwapChain(
        nullptr,                    // 기본 어댑터
        D3D_DRIVER_TYPE_HARDWARE,   // 하드웨어 가속
        nullptr,                    // 소프트웨어 래스터라이저 사용 안 함
        0,                          // 플래그
        featureLevels,              // 기능 레벨 배열
        1,                          // 기능 레벨 개수
        D3D11_SDK_VERSION,          // SDK 버전
        &sd,                        // 스왑체인 설정
        &swapChain,                 // 생성된 스왑체인
        &device,                    // 생성된 디바이스
        &featureLevel,              // 실제 사용되는 기능 레벨
        &deviceContext              // 즉시 컨텍스트
    ))

    ComPtr<ID3D11Texture2D> backBuffer = nullptr;
    CHECK_HR(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), static_cast<LPVOID*>(&backBuffer)))
    CHECK_HR(device->CreateRenderTargetView(backBuffer.Get(), nullptr, renderTargetView.GetAddressOf()))
    backBuffer->Release();
    deviceContext->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), nullptr);

    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT)width;
    vp.Height = (FLOAT)height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    deviceContext->RSSetViewports(1, &vp);

    return true;
}

void GraphicDevice::Render() noexcept
{
    float ClearColor[4] = { 0.0f, 0.2f, 0.4f, 1.0f };

    deviceContext->ClearRenderTargetView(renderTargetView.Get(), ClearColor);
    swapChain->Present(0, 0);
}

void GraphicDevice::Clear() noexcept
{
    deviceContext->ClearState();
}