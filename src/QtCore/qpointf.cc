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

#include "qpointf.h"

using namespace v8;

Nan::Persistent<Function> QPointFWrap::constructor;

// Supported implementations:
//   QPointF (qreal x, qreal y)
QPointFWrap::QPointFWrap(Nan::NAN_METHOD_ARGS_TYPE info) : q_(NULL) {
  if (info[0]->IsNumber() && info[1]->IsNumber()) {
    q_ = new QPointF(info[0]->NumberValue(), info[1]->NumberValue());
  } else {
    q_ = new QPointF;
  }
}

QPointFWrap::~QPointFWrap() {
  delete q_;
}

NAN_MODULE_INIT(QPointFWrap::Initialize) {
  // Prepare constructor template
  Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("QPointF").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);  
  
  // Prototype
  Nan::SetPrototypeMethod(tpl, "x", X);
  Nan::SetPrototypeMethod(tpl, "y", Y);
  Nan::SetPrototypeMethod(tpl, "isNull", IsNull);
  
  Local<Function> constructorFunction = Nan::GetFunction(tpl).ToLocalChecked();
  constructor.Reset(constructorFunction);
  Nan::Set(target, Nan::New("QPointF").ToLocalChecked(), constructorFunction);
}

NAN_METHOD(QPointFWrap::New) {
  Nan::HandleScope scope;
  
  QPointFWrap* w = new QPointFWrap(info);
  w->Wrap(info.This());
}

Handle<Value> QPointFWrap::NewInstance(QPointF q) {
  Nan::EscapableHandleScope scope;
  
  Local<Object> instance = Nan::NewInstance(Nan::New(constructor), 0, NULL).ToLocalChecked();
  QPointFWrap* w = ObjectWrap::Unwrap<QPointFWrap>(instance);
  w->SetWrapped(q);
  
  return scope.Escape(instance);
}

NAN_METHOD(QPointFWrap::X) {
  QPointFWrap* w = ObjectWrap::Unwrap<QPointFWrap>(info.This());
  QPointF* q = w->GetWrapped();
  
  info.GetReturnValue().Set(Nan::New<Number>(q->x()));
}

NAN_METHOD(QPointFWrap::Y) {
  QPointFWrap* w = ObjectWrap::Unwrap<QPointFWrap>(info.This());
  QPointF* q = w->GetWrapped();
  
  info.GetReturnValue().Set(Nan::New<Number>(q->y()));
}

NAN_METHOD(QPointFWrap::IsNull) {
  QPointFWrap* w = ObjectWrap::Unwrap<QPointFWrap>(info.This());
  QPointF* q = w->GetWrapped();
  
  info.GetReturnValue().Set(Nan::New<Boolean>(q->isNull()));
}
