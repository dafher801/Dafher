#ifndef __GRAPHICDEVICE_H__
#define __GRAPHICDEVICE_H__

#include "stdafx.h"

class GraphicDevice
{
public:
	GraphicDevice() noexcept = default;
	~GraphicDevice() noexcept = default;

public:
	bool Init() noexcept;

	void Render() noexcept;

	void Clear() noexcept;

private:
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> deviceContext;
	ComPtr<IDXGISwapChain> swapChain;
	ComPtr<ID3D11RenderTargetView> renderTargetView;
};

#endif