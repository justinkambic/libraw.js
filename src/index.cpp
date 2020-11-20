#include "libraw_wrapper.h"

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
  return LibRawWrapper::Init(env, exports);
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)
