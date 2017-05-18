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

#include <node.h>
#include <nan.h>
#include "../qt_v8.h"
#include "../QtWidgets/qwidget.h"
#include "qtesteventlist.h"

using namespace v8;

Nan::Persistent<FunctionTemplate> QTestEventListWrap::prototype;
Nan::Persistent<Function> QTestEventListWrap::constructor;

QTestEventListWrap::QTestEventListWrap() {
  q_ = new QTestEventList();
}

QTestEventListWrap::~QTestEventListWrap() {
  delete q_;
}

NAN_MODULE_INIT(QTestEventListWrap::Initialize) {
  // Prepare constructor template
  Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("QTestEventList").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);  

  // Prototype
  Nan::SetPrototypeMethod(tpl, "addMouseClick", AddMouseClick);
  Nan::SetPrototypeMethod(tpl, "addKeyPress", AddKeyPress);
  Nan::SetPrototypeMethod(tpl, "simulate", Simulate);

  prototype.Reset(tpl);
  Local<Function> function = Nan::GetFunction(tpl).ToLocalChecked();
  constructor.Reset(function);
  Nan::Set(target, Nan::New("QTestEventList").ToLocalChecked(), function);
}

NAN_METHOD(QTestEventListWrap::New) {
  QTestEventListWrap* w = new QTestEventListWrap();
  w->Wrap(info.This());
}

NAN_METHOD(QTestEventListWrap::AddMouseClick) {
  QTestEventListWrap* w = ObjectWrap::Unwrap<QTestEventListWrap>(info.This());
  QTestEventList* q = w->GetWrapped();

  q->addMouseClick((Qt::MouseButton)info[0]->IntegerValue());

  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(QTestEventListWrap::AddKeyPress) {
  QTestEventListWrap* w = ObjectWrap::Unwrap<QTestEventListWrap>(info.This());
  QTestEventList* q = w->GetWrapped();

  if (info[0]->IsString()) {
    q->addKeyPress((*v8::String::Value(info[0]->ToString()))[0]);
  }
  else {
    q->addKeyPress((Qt::Key)info[0]->IntegerValue());
  }

  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(QTestEventListWrap::Simulate) {
  QTestEventListWrap* w = ObjectWrap::Unwrap<QTestEventListWrap>(info.This());
  QTestEventList* q = w->GetWrapped();

  QWidgetWrap* widget_wrap = node::ObjectWrap::Unwrap<QWidgetWrap>(
      info[0]->ToObject());
  QWidget* widget = widget_wrap->GetWrapped();

  q->simulate(widget);

  info.GetReturnValue().Set(Nan::Undefined());
}
