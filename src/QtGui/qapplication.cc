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

#include "qapplication.h"

using namespace v8;

Nan::Persistent<FunctionTemplate> QApplicationWrap::prototype;
Nan::Persistent<Function> QApplicationWrap::constructor;

int QApplicationWrap::argc_ = 0;
char** QApplicationWrap::argv_ = NULL;

QApplicationWrap::QApplicationWrap() {
  q_ = new QApplication(argc_, argv_);
}

QApplicationWrap::~QApplicationWrap() {
  delete q_;
}

NAN_MODULE_INIT(QApplicationWrap::Initialize) {
  // Prepare constructor template
  Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("QApplication").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);  
  
  // Prototype
  Nan::SetPrototypeMethod(tpl, "processEvents", ProcessEvents);
  Nan::SetPrototypeMethod(tpl, "exec", Exec);
  
  prototype.Reset(tpl);
  Local<Function> function = Nan::GetFunction(tpl).ToLocalChecked();
  constructor.Reset(function);
  Nan::Set(target, Nan::New("QApplication").ToLocalChecked(), function);
}

NAN_METHOD(QApplicationWrap::New) {
  QApplicationWrap* w = new QApplicationWrap();
  w->Wrap(info.This());
}

NAN_METHOD(QApplicationWrap::ProcessEvents) {
  QApplicationWrap* w = ObjectWrap::Unwrap<QApplicationWrap>(info.This());
  QApplication* q = w->GetWrapped();
  
  q->processEvents();
  
  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(QApplicationWrap::Exec) {
  QApplicationWrap* w = ObjectWrap::Unwrap<QApplicationWrap>(info.This());
  QApplication* q = w->GetWrapped();
  
  q->exec();
  
  info.GetReturnValue().Set(Nan::Undefined());
}
