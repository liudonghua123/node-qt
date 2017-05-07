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

#include "qmatrix.h"
#include "../qt_v8.h"

using namespace v8;

Nan::Persistent<Function> QMatrixWrap::constructor;

// Supported implementations:
//   QMatrix ( )
//   QMatrix ( qreal m11, qreal m12, qreal m21, qreal m22, qreal dx, qreal dy )
//   QMatrix ( QMatrix matrix )
QMatrixWrap::QMatrixWrap(Nan::NAN_METHOD_ARGS_TYPE info) : q_(NULL) {
  if (info.Length() == 0) {
    // QMatrix ( )

    q_ = new QMatrix;
  } else if (info[0]->IsObject()) {
    // QMatrix ( QMatrix matrix )

    QString arg0_constructor = 
        qt_v8::ToQString(info[0]->ToObject()->GetConstructorName());

    if (arg0_constructor != "QMatrix")
      Nan::ThrowError(Exception::TypeError(
        Nan::New("QMatrix::QMatrix: bad argument").ToLocalChecked()));

    // Unwrap obj
    QMatrixWrap* q_wrap = ObjectWrap::Unwrap<QMatrixWrap>(
        info[0]->ToObject());
    QMatrix* q = q_wrap->GetWrapped();

    q_ = new QMatrix(*q);
  } else if (info.Length() == 6) {
    // QMatrix(qreal m11, qreal m12, qreal m21, qreal m22, qreal dx, qreal dy)

    q_ = new QMatrix(info[0]->NumberValue(), info[1]->NumberValue(),
                     info[2]->NumberValue(), info[3]->NumberValue(),
                     info[4]->NumberValue(), info[5]->NumberValue());
  }
}

QMatrixWrap::~QMatrixWrap() {
  delete q_;
}

void QMatrixWrap::Initialize(Handle<Object> target) {
  // Prepare constructor template
  Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("QMatrix").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);  

  // Prototype
  Nan::SetPrototypeMethod(tpl, "m11", M11);
  Nan::SetPrototypeMethod(tpl, "m12", M12);
  Nan::SetPrototypeMethod(tpl, "m21", M21);
  Nan::SetPrototypeMethod(tpl, "m22", M22);
  Nan::SetPrototypeMethod(tpl, "dx", Dx);
  Nan::SetPrototypeMethod(tpl, "dy", Dy);
  Nan::SetPrototypeMethod(tpl, "translate", Translate);
  Nan::SetPrototypeMethod(tpl, "scale", Scale);

  Local<Function> constructorFunction = Nan::GetFunction(tpl).ToLocalChecked();
  constructor.Reset(constructorFunction);
  Nan::Set(target, Nan::New("QMatrix").ToLocalChecked(), constructorFunction);
}

NAN_METHOD(QMatrixWrap::New) {
  QMatrixWrap* w = new QMatrixWrap(info);
  w->Wrap(info.This());
}

Handle<Value> QMatrixWrap::NewInstance(QMatrix q) {
  Nan::EscapableHandleScope scope;
  
  Local<Object> instance = Nan::NewInstance(Nan::New(constructor), 0, NULL).ToLocalChecked();
  QMatrixWrap* w = node::ObjectWrap::Unwrap<QMatrixWrap>(instance);
  w->SetWrapped(q);

  return scope.Escape(instance);
}

NAN_METHOD(QMatrixWrap::M11) {
  QMatrixWrap* w = ObjectWrap::Unwrap<QMatrixWrap>(info.This());
  QMatrix* q = w->GetWrapped();

  info.GetReturnValue().Set(Nan::New(q->m11()));
}

NAN_METHOD(QMatrixWrap::M12) {
  QMatrixWrap* w = ObjectWrap::Unwrap<QMatrixWrap>(info.This());
  QMatrix* q = w->GetWrapped();

  info.GetReturnValue().Set(Nan::New(q->m12()));
}

NAN_METHOD(QMatrixWrap::M21) {
  QMatrixWrap* w = ObjectWrap::Unwrap<QMatrixWrap>(info.This());
  QMatrix* q = w->GetWrapped();

  info.GetReturnValue().Set(Nan::New(q->m21()));
}

NAN_METHOD(QMatrixWrap::M22) {
  QMatrixWrap* w = ObjectWrap::Unwrap<QMatrixWrap>(info.This());
  QMatrix* q = w->GetWrapped();

  info.GetReturnValue().Set(Nan::New(q->m22()));
}

NAN_METHOD(QMatrixWrap::Dx) {
  QMatrixWrap* w = ObjectWrap::Unwrap<QMatrixWrap>(info.This());
  QMatrix* q = w->GetWrapped();

  info.GetReturnValue().Set(Nan::New(q->dx()));
}

NAN_METHOD(QMatrixWrap::Dy) {
  QMatrixWrap* w = ObjectWrap::Unwrap<QMatrixWrap>(info.This());
  QMatrix* q = w->GetWrapped();

  info.GetReturnValue().Set(Nan::New(q->dy()));
}

NAN_METHOD(QMatrixWrap::Translate) {
  QMatrixWrap* w = ObjectWrap::Unwrap<QMatrixWrap>(info.This());
  QMatrix* q = w->GetWrapped();

  q->translate(info[0]->NumberValue(), info[1]->NumberValue());

  info.GetReturnValue().Set(info.This());
}

NAN_METHOD(QMatrixWrap::Scale) {
  QMatrixWrap* w = ObjectWrap::Unwrap<QMatrixWrap>(info.This());
  QMatrix* q = w->GetWrapped();

  q->scale(info[0]->NumberValue(), info[1]->NumberValue());

  info.GetReturnValue().Set(info.This());
}
