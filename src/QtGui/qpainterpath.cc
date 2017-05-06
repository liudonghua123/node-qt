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

#include "../QtCore/qpointf.h"
#include "qpainterpath.h"
#include "../qt_v8.h"

using namespace v8;

Nan::Persistent<Function> QPainterPathWrap::constructor;

// Supported implementations:
//   QPainterPath ( ??? )
QPainterPathWrap::QPainterPathWrap(Nan::NAN_METHOD_ARGS_TYPE info) {
  q_ = new QPainterPath();
}

QPainterPathWrap::~QPainterPathWrap() {
  delete q_;
}

void QPainterPathWrap::Initialize(Handle<Object> target) {
  // Prepare constructor template
  Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("QPainterPath").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);  

  // Prototype
  Nan::SetPrototypeMethod(tpl, "moveTo", MoveTo);
  Nan::SetPrototypeMethod(tpl, "lineTo", LineTo);
  Nan::SetPrototypeMethod(tpl, "currentPosition", CurrentPosition);
  Nan::SetPrototypeMethod(tpl, "closeSubpath", CloseSubpath);

  Local<Function> constructorFunction = Nan::GetFunction(tpl).ToLocalChecked();
  constructor.Reset(constructorFunction);
  Nan::Set(target, Nan::New("QPainterPath").ToLocalChecked(), constructorFunction);
}

NAN_METHOD(QPainterPathWrap::New) {
  QPainterPathWrap* w = new QPainterPathWrap(info);
  w->Wrap(info.This());
}

// Supported versions:
//   moveTo( QPointF() )
NAN_METHOD(QPainterPathWrap::MoveTo) {
  QPainterPathWrap* w = ObjectWrap::Unwrap<QPainterPathWrap>(info.This());
  QPainterPath* q = w->GetWrapped();

  QString arg0_constructor;
  if (info[0]->IsObject()) {
    arg0_constructor = 
        qt_v8::ToQString(info[0]->ToObject()->GetConstructorName());
  }
  
  if (arg0_constructor != "QPointF")
    return Nan::ThrowError(Exception::TypeError(
      Nan::New<String>("QPainterPathWrap::MoveTo: argument not recognized").ToLocalChecked()));

  // moveTo( QPointF point )
  QPointFWrap* pointf_wrap = ObjectWrap::Unwrap<QPointFWrap>(
      info[0]->ToObject());
  QPointF* pointf = pointf_wrap->GetWrapped();

  q->moveTo(*pointf);

  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(QPainterPathWrap::CurrentPosition) {
  QPainterPathWrap* w = ObjectWrap::Unwrap<QPainterPathWrap>(info.This());
  QPainterPath* q = w->GetWrapped();

  info.GetReturnValue().Set(QPointFWrap::NewInstance(q->currentPosition()));
}

// Supported versions:
//   lineTo( QPointF() )
NAN_METHOD(QPainterPathWrap::LineTo) {
  QPainterPathWrap* w = ObjectWrap::Unwrap<QPainterPathWrap>(info.This());
  QPainterPath* q = w->GetWrapped();

  QString arg0_constructor;
  if (info[0]->IsObject()) {
    arg0_constructor = 
        qt_v8::ToQString(info[0]->ToObject()->GetConstructorName());
  }
  
  if (arg0_constructor != "QPointF")
    return Nan::ThrowError(Exception::TypeError(
      Nan::New<String>("QPainterPathWrap::MoveTo: argument not recognized").ToLocalChecked()));

  // lineTo( QPointF point )
  QPointFWrap* pointf_wrap = ObjectWrap::Unwrap<QPointFWrap>(
      info[0]->ToObject());
  QPointF* pointf = pointf_wrap->GetWrapped();

  q->lineTo(*pointf);

  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(QPainterPathWrap::CloseSubpath) {
  QPainterPathWrap* w = ObjectWrap::Unwrap<QPainterPathWrap>(info.This());
  QPainterPath* q = w->GetWrapped();

  q->closeSubpath();
  
  info.GetReturnValue().Set(Nan::Undefined());
}
