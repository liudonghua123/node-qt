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
#include <QWidget>

//
// QWidgetImpl()
// Extends QWidget to implement virtual methods from QWidget
//
class QWidgetImpl : public QWidget {
 public:
  QWidgetImpl(QWidgetImpl* parent);
  ~QWidgetImpl();  
  Nan::Persistent<v8::Function> paintEventCallback_;
  Nan::Persistent<v8::Function> mousePressCallback_;
  Nan::Persistent<v8::Function> mouseReleaseCallback_;
  Nan::Persistent<v8::Function> mouseMoveCallback_;
  Nan::Persistent<v8::Function> keyPressCallback_;
  Nan::Persistent<v8::Function> keyReleaseCallback_;

 private:
  void paintEvent(QPaintEvent* e);
  void mousePressEvent(QMouseEvent* e);
  void mouseReleaseEvent(QMouseEvent* e);
  void mouseMoveEvent(QMouseEvent* e);
  void keyPressEvent(QKeyEvent* e);
  void keyReleaseEvent(QKeyEvent* e);
};

//
// QWidgetWrap()
//
class QWidgetWrap : public node::ObjectWrap {
 public:
  static void Initialize(v8::Handle<v8::Object> target);
  QWidgetImpl* GetWrapped() const { return q_; };

 private:
  QWidgetWrap(QWidgetImpl* parent);
  ~QWidgetWrap();
  static Nan::Persistent<v8::Function> constructor;
  static NAN_METHOD(New);

  // Wrapped methods
  static NAN_METHOD(Resize);
  static NAN_METHOD(Show);
  static NAN_METHOD(Close);
  static NAN_METHOD(Size);
  static NAN_METHOD(Width);
  static NAN_METHOD(Height);
  static NAN_METHOD(ObjectName);
  static NAN_METHOD(SetObjectName);
  static NAN_METHOD(Parent);
  static NAN_METHOD(Update);
  static NAN_METHOD(SetMouseTracking);
  static NAN_METHOD(HasMouseTracking);
  static NAN_METHOD(SetFocusPolicy);
  static NAN_METHOD(Move);
  static NAN_METHOD(X);
  static NAN_METHOD(Y);

  // QUIRK
  // Event binding. These functions bind implemented event handlers above
  // to the given callbacks. This is necessary as in Qt such handlers
  // are virtual and we can't dynamically implement them from JS
  static NAN_METHOD(PaintEvent);
  static NAN_METHOD(MousePressEvent);
  static NAN_METHOD(MouseReleaseEvent);
  static NAN_METHOD(MouseMoveEvent);
  static NAN_METHOD(KeyPressEvent);
  static NAN_METHOD(KeyReleaseEvent);

  // Wrapped object
  QWidgetImpl* q_;
};
