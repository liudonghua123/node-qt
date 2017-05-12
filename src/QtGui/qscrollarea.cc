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

#include <QFrame>
#include "../qt_v8.h"
#include "../QtCore/qsize.h"
#include "qscrollarea.h"
#include "qwidget.h"
#include "qscrollbar.h"

using namespace v8;

Nan::Persistent<FunctionTemplate> QScrollAreaWrap::prototype;
Nan::Persistent<Function> QScrollAreaWrap::constructor;

// Supported implementations:
//   QScrollArea ( )
//   QScrollArea ( QWidget widget )
QScrollAreaWrap::QScrollAreaWrap(Nan::NAN_METHOD_ARGS_TYPE info) {
  if (info.Length() == 0) {
    // QScrollArea ( )

    q_ = new QScrollArea;
    return;
  }

  // QScrollArea ( QWidget widget )

  QString arg0_constructor = 
      qt_v8::ToQString(info[0]->ToObject()->GetConstructorName());

  if (arg0_constructor != "QWidget")
    Nan::ThrowError(Exception::TypeError(
      Nan::New("QScrollArea::constructor: bad argument").ToLocalChecked()));

  // Unwrap obj
  QWidgetWrap* q_wrap = ObjectWrap::Unwrap<QWidgetWrap>(
      info[0]->ToObject());
  QWidget* q = q_wrap->GetWrapped();

  q_ = new QScrollArea(q);
}

QScrollAreaWrap::~QScrollAreaWrap() {
  delete q_;
}

NAN_MODULE_INIT(QScrollAreaWrap::Initialize) {
  // Prepare constructor template
  Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("QScrollArea").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);  

  // Wrapped methods
  Nan::SetPrototypeMethod(tpl, "resize", Resize);
  Nan::SetPrototypeMethod(tpl, "show", Show);
  Nan::SetPrototypeMethod(tpl, "size", Size);
  Nan::SetPrototypeMethod(tpl, "width", Width);
  Nan::SetPrototypeMethod(tpl, "height", Height);
  Nan::SetPrototypeMethod(tpl, "parent", Parent);
  Nan::SetPrototypeMethod(tpl, "objectName", ObjectName);
  Nan::SetPrototypeMethod(tpl, "setObjectName", SetObjectName);
  Nan::SetPrototypeMethod(tpl, "update", Update);
  Nan::SetPrototypeMethod(tpl, "setFocusPolicy", SetFocusPolicy);
  Nan::SetPrototypeMethod(tpl, "move", Move);
  Nan::SetPrototypeMethod(tpl, "x", X);
  Nan::SetPrototypeMethod(tpl, "y", Y);

  // QScrollArea-specific
  Nan::SetPrototypeMethod(tpl, "setWidget", SetWidget);
  Nan::SetPrototypeMethod(tpl, "widget", Widget);
  Nan::SetPrototypeMethod(tpl, "setFrameShape", SetFrameShape);
  Nan::SetPrototypeMethod(tpl, "setVerticalScrollBarPolicy", SetVerticalScrollBarPolicy);
  Nan::SetPrototypeMethod(tpl, "setHorizontalScrollBarPolicy", SetHorizontalScrollBarPolicy);
  Nan::SetPrototypeMethod(tpl, "verticalScrollBar", VerticalScrollBar);
  Nan::SetPrototypeMethod(tpl, "horizontalScrollBar", HorizontalScrollBar);

  prototype.Reset(tpl);
  Local<Function> function = Nan::GetFunction(tpl).ToLocalChecked();
  constructor.Reset(function);
  Nan::Set(target, Nan::New("QScrollArea").ToLocalChecked(), function);
}

NAN_METHOD(QScrollAreaWrap::New) {
  QScrollAreaWrap* w = new QScrollAreaWrap(info);
  w->Wrap(info.This());
}

NAN_METHOD(QScrollAreaWrap::Resize) {
  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(info.This());
  QScrollArea* q = w->GetWrapped();

  q->resize(info[0]->NumberValue(), info[1]->NumberValue());

  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(QScrollAreaWrap::Show) {
  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(info.This());
  QScrollArea* q = w->GetWrapped();

  q->show();

  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(QScrollAreaWrap::Close) {
  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(info.This());
  QScrollArea* q = w->GetWrapped();

  q->close();

  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(QScrollAreaWrap::Size) {
  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(info.This());
  QScrollArea* q = w->GetWrapped();

  info.GetReturnValue().Set(QSizeWrap::NewInstance(q->size()));
}

NAN_METHOD(QScrollAreaWrap::Width) {
  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(info.This());
  QScrollArea* q = w->GetWrapped();

  info.GetReturnValue().Set(Nan::New(q->width()));
}

NAN_METHOD(QScrollAreaWrap::Height) {
  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(info.This());
  QScrollArea* q = w->GetWrapped();

  info.GetReturnValue().Set(Nan::New(q->height()));
}

NAN_METHOD(QScrollAreaWrap::ObjectName) {
  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(info.This());
  QScrollArea* q = w->GetWrapped();

  info.GetReturnValue().Set(qt_v8::FromQString(q->objectName()));
}

NAN_METHOD(QScrollAreaWrap::SetObjectName) {
  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(info.This());
  QScrollArea* q = w->GetWrapped();

  q->setObjectName(qt_v8::ToQString(info[0]->ToString()));

  info.GetReturnValue().Set(Nan::Undefined());
}

//
// QUIRK:
// Here: Parent() returns the parent's name
// Qt: Parent() returns QObject
// Intended mostly for sanity checks
//
NAN_METHOD(QScrollAreaWrap::Parent) {
  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(info.This());
  QScrollArea* q = w->GetWrapped();

  info.GetReturnValue().Set(qt_v8::FromQString(q->parent()->objectName()));
}

NAN_METHOD(QScrollAreaWrap::Update) {
  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(info.This());
  QScrollArea* q = w->GetWrapped();

  q->update();

  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(QScrollAreaWrap::SetWidget) {
  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(info.This());
  QScrollArea* q = w->GetWrapped();

  QString arg0_constructor;
  if (info[0]->IsObject()) {
    arg0_constructor = 
        qt_v8::ToQString(info[0]->ToObject()->GetConstructorName());
  }

  if (arg0_constructor != "QWidget")
    return Nan::ThrowError(Exception::TypeError(
      Nan::New("QScrollArea::SetWidget: bad argument").ToLocalChecked()));

  // Unwrap obj
  QWidgetWrap* widget_wrap = ObjectWrap::Unwrap<QWidgetWrap>(
      info[0]->ToObject());
  QWidget* widget = widget_wrap->GetWrapped();

  q->setWidget(widget);

  info.GetReturnValue().Set(Nan::Undefined());
}

// QUIRK:
// Does not return QWidget. Returns 1 if child widget exists, 0 otherwise
NAN_METHOD(QScrollAreaWrap::Widget) {
  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(info.This());
  QScrollArea* q = w->GetWrapped();

  int retvalue = q->widget() ? 1 : 0;

  info.GetReturnValue().Set(Nan::New(retvalue));
}

NAN_METHOD(QScrollAreaWrap::SetFrameShape) {
  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(info.This());
  QScrollArea* q = w->GetWrapped();

  q->setFrameShape((QFrame::Shape)(info[0]->IntegerValue()));

  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(QScrollAreaWrap::SetFocusPolicy) {
  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(info.This());
  QScrollArea* q = w->GetWrapped();

  q->setFocusPolicy((Qt::FocusPolicy)(info[0]->IntegerValue()));

  info.GetReturnValue().Set(Nan::Undefined());
}

// Supported implementations:
//    move (int x, int y)
NAN_METHOD(QScrollAreaWrap::Move) {
  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(info.This());
  QScrollArea* q = w->GetWrapped();

  q->move(info[0]->IntegerValue(), info[1]->IntegerValue());

  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(QScrollAreaWrap::X) {
  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(info.This());
  QScrollArea* q = w->GetWrapped();

  info.GetReturnValue().Set(Nan::New(q->x()));
}

NAN_METHOD(QScrollAreaWrap::Y) {
  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(info.This());
  QScrollArea* q = w->GetWrapped();

  info.GetReturnValue().Set(Nan::New(q->y()));
}

NAN_METHOD(QScrollAreaWrap::SetVerticalScrollBarPolicy) {
  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(info.This());
  QScrollArea* q = w->GetWrapped();

  q->setVerticalScrollBarPolicy((Qt::ScrollBarPolicy)(info[0]->IntegerValue()));

  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(QScrollAreaWrap::SetHorizontalScrollBarPolicy) {
  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(info.This());
  QScrollArea* q = w->GetWrapped();

  q->setHorizontalScrollBarPolicy((Qt::ScrollBarPolicy)
      (info[0]->IntegerValue()));

  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(QScrollAreaWrap::HorizontalScrollBar) {
  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(info.This());
  QScrollArea* q = w->GetWrapped();

  info.GetReturnValue().Set(
    QScrollBarWrap::NewInstance(q->horizontalScrollBar()));
}

NAN_METHOD(QScrollAreaWrap::VerticalScrollBar) {
  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(info.This());
  QScrollArea* q = w->GetWrapped();

  info.GetReturnValue().Set(
      QScrollBarWrap::NewInstance(q->verticalScrollBar()));
}
