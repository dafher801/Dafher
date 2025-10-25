#ifndef __GRAPHICS_DEVICE_H__
#define __GRAPHICS_DEVICE_H__

#include "Stdafx.h"

class GraphicDevice
{
public:
	inline GraphicDevice() noexcept
		: _aspectRatio(0.0f)
	{
	}

	GraphicDevice(const GraphicDevice& graphicDevice) = delete;
	GraphicDevice(GraphicDevice&& graphicDevice) noexcept = delete;
	GraphicDevice& operator=(const GraphicDevice& graphicDevice) = delete;
	GraphicDevice& operator=(GraphicDevice&& graphicDevice) = delete;

public:
	~GraphicDevice() noexcept = default;

public:
	bool Init(HWND hWnd) noexcept;
	void BeginFrame() noexcept;
	void EndFrame() noexcept;
	void Clear() noexcept;

public:
	inline ComPtr<ID3D11Device> GetD11Device() const noexcept
	{
		return _device;
	}

	inline ComPtr<IDXGISwapChain> GetSwapChain() const noexcept
	{
		return _swapChain;
	}

	inline ComPtr<ID3D11DeviceContext> GetContext() const noexcept
	{
		return _context;
	}

	inline float GetAspectRatio() const noexcept
	{
		return _aspectRatio;
	}

private:
	ComPtr<ID3D11Device> _device;
	ComPtr<ID3D11DeviceContext> _context;
	ComPtr<IDXGISwapChain> _swapChain;
	ComPtr<ID3D11RenderTargetView> _renderTargetView;

	float _aspectRatio;
};

#endif