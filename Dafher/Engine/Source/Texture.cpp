#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_RESIZE2_IMPLEMENTATION

#include "stb_image.h"
#include "stb_image_resize2.h"

bool Texture::LoadFromFile(const ComPtr<ID3D11Device>& device, const std::wstring& filePath) noexcept
{
    _device = device;

    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    assert(file.is_open());

    std::streamsize fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<unsigned char> buffer(fileSize);
    bool readSuccess = file.read(reinterpret_cast<char*>(buffer.data()), fileSize).good();
    assert(readSuccess);

    bool isLoaded = LoadImageData(buffer.data(), buffer.size());
	assert(isLoaded);

    _width = _originalWidth;
    _height = _originalHeight;

    return CreateTexture();
}

bool Texture::LoadImageData(const void* data, size_t dataSize) noexcept
{
    int channels;
    int imageWidth, imageHeight;
    unsigned char* imageData = stbi_load_from_memory(
        static_cast<const unsigned char*>(data),
        static_cast<int>(dataSize),
        &imageWidth, &imageHeight, &channels, 4
    );

	assert(imageData != nullptr);

    _originalWidth = static_cast<uint32>(imageWidth);
    _originalHeight = static_cast<uint32>(imageHeight);
    _format = DXGI_FORMAT_R8G8B8A8_UNORM;

    size_t dataLength = _originalWidth * _originalHeight * 4;
    _originalImageData.assign(imageData, imageData + dataLength);

    stbi_image_free(imageData);

    return true;
}

bool Texture::Resize(uint32 width, uint32 height) noexcept
{
    if (_width == width && _height == height && _texture2D != nullptr)
    {
        return true;
    }

    _width = width;
    _height = height;

    return CreateTexture();
}

bool Texture::CreateTexture() noexcept
{
	std::vector<unsigned char> finalImageData;
    bool needResize = (_width != _originalWidth) || (_height != _originalHeight);

    if (needResize)
    {
		finalImageData.assign(_width * _height * 4, 0);
        stbir_resize_uint8_linear(
            _originalImageData.data(), _originalWidth, _originalHeight, 0,
            finalImageData.data(), _width, _height, 0,
            STBIR_RGBA
        );
    }
    else
    {
        finalImageData = _originalImageData;
    }

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
    initData.pSysMem = finalImageData.data();
    initData.SysMemPitch = _width * 4;
    initData.SysMemSlicePitch = 0;

    _texture2D.Reset();
    _shaderResourceView.Reset();

    ASSERT_HR(_device->CreateTexture2D(&textureDesc, &initData, &_texture2D));

    finalImageData.clear();

    return CreateShaderResourceView();
}

bool Texture::CreateShaderResourceView() noexcept
{
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = _format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;
    srvDesc.Texture2D.MostDetailedMip = 0;

    ASSERT_HR(_device->CreateShaderResourceView(_texture2D.Get(), &srvDesc, &_shaderResourceView));

    return true;
}