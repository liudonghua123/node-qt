// Copyright (c) 2012, Artur Adib
// All rights reserved.
//
// Author(s): Artur Adib <aadib@mozilla.com>
//
// You may use this file under the terms of the New BSD license as follows:
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of Artur Adib nor the
//       names of contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
// ARE DISCLAIMED. IN NO EVENT SHALL ARTUR ADIB BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF 
// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "qimage.h"
#include "../qt_v8.h"

using namespace v8;

Nan::Persistent<FunctionTemplate> QImageWrap::prototype;
Nan::Persistent<Function> QImageWrap::constructor;

// Supported implementations:
//   QImage ( )
//   QImage ( QString filename )
QImageWrap::QImageWrap(Nan::NAN_METHOD_ARGS_TYPE info) {
  if (info[0]->IsString()) {
    // QImage ( QString filename ) 
    q_ = new QImage(qt_v8::ToQString(info[0]->ToString()));
    return;
  }

  // QImage ( )
  q_ = new QImage(qt_v8::ToQString(info[0]->ToString()));  
}

QImageWrap::~QImageWrap() {
  delete q_;
}

NAN_MODULE_INIT(QImageWrap::Initialize) {
  // Prepare constructor template
  Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("QImage").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);  

  // Prototype
  Nan::SetPrototypeMethod(tpl, "isNull", IsNull);

  prototype.Reset(tpl);
  Local<Function> function = Nan::GetFunction(tpl).ToLocalChecked();
  constructor.Reset(function);
  Nan::Set(target, Nan::New("QImage").ToLocalChecked(), function);
}

NAN_METHOD(QImageWrap::New) {
  QImageWrap* w = new QImageWrap(info);
  w->Wrap(info.This());
}

NAN_METHOD(QImageWrap::IsNull) {
  QImageWrap* w = ObjectWrap::Unwrap<QImageWrap>(info.This());
  QImage* q = w->GetWrapped();

  info.GetReturnValue().Set(Nan::New(q->isNull()));
}
