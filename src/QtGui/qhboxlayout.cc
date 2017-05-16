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

#include "qhboxlayout.h"
#include "../qt_v8.h"
#include "qwidget.h"

using namespace v8;

Nan::Persistent<FunctionTemplate> QHBoxLayoutWrap::prototype;
Nan::Persistent<Function> QHBoxLayoutWrap::constructor;

QHBoxLayoutWrap::QHBoxLayoutWrap(Nan::NAN_METHOD_ARGS_TYPE info) {
  if (info.Length() == 1 && qt_v8::InstanceOf(info[0], &QWidgetWrap::prototype)) {
    QWidgetWrap* widgetWrapper = ObjectWrap::Unwrap<QWidgetWrap>(
        info[0]->ToObject());

    q_ = new QHBoxLayout(widgetWrapper->GetWrapped());
  }
  else {
    Nan::ThrowError(Exception::TypeError(
      Nan::New("QHBoxLayout::QHBoxLayout: bad argument").ToLocalChecked()));
  }
}

QHBoxLayoutWrap::~QHBoxLayoutWrap() {
  delete q_;
}

NAN_MODULE_INIT(QHBoxLayoutWrap::Initialize) {
  // Prepare constructor template
  Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("QHBoxLayout").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);  

  // Prototype
  Nan::SetPrototypeMethod(tpl, "addWidget", AddWidget);

  prototype.Reset(tpl);
  Local<Function> function = Nan::GetFunction(tpl).ToLocalChecked();
  constructor.Reset(function);
  Nan::Set(target, Nan::New("QHBoxLayout").ToLocalChecked(), function);
}

NAN_METHOD(QHBoxLayoutWrap::New) {
  QHBoxLayoutWrap* w = new QHBoxLayoutWrap(info);
  w->Wrap(info.This());
}

NAN_METHOD(QHBoxLayoutWrap::AddWidget) {
  QHBoxLayoutWrap* w = node::ObjectWrap::Unwrap<QHBoxLayoutWrap>(info.This());
  QHBoxLayout* q = w->GetWrapped();
  
  if (info.Length() == 1 && qt_v8::InstanceOf(info[0], &QWidgetWrap::prototype)) {
    QWidgetWrap* widgetWrapper = ObjectWrap::Unwrap<QWidgetWrap>(
        info[0]->ToObject());
  
    q->addWidget(widgetWrapper->GetWrapped());
  }
  else {
    Nan::ThrowError(Exception::TypeError(
      Nan::New("QHBoxLayout::AddWidget: bad argument").ToLocalChecked()));
  }

  info.GetReturnValue().Set(Nan::Undefined());
}