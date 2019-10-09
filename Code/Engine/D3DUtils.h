#pragma once

#if SE_ENABLE_DIRECT3D11

template <typename T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

#endif