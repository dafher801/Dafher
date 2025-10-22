#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "Stdafx.h"

class Texture
{
public:
    inline Texture() noexcept
        : _width(0)
        , _height(0)
        , _originalWidth(0)
        , _originalHeight(0)
        , _format(DXGI_FORMAT_UNKNOWN)
    {
    }

    ~Texture() noexcept = default;

public:
    inline ID3D11ShaderResourceView* GetShaderResourceView() const noexcept
    {
        return _shaderResourceView.Get();
    }

    inline ID3D11Texture2D* GetTexture2D() const noexcept
    {
        return _texture2D.Get();
    }

    inline uint32 GetWidth() const noexcept
    {
        return _width;
	}

    inline uint32 GetHeight() const noexcept
    {
        return _height;
	}

    inline DXGI_FORMAT GetFormat() const noexcept
    {
        return _format;
    }

    inline bool HasImageData() const noexcept
    {
        return !_originalImageData.empty();
    }

public:
    bool LoadFromFile(const ComPtr<ID3D11Device>& device, const std::wstring& filePath) noexcept;
    bool Resize(uint32 width, uint32 height) noexcept;

private:
    bool LoadImageData(const void* data, size_t dataSize) noexcept;
    bool CreateTexture() noexcept;
    bool CreateShaderResourceView() noexcept;
    
private:
    ComPtr<ID3D11Texture2D> _texture2D;
    ComPtr<ID3D11ShaderResourceView> _shaderResourceView;
    ComPtr<ID3D11Device> _device;

    std::vector<unsigned char> _originalImageData;
    uint32 _originalWidth;
    uint32 _originalHeight;

    uint32 _width;
    uint32 _height;
    DXGI_FORMAT _format;
};

#endif