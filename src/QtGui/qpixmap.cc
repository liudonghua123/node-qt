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

#include "../qt_v8.h"
#include "qpixmap.h"
#include "qcolor.h"

using namespace v8;

Nan::Persistent<Function> QPixmapWrap::constructor;

QPixmapWrap::QPixmapWrap(int width, int height) : q_(NULL) {
  q_ = new QPixmap(width, height);
}
QPixmapWrap::~QPixmapWrap() {
  delete q_;
}

void QPixmapWrap::Initialize(Handle<Object> target) {
  // Prepare constructor template
  Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("QPixmap").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);  

  // Prototype
  Nan::SetPrototypeMethod(tpl, "width", Width);
  Nan::SetPrototypeMethod(tpl, "height", Height);
  Nan::SetPrototypeMethod(tpl, "save", Save);
  Nan::SetPrototypeMethod(tpl, "fill", Fill);

  Local<Function> constructorFunction = Nan::GetFunction(tpl).ToLocalChecked();
  constructor.Reset(constructorFunction);
  Nan::Set(target, Nan::New("QPixmap").ToLocalChecked(), constructorFunction);
}

NAN_METHOD(QPixmapWrap::New) {
  QPixmapWrap* w = new QPixmapWrap(info[0]->IntegerValue(), 
      info[1]->IntegerValue());
  w->Wrap(info.This());
}

Handle<Value> QPixmapWrap::NewInstance(QPixmap q) {
  Nan::EscapableHandleScope scope;
  
  Local<Object> instance = Nan::NewInstance(Nan::New(constructor), 0, NULL).ToLocalChecked();
  QPixmapWrap* w = node::ObjectWrap::Unwrap<QPixmapWrap>(instance);
  w->SetWrapped(q);

  return scope.Escape(instance);
}

NAN_METHOD(QPixmapWrap::Width) {
  QPixmapWrap* w = ObjectWrap::Unwrap<QPixmapWrap>(info.This());
  QPixmap* q = w->GetWrapped();

  info.GetReturnValue().Set(Nan::New(q->width()));
}

NAN_METHOD(QPixmapWrap::Height) {
  QPixmapWrap* w = ObjectWrap::Unwrap<QPixmapWrap>(info.This());
  QPixmap* q = w->GetWrapped();

  info.GetReturnValue().Set(Nan::New(q->height()));
}

NAN_METHOD(QPixmapWrap::Save) {
  QPixmapWrap* w = ObjectWrap::Unwrap<QPixmapWrap>(info.This());
  QPixmap* q = w->GetWrapped();

  QString file(qt_v8::ToQString(info[0]->ToString()));

  info.GetReturnValue().Set(Nan::New(q->save(file)));
}

// Supports:
//    fill()
//    fill(QColor color)
NAN_METHOD(QPixmapWrap::Fill) {
  QPixmapWrap* w = ObjectWrap::Unwrap<QPixmapWrap>(info.This());
  QPixmap* q = w->GetWrapped();

  if (info[0]->IsObject()) {
    // Unwrap QColor
    QColorWrap* color_wrap = ObjectWrap::Unwrap<QColorWrap>(
        info[0]->ToObject());
    QColor* color = color_wrap->GetWrapped();

    q->fill(*color);
  } else {
    q->fill();
  }

  info.GetReturnValue().Set(Nan::Undefined());
}
