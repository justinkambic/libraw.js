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
        InstanceMethod("getXmp", &LibRawWrapper::GetXmpData),
        InstanceMethod("cameraCount", &LibRawWrapper::CameraCount),
        InstanceMethod("cameraList", &LibRawWrapper::CameraList),
        InstanceMethod("open_file", &LibRawWrapper::OpenFile),
        InstanceMethod("open_buffer", &LibRawWrapper::OpenBuffer),
        InstanceMethod("unpack", &LibRawWrapper::Unpack),
        InstanceMethod("unpack_thumb", &LibRawWrapper::UnpackThumb),
        InstanceMethod("recycle", &LibRawWrapper::Recycle),
        InstanceMethod("error_count", &LibRawWrapper::ErrorCount),
        InstanceMethod("recycle_datastream", &LibRawWrapper::RecycleDatastream),
        InstanceMethod("strerror", &LibRawWrapper::StrError),
        InstanceMethod("version", &LibRawWrapper::Version),
        InstanceMethod("versionNumber", &LibRawWrapper::VersionNumber)
      }
    );

  exports.Set("LibRawWrapper", func);
  return exports;
}

LibRawWrapper::LibRawWrapper(const Napi::CallbackInfo& info): Napi::ObjectWrap<LibRawWrapper>(info) {
  this->processor_ = new LibRaw();
}

Napi::Value LibRawWrapper::GetThumbnail(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  
  if (this->processor_->imgdata.thumbnail.thumb)
  {
    return Napi::Buffer<char>::New(
      env,
      this->processor_->imgdata.thumbnail.thumb,
      this->processor_->imgdata.thumbnail.tlength
    );
  }

  Napi::Error::New(
    env,
    "Thumbnail is not unpacked or is null."
  ).ThrowAsJavaScriptException();

  return env.Undefined();
}

Napi::Value LibRawWrapper::GetXmpData(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  char *xmp = this->processor_->imgdata.idata.xmpdata;
  if (xmp)
  {
    return Napi::Buffer<char>::New(
      env,
      xmp,
      this->processor_->imgdata.idata.xmplen
    );
  }
  return Napi::Object::New(env);
}

Napi::Value LibRawWrapper::GetMetadata(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  libraw_data_t data = this->processor_->imgdata;
  return WrapLibRawData(&env, &data);
}

Napi::Value LibRawWrapper::OpenFile(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "openFile received an invalid argument, filename must be a string.").ThrowAsJavaScriptException();
  }
  if (info.Length() == 2 && !info[1].IsNumber()) {
    Napi::TypeError::New(env, "openFile received an invalid argument, bigfile_size must be a number.").ThrowAsJavaScriptException();
  }
  Napi::String filename = info[0].As<Napi::String>();
  int ret;
  if (info.Length() == 2) {
    Napi::Number bigfile_size = info[1].As<Napi::Number>();
    ret = this->processor_->open_file(
      filename.Utf8Value().c_str(),
      bigfile_size.Int64Value()
    );
  } else {
    ret = this->processor_->open_file(filename.Utf8Value().c_str());
  }

  return Napi::Value::From(env, ret);
}

Napi::Value LibRawWrapper::OpenBuffer(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() != 1 || !info[0].IsBuffer()) {
    Napi::TypeError::New(env, "openBuffer received a null argument, buffer is required.").ThrowAsJavaScriptException();
  }
  Napi::Buffer<char> buffer = info[0].As<Napi::Buffer<char>>();
  return Napi::Value::From(
    env,
    this->processor_->open_buffer(buffer.Data(), buffer.Length())
  );
}

Napi::Value LibRawWrapper::Unpack(const Napi::CallbackInfo& info) {
  return Napi::Value::From(
    info.Env(),
    this->processor_->unpack()
  );
}

Napi::Value LibRawWrapper::UnpackThumb(const Napi::CallbackInfo& info) {
  return Napi::Value::From(
    info.Env(),
    this->processor_->unpack_thumb()
  );
}

void LibRawWrapper::Recycle(const Napi::CallbackInfo& info) {
  this->processor_->recycle();
}

Napi::Value LibRawWrapper::ErrorCount(const Napi::CallbackInfo& info) {
  return Napi::Value::From(
    info.Env(),
    this->processor_->error_count()
  );
}

Napi::Value LibRawWrapper::VersionNumber(const Napi::CallbackInfo& info) {
  return Napi::Value::From(
    info.Env(),
    this->processor_->versionNumber()
  );
}

Napi::Value LibRawWrapper::Version(const Napi::CallbackInfo& info) {
  return Napi::Value::From(
    info.Env(),
    this->processor_->version()
  );
}

Napi::Value LibRawWrapper::CameraCount(const Napi::CallbackInfo& info) {
  return Napi::Value::From(
    info.Env(),
    this->processor_->cameraCount()
  );
}

/*
 * `cameraList` provides a null-terminated list of camera names.
 * This function finds the limit and then allocates an Napi::Array with the
 * found limit, and loads the values to return.
 * 
 * This function can almost certainly be refactored for better efficiency and readability.
 */
Napi::Value LibRawWrapper::CameraList(const Napi::CallbackInfo& info) {
  const char **cameraList = this->processor_->cameraList();
  size_t i = 0;
  while (const char* c = cameraList[i]) {
    i++;
    if (cameraList[i] == nullptr) break;
  }
  Napi::Array cameraListArray = Napi::Array::New(info.Env(), i);
  for (size_t idx = 0; idx < i; idx++) {
    cameraListArray[idx] = cameraList[idx];
  }
  return cameraListArray;
}

Napi::Value LibRawWrapper::StrError(const Napi::CallbackInfo& info) {
  return Napi::Value::From(
    info.Env(),
    this->processor_->strerror(info[0].As<Napi::Number>().Int32Value())
  );
}

void LibRawWrapper::RecycleDatastream(const Napi::CallbackInfo& info) {
  this->processor_->recycle_datastream();
}

LibRawWrapper::~LibRawWrapper()
{
  this->processor_->recycle();
  delete this->processor_;
}
