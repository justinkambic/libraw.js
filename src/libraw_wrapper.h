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
    Napi::Value ExtractTiff(const Napi::CallbackInfo &info);
    void RecycleDatastream(const Napi::CallbackInfo& info);
    void Recycle(const Napi::CallbackInfo& info);
  private:
    LibRaw* processor_;
};