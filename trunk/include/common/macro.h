#pragma once

#ifdef _DEBUG
    #define INLINE __inline
#else
    #define INLINE __forceinline
#endif

#define SAFE_DELETE(p)       { if(nullptr != p){ delete   p; p = nullptr; }}
#define SAFE_DELETE_ARRAY(p) { if(nullptr != p){ delete[] p; p = nullptr; }}

#define SAFE_RELEASE(p) { if(nullptr != p){ p->Release(); p = nullptr; }}

#define GET_ACCESSOR(x, y) INLINE x y() { return m_##y; }
#define SET_ACCESSOR(x, y) INLINE void y(x t) { m_##y = t; }

#define GET_SET_ACCESSOR(x, y) GET_ACCESSOR(x, y); SET_ACCESSOR(x, y)
#define GET_ACCESSOR_CONST(x, y) INLINE x y() const { return m_##y; }

