#pragma once

#include <node.h>
#include <nan.h>
#include "qmouseevent.h"
#include "qkeyevent.h"

class QWidgetWrapBase : public node::ObjectWrap {
 public:
  ~QWidgetWrapBase();
  
  static void Inherit(v8::Local<v8::FunctionTemplate> tpl);
  
  void paintEvent(QPaintEvent* e);
  void mousePressEvent(QMouseEvent* e);
  void mouseReleaseEvent(QMouseEvent* e);
  void mouseMoveEvent(QMouseEvent* e);
  void keyPressEvent(QKeyEvent* e);
  void keyReleaseEvent(QKeyEvent* e);
  
 private:
  Nan::Persistent<v8::Function> paintEventCallback;
  Nan::Persistent<v8::Function> mousePressCallback;
  Nan::Persistent<v8::Function> mouseReleaseCallback;
  Nan::Persistent<v8::Function> mouseMoveCallback;
  Nan::Persistent<v8::Function> keyPressCallback;
  Nan::Persistent<v8::Function> keyReleaseCallback;
  
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
};
