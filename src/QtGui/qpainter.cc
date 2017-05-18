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
#include "qpainter.h"
#include "qpixmap.h"
#include "qcolor.h"
#include "qpen.h"
#include "qbrush.h"
#include "qimage.h"
#include "qpainterpath.h"
#include "qfont.h"
#include "qmatrix.h"
#include "../QtWidgets/qwidget.h"

using namespace v8;

Nan::Persistent<FunctionTemplate> QPainterWrap::prototype;
Nan::Persistent<Function> QPainterWrap::constructor;

QPainterWrap::QPainterWrap() {
  q_ = new QPainter();
}
QPainterWrap::~QPainterWrap() {
  delete q_;
}

NAN_MODULE_INIT(QPainterWrap::Initialize) {
  // Prepare constructor template
  Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("QPainter").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);  

  // Prototype
  Nan::SetPrototypeMethod(tpl, "begin", Begin);
  Nan::SetPrototypeMethod(tpl, "end", End);
  Nan::SetPrototypeMethod(tpl, "isActive", IsActive);
  Nan::SetPrototypeMethod(tpl, "save", Save);
  Nan::SetPrototypeMethod(tpl, "restore", Restore);
  Nan::SetPrototypeMethod(tpl, "setPen", SetPen);
  Nan::SetPrototypeMethod(tpl, "setFont", SetFont);
  Nan::SetPrototypeMethod(tpl, "setMatrix", SetMatrix);
  Nan::SetPrototypeMethod(tpl, "fillRect", FillRect);
  Nan::SetPrototypeMethod(tpl, "drawText", DrawText);
  Nan::SetPrototypeMethod(tpl, "drawPixmap", DrawPixmap);
  Nan::SetPrototypeMethod(tpl, "drawImage", DrawImage);
  Nan::SetPrototypeMethod(tpl, "strokePath", StrokePath);

  prototype.Reset(tpl);
  Local<Function> function = Nan::GetFunction(tpl).ToLocalChecked();
  constructor.Reset(function);
  Nan::Set(target, Nan::New("QPainter").ToLocalChecked(), function);
}

NAN_METHOD(QPainterWrap::New) {
  if (info.Length()>0) {
    return Nan::ThrowError(Exception::TypeError(
        Nan::New("QPainterWrap: use begin() for initialization").ToLocalChecked()));
  }

  QPainterWrap* w = new QPainterWrap();
  w->Wrap(info.This());
}

NAN_METHOD(QPainterWrap::Begin) {
  QPainterWrap* w = ObjectWrap::Unwrap<QPainterWrap>(info.This());
  QPainter* q = w->GetWrapped();

  if (!info[0]->IsObject())
    return Nan::ThrowError(Exception::TypeError(
        Nan::New("QPainterWrap:Begin: bad arguments").ToLocalChecked()));

  QString constructor_name = 
    qt_v8::ToQString(info[0]->ToObject()->GetConstructorName());
  
  // Determine argument type (from its constructor) so we can unwrap it
  if (constructor_name == "QPixmap") {
    // QPixmap
    QPixmapWrap* pixmap_wrap = ObjectWrap::Unwrap<QPixmapWrap>(
        info[0]->ToObject());
    QPixmap* pixmap = pixmap_wrap->GetWrapped();

    info.GetReturnValue().Set(Nan::New(q->begin(pixmap)));
  } else if (constructor_name == "QWidget") {
    // QWidget
    QWidgetWrap* widget_wrap = ObjectWrap::Unwrap<QWidgetWrap>(
        info[0]->ToObject());
    QWidget* widget = widget_wrap->GetWrapped();

    info.GetReturnValue().Set(Nan::New(q->begin(widget)));
  }
  else {
    // Unknown argument type
    info.GetReturnValue().Set(Nan::False());
  }
}

NAN_METHOD(QPainterWrap::End) {
  QPainterWrap* w = ObjectWrap::Unwrap<QPainterWrap>(info.This());
  QPainter* q = w->GetWrapped();

  info.GetReturnValue().Set(Nan::New(q->end()));
}

NAN_METHOD(QPainterWrap::IsActive) {
  QPainterWrap* w = ObjectWrap::Unwrap<QPainterWrap>(info.This());
  QPainter* q = w->GetWrapped();

  info.GetReturnValue().Set(Nan::New(q->isActive()));
}

NAN_METHOD(QPainterWrap::Save) {
  QPainterWrap* w = ObjectWrap::Unwrap<QPainterWrap>(info.This());
  QPainter* q = w->GetWrapped();

  q->save();

  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(QPainterWrap::Restore) {
  QPainterWrap* w = ObjectWrap::Unwrap<QPainterWrap>(info.This());
  QPainter* q = w->GetWrapped();

  q->restore();

  info.GetReturnValue().Set(Nan::Undefined());
}

// Supported implementations:
//   setPen( QPen pen )
NAN_METHOD(QPainterWrap::SetPen) {
  QPainterWrap* w = ObjectWrap::Unwrap<QPainterWrap>(info.This());
  QPainter* q = w->GetWrapped();

  QString arg0_constructor;
  if (info[0]->IsObject()) {
    arg0_constructor = 
        qt_v8::ToQString(info[0]->ToObject()->GetConstructorName());
  }

  if (arg0_constructor != "QPen")
    return Nan::ThrowError(Exception::TypeError(
      Nan::New("QPainterWrap::SetPen: bad argument").ToLocalChecked()));

  // Unwrap obj
  QPenWrap* pen_wrap = ObjectWrap::Unwrap<QPenWrap>(
      info[0]->ToObject());
  QPen* pen = pen_wrap->GetWrapped();

  q->setPen(*pen);

  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(QPainterWrap::SetFont) {
  QPainterWrap* w = ObjectWrap::Unwrap<QPainterWrap>(info.This());
  QPainter* q = w->GetWrapped();

  QString arg0_constructor;
  if (info[0]->IsObject()) {
    arg0_constructor = 
        qt_v8::ToQString(info[0]->ToObject()->GetConstructorName());
  }

  if (arg0_constructor != "QFont")
    return Nan::ThrowError(Exception::TypeError(
      Nan::New("QPainterWrap::SetFont: bad argument").ToLocalChecked()));

  // Unwrap obj
  QFontWrap* font_wrap = ObjectWrap::Unwrap<QFontWrap>(
      info[0]->ToObject());
  QFont* font = font_wrap->GetWrapped();

  q->setFont(*font);

  info.GetReturnValue().Set(Nan::Undefined());
}

// This seems to be undocumented in Qt, but it exists!
NAN_METHOD(QPainterWrap::SetMatrix) {
  QPainterWrap* w = ObjectWrap::Unwrap<QPainterWrap>(info.This());
  QPainter* q = w->GetWrapped();

  QString arg0_constructor;
  if (info[0]->IsObject()) {
    arg0_constructor = 
        qt_v8::ToQString(info[0]->ToObject()->GetConstructorName());
  }

  if (arg0_constructor != "QMatrix")
    return Nan::ThrowError(Exception::TypeError(
      Nan::New("QPainterWrap::SetMatrix: bad argument").ToLocalChecked()));

  // Unwrap obj
  QMatrixWrap* matrix_wrap = ObjectWrap::Unwrap<QMatrixWrap>(
      info[0]->ToObject());
  QMatrix* matrix = matrix_wrap->GetWrapped();

  q->setMatrix(*matrix, info[1]->BooleanValue());

  info.GetReturnValue().Set(Nan::Undefined());
}

// Supported versions:
//   fillRect(int x, int y, int w, int h, QBrush brush)
//   fillRect(int x, int y, int w, int h, QColor color)
//   fillRect(int x, int y, int w, int h, Qt::GlobalColor color)
NAN_METHOD(QPainterWrap::FillRect) {
  QPainterWrap* w = ObjectWrap::Unwrap<QPainterWrap>(info.This());
  QPainter* q = w->GetWrapped();

  if (!info[0]->IsNumber() || !info[1]->IsNumber() || !info[2]->IsNumber() ||
      !info[3]->IsNumber())
    info.GetReturnValue().Set(Nan::Undefined());
      
  QString arg4_constructor;
  if (info[4]->IsObject()) {
    arg4_constructor = 
        qt_v8::ToQString(info[4]->ToObject()->GetConstructorName());
  }
  
  if (arg4_constructor == "QBrush") {
    // fillRect(int x, int y, int w, int h, QBrush brush)

    // Unwrap QBrush
    QBrushWrap* brush_wrap = ObjectWrap::Unwrap<QBrushWrap>(
        info[4]->ToObject());
    QBrush* brush = brush_wrap->GetWrapped();

    q->fillRect(info[0]->IntegerValue(), info[1]->IntegerValue(),
                info[2]->IntegerValue(), info[3]->IntegerValue(), 
                *brush);
  } else if (arg4_constructor == "QColor") {
    // fillRect(int x, int y, int w, int h, QColor color)

    // Unwrap QColor
    QColorWrap* color_wrap = ObjectWrap::Unwrap<QColorWrap>(
        info[4]->ToObject());
    QColor* color = color_wrap->GetWrapped();

    q->fillRect(info[0]->IntegerValue(), info[1]->IntegerValue(),
                info[2]->IntegerValue(), info[3]->IntegerValue(), 
                *color);
  } else if (info[4]->IsNumber()) {
    // fillRect(int x, int y, int w, int h, Qt::GlobalColor color)

    q->fillRect(info[0]->IntegerValue(), info[1]->IntegerValue(),
                info[2]->IntegerValue(), info[3]->IntegerValue(), 
                (Qt::GlobalColor)info[4]->IntegerValue());
  } else {
    return Nan::ThrowError(Exception::TypeError(
        Nan::New("QPainterWrap:fillRect: bad arguments").ToLocalChecked()));
  }

  info.GetReturnValue().Set(Nan::Undefined());
}

// Supported versions:
//   drawText(int x, int y, "text")
NAN_METHOD(QPainterWrap::DrawText) {
  QPainterWrap* w = ObjectWrap::Unwrap<QPainterWrap>(info.This());
  QPainter* q = w->GetWrapped();

  if (!info[0]->IsNumber() || !info[1]->IsNumber() || !info[2]->IsString())
    return Nan::ThrowError(Exception::TypeError(
        Nan::New("QPainterWrap:DrawText: bad arguments").ToLocalChecked()));
      
  q->drawText(info[0]->IntegerValue(), info[1]->IntegerValue(), 
      qt_v8::ToQString(info[2]->ToString()));

  info.GetReturnValue().Set(Nan::Undefined());
}

// Supported versions:
//   drawPixmap(int x, int y, QPixmap pixmap)
NAN_METHOD(QPainterWrap::DrawPixmap) {
  QPainterWrap* w = ObjectWrap::Unwrap<QPainterWrap>(info.This());
  QPainter* q = w->GetWrapped();

  QString arg2_constructor;
  if (info[2]->IsObject()) {
    arg2_constructor = 
        qt_v8::ToQString(info[2]->ToObject()->GetConstructorName());
  }

  if (arg2_constructor != "QPixmap") {
    return Nan::ThrowError(Exception::TypeError(
      Nan::New("QPainterWrap::DrawPixmap: pixmap argument not recognized").ToLocalChecked()));
  }
  
  // Unwrap QPixmap
  QPixmapWrap* pixmap_wrap = ObjectWrap::Unwrap<QPixmapWrap>(
      info[2]->ToObject());
  QPixmap* pixmap = pixmap_wrap->GetWrapped();

  if (pixmap->isNull()) {
    return Nan::ThrowError(Exception::TypeError(
      Nan::New("QPainterWrap::DrawPixmap: pixmap is null, no size set?").ToLocalChecked()));
  }

  q->drawPixmap(info[0]->IntegerValue(), info[1]->IntegerValue(), *pixmap);

  info.GetReturnValue().Set(Nan::Undefined());
}

// Supported versions:
//   drawImage( int x, int y, QImage image )
NAN_METHOD(QPainterWrap::DrawImage) {
  QPainterWrap* w = ObjectWrap::Unwrap<QPainterWrap>(info.This());
  QPainter* q = w->GetWrapped();

  QString arg2_constructor;
  if (info[2]->IsObject()) {
    arg2_constructor = 
        qt_v8::ToQString(info[2]->ToObject()->GetConstructorName());
  }

  if (arg2_constructor != "QImage") {
    return Nan::ThrowError(Exception::TypeError(
      Nan::New("QPainterWrap::DrawImage: image argument not recognized").ToLocalChecked()));
  }
  
  // Unwrap QImage
  QImageWrap* image_wrap = ObjectWrap::Unwrap<QImageWrap>(
      info[2]->ToObject());
  QImage* image = image_wrap->GetWrapped();

  if (image->isNull()) {
    return Nan::ThrowError(Exception::TypeError(
      Nan::New("QPainterWrap::DrawImage: image is null, no size set?").ToLocalChecked()));
  }

  q->drawImage(info[0]->IntegerValue(), info[1]->IntegerValue(), *image);

  info.GetReturnValue().Set(Nan::Undefined());
}

// Supported versions:
//   strokePath( QPainterPath path, QPen pen )
NAN_METHOD(QPainterWrap::StrokePath) {
  QPainterWrap* w = ObjectWrap::Unwrap<QPainterWrap>(info.This());
  QPainter* q = w->GetWrapped();

  QString arg0_constructor;
  if (info[0]->IsObject()) {
    arg0_constructor = 
        qt_v8::ToQString(info[0]->ToObject()->GetConstructorName());
  }

  if (arg0_constructor != "QPainterPath") {
    return Nan::ThrowError(Exception::TypeError(
      Nan::New("QPainterWrap::StrokePath: bad arguments").ToLocalChecked()));
  }
  
  QString arg1_constructor;
  if (info[1]->IsObject()) {
    arg1_constructor = 
        qt_v8::ToQString(info[1]->ToObject()->GetConstructorName());
  }

  if (arg1_constructor != "QPen") {
    return Nan::ThrowError(Exception::TypeError(
      Nan::New("QPainterWrap::StrokePath: bad arguments").ToLocalChecked()));
  }

  // Unwrap QPainterPath
  QPainterPathWrap* path_wrap = ObjectWrap::Unwrap<QPainterPathWrap>(
      info[0]->ToObject());
  QPainterPath* path = path_wrap->GetWrapped();

  // Unwrap QPen
  QPenWrap* pen_wrap = ObjectWrap::Unwrap<QPenWrap>(
      info[1]->ToObject());
  QPen* pen = pen_wrap->GetWrapped();

  q->strokePath(*path, *pen);

  info.GetReturnValue().Set(Nan::Undefined());
}
