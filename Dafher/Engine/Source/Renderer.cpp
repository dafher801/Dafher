#include "Renderer.h"
#include "Transform.h"
#include "Texture.h"
#include "Engine.h"
#include "GraphicDevice.h"

bool Renderer::Init(const ComPtr<ID3D11Device>& device, const ComPtr<ID3D11DeviceContext>& deviceContext) noexcept
{
	_device = device;
	_deviceContext = deviceContext;

    CreateShaders();
	CreateBuffers();
	CreateStates();

	return true;
}

void Renderer::Draw(Texture* texture, const Matrix& worldMatrix) noexcept
{
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    ASSERT_HR(_deviceContext->Map(_quadConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

    Vector3 cameraPosition = Vector3(0.0f, 0.0f, 10.0f);
    Vector3 targetPosition = Vector3(0.0f, 0.0f, 0.0f); 
    Vector3 upVector = Vector3(0.0f, 1.0f, 0.0f);       

    Matrix viewMatrix = DirectX::XMMatrixLookAtLH(
        DirectX::XMLoadFloat3(reinterpret_cast<const DirectX::XMFLOAT3*>(&cameraPosition)),
        DirectX::XMLoadFloat3(reinterpret_cast<const DirectX::XMFLOAT3*>(&targetPosition)),
        DirectX::XMLoadFloat3(reinterpret_cast<const DirectX::XMFLOAT3*>(&upVector))
    );

    ComPtr<IDXGISwapChain> swapChain = Engine::GetInstance()->GetDevice()->GetSwapChain();

    DXGI_SWAP_CHAIN_DESC desc;
    swapChain->GetDesc(&desc);

    float halfWidth = static_cast<float>(desc.BufferDesc.Width) / 2.0f;
    float halfHeight = static_cast<float>(desc.BufferDesc.Height) / 2.0f;

    Matrix projectionMatrix = DirectX::XMMatrixOrthographicOffCenterLH(
        halfWidth, -halfWidth,
        -halfHeight, halfHeight,
        0.0f, 1000.0f
    );

    MatrixData* matrixData = static_cast<MatrixData*>(mappedResource.pData);
    matrixData->_world = DirectX::XMMatrixTranspose(worldMatrix);
    matrixData->_view = DirectX::XMMatrixTranspose(viewMatrix);
    matrixData->_projection = DirectX::XMMatrixTranspose(projectionMatrix);

    _deviceContext->Unmap(_quadConstantBuffer.Get(), 0);
    _deviceContext->RSSetState(_rasterizerState.Get());

    float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    _deviceContext->OMSetBlendState(_blendState.Get(), blendFactor, 0xffffffff);
    _deviceContext->VSSetShader(_vertexShader.Get(), nullptr, 0);
    _deviceContext->PSSetShader(_pixelShader.Get(), nullptr, 0);
    _deviceContext->IASetInputLayout(_inputLayout.Get());

    _deviceContext->VSSetConstantBuffers(0, 1, _quadConstantBuffer.GetAddressOf());

    ID3D11ShaderResourceView* shaderResourceView = texture->GetShaderResourceView();
    _deviceContext->PSSetShaderResources(0, 1, &shaderResourceView);
    _deviceContext->PSSetSamplers(0, 1, _samplerState.GetAddressOf());

    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    _deviceContext->IASetVertexBuffers(0, 1, _quadVertexBuffer.GetAddressOf(), &stride, &offset);
    _deviceContext->IASetIndexBuffer(_quadIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    _deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    _deviceContext->DrawIndexed(6, 0, 0);
}

bool Renderer::CreateShaders() noexcept
{
    const char* vertexShaderSource = R"(
        cbuffer MatrixBuffer : register(b0)
        {
            matrix World;
            matrix View;
            matrix Projection;
        };

        struct VS_INPUT
        {
            float3 Position : POSITION;
            float2 TexCoord : TEXCOORD;
        };

        struct PS_INPUT
        {
            float4 Position : SV_POSITION;
            float2 TexCoord : TEXCOORD;
        };

        PS_INPUT main(VS_INPUT input)
        {
            PS_INPUT output;
            
            float4 worldPos = mul(float4(input.Position, 1.0f), World);
            float4 viewPos = mul(worldPos, View);
            output.Position = mul(viewPos, Projection);
            output.TexCoord = input.TexCoord;
            
            return output;
        }
    )";

    ComPtr<ID3DBlob> vertexShaderBlob;
    ComPtr<ID3DBlob> errorBlob;

    ASSERT_HR(D3DCompile(vertexShaderSource, strlen(vertexShaderSource),
        nullptr, nullptr, nullptr, "main", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS,
        0, &vertexShaderBlob, &errorBlob));

    ASSERT_HR(_device->CreateVertexShader(vertexShaderBlob->GetBufferPointer(),
        vertexShaderBlob->GetBufferSize(), nullptr, &_vertexShader));

    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    ASSERT_HR(_device->CreateInputLayout(layout, ARRAYSIZE(layout), 
        vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), &_inputLayout));

    const char* pixelShaderSource = R"(
        Texture2D MainTexture : register(t0);
        SamplerState MainSampler : register(s0);

        struct PS_INPUT
        {
            float4 Position : SV_POSITION;
            float2 TexCoord : TEXCOORD;
        };

        float4 main(PS_INPUT input) : SV_TARGET
        {
            return MainTexture.Sample(MainSampler, input.TexCoord);
        }
    )";

    ComPtr<ID3DBlob> pixelShaderBlob;

    ASSERT_HR(D3DCompile(pixelShaderSource, strlen(pixelShaderSource),
        nullptr, nullptr, nullptr, "main", "ps_5_0",
        D3DCOMPILE_ENABLE_STRICTNESS, 0, &pixelShaderBlob, &errorBlob));

    ASSERT_HR(_device->CreatePixelShader(pixelShaderBlob->GetBufferPointer(),
        pixelShaderBlob->GetBufferSize(), nullptr, &_pixelShader));

    return true;
}

bool Renderer::CreateBuffers() noexcept
{
    Vertex vertices[] = {
        { 0.0f, 1.0f, 0.0f, 0.0f, 0.0f },
        { 1.0f, 1.0f, 0.0f, 1.0f, 0.0f },
        { 1.0f, 0.0f, 0.0f, 1.0f, 1.0f },
        { 0.0f, 0.0f, 0.0f, 0.0f, 1.0f } 
    };

    D3D11_BUFFER_DESC vertexBufferDesc = {};
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(vertices);
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA vertexBufferData = {};
    vertexBufferData.pSysMem = vertices;

    ASSERT_HR(_device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &_quadVertexBuffer));

    uint32 indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    D3D11_BUFFER_DESC indexBufferDesc = {};
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(indices);
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA indexBufferData = {};
    indexBufferData.pSysMem = indices;

    ASSERT_HR(_device->CreateBuffer(&indexBufferDesc, &indexBufferData, &_quadIndexBuffer));

    D3D11_BUFFER_DESC constantBufferDesc = {};
    constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    constantBufferDesc.ByteWidth = sizeof(MatrixData);
    constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    ASSERT_HR(_device->CreateBuffer(&constantBufferDesc, nullptr, &_quadConstantBuffer));

    return true;
}

bool Renderer::CreateStates() noexcept
{
    D3D11_RASTERIZER_DESC rasterizerDesc = {};
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.CullMode = D3D11_CULL_NONE;
    rasterizerDesc.FrontCounterClockwise = FALSE;
    rasterizerDesc.DepthClipEnable = TRUE;

    ASSERT_HR(_device->CreateRasterizerState(&rasterizerDesc, &_rasterizerState));

    D3D11_SAMPLER_DESC samplerDesc = {};
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = 1;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    samplerDesc.MinLOD = 0.0f;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    ASSERT_HR(_device->CreateSamplerState(&samplerDesc, &_samplerState));

    D3D11_BLEND_DESC blendDesc = {};
    blendDesc.AlphaToCoverageEnable = FALSE;
    blendDesc.IndependentBlendEnable = FALSE;
    blendDesc.RenderTarget[0].BlendEnable = TRUE;
    blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    ASSERT_HR(_device->CreateBlendState(&blendDesc, &_blendState));

    return true;
}
