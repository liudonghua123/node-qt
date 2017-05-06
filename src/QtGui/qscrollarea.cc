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

#include <node.h>
#include <QFrame>
#include "../qt_v8.h"
#include "../QtCore/qsize.h"
#include "qscrollarea.h"
#include "qwidget.h"
#include "qscrollbar.h"

using namespace v8;

Persistent<Function> QScrollAreaWrap::constructor;

// Supported implementations:
//   QScrollArea ( )
//   QScrollArea ( QWidget widget )
QScrollAreaWrap::QScrollAreaWrap(const v8::FunctionCallbackInfo<v8::Value>& args) {
  if (args.Length() == 0) {
    // QScrollArea ( )

    q_ = new QScrollArea;
    return;
  }

  // QScrollArea ( QWidget widget )

  QString arg0_constructor = 
      qt_v8::ToQString(args[0]->ToObject()->GetConstructorName());

  if (arg0_constructor != "QWidget")
    ThrowException(Exception::TypeError(
      String::New("QScrollArea::constructor: bad argument")));

  // Unwrap obj
  QWidgetWrap* q_wrap = ObjectWrap::Unwrap<QWidgetWrap>(
      args[0]->ToObject());
  QWidget* q = q_wrap->GetWrapped();

  q_ = new QScrollArea(q);
}

QScrollAreaWrap::~QScrollAreaWrap() {
  delete q_;
}

void QScrollAreaWrap::Initialize(Handle<Object> target) {
  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  tpl->SetClassName(Nan::New("QScrollArea").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);  

  // Wrapped methods
  tpl->PrototypeTemplate()->Set(Nan::New("resize").ToLocalChecked(),
      FunctionTemplate::New(Resize)->GetFunction());
  tpl->PrototypeTemplate()->Set(Nan::New("show").ToLocalChecked(),
      FunctionTemplate::New(Show)->GetFunction());
  tpl->PrototypeTemplate()->Set(Nan::New("size").ToLocalChecked(),
      FunctionTemplate::New(Size)->GetFunction());
  tpl->PrototypeTemplate()->Set(Nan::New("width").ToLocalChecked(),
      FunctionTemplate::New(Width)->GetFunction());
  tpl->PrototypeTemplate()->Set(Nan::New("height").ToLocalChecked(),
      FunctionTemplate::New(Height)->GetFunction());
  tpl->PrototypeTemplate()->Set(Nan::New("parent").ToLocalChecked(),
      FunctionTemplate::New(Parent)->GetFunction());
  tpl->PrototypeTemplate()->Set(Nan::New("objectName").ToLocalChecked(),
      FunctionTemplate::New(ObjectName)->GetFunction());
  tpl->PrototypeTemplate()->Set(Nan::New("setObjectName").ToLocalChecked(),
      FunctionTemplate::New(SetObjectName)->GetFunction());
  tpl->PrototypeTemplate()->Set(Nan::New("update").ToLocalChecked(),
      FunctionTemplate::New(Update)->GetFunction());
  tpl->PrototypeTemplate()->Set(Nan::New("setFocusPolicy").ToLocalChecked(),
      FunctionTemplate::New(SetFocusPolicy)->GetFunction());
  tpl->PrototypeTemplate()->Set(Nan::New("move").ToLocalChecked(),
      FunctionTemplate::New(Move)->GetFunction());
  tpl->PrototypeTemplate()->Set(Nan::New("x").ToLocalChecked(),
      FunctionTemplate::New(X)->GetFunction());
  tpl->PrototypeTemplate()->Set(Nan::New("y").ToLocalChecked(),
      FunctionTemplate::New(Y)->GetFunction());

  // QScrollArea-specific
  tpl->PrototypeTemplate()->Set(Nan::New("setWidget").ToLocalChecked(),
      FunctionTemplate::New(SetWidget)->GetFunction());
  tpl->PrototypeTemplate()->Set(Nan::New("widget").ToLocalChecked(),
      FunctionTemplate::New(Widget)->GetFunction());
  tpl->PrototypeTemplate()->Set(Nan::New("setFrameShape").ToLocalChecked(),
      FunctionTemplate::New(SetFrameShape)->GetFunction());
  tpl->PrototypeTemplate()->Set(Nan::New("setVerticalScrollBarPolicy").ToLocalChecked(),
      FunctionTemplate::New(SetVerticalScrollBarPolicy)->GetFunction());
  tpl->PrototypeTemplate()->Set(Nan::New("setHorizontalScrollBarPolicy").ToLocalChecked(),
      FunctionTemplate::New(SetHorizontalScrollBarPolicy)->GetFunction());
  tpl->PrototypeTemplate()->Set(Nan::New("verticalScrollBar").ToLocalChecked(),
      FunctionTemplate::New(VerticalScrollBar)->GetFunction());
  tpl->PrototypeTemplate()->Set(Nan::New("horizontalScrollBar").ToLocalChecked(),
      FunctionTemplate::New(HorizontalScrollBar)->GetFunction());

  constructor = Persistent<Function>::New(tpl->GetFunction());
  target->Set(Nan::New("QScrollArea").ToLocalChecked(), constructor);
}

Handle<Value> QScrollAreaWrap::New(const FunctionCallbackInfo<Value>& args) {
  HandleScope scope;

  QScrollAreaWrap* w = new QScrollAreaWrap(args);
  w->Wrap(args.This());

  return args.This();
}

Handle<Value> QScrollAreaWrap::Resize(const FunctionCallbackInfo<Value>& args) {
  HandleScope scope;

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  q->resize(args[0]->NumberValue(), args[1]->NumberValue());

  return scope.Close(Undefined());
}

Handle<Value> QScrollAreaWrap::Show(const FunctionCallbackInfo<Value>& args) {
  HandleScope scope;

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  q->show();

  return scope.Close(Undefined());
}

Handle<Value> QScrollAreaWrap::Close(const FunctionCallbackInfo<Value>& args) {
  HandleScope scope;

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  q->close();

  return scope.Close(Undefined());
}

Handle<Value> QScrollAreaWrap::Size(const FunctionCallbackInfo<Value>& args) {
  HandleScope scope;

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  return scope.Close( QSizeWrap::NewInstance(q->size()) );
}

Handle<Value> QScrollAreaWrap::Width(const FunctionCallbackInfo<Value>& args) {
  HandleScope scope;

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  return scope.Close( Integer::New(q->width()) );
}

Handle<Value> QScrollAreaWrap::Height(const FunctionCallbackInfo<Value>& args) {
  HandleScope scope;

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  return scope.Close( Integer::New(q->height()) );
}

Handle<Value> QScrollAreaWrap::ObjectName(const FunctionCallbackInfo<Value>& args) {
  HandleScope scope;

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  return scope.Close(qt_v8::FromQString(q->objectName()));
}

Handle<Value> QScrollAreaWrap::SetObjectName(const FunctionCallbackInfo<Value>& args) {
  HandleScope scope;

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  q->setObjectName(qt_v8::ToQString(args[0]->ToString()));

  return scope.Close(Undefined());
}

//
// QUIRK:
// Here: Parent() returns the parent's name
// Qt: Parent() returns QObject
// Intended mostly for sanity checks
//
Handle<Value> QScrollAreaWrap::Parent(const FunctionCallbackInfo<Value>& args) {
  HandleScope scope;

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  return scope.Close(qt_v8::FromQString(q->parent()->objectName()));
}

Handle<Value> QScrollAreaWrap::Update(const FunctionCallbackInfo<Value>& args) {
  HandleScope scope;

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  q->update();

  return scope.Close(Undefined());
}

Handle<Value> QScrollAreaWrap::SetWidget(const FunctionCallbackInfo<Value>& args) {
  HandleScope scope;

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  QString arg0_constructor;
  if (args[0]->IsObject()) {
    arg0_constructor = 
        qt_v8::ToQString(args[0]->ToObject()->GetConstructorName());
  }

  if (arg0_constructor != "QWidget")
    return ThrowException(Exception::TypeError(
      String::New("QScrollArea::SetWidget: bad argument")));

  // Unwrap obj
  QWidgetWrap* widget_wrap = ObjectWrap::Unwrap<QWidgetWrap>(
      args[0]->ToObject());
  QWidget* widget = widget_wrap->GetWrapped();

  q->setWidget(widget);

  return scope.Close(Undefined());
}

// QUIRK:
// Does not return QWidget. Returns 1 if child widget exists, 0 otherwise
Handle<Value> QScrollAreaWrap::Widget(const FunctionCallbackInfo<Value>& args) {
  HandleScope scope;

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  int retvalue = q->widget() ? 1 : 0;

  return scope.Close(Integer::New(retvalue));
}

Handle<Value> QScrollAreaWrap::SetFrameShape(const FunctionCallbackInfo<Value>& args) {
  HandleScope scope;

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  q->setFrameShape((QFrame::Shape)(args[0]->IntegerValue()));

  return scope.Close(Undefined());
}

Handle<Value> QScrollAreaWrap::SetFocusPolicy(const FunctionCallbackInfo<Value>& args) {
  HandleScope scope;

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  q->setFocusPolicy((Qt::FocusPolicy)(args[0]->IntegerValue()));

  return scope.Close(Undefined());
}

// Supported implementations:
//    move (int x, int y)
Handle<Value> QScrollAreaWrap::Move(const FunctionCallbackInfo<Value>& args) {
  HandleScope scope;

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  q->move(args[0]->IntegerValue(), args[1]->IntegerValue());

  return scope.Close(Undefined());
}

Handle<Value> QScrollAreaWrap::X(const FunctionCallbackInfo<Value>& args) {
  HandleScope scope;

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  return scope.Close(Integer::New(q->x()));
}

Handle<Value> QScrollAreaWrap::Y(const FunctionCallbackInfo<Value>& args) {
  HandleScope scope;

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  return scope.Close(Integer::New(q->y()));
}

Handle<Value> 
  QScrollAreaWrap::SetVerticalScrollBarPolicy(const FunctionCallbackInfo<Value>& args) {
  HandleScope scope;

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  q->setVerticalScrollBarPolicy((Qt::ScrollBarPolicy)(args[0]->IntegerValue()));

  return scope.Close(Undefined());
}

Handle<Value> 
  QScrollAreaWrap::SetHorizontalScrollBarPolicy(const FunctionCallbackInfo<Value>& args) {
  HandleScope scope;

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  q->setHorizontalScrollBarPolicy((Qt::ScrollBarPolicy)
      (args[0]->IntegerValue()));

  return scope.Close(Undefined());
}

Handle<Value> QScrollAreaWrap::HorizontalScrollBar(const FunctionCallbackInfo<Value>& args) {
  HandleScope scope;

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  return scope.Close( 
    QScrollBarWrap::NewInstance(q->horizontalScrollBar()) );
}

Handle<Value> QScrollAreaWrap::VerticalScrollBar(const FunctionCallbackInfo<Value>& args) {
  HandleScope scope;

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  return scope.Close( 
      QScrollBarWrap::NewInstance(q->verticalScrollBar()) );
}
