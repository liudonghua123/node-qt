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

#include "qplaintextedit.h"
#include "../qt_v8.h"

using namespace v8;

//
// QPlainTextEditImpl()
// Extends QPlainTextEdit to implement virtual methods from QPlainTextEdit
//
class QPlainTextEditImpl : public QPlainTextEdit {
 public:
  QPlainTextEditImpl(QWidget* parent, QPlainTextEditWrap* wrapper) : QPlainTextEdit(parent) {
    this->wrapper = wrapper;
  }

 protected:
  QPlainTextEditWrap* wrapper;
  void paintEvent(QPaintEvent* e) {
    QPlainTextEdit::paintEvent(e);
    wrapper->paintEvent(e);
  }
  void mousePressEvent(QMouseEvent* e) {
    QPlainTextEdit::mousePressEvent(e);
    wrapper->mousePressEvent(e);
  }
  void mouseReleaseEvent(QMouseEvent* e) {
    QPlainTextEdit::mouseReleaseEvent(e);
    wrapper->mouseReleaseEvent(e);
  }
  void mouseMoveEvent(QMouseEvent* e) {
    QPlainTextEdit::mouseMoveEvent(e);
    wrapper->mouseMoveEvent(e);
  }
  void keyPressEvent(QKeyEvent* e) {
    QPlainTextEdit::keyPressEvent(e);
    wrapper->keyPressEvent(e);
  }
  void keyReleaseEvent(QKeyEvent* e) {
    QPlainTextEdit::keyReleaseEvent(e);
    wrapper->keyReleaseEvent(e);
  }
};


Nan::Persistent<FunctionTemplate> QPlainTextEditWrap::prototype;
Nan::Persistent<Function> QPlainTextEditWrap::constructor;

QPlainTextEditWrap::QPlainTextEditWrap(Nan::NAN_METHOD_ARGS_TYPE info) {
  QWidget* parent = NULL;
  
  if (info.Length() == 1 && qt_v8::InstanceOf(info[0], &QWidgetWrap::prototype)) {
    QWidgetWrap* widgetWrapper = ObjectWrap::Unwrap<QWidgetWrap>(
        info[0]->ToObject());
    parent = widgetWrapper->GetWrapped();
  }
  
  q_ = new QPlainTextEditImpl(parent, this);
}

QPlainTextEditWrap::~QPlainTextEditWrap() {
  delete q_;
}

NAN_MODULE_INIT(QPlainTextEditWrap::Initialize) {
  // Prepare constructor template
  Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
  tpl->Inherit(Nan::New(QWidgetWrap::prototype));
  tpl->SetClassName(Nan::New("QPlainTextEdit").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);  

  // Prototype
  Nan::SetPrototypeMethod(tpl, "toPlainText", ToPlainText);
  Nan::SetPrototypeMethod(tpl, "setPlainText", SetPlainText);

  prototype.Reset(tpl);
  Local<Function> function = Nan::GetFunction(tpl).ToLocalChecked();
  constructor.Reset(function);
  Nan::Set(target, Nan::New("QPlainTextEdit").ToLocalChecked(), function);
}

NAN_METHOD(QPlainTextEditWrap::New) {
  QPlainTextEditWrap* w = new QPlainTextEditWrap(info);
  w->Wrap(info.This());
}

NAN_METHOD(QPlainTextEditWrap::ToPlainText) {
  QPlainTextEditWrap* w = ObjectWrap::Unwrap<QPlainTextEditWrap>(info.This());
  QPlainTextEdit* q = w->GetWrapped();

  info.GetReturnValue().Set(qt_v8::FromQString(q->toPlainText()));
}

NAN_METHOD(QPlainTextEditWrap::SetPlainText) {
  QPlainTextEditWrap* w = ObjectWrap::Unwrap<QPlainTextEditWrap>(info.This());
  QPlainTextEdit* q = w->GetWrapped();

  if (info[0]->IsString()) {
    q->setPlainText(qt_v8::ToQString(info[0]->ToString()));
  }
  else {
    Nan::ThrowError(Exception::TypeError(
      Nan::New("QPlainTextEdit::SetPlainText: bad argument").ToLocalChecked()));
  }

  info.GetReturnValue().Set(Nan::Undefined());
}
