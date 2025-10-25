#ifndef __TEXTURE_MANAGER_H__
#define __TEXTURE_MANAGER_H__

#include "Stdafx.h"
#include "Texture.h"

class TextureManager
{
public:
    TextureManager() noexcept = default;

	TextureManager(const TextureManager& textureManager) noexcept = delete;
	TextureManager(TextureManager&& textureManager) noexcept = delete;
	TextureManager& operator=(const TextureManager& textureManager) noexcept = delete;
	TextureManager& operator=(TextureManager&& textureManager) noexcept = delete;

public:
    ~TextureManager() noexcept = default;

public:
    bool Init(const ComPtr<ID3D11Device>& device) noexcept;
	void Clear() noexcept;

public:
    inline Texture* GetTexture(const std::string& key) const noexcept
    {
        return _textures.at(key).get();
    }

private:
	void RecursiveLoad(const std::wstring& folderPath) noexcept;
    Texture* Load(const std::string& key, const std::wstring& filePath) noexcept;
    bool IsImageFile(const std::wstring& fileName) noexcept;
    std::string GetFileNameWithoutExtension(const std::wstring& fileName) noexcept;

private:
    std::unordered_map<std::string, std::unique_ptr<Texture>> _textures;

    ComPtr<ID3D11Device> _device;
};

#endif