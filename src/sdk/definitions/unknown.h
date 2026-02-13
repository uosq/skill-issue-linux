#pragma once

#include "windows_base.h"

typedef interface IUnknown IUnknown;

DEFINE_GUID(IID_IUnknown, 0x00000000,0x0000,0x0000,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46)

#ifdef __cplusplus
struct IUnknown {

public:

  virtual HRESULT QueryInterface(REFIID riid, void** ppvObject) = 0;

  virtual uint32_t AddRef()  = 0;
  virtual uint32_t Release() = 0;

};
#else
typedef struct IUnknownVtbl
{
BEGIN_INTERFACE

  HRESULT (STDMETHODCALLTYPE *QueryInterface)(
    IUnknown *This,
    REFIID riid,
    void **ppvObject
  );
  uint32_t (STDMETHODCALLTYPE *AddRef)(IUnknown *This);
  uint32_t (STDMETHODCALLTYPE *Release)(IUnknown *This);

END_INTERFACE
} IUnknownVtbl;

interface IUnknown
{
    CONST_VTBL struct IUnknownVtbl *lpVtbl;
};

#define IUnknown_AddRef(This) ((This)->lpVtbl->AddRef(This))
#define IUnknown_Release(This) ((This)->lpVtbl->Release(This))

#endif // __cplusplus

DECLARE_UUIDOF_HELPER(IUnknown, 0x00000000,0x0000,0x0000,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46)

#define IID_PPV_ARGS(ppType) __uuidof(decltype(**(ppType))), [](auto** pp) { (void)static_cast<IUnknown*>(*pp); return reinterpret_cast<void**>(pp); }(ppType)