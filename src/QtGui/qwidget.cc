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
#include "../QtCore/qsize.h"
#include "qwidget.h"
#include "qmouseevent.h"
#include "qkeyevent.h"

using namespace v8;

Nan::Persistent<Function> QWidgetWrap::constructor;

//
// QWidgetImpl()
//

QWidgetImpl::QWidgetImpl(QWidgetImpl* parent) : QWidget(parent) {
}

QWidgetImpl::~QWidgetImpl() {
  delete &paintEventCallback_;
  delete &mousePressCallback_;
  delete &mouseReleaseCallback_;
  delete &mouseMoveCallback_;
  delete &keyPressCallback_;
  delete &keyReleaseCallback_;
}

void QWidgetImpl::paintEvent(QPaintEvent* e) {
  if (paintEventCallback_.IsEmpty()) {
    return;
  }

  const unsigned argc = 0;
  Handle<Value> argv[1] = {};
    
  Nan::Callback(Nan::New(paintEventCallback_)).Call(argc, argv);
}

void QWidgetImpl::mousePressEvent(QMouseEvent* e) {
  e->ignore(); // ensures event bubbles up

  if (mousePressCallback_.IsEmpty()) {
    return;
  }

  const unsigned argc = 1;
  Handle<Value> argv[argc] = {
    QMouseEventWrap::NewInstance(*e)
  };
  
  Nan::Callback(Nan::New(mousePressCallback_)).Call(argc, argv);
}

void QWidgetImpl::mouseReleaseEvent(QMouseEvent* e) {
  e->ignore(); // ensures event bubbles up

  if (mouseReleaseCallback_.IsEmpty()) {
    return;
  }

  const unsigned argc = 1;
  Handle<Value> argv[argc] = {
    QMouseEventWrap::NewInstance(*e)
  };
  
  Nan::Callback(Nan::New(mouseReleaseCallback_)).Call(argc, argv);
}

void QWidgetImpl::mouseMoveEvent(QMouseEvent* e) {
  e->ignore(); // ensures event bubbles up

  if (mouseMoveCallback_.IsEmpty()) {
    return;
  }

  const unsigned argc = 1;
  Handle<Value> argv[argc] = {
    QMouseEventWrap::NewInstance(*e)
  };
  
  Nan::Callback(Nan::New(mouseMoveCallback_)).Call(argc, argv);
}

void QWidgetImpl::keyPressEvent(QKeyEvent* e) {
  e->ignore(); // ensures event bubbles up

  if (keyPressCallback_.IsEmpty()) {
    return;
  }

  const unsigned argc = 1;
  Handle<Value> argv[argc] = {
    QKeyEventWrap::NewInstance(*e)
  };
  
  Nan::Callback(Nan::New(keyPressCallback_)).Call(argc, argv);
}

void QWidgetImpl::keyReleaseEvent(QKeyEvent* e) {
  e->ignore(); // ensures event bubbles up

  if (keyReleaseCallback_.IsEmpty()) {
    return;
  }

  const unsigned argc = 1;
  Handle<Value> argv[argc] = {
    QKeyEventWrap::NewInstance(*e)
  };
  
  Nan::Callback(Nan::New(keyReleaseCallback_)).Call(argc, argv);
}

//
// QWidgetWrap()
//

QWidgetWrap::QWidgetWrap(QWidgetImpl* parent) {
  q_ = new QWidgetImpl(parent);
}

QWidgetWrap::~QWidgetWrap() {
  delete q_;
}

void QWidgetWrap::Initialize(Handle<Object> target) {
  // Prepare constructor template
  Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("QWidget").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);  

  // Wrapped methods
  Nan::SetPrototypeMethod(tpl, "resize", Resize);
  Nan::SetPrototypeMethod(tpl, "show", Show);
  Nan::SetPrototypeMethod(tpl, "close", Close);
  Nan::SetPrototypeMethod(tpl, "size", Size);
  Nan::SetPrototypeMethod(tpl, "width", Width);
  Nan::SetPrototypeMethod(tpl, "height", Height);
  Nan::SetPrototypeMethod(tpl, "parent", Parent);
  Nan::SetPrototypeMethod(tpl, "objectName", ObjectName);
  Nan::SetPrototypeMethod(tpl, "setObjectName", SetObjectName);
  Nan::SetPrototypeMethod(tpl, "update", Update);
  Nan::SetPrototypeMethod(tpl, "hasMouseTracking", HasMouseTracking);
  Nan::SetPrototypeMethod(tpl, "setMouseTracking", SetMouseTracking);
  Nan::SetPrototypeMethod(tpl, "setFocusPolicy", SetFocusPolicy);
  Nan::SetPrototypeMethod(tpl, "move", Move);
  Nan::SetPrototypeMethod(tpl, "x", X);
  Nan::SetPrototypeMethod(tpl, "y", Y);

  // Events
  Nan::SetPrototypeMethod(tpl, "paintEvent", PaintEvent);
  Nan::SetPrototypeMethod(tpl, "mousePressEvent", MousePressEvent);
  Nan::SetPrototypeMethod(tpl, "mouseReleaseEvent", MouseReleaseEvent);
  Nan::SetPrototypeMethod(tpl, "mouseMoveEvent", MouseMoveEvent);
  Nan::SetPrototypeMethod(tpl, "keyPressEvent", KeyPressEvent);
  Nan::SetPrototypeMethod(tpl, "keyReleaseEvent", KeyReleaseEvent);

  Local<Function> constructorFunction = Nan::GetFunction(tpl).ToLocalChecked();
  constructor.Reset(constructorFunction);
  Nan::Set(target, Nan::New("QWidget").ToLocalChecked(), constructorFunction);
}

NAN_METHOD(QWidgetWrap::New) {
  QWidgetImpl* q_parent = 0;

  if (info.Length() > 0) {
    QWidgetWrap* w_parent = node::ObjectWrap::Unwrap<QWidgetWrap>(info[0]->ToObject());
    q_parent = w_parent->GetWrapped();
  }

  QWidgetWrap* w = new QWidgetWrap(q_parent);
  w->Wrap(info.This());
}

NAN_METHOD(QWidgetWrap::Resize) {
  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(info.This());
  QWidgetImpl* q = w->GetWrapped();

  q->resize(info[0]->NumberValue(), info[1]->NumberValue());

  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(QWidgetWrap::Show) {
  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(info.This());
  QWidgetImpl* q = w->GetWrapped();

  q->show();

  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(QWidgetWrap::Close) {
  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(info.This());
  QWidgetImpl* q = w->GetWrapped();

  q->close();

  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(QWidgetWrap::Size) {
  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(info.This());
  QWidgetImpl* q = w->GetWrapped();

  info.GetReturnValue().Set(QSizeWrap::NewInstance(q->size()));
}

NAN_METHOD(QWidgetWrap::Width) {
  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(info.This());
  QWidgetImpl* q = w->GetWrapped();

  info.GetReturnValue().Set(Nan::New<Integer>(q->width()));
}

NAN_METHOD(QWidgetWrap::Height) {
  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(info.This());
  QWidgetImpl* q = w->GetWrapped();

  info.GetReturnValue().Set(Nan::New<Integer>(q->height()));
}

NAN_METHOD(QWidgetWrap::ObjectName) {
  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(info.This());
  QWidgetImpl* q = w->GetWrapped();

  info.GetReturnValue().Set(qt_v8::FromQString(q->objectName()));
}

NAN_METHOD(QWidgetWrap::SetObjectName) {
  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(info.This());
  QWidgetImpl* q = w->GetWrapped();

  q->setObjectName(qt_v8::ToQString(info[0]->ToString()));

  info.GetReturnValue().Set(Nan::Undefined());
}

//
// QUIRK:
// Here: Parent() returns the parent's name
// Qt: Parent() returns QObject
// Intended mostly for sanity checks
//
NAN_METHOD(QWidgetWrap::Parent) {
  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(info.This());
  QWidgetImpl* q = w->GetWrapped();

  info.GetReturnValue().Set(qt_v8::FromQString(q->parent()->objectName()));
}

//
// PaintEvent()
// Binds a callback to Qt's event
//
NAN_METHOD(QWidgetWrap::PaintEvent) {
  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(info.This());
  QWidgetImpl* q = w->GetWrapped();

  if (info[0]->IsFunction()) {
    q->paintEventCallback_.Reset(Local<Function>::Cast(info[0]));
  }

  info.GetReturnValue().Set(Nan::Undefined());
}

//
// MousePressEvent()
// Binds a callback to Qt's event
//
NAN_METHOD(QWidgetWrap::MousePressEvent) {
  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(info.This());
  QWidgetImpl* q = w->GetWrapped();

  if (info[0]->IsFunction()) {
    q->mousePressCallback_.Reset(Local<Function>::Cast(info[0]));
  }

  info.GetReturnValue().Set(Nan::Undefined());
}

//
// MouseReleaseEvent()
// Binds a callback to Qt's event
//
NAN_METHOD(QWidgetWrap::MouseReleaseEvent) {
  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(info.This());
  QWidgetImpl* q = w->GetWrapped();

  if (info[0]->IsFunction()) {
    q->mouseReleaseCallback_.Reset(Local<Function>::Cast(info[0]));
  }

  info.GetReturnValue().Set(Nan::Undefined());
}

//
// MouseMoveEvent()
// Binds a callback to Qt's event
//
NAN_METHOD(QWidgetWrap::MouseMoveEvent) {
  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(info.This());
  QWidgetImpl* q = w->GetWrapped();

  if (info[0]->IsFunction()) {
    q->mouseMoveCallback_.Reset(Local<Function>::Cast(info[0]));
  }

  info.GetReturnValue().Set(Nan::Undefined());
}

//
// KeyPressEvent()
// Binds a callback to Qt's event
//
NAN_METHOD(QWidgetWrap::KeyPressEvent) {
  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(info.This());
  QWidgetImpl* q = w->GetWrapped();

  if (info[0]->IsFunction()) {
    q->keyPressCallback_.Reset(Local<Function>::Cast(info[0]));
  }

  info.GetReturnValue().Set(Nan::Undefined());
}

//
// KeyReleaseEvent()
// Binds a callback to Qt's event
//
NAN_METHOD(QWidgetWrap::KeyReleaseEvent) {
  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(info.This());
  QWidgetImpl* q = w->GetWrapped();

  if (info[0]->IsFunction()) {
    q->keyReleaseCallback_.Reset(Local<Function>::Cast(info[0]));
  }

  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(QWidgetWrap::Update) {
  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(info.This());
  QWidgetImpl* q = w->GetWrapped();

  q->update();

  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(QWidgetWrap::HasMouseTracking) {
  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(info.This());
  QWidgetImpl* q = w->GetWrapped();

  info.GetReturnValue().Set(Nan::New<Boolean>(q->hasMouseTracking()));
}

NAN_METHOD(QWidgetWrap::SetMouseTracking) {
  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(info.This());
  QWidgetImpl* q = w->GetWrapped();

  q->setMouseTracking(info[0]->BooleanValue());

  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(QWidgetWrap::SetFocusPolicy) {
  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(info.This());
  QWidgetImpl* q = w->GetWrapped();

  q->setFocusPolicy((Qt::FocusPolicy)(info[0]->IntegerValue()));

  info.GetReturnValue().Set(Nan::Undefined());
}

// Supported implementations:
//    move (int x, int y)
NAN_METHOD(QWidgetWrap::Move) {
  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(info.This());
  QWidgetImpl* q = w->GetWrapped();

  q->move(info[0]->IntegerValue(), info[1]->IntegerValue());

  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(QWidgetWrap::X) {
  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(info.This());
  QWidgetImpl* q = w->GetWrapped();

  info.GetReturnValue().Set(Nan::New<Integer>(q->x()));
}

NAN_METHOD(QWidgetWrap::Y) {
  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(info.This());
  QWidgetImpl* q = w->GetWrapped();

  info.GetReturnValue().Set(Nan::New<Integer>(q->y()));
}
