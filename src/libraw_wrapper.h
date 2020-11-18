#include <napi.h>
#include "libraw/libraw.h"

class LibRawWrapper: public Napi::ObjectWrap<LibRawWrapper> {
  public:
    static Napi::Object Init(Napi::Env& env, Napi::Object& exports);
    LibRawWrapper(const Napi::CallbackInfo& info);
    ~LibRawWrapper();
    Napi::Value GetMetadata(const Napi::CallbackInfo& info);
    Napi::Value GetThumbnail(const Napi::CallbackInfo& info);
    Napi::Value GetXmpData(const Napi::CallbackInfo& info);
  private:
    LibRaw* processor_;
};