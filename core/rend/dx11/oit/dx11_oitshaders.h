/*
	Copyright 2021 flyinghead

	This file is part of Flycast.

    Flycast is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    Flycast is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Flycast.  If not, see <https://www.gnu.org/licenses/>.
*/
#pragma once
#include <unordered_map>
#include <memory>
#include <d3d11.h>
#include <d3dcompiler.h>
#include "types.h"
#include "windows/comptr.h"
#include "../dx11_shaders.h"

class DX11OITShaders : CachedDX11Shaders
{
public:
	enum Pass { Depth, Color, OIT };

	void init(const ComPtr<ID3D11Device>& device, pD3DCompile D3DCompile);

	const ComPtr<ID3D11PixelShader>& getShader(bool pp_Texture, bool pp_UseAlpha, bool pp_IgnoreTexA, u32 pp_ShadInstr,
			bool pp_Offset, u32 pp_FogCtrl, bool pp_BumpMap, bool fog_clamping,
			bool palette, bool gouraud, bool alphaTest, bool clipInside, bool nearestWrapFix, bool twoVolumes, Pass pass);
	const ComPtr<ID3D11VertexShader>& getVertexShader(bool gouraud);
	const ComPtr<ID3D11PixelShader>& getModVolShader();
	const ComPtr<ID3D11VertexShader>& getMVVertexShader();
	const ComPtr<ID3D11PixelShader>& getFinalShader();
	const ComPtr<ID3D11PixelShader>& getTrModVolShader(int type);
	const ComPtr<ID3D11VertexShader>& getFinalVertexShader();

	void term()
	{
		saveCache(CacheFile);
		shaders.clear();
		gouraudVertexShader.reset();
		flatVertexShader.reset();
		modVolShader.reset();
		modVolVertexShader.reset();
		for (auto& shader : trModVolShaders)
			shader.reset();
		finalShader.reset();
		clearShader.reset();
		finalVertexShader.reset();
		device.reset();
	}
	ComPtr<ID3DBlob> getVertexShaderBlob();
	ComPtr<ID3DBlob> getMVVertexShaderBlob();

private:
	ComPtr<ID3DBlob> compileShader(const char *source, const char* function, const char* profile, const D3D_SHADER_MACRO *pDefines);
	ComPtr<ID3D11VertexShader> compileVS(const char *source, const char* function, const D3D_SHADER_MACRO *pDefines);
	ComPtr<ID3D11PixelShader> compilePS(const char *source, const char* function, const D3D_SHADER_MACRO *pDefines);

	ComPtr<ID3D11Device> device;
	std::unordered_map<u32, ComPtr<ID3D11PixelShader>> shaders;
	ComPtr<ID3D11VertexShader> gouraudVertexShader;
	ComPtr<ID3D11VertexShader> flatVertexShader;
	ComPtr<ID3D11PixelShader> modVolShader;
	ComPtr<ID3D11VertexShader> modVolVertexShader;

	ComPtr<ID3D11PixelShader> trModVolShaders[4];
	ComPtr<ID3D11PixelShader> finalShader;
	ComPtr<ID3D11PixelShader> clearShader;
	ComPtr<ID3D11VertexShader> finalVertexShader;
	pD3DCompile D3DCompile = nullptr;

	constexpr static const char *CacheFile = "dx11oit_shader_cache.bin";
};
