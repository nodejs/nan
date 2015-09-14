// Copyright Joyent, Inc. and other Node contributors.
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to permit
// persons to whom the Software is furnished to do so, subject to the
// following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN
// NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
// USE OR OTHER DEALINGS IN THE SOFTWARE.

#ifndef NAN_TYPEDARRAY_CONTENTS_H_
#define NAN_TYPEDARRAY_CONTENTS_H_

template<typename T>
class TypedArrayContents {
 public:
  NAN_INLINE explicit TypedArrayContents(v8::Local<v8::Value> from) :
      length_(0), data_(NULL) {

#if defined(V8_MAJOR_VERSION) && (V8_MAJOR_VERSION > 4 ||                      \
  (V8_MAJOR_VERSION == 4 && defined(V8_MINOR_VERSION) && V8_MINOR_VERSION >= 3))

    if(from->IsArrayBufferView()) {
      v8::Local<v8::ArrayBufferView> array =
        v8::Local<v8::ArrayBufferView>::Cast(from);

      int byteLength = array->ByteLength();
      int byteOffset = array->ByteOffset();
      v8::Local<v8::ArrayBuffer> buffer = array->Buffer();

      void* data = buffer->GetContents().Data();
      length_ = byteLength / sizeof(T);
      data_   = (T*)((void*)((char*)data + byteOffset));
    }

#elif NODE_MODULE_VERSION >= NODE_0_8_MODULE_VERSION

    if (from->IsObject() && !from->IsNull()) {

      v8::Local<v8::Object> array = v8::Local<v8::Object>::Cast(from);

      v8::Local<v8::Value> byteLength = array->Get(
        Nan::New<v8::String>("byteLength").ToLocalChecked());
      v8::Local<v8::Value> byteOffset = array->Get(
        Nan::New<v8::String>("byteOffset").ToLocalChecked());

      if(byteLength->IsUint32() && byteOffset->IsUint32()) {

        unsigned int length = byteLength->Uint32Value();
        void* data = array->GetIndexedPropertiesExternalArrayData();

        if(data) {
          length_ = length / sizeof(T);
          data_   = (T*)((void*)((char*)data));
        }
      }
    }

#endif
  }

  NAN_INLINE int length() const         { return length_; }
  NAN_INLINE T* operator*()             { return data_;   }
  NAN_INLINE const T* operator*() const { return data_;   }

 private:
   int  length_;
   T*   data_;
};

#endif  // NAN_TYPEDARRAY_CONTENTS_H_
