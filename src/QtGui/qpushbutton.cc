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

#include "qpushbutton.h"
#include "qwidget.h"
#include "../qt_v8.h"

using namespace v8;

Nan::Persistent<Function> QPushButtonWrap::constructor;

QPushButtonWrap::QPushButtonWrap(Nan::NAN_METHOD_ARGS_TYPE info) {
  if (info[0]->IsString() && info[1]->IsObject()) {
    QString text = qt_v8::ToQString(info[0]->ToString());
    
    QString widgetConstructor = 
        qt_v8::ToQString(info[1]->ToObject()->GetConstructorName());
    
    if (widgetConstructor != "QWidget") {
      Nan::ThrowError(Exception::TypeError(
        Nan::New("QPushButton::QPushButton: bad argument").ToLocalChecked()));
    }
    
    QWidgetWrap* widgetWrapper = ObjectWrap::Unwrap<QWidgetWrap>(
        info[1]->ToObject());
    QWidget* widget = widgetWrapper->GetWrapped();
    
    q_ = new QPushButton(text, widget);
  }
  else {
    Nan::ThrowError(Exception::TypeError(
      Nan::New("QPushButton::QPushButton: bad argument").ToLocalChecked()));
  }
}

QPushButtonWrap::~QPushButtonWrap() {
  delete q_;
}

void QPushButtonWrap::Initialize(Handle<Object> target) {
  // Prepare constructor template
  Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("QPushButton").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);  

  // Prototype
  Nan::SetPrototypeMethod(tpl, "setText", SetText);

  Local<Function> constructorFunction = Nan::GetFunction(tpl).ToLocalChecked();
  constructor.Reset(constructorFunction);
  Nan::Set(target, Nan::New("QPushButton").ToLocalChecked(), constructorFunction);
}

NAN_METHOD(QPushButtonWrap::New) {
  QPushButtonWrap* w = new QPushButtonWrap(info);
  w->Wrap(info.This());
}

NAN_METHOD(QPushButtonWrap::SetText) {
  QPushButtonWrap* w = ObjectWrap::Unwrap<QPushButtonWrap>(info.This());
  QPushButton* q = w->GetWrapped();
  
  if (info[0]->IsString()) {
    q->setText(qt_v8::ToQString(info[0]->ToString()));
  }
  else {
    Nan::ThrowError(Exception::TypeError(
      Nan::New("QPushButton::QPushButton: bad argument").ToLocalChecked()));
  }

  info.GetReturnValue().Set(Nan::Undefined());
}
