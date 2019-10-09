#pragma once

enum class RenderBackend
{
	Direct3D11
};


struct RenderConfig
{
	RenderBackend gapi = RenderBackend::Direct3D11;
};