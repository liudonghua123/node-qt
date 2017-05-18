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

#include "qboxlayout.h"
#include "../qt_v8.h"
#include "qwidget.h"

using namespace v8;

Nan::Persistent<FunctionTemplate> QBoxLayoutWrap::prototype;
Nan::Persistent<Function> QBoxLayoutWrap::constructor;

QBoxLayoutWrap::QBoxLayoutWrap(Nan::NAN_METHOD_ARGS_TYPE info) {
  if (info.Length() == 2 && info[0]->IsNumber() && qt_v8::InstanceOf(info[1], &QWidgetWrap::prototype)) {
    QWidgetWrap* widgetWrapper = ObjectWrap::Unwrap<QWidgetWrap>(
        info[1]->ToObject());

    q_ = new QBoxLayout((QBoxLayout::Direction) info[0]->NumberValue(), widgetWrapper->GetWrapped());
  }
  else {
    Nan::ThrowError(Exception::TypeError(
      Nan::New("QBoxLayout::QBoxLayout: bad argument").ToLocalChecked()));
  }
}

QBoxLayoutWrap::~QBoxLayoutWrap() {
  delete q_;
}

NAN_MODULE_INIT(QBoxLayoutWrap::Initialize) {
  // Prepare constructor template
  Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("QBoxLayout").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);  

  // Prototype
  Nan::SetPrototypeMethod(tpl, "addWidget", AddWidget);
  Nan::SetPrototypeMethod(tpl, "setSpacing", SetSpacing);
  Nan::SetPrototypeMethod(tpl, "setContentsMargins", SetContentsMargins);

  prototype.Reset(tpl);
  Local<Function> function = Nan::GetFunction(tpl).ToLocalChecked();
  constructor.Reset(function);
  Nan::Set(target, Nan::New("QBoxLayout").ToLocalChecked(), function);
}

NAN_METHOD(QBoxLayoutWrap::New) {
  QBoxLayoutWrap* w = new QBoxLayoutWrap(info);
  w->Wrap(info.This());
}

// Supported implementations:
//    addWidget (QWidget widget)
//    addWidget (QWidget widget, int stretch)
NAN_METHOD(QBoxLayoutWrap::AddWidget) {
  QBoxLayoutWrap* w = node::ObjectWrap::Unwrap<QBoxLayoutWrap>(info.This());
  QBoxLayout* q = w->GetWrapped();
  
  if (info.Length() >= 1 && qt_v8::InstanceOf(info[0], &QWidgetWrap::prototype)) {
    QWidgetWrap* widgetWrapper = ObjectWrap::Unwrap<QWidgetWrap>(
        info[0]->ToObject());
  
    if (info.Length() == 2 && info[1]->IsNumber()) {
      q->addWidget(widgetWrapper->GetWrapped(), info[1]->NumberValue());
    }
    else {
      q->addWidget(widgetWrapper->GetWrapped());
    }
  }
  else {
    Nan::ThrowError(Exception::TypeError(
      Nan::New("QBoxLayout::AddWidget: bad argument").ToLocalChecked()));
  }

  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(QBoxLayoutWrap::SetSpacing) {
  if (info.Length() == 1 && info[0]->IsNumber()) {
    QBoxLayoutWrap* w = node::ObjectWrap::Unwrap<QBoxLayoutWrap>(info.This());
    QBoxLayout* q = w->GetWrapped();
    
    q->setSpacing(info[0]->NumberValue());
  }
  else {
    Nan::ThrowError(Exception::TypeError(
      Nan::New("QBoxLayout::AddWidget: bad argument").ToLocalChecked()));
  }
  
  info.GetReturnValue().Set(Nan::Undefined());
}

// Supported implementations:
//    setContentsMargins (int left, int top, int right, int bottom)
NAN_METHOD(QBoxLayoutWrap::SetContentsMargins) {
  QBoxLayoutWrap* w = node::ObjectWrap::Unwrap<QBoxLayoutWrap>(info.This());
  QBoxLayout* q = w->GetWrapped();
  
  q->setContentsMargins(info[0]->NumberValue(), info[1]->NumberValue(), info[2]->NumberValue(), info[3]->NumberValue());

  info.GetReturnValue().Set(Nan::Undefined());
}