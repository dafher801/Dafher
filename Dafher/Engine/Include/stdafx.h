#ifndef __STDAFX_H__
#define __STDAFX_H__

#include <Windows.h>
#include <tchar.h>
#include <d3d11.h>
#include <memory>
#include <wrl/client.h>

#pragma comment(lib, "Engine.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;
using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;

using namespace std;
using namespace Microsoft::WRL;

#define CHECK_HR(hr) if (FAILED(hr)) { return false; }
#define CHECK_INIT(init) if (init) { return false; }

#endif