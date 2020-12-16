#include <napi.h>
#include "libraw/libraw.h"

class LibRawWrapper: public Napi::ObjectWrap<LibRawWrapper> {
  public:
    static Napi::Object Init(Napi::Env& env, Napi::Object& exports);
    LibRawWrapper(const Napi::CallbackInfo& info);
    ~LibRawWrapper();
    Napi::Value CameraCount(const Napi::CallbackInfo& info);
    Napi::Value CameraList(const Napi::CallbackInfo& info);
    Napi::Value GetMetadata(const Napi::CallbackInfo& info);
    Napi::Value GetThumbnail(const Napi::CallbackInfo& info);
    Napi::Value GetXmpData(const Napi::CallbackInfo& info);
    Napi::Value OpenFile(const Napi::CallbackInfo& info);
    Napi::Value OpenBuffer(const Napi::CallbackInfo& info);
    Napi::Value Unpack(const Napi::CallbackInfo& info);
    Napi::Value UnpackThumb(const Napi::CallbackInfo& info);
    Napi::Value ErrorCount(const Napi::CallbackInfo& info);
    Napi::Value Version(const Napi::CallbackInfo& info);
    Napi::Value VersionNumber(const Napi::CallbackInfo& info);
    Napi::Value StrError(const Napi::CallbackInfo& info);
    void RecycleDatastream(const Napi::CallbackInfo& info);
    void Recycle(const Napi::CallbackInfo& info);
  private:
    LibRaw* processor_;
};