#include "Texture.h"
#include <assert.h>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

bool Texture::LoadFromFile(const ComPtr<ID3D11Device>& device, const std::wstring& filePath) noexcept
{
    int size = WideCharToMultiByte(CP_UTF8, 0, filePath.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::vector<char> buffer(size);
    WideCharToMultiByte(CP_UTF8, 0, filePath.c_str(), -1, buffer.data(), size, nullptr, nullptr);

    int channels;
    int imageWidth, imageHeight;
    unsigned char* imageData = stbi_load(buffer.data(), &imageWidth, &imageHeight, &channels, 4);

    if (!imageData)
    {
        return false;
    }

    _width = static_cast<uint32>(imageWidth);
    _height = static_cast<uint32>(imageHeight);
    _format = DXGI_FORMAT_R8G8B8A8_UNORM;

    D3D11_TEXTURE2D_DESC textureDesc = {};
    textureDesc.Width = _width;
    textureDesc.Height = _height;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = _format;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = imageData;
    initData.SysMemPitch = _width * 4;
    initData.SysMemSlicePitch = 0;

	ASSERT_HR(device->CreateTexture2D(&textureDesc, &initData, &_texture2D));

    stbi_image_free(imageData);

    CreateShaderResourceView(device);

    return true;
}

bool Texture::CreateFromMemory(const ComPtr<ID3D11Device>& device, const void* data, size_t dataSize) noexcept
{
    if (!device || !data)
    {
        return false;
    }

    int channels;
    int imageWidth, imageHeight;
    unsigned char* imageData = stbi_load_from_memory(
        static_cast<const unsigned char*>(data),
        static_cast<int>(dataSize),
        &imageWidth, &imageHeight, &channels, 4
    );

    if (!imageData)
    {
        return false;
    }

    _width = static_cast<uint32>(imageWidth);
    _height = static_cast<uint32>(imageHeight);
    _format = DXGI_FORMAT_R8G8B8A8_UNORM;

    D3D11_TEXTURE2D_DESC textureDesc = {};
    textureDesc.Width = _width;
    textureDesc.Height = _height;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = _format;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = imageData;
    initData.SysMemPitch = _width * 4;
    initData.SysMemSlicePitch = 0;
    
	ASSERT_HR(device->CreateTexture2D(&textureDesc, &initData, &_texture2D));
    
    stbi_image_free(imageData);

    CreateShaderResourceView(device);

    return true;
}

bool Texture::CreateShaderResourceView(const ComPtr<ID3D11Device>& device) noexcept
{
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = _format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;
    srvDesc.Texture2D.MostDetailedMip = 0;

    ASSERT_HR(device->CreateShaderResourceView(_texture2D.Get(), &srvDesc, &_shaderResourceView));

    return true;
}