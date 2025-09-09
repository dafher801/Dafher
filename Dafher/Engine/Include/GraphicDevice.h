#ifndef __GRAPHICDEVICE_H__
#define __GRAPHICDEVICE_H__

#include "Stdafx.h"
#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")

struct Vertex {
	float x, y, z;
	float r, g, b, a;
};

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
	bool CreateShaders() noexcept;
	bool InitializeTriangle() noexcept;

private:
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> deviceContext;
	ComPtr<IDXGISwapChain> swapChain;
	ComPtr<ID3D11RenderTargetView> renderTargetView;

	ComPtr<ID3D11Buffer> vertexBuffer;
	ComPtr<ID3D11VertexShader> vertexShader;
	ComPtr<ID3D11PixelShader> pixelShader;
	ComPtr<ID3D11InputLayout> inputLayout;
};

#endif