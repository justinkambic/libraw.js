/*
 * libraw.js - node wrapper for LibRaw
 * Copyright (C) 2020-2021  Justin Kambic
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
 * USA
 *
 * Direct further questions to justinkambic.github@gmail.com.
 */

#include <napi.h>
#include "async_libraw_wrapper.h"
#include "libraw/libraw.h"
#include "wraptypes.h"

bool ProcessOption(Napi::Object *options, std::string key, bool defaultValue)
{
  if (
      options->IsUndefined()
      || options->IsNull()
      || !options->Has(key)
      || !options->Get(key).IsBoolean()
    )
    return defaultValue;

  return options->Get(key).ToBoolean();
}

Napi::Value Process(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  Napi::Buffer<char> buf = info[0].As<Napi::Buffer<char>>();

  if (buf.IsUndefined())
  {
    Napi::Error::New(env, "Buffer for LibRaw processor cannot be undefined").ThrowAsJavaScriptException();
    return env.Undefined();
  }
  if (buf.IsNull())
  {
    Napi::Error::New(env, "Buffer for LibRaw processor cannot be null").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  Napi::Object options = info[1].As<Napi::Object>();
  bool shouldExtractThumbnail = ProcessOption(&options, "shouldExtractThumbnail", true);
  bool shouldExtractMetadata = ProcessOption(&options, "shouldExtractMetadata", true);


  Napi::Object result = Napi::Object::New(env);

  LibRaw *lr = new LibRaw();
  lr->open_buffer(buf.Data(), buf.Length());

  if (shouldExtractThumbnail)
  {
    lr->unpack_thumb();
    libraw_data_t data = lr->imgdata;
    result.Set("thumbnail_buffer", Napi::Buffer<char>::New(env, data.thumbnail.thumb, data.thumbnail.tlength));
  }

  if (shouldExtractMetadata)
  {
    libraw_data_t data = lr->imgdata;
    result.Set("metadata", WrapLibRawData(&env, &data));
  }

  lr->recycle();
  delete lr;

  return result;
}

Napi::Object AsyncLibRawWrapper::Init(Napi::Env env, Napi::Object exports)
{
  exports.Set(Napi::String::New(env, "processRawImage"), Napi::Function::New(env, Process));

  return exports;
}
