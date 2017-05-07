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

#include "qfont.h"
#include "../qt_v8.h"

using namespace v8;

Nan::Persistent<Function> QFontWrap::constructor;

// Supported implementations:
//   QFont ( )
//   QFont ( const QString & family, int pointSize = -1, int weight = -1, 
//     bool italic = false )
//   QFont ( QFont font )
QFontWrap::QFontWrap(Nan::NAN_METHOD_ARGS_TYPE info) : q_(NULL) {
  if (info.Length() == 0) {
    // QFont ()

    q_ = new QFont;
    return;
  }

  // QFont ( QFont font )

  if (info.Length() == 1 && info[0]->IsObject()) {
    QString arg0_constructor = 
        qt_v8::ToQString(info[0]->ToObject()->GetConstructorName());

    if (arg0_constructor != "QFont")
      Nan::ThrowError(Exception::TypeError(
        Nan::New("QFont::QFont: bad argument").ToLocalChecked()));

    // Unwrap obj
    QFontWrap* q_wrap = ObjectWrap::Unwrap<QFontWrap>(
        info[0]->ToObject());
    QFont* q = q_wrap->GetWrapped();

    q_ = new QFont(*q);
  }

  // QFont ( const QString & family, int pointSize = -1, int weight = -1, 
  //   bool italic = false )

  if (info.Length() == 1 && info[0]->IsString()) {
    q_ = new QFont(qt_v8::ToQString(info[0]->ToString()));
    return;
  }

  if (info.Length() == 2) {
    q_ = new QFont(qt_v8::ToQString(info[0]->ToString()), 
        info[1]->IntegerValue());
    return;
  }

  if (info.Length() == 3) {
    q_ = new QFont(qt_v8::ToQString(info[0]->ToString()), 
        info[1]->IntegerValue(), info[2]->IntegerValue());
    return;
  }

  if (info.Length() == 4) {
    q_ = new QFont(qt_v8::ToQString(info[0]->ToString()), 
        info[1]->IntegerValue(), info[2]->IntegerValue(),
        info[3]->BooleanValue());
    return;
  }
}

QFontWrap::~QFontWrap() {
  delete q_;
}

void QFontWrap::Initialize(Handle<Object> target) {
  // Prepare constructor template
  Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("QFont").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);  

  // Prototype
  Nan::SetPrototypeMethod(tpl, "setFamily", SetFamily);
  Nan::SetPrototypeMethod(tpl, "family", Family);
  Nan::SetPrototypeMethod(tpl, "setPixelSize", SetPixelSize);
  Nan::SetPrototypeMethod(tpl, "pixelSize", PixelSize);
  Nan::SetPrototypeMethod(tpl, "setPointSize", SetPointSize);
  Nan::SetPrototypeMethod(tpl, "pointSize", PointSize);
  Nan::SetPrototypeMethod(tpl, "setPointSizeF", SetPointSizeF);
  Nan::SetPrototypeMethod(tpl, "pointSizeF", PointSizeF);

  Local<Function> constructorFunction = Nan::GetFunction(tpl).ToLocalChecked();
  constructor.Reset(constructorFunction);
  Nan::Set(target, Nan::New("QFont").ToLocalChecked(), constructorFunction);
}

NAN_METHOD(QFontWrap::New) {
  QFontWrap* w = new QFontWrap(info);
  w->Wrap(info.This());
}

Handle<Value> QFontWrap::NewInstance(QFont q) {
  Nan::EscapableHandleScope scope;
  
  Local<Object> instance = Nan::NewInstance(Nan::New(constructor), 0, NULL).ToLocalChecked();
  QFontWrap* w = node::ObjectWrap::Unwrap<QFontWrap>(instance);
  w->SetWrapped(q);

  return scope.Escape(instance);
}

NAN_METHOD(QFontWrap::SetFamily) {
  QFontWrap* w = ObjectWrap::Unwrap<QFontWrap>(info.This());
  QFont* q = w->GetWrapped();

  q->setFamily(qt_v8::ToQString(info[0]->ToString()));

  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(QFontWrap::Family) {
  QFontWrap* w = ObjectWrap::Unwrap<QFontWrap>(info.This());
  QFont* q = w->GetWrapped();

  info.GetReturnValue().Set(qt_v8::FromQString(q->family()));
}

NAN_METHOD(QFontWrap::SetPixelSize) {
  QFontWrap* w = ObjectWrap::Unwrap<QFontWrap>(info.This());
  QFont* q = w->GetWrapped();

  q->setPixelSize(info[0]->IntegerValue());

  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(QFontWrap::PixelSize) {
  QFontWrap* w = ObjectWrap::Unwrap<QFontWrap>(info.This());
  QFont* q = w->GetWrapped();

  info.GetReturnValue().Set(Nan::New(q->pixelSize()));
}

NAN_METHOD(QFontWrap::SetPointSize) {
  QFontWrap* w = ObjectWrap::Unwrap<QFontWrap>(info.This());
  QFont* q = w->GetWrapped();

  q->setPointSize(info[0]->IntegerValue());

  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(QFontWrap::PointSize) {
  QFontWrap* w = ObjectWrap::Unwrap<QFontWrap>(info.This());
  QFont* q = w->GetWrapped();

  info.GetReturnValue().Set(Nan::New(q->pointSize()));
}

NAN_METHOD(QFontWrap::SetPointSizeF) {
  QFontWrap* w = ObjectWrap::Unwrap<QFontWrap>(info.This());
  QFont* q = w->GetWrapped();

  q->setPointSizeF(info[0]->NumberValue());

  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(QFontWrap::PointSizeF) {
  QFontWrap* w = ObjectWrap::Unwrap<QFontWrap>(info.This());
  QFont* q = w->GetWrapped();

  info.GetReturnValue().Set(Nan::New(q->pointSizeF()));
}
