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
    vp.Width = static_cast<FLOAT>(width);
    vp.Height = static_cast<FLOAT>(height);
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    deviceContext->RSSetViewports(1, &vp);

    CHECK_INIT(!CreateShaders())
    CHECK_INIT(!InitializeTriangle())

    return true;
}

void GraphicDevice::Render() noexcept
{
    float ClearColor[4] = { 0.0f, 0.2f, 0.4f, 1.0f };

    deviceContext->ClearRenderTargetView(renderTargetView.Get(), ClearColor);

    deviceContext->VSSetShader(vertexShader.Get(), nullptr, 0);
    deviceContext->PSSetShader(pixelShader.Get(), nullptr, 0);
    deviceContext->IASetInputLayout(inputLayout.Get());

    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    deviceContext->Draw(3, 0);
    swapChain->Present(0, 0);
}

void GraphicDevice::Clear() noexcept
{
    deviceContext->ClearState();
}

bool GraphicDevice::CreateShaders() noexcept
{
    // 정점 셰이더 코드
    const char* vsSource = R"(
        struct VSInput
        {
            float3 position : POSITION;
            float4 color    : COLOR;
        };

        struct VSOutput
        {
            float4 position : SV_POSITION;
            float4 color    : COLOR;
        };

        VSOutput main(VSInput input)
        {
            VSOutput output;
            output.position = float4(input.position, 1.0f);
            output.color = input.color;
            return output;
        }
    )";

    // 픽셀 셰이더 코드
    const char* psSource = R"(
        struct PSInput
        {
            float4 position : SV_POSITION;
            float4 color    : COLOR;
        };

        float4 main(PSInput input) : SV_TARGET
        {
            return input.color;
        }
    )";

    ComPtr<ID3DBlob> vsBlob, psBlob, errorBlob;

    // 정점 셰이더 컴파일
    HRESULT hr = D3DCompile(vsSource, strlen(vsSource), nullptr, nullptr, nullptr,
        "main", "vs_5_0", 0, 0, &vsBlob, &errorBlob);
    if (FAILED(hr))
    {
        if (errorBlob) OutputDebugStringA((char*)errorBlob->GetBufferPointer());
        return false;
    }

    // 픽셀 셰이더 컴파일
    hr = D3DCompile(psSource, strlen(psSource), nullptr, nullptr, nullptr,
        "main", "ps_5_0", 0, 0, &psBlob, &errorBlob);
    if (FAILED(hr))
    {
        if (errorBlob) OutputDebugStringA((char*)errorBlob->GetBufferPointer());
        return false;
    }

    // 셰이더 객체 생성
    CHECK_HR(device->CreateVertexShader(vsBlob->GetBufferPointer(),
        vsBlob->GetBufferSize(), nullptr, &vertexShader))
    CHECK_HR(device->CreatePixelShader(psBlob->GetBufferPointer(),
        psBlob->GetBufferSize(), nullptr, &pixelShader))

        // 입력 레이아웃 생성
    D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    CHECK_HR(device->CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc),
        vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &inputLayout))

    return true;
}

bool GraphicDevice::InitializeTriangle() noexcept
{
    Vertex vertices[] = {
        {  0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f }, // 상단 - 빨간색
        {  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f }, // 우하단 - 초록색
        { -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f }  // 좌하단 - 파란색
    };

    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = sizeof(vertices);
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = vertices;

    CHECK_HR(device->CreateBuffer(&bufferDesc, &initData, &vertexBuffer))

    return true;
}
