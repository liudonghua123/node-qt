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

#include "qlabel.h"
#include "../qt_v8.h"

using namespace v8;

//
// QLabelImpl()
// Extends QLabel to implement virtual methods from QLabel
//
class QLabelImpl : public QLabel {
 public:
  QLabelImpl(QString text, QWidget* parent, QLabelWrap* wrapper) : QLabel(text, parent) {
    this->wrapper = wrapper;
  }

 protected:
  QLabelWrap* wrapper;
  void paintEvent(QPaintEvent* e) {
    QLabel::paintEvent(e);
    wrapper->paintEvent(e);
  }
  void mousePressEvent(QMouseEvent* e) {
    QLabel::mousePressEvent(e);
    wrapper->mousePressEvent(e);
  }
  void mouseReleaseEvent(QMouseEvent* e) {
    QLabel::mouseReleaseEvent(e);
    wrapper->mouseReleaseEvent(e);
  }
  void mouseMoveEvent(QMouseEvent* e) {
    QLabel::mouseMoveEvent(e);
    wrapper->mouseMoveEvent(e);
  }
  void keyPressEvent(QKeyEvent* e) {
    QLabel::keyPressEvent(e);
    wrapper->keyPressEvent(e);
  }
  void keyReleaseEvent(QKeyEvent* e) {
    QLabel::keyReleaseEvent(e);
    wrapper->keyReleaseEvent(e);
  }
};


Nan::Persistent<FunctionTemplate> QLabelWrap::prototype;
Nan::Persistent<Function> QLabelWrap::constructor;

QLabelWrap::QLabelWrap(Nan::NAN_METHOD_ARGS_TYPE info) {
  if (info[0]->IsString() && info[1]->IsObject()) {
    QString text = qt_v8::ToQString(info[0]->ToString());

    QString widgetConstructor = 
        qt_v8::ToQString(info[1]->ToObject()->GetConstructorName());

    if (widgetConstructor != "QWidget") {
      Nan::ThrowError(Exception::TypeError(
        Nan::New("QLabel::QLabel: bad argument").ToLocalChecked()));
    }

    QWidgetWrap* widgetWrapper = ObjectWrap::Unwrap<QWidgetWrap>(
        info[1]->ToObject());
    QWidget* widget = widgetWrapper->GetWrapped();

    q_ = new QLabelImpl(text, widget, this);
  }
  else {
    Nan::ThrowError(Exception::TypeError(
      Nan::New("QLabel::QLabel: bad argument").ToLocalChecked()));
  }
}

QLabelWrap::~QLabelWrap() {
  delete q_;
}

NAN_MODULE_INIT(QLabelWrap::Initialize) {
  // Prepare constructor template
  Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
  tpl->Inherit(Nan::New(QWidgetWrap::prototype));
  tpl->SetClassName(Nan::New("QLabel").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);  

  // Prototype
  Nan::SetPrototypeMethod(tpl, "setText", SetText);

  prototype.Reset(tpl);
  Local<Function> function = Nan::GetFunction(tpl).ToLocalChecked();
  constructor.Reset(function);
  Nan::Set(target, Nan::New("QLabel").ToLocalChecked(), function);
}

NAN_METHOD(QLabelWrap::New) {
  QLabelWrap* w = new QLabelWrap(info);
  w->Wrap(info.This());
}

NAN_METHOD(QLabelWrap::SetText) {
  QLabelWrap* w = ObjectWrap::Unwrap<QLabelWrap>(info.This());
  QLabel* q = w->GetWrapped();

  if (info[0]->IsString()) {
    q->setText(qt_v8::ToQString(info[0]->ToString()));
  }
  else {
    Nan::ThrowError(Exception::TypeError(
      Nan::New("QLabel::SetText: bad argument").ToLocalChecked()));
  }

  info.GetReturnValue().Set(Nan::Undefined());
}
