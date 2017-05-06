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

#include "qcolor.h"
#include "../qt_v8.h"

using namespace v8;

Nan::Persistent<Function> QColorWrap::constructor;

// Supported implementations:
//   QColor ( int r, int g, int b, int a = 255 )
//   QColor ( QString color )
//   QColor ( QColor )
QColorWrap::QColorWrap(Nan::NAN_METHOD_ARGS_TYPE info) {
  if (info.Length() >= 3) {
    // QColor ( int r, int g, int b, int a = 255 )
    q_ = new QColor(
        info[0]->IntegerValue(), 
        info[1]->IntegerValue(),
        info[2]->IntegerValue(), 
        info[3]->IsNumber() ? info[3]->IntegerValue() : 255
    );
  } else if (info[0]->IsString()) {
    // QColor ( QString color )
    q_ = new QColor(qt_v8::ToQString(info[0]->ToString()));
  } else if (info[0]->IsObject()) {
    // QColor ( QColor color )
    QString arg0_constructor = 
        qt_v8::ToQString(info[0]->ToObject()->GetConstructorName());

    if (arg0_constructor != "QColor")
      Nan::ThrowError(Exception::TypeError(
        Nan::New<String>("QColor::QColor: bad argument").ToLocalChecked()));

    // Unwrap obj
    QColorWrap* q_wrap = ObjectWrap::Unwrap<QColorWrap>(
        info[0]->ToObject());
    QColor* q = q_wrap->GetWrapped();

    q_ = new QColor(*q);
  }
}

QColorWrap::~QColorWrap() {
  delete q_;
}

void QColorWrap::Initialize(Handle<Object> target) {
  // Prepare constructor template
  Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("QColor").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);  

  // Prototype
  Nan::SetPrototypeMethod(tpl, "red", Red);
  Nan::SetPrototypeMethod(tpl, "green", Green);
  Nan::SetPrototypeMethod(tpl, "blue", Blue);
  Nan::SetPrototypeMethod(tpl, "alpha", Alpha);
  Nan::SetPrototypeMethod(tpl, "name", Name);

  Local<Function> constructorFunction = Nan::GetFunction(tpl).ToLocalChecked();
  constructor.Reset(constructorFunction);
  Nan::Set(target, Nan::New("QColor").ToLocalChecked(), constructorFunction);
}

NAN_METHOD(QColorWrap::New) {
  QColorWrap* w = new QColorWrap(info);
  w->Wrap(info.This());
}

NAN_METHOD(QColorWrap::Red) {
  QColorWrap* w = ObjectWrap::Unwrap<QColorWrap>(info.This());
  QColor* q = w->GetWrapped();

  info.GetReturnValue().Set(Nan::New<Number>(q->red()));
}

NAN_METHOD(QColorWrap::Green) {
  QColorWrap* w = ObjectWrap::Unwrap<QColorWrap>(info.This());
  QColor* q = w->GetWrapped();

  info.GetReturnValue().Set(Nan::New<Number>(q->green()));
}

NAN_METHOD(QColorWrap::Blue) {
  QColorWrap* w = ObjectWrap::Unwrap<QColorWrap>(info.This());
  QColor* q = w->GetWrapped();

  info.GetReturnValue().Set(Nan::New<Number>(q->blue()));
}

NAN_METHOD(QColorWrap::Alpha) {
  QColorWrap* w = ObjectWrap::Unwrap<QColorWrap>(info.This());
  QColor* q = w->GetWrapped();

  info.GetReturnValue().Set(Nan::New<Number>(q->alpha()));
}

NAN_METHOD(QColorWrap::Name) {
  QColorWrap* w = ObjectWrap::Unwrap<QColorWrap>(info.This());
  QColor* q = w->GetWrapped();

  QString name = q->name();

  info.GetReturnValue().Set(qt_v8::FromQString(name));
}
