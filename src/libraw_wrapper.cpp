#include <napi.h>
#include "libraw_wrapper.h"
#include "wraptypes.h"
#include <fstream>

Napi::Object LibRawWrapper::Init(Napi::Env& env, Napi::Object& exports) {
  Napi::HandleScope scope(env);

  Napi::Function func =
    DefineClass(
      env,
      "LibRawWrapper",
      {
        InstanceMethod("getMetadata", &LibRawWrapper::GetMetadata),
        InstanceMethod("getThumbnail", &LibRawWrapper::GetThumbnail),
        InstanceMethod("getXmp", &LibRawWrapper::GetXmpData)
      }
    );

  exports.Set("LibRawWrapper", func);
  return exports;
}

LibRawWrapper::LibRawWrapper(const Napi::CallbackInfo& info): Napi::ObjectWrap<LibRawWrapper>(info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  if (info.Length() < 1)
  {
    Napi::TypeError::New(env, "Expected path to image").ThrowAsJavaScriptException();
    return;
  }
  if (!info[0].IsString() && (!info[0].IsBuffer() && info.Length() > 1))
  {
    Napi::TypeError::New(env, "Expected path to image or buffer with size").ThrowAsJavaScriptException();
    return;
  }

  if (info[0].IsString())
  {
    Napi::String path = info[0].As<Napi::String>();
    std::fstream f(path.Utf8Value().c_str());
    if (!f.good())
    {
      Napi::TypeError::New(env, "File does not exist").ThrowAsJavaScriptException();
      return;
    }
    this->processor_ = new LibRaw();
    this->processor_->open_file(path.Utf8Value().c_str());
  }
  else if (info[0].IsBuffer() && info[1].IsNumber())
  {
    this->processor_ = new LibRaw();
    Napi::Buffer<char> buff = info[0].As<Napi::Buffer<char>>();
    this->processor_->open_buffer(buff.Data(), buff.Length());
  }
  else
  {
    Napi::TypeError::New(env, "LibRaw constructor did not receive required parameters.").ThrowAsJavaScriptException();
  }
}

Napi::Value LibRawWrapper::GetThumbnail(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::Object o = Napi::Object::New(env);
  
  this->processor_->unpack_thumb();
  if (this->processor_->imgdata.thumbnail.thumb)
  {
    Napi::Buffer<char> b = Napi::Buffer<char>::New(
      env,
      this->processor_->imgdata.thumbnail.thumb,
      this->processor_->imgdata.thumbnail.tlength
    );
    return b;
  }

  return o;
}

Napi::Value LibRawWrapper::GetXmpData(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::Object o = Napi::Object::New(env);

  char *xmp = this->processor_->imgdata.idata.xmpdata;
  if (xmp)
  {
    Napi::Buffer<char> b = Napi::Buffer<char>::New(
      env,
      xmp,
      this->processor_->imgdata.idata.xmplen
    );
    return b;
  }
  return o;
}

Napi::Value LibRawWrapper::GetMetadata(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  libraw_data_t data = this->processor_->imgdata;
  return WrapLibRawData(&env, &data);
}

LibRawWrapper::~LibRawWrapper()
{
  delete this->processor_;
}
