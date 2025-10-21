#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "Stdafx.h"

class Renderer
{	
public:
	Renderer() noexcept = default;
	~Renderer() noexcept = default;

	Renderer(const Renderer& renderer) noexcept = delete;
	Renderer& operator=(const Renderer& renderer) noexcept = delete;
	Renderer(Renderer&& renderer) noexcept = delete;

public:
	bool Init(const ComPtr<ID3D11Device>& device, const ComPtr<ID3D11DeviceContext>& deviceContext) noexcept;
	void Draw(class Texture* texture, const Matrix& worldMatrix) noexcept;

private:
	bool CreateShaders() noexcept;
	bool CreateBuffers() noexcept;
	bool CreateStates() noexcept;

private:
	ComPtr<ID3D11Device> _device;
	ComPtr<ID3D11DeviceContext> _deviceContext;

	ComPtr<ID3D11VertexShader> _vertexShader;
	ComPtr<ID3D11PixelShader> _pixelShader;
	ComPtr<ID3D11InputLayout> _inputLayout;

	ComPtr<ID3D11Buffer> _quadVertexBuffer;
	ComPtr<ID3D11Buffer> _quadIndexBuffer;
	ComPtr<ID3D11Buffer> _quadConstantBuffer;

	ComPtr<ID3D11RasterizerState> _rasterizerState;
	ComPtr<ID3D11SamplerState> _samplerState;
	ComPtr<ID3D11BlendState> _blendState;
};

#endif