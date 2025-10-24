#ifndef __STDAFX_H__
#define __STDAFX_H__

#include <Windows.h>
#include <tchar.h>
#include <assert.h>
#include <algorithm>
#include <cctype>
#include <cwctype>
#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>
#include <cmath>
#include <chrono>
#include <memory_resource>
#include <fstream>

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <wrl/client.h>
#include <SimpleMath.h>

#pragma comment(lib, "Engine.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;
using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;

using namespace Microsoft::WRL;

using Vector2 = DirectX::SimpleMath::Vector2;
using Vector3 = DirectX::SimpleMath::Vector3;
using Vector4 = DirectX::SimpleMath::Vector4;
using Quaternion = DirectX::SimpleMath::Quaternion;
using Matrix = DirectX::SimpleMath::Matrix;
using Color = DirectX::SimpleMath::Color;

struct Vertex
{
	float _x, _y, _z;
    float _u, _v;
};

struct MatrixData
{
	Matrix _world;
	Matrix _view;
	Matrix _projection;
};

#define ASSERT_HR(__HR__) { HRESULT HR = __HR__; assert(SUCCEEDED(HR)); }

#define CREATE(__TYPE_NAME__) \
    template<typename... Args> \
    static __TYPE_NAME__* Create(Args&&... args) \
    { \
        __TYPE_NAME__* object = new __TYPE_NAME__(std::forward<Args>(args)...); \
        if (object && object->Init()) \
        { \
            return object; \
        } \
        delete object; \
        return nullptr; \
    }

#endif