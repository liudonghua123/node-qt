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

#pragma once

#include <node.h>
#include <nan.h>
#include <QPushButton>
#include "qwidget.h"
#include "qwidgetwrapbase.h"

class QPushButtonImpl;

class QPushButtonWrap : public QWidgetWrapBase {
 public:
  static Nan::Persistent<v8::FunctionTemplate> prototype;
  static NAN_MODULE_INIT(Initialize);
  QPushButtonImpl* GetWrapped() const { return q_; };

 private:
  static Nan::Persistent<v8::Function> constructor;
  QPushButtonWrap(Nan::NAN_METHOD_ARGS_TYPE info);
  ~QPushButtonWrap();
  static NAN_METHOD(New);

  // Wrapped methods
  static NAN_METHOD(SetText);

  // Wrapped object
  QPushButtonImpl* q_;
};

//
// QPushButtonWrap()
// Extends QPushButton to implement virtual methods from QPushButton
//
class QPushButtonImpl : public QPushButton {
 public:
  QPushButtonImpl(QString text, QWidgetImpl* parent, QPushButtonWrap* wrapper) : QPushButton(text, parent) {
    this->wrapper = wrapper;
  }

 protected:
  QPushButtonWrap* wrapper;
  void paintEvent(QPaintEvent* e) {
    QPushButton::paintEvent(e);
    wrapper->paintEvent(e);
  }
  void mousePressEvent(QMouseEvent* e) {
    QPushButton::mousePressEvent(e);
    wrapper->mousePressEvent(e);
  }
  void mouseReleaseEvent(QMouseEvent* e) {
    QPushButton::mouseReleaseEvent(e);
    wrapper->mouseReleaseEvent(e);
  }
  void mouseMoveEvent(QMouseEvent* e) {
    QPushButton::mouseMoveEvent(e);
    wrapper->mouseMoveEvent(e);
  }
  void keyPressEvent(QKeyEvent* e) {
    QPushButton::keyPressEvent(e);
    wrapper->keyPressEvent(e);
  }
  void keyReleaseEvent(QKeyEvent* e) {
    QPushButton::keyReleaseEvent(e);
    wrapper->keyReleaseEvent(e);
  }
};
