#include "qwidgetwrapbase.h"

using namespace v8;

//
// QWidgetWrapBase()
//
QWidgetWrapBase::~QWidgetWrapBase() {
  delete &paintEventCallback;
  delete &mousePressCallback;
  delete &mouseReleaseCallback;
  delete &mouseMoveCallback;
  delete &keyPressCallback;
  delete &keyReleaseCallback;
}

void QWidgetWrapBase::Inherit(Local<FunctionTemplate> tpl) {
  Nan::SetPrototypeMethod(tpl, "paintEvent", PaintEvent);
  Nan::SetPrototypeMethod(tpl, "mousePressEvent", MousePressEvent);
  Nan::SetPrototypeMethod(tpl, "mouseReleaseEvent", MouseReleaseEvent);
  Nan::SetPrototypeMethod(tpl, "mouseMoveEvent", MouseMoveEvent);
  Nan::SetPrototypeMethod(tpl, "keyPressEvent", KeyPressEvent);
  Nan::SetPrototypeMethod(tpl, "keyReleaseEvent", KeyReleaseEvent);
}

//
// PaintEvent()
// Binds a callback to Qt's event
//
NAN_METHOD(QWidgetWrapBase::PaintEvent) {
  QWidgetWrapBase* w = node::ObjectWrap::Unwrap<QWidgetWrapBase>(info.This());

  if (info[0]->IsFunction()) {
    w->paintEventCallback.Reset(Local<Function>::Cast(info[0]));
  }

  info.GetReturnValue().Set(Nan::Undefined());
}

//
// MousePressEvent()
// Binds a callback to Qt's event
//
NAN_METHOD(QWidgetWrapBase::MousePressEvent) {
  QWidgetWrapBase* w = node::ObjectWrap::Unwrap<QWidgetWrapBase>(info.This());

  if (info[0]->IsFunction()) {
    w->mousePressCallback.Reset(Local<Function>::Cast(info[0]));
  }

  info.GetReturnValue().Set(Nan::Undefined());
}

//
// MouseReleaseEvent()
// Binds a callback to Qt's event
//
NAN_METHOD(QWidgetWrapBase::MouseReleaseEvent) {
  QWidgetWrapBase* w = node::ObjectWrap::Unwrap<QWidgetWrapBase>(info.This());

  if (info[0]->IsFunction()) {
    w->mouseReleaseCallback.Reset(Local<Function>::Cast(info[0]));
  }

  info.GetReturnValue().Set(Nan::Undefined());
}

//
// MouseMoveEvent()
// Binds a callback to Qt's event
//
NAN_METHOD(QWidgetWrapBase::MouseMoveEvent) {
  QWidgetWrapBase* w = node::ObjectWrap::Unwrap<QWidgetWrapBase>(info.This());

  if (info[0]->IsFunction()) {
    w->mouseMoveCallback.Reset(Local<Function>::Cast(info[0]));
  }

  info.GetReturnValue().Set(Nan::Undefined());
}

//
// KeyPressEvent()
// Binds a callback to Qt's event
//
NAN_METHOD(QWidgetWrapBase::KeyPressEvent) {
  QWidgetWrapBase* w = node::ObjectWrap::Unwrap<QWidgetWrapBase>(info.This());

  if (info[0]->IsFunction()) {
    w->keyPressCallback.Reset(Local<Function>::Cast(info[0]));
  }

  info.GetReturnValue().Set(Nan::Undefined());
}

//
// KeyReleaseEvent()
// Binds a callback to Qt's event
//
NAN_METHOD(QWidgetWrapBase::KeyReleaseEvent) {
  QWidgetWrapBase* w = node::ObjectWrap::Unwrap<QWidgetWrapBase>(info.This());

  if (info[0]->IsFunction()) {
    w->keyReleaseCallback.Reset(Local<Function>::Cast(info[0]));
  }

  info.GetReturnValue().Set(Nan::Undefined());
}

void QWidgetWrapBase::paintEvent(QPaintEvent* e) {
  if (paintEventCallback.IsEmpty()) {
    return;
  }

  const unsigned argc = 0;
  Handle<Value> argv[1] = {};

  Nan::Callback(Nan::New(paintEventCallback)).Call(argc, argv);
}

void QWidgetWrapBase::mousePressEvent(QMouseEvent* e) {
  e->ignore(); // ensures event bubbles up

  if (mousePressCallback.IsEmpty()) {
    return;
  }

  const unsigned argc = 1;
  Handle<Value> argv[argc] = {
    QMouseEventWrap::NewInstance(*e)
  };

  Nan::Callback(Nan::New(mousePressCallback)).Call(argc, argv);
}

void QWidgetWrapBase::mouseReleaseEvent(QMouseEvent* e) {
  e->ignore(); // ensures event bubbles up

  if (mouseReleaseCallback.IsEmpty()) {
    return;
  }

  const unsigned argc = 1;
  Handle<Value> argv[argc] = {
    QMouseEventWrap::NewInstance(*e)
  };

  Nan::Callback(Nan::New(mouseReleaseCallback)).Call(argc, argv);
}

void QWidgetWrapBase::mouseMoveEvent(QMouseEvent* e) {
  e->ignore(); // ensures event bubbles up

  if (mouseMoveCallback.IsEmpty()) {
    return;
  }

  const unsigned argc = 1;
  Handle<Value> argv[argc] = {
    QMouseEventWrap::NewInstance(*e)
  };

  Nan::Callback(Nan::New(mouseMoveCallback)).Call(argc, argv);
}

void QWidgetWrapBase::keyPressEvent(QKeyEvent* e) {
  e->ignore(); // ensures event bubbles up

  if (keyPressCallback.IsEmpty()) {
    return;
  }

  const unsigned argc = 1;
  Handle<Value> argv[argc] = {
    QKeyEventWrap::NewInstance(*e)
  };

  Nan::Callback(Nan::New(keyPressCallback)).Call(argc, argv);
}

void QWidgetWrapBase::keyReleaseEvent(QKeyEvent* e) {
  e->ignore(); // ensures event bubbles up

  if (keyReleaseCallback.IsEmpty()) {
    return;
  }

  const unsigned argc = 1;
  Handle<Value> argv[argc] = {
    QKeyEventWrap::NewInstance(*e)
  };

  Nan::Callback(Nan::New(keyReleaseCallback)).Call(argc, argv);
}