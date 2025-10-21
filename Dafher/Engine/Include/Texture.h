#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "Stdafx.h"

class Texture
{
public:
    inline Texture() noexcept
        : _width(0)
        , _height(0)
        , _format(DXGI_FORMAT_UNKNOWN)
    {
    }

    ~Texture() noexcept = default;

public:
    bool LoadFromFile(const ComPtr<ID3D11Device>& device, const std::wstring& filePath) noexcept;
    bool CreateFromMemory(const ComPtr<ID3D11Device>& device, const void* data, size_t dataSize) noexcept;

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

private:
    bool CreateShaderResourceView(const ComPtr<ID3D11Device>& device) noexcept;

private:
    ComPtr<ID3D11Texture2D> _texture2D;
    ComPtr<ID3D11ShaderResourceView> _shaderResourceView;

    uint32 _width;
    uint32 _height;
    DXGI_FORMAT _format;
};

#endif