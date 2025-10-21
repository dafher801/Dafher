#include "TextureManager.h"
#include "Engine.h"
#include "GraphicDevice.h"

bool TextureManager::Init(const ComPtr<ID3D11Device>& device) noexcept
{
	_device = device;

	RecursiveLoad(L"../Resources/");

    return true;
}

void TextureManager::Clear() noexcept
{
    _textures.clear();
}

void TextureManager::RecursiveLoad(const std::wstring& folderPath) noexcept
{
    std::wstring searchPath = folderPath + L"*.*";

    WIN32_FIND_DATAW findData;
    HANDLE hFind = FindFirstFileW(searchPath.c_str(), &findData);

	assert(hFind != INVALID_HANDLE_VALUE);

    do
    {
        std::wstring fileName = std::wstring(findData.cFileName);

        if (fileName == L"." || fileName == L"..")
        {
            continue;
        }

        std::wstring fullPath = folderPath + fileName;

        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            RecursiveLoad(fullPath + L"/");
        }
        else if (IsImageFile(fileName))
        {
            std::string key = GetFileNameWithoutExtension(fileName);
            Load(key, fullPath);

            std::string debugMsg = "Loaded: " + key + "\n";
            OutputDebugStringA(debugMsg.c_str());
        }

    } while (FindNextFileW(hFind, &findData));

    FindClose(hFind);
}

Texture* TextureManager::Load(const std::string& key, const std::wstring& filePath) noexcept
{
    auto it = _textures.find(key);
    if (it != _textures.end())
    {
        return it->second.get();
    }

	auto texture = std::make_unique<Texture>();
    bool enabled = texture->LoadFromFile(_device, filePath);
    assert(enabled);
    
    Texture* ret = texture.get();
    _textures[key] = std::move(texture);
    return ret;
}

bool TextureManager::IsImageFile(const std::wstring& fileName) noexcept
{
    size_t dotPos = fileName.find_last_of(L'.');

    std::wstring ext = fileName.substr(dotPos);
    std::transform(ext.begin(), ext.end(), ext.begin(), ::towlower);

    return ext == L".png" || ext == L".jpg" || ext == L".jpeg" ||
        ext == L".bmp" || ext == L".tga" || ext == L".dds";
}

std::string TextureManager::GetFileNameWithoutExtension(const std::wstring& fileName) noexcept
{
    size_t dotPos = fileName.find_last_of(L'.');
    std::wstring nameWithoutExt = fileName.substr(0, dotPos);

    int size = WideCharToMultiByte(CP_UTF8, 0, 
        nameWithoutExt.c_str(), -1, nullptr, 0, nullptr, nullptr);

    std::string result(size - 1, 0);
    WideCharToMultiByte(CP_UTF8, 0, nameWithoutExt.c_str(),
        -1, &result[0], size, nullptr, nullptr);

    return result;
}
