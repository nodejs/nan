/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#ifndef NAN_TYPEDARRAY_CONTENTS_H_
#define NAN_TYPEDARRAY_CONTENTS_H_

template<typename T>
class TypedArrayContents {
 public:
  NAN_INLINE explicit TypedArrayContents(v8::Local<v8::Value> from) :
      length_(0), data_(NULL) {

#if defined(V8_MAJOR_VERSION) && (V8_MAJOR_VERSION > 4 ||                      \
  (V8_MAJOR_VERSION == 4 && defined(V8_MINOR_VERSION) && V8_MINOR_VERSION >= 3))

    if (from->IsArrayBufferView()) {
      v8::Local<v8::ArrayBufferView> array =
        v8::Local<v8::ArrayBufferView>::Cast(from);

      const size_t    byteLength = array->ByteLength();
      const ptrdiff_t byteOffset = array->ByteOffset();
      v8::Local<v8::ArrayBuffer> buffer = array->Buffer();

      char* data = reinterpret_cast<char*>(buffer->GetContents().Data());

      length_ = byteLength / sizeof(T);
      data_   = reinterpret_cast<T*>(data + byteOffset);
    }

#elif NODE_MODULE_VERSION >= NODE_0_8_MODULE_VERSION

    if (from->IsObject() && !from->IsNull()) {

      v8::Local<v8::Object> array = v8::Local<v8::Object>::Cast(from);

      MaybeLocal<v8::Value> byteLength = Get(array,
        Nan::New<v8::String>("byteLength").ToLocalChecked());
      MaybeLocal<v8::Value> byteOffset = Get(array,
        Nan::New<v8::String>("byteOffset").ToLocalChecked());

      if (!byteLength.IsEmpty() && byteLength.ToLocalChecked()->IsUint32() &&
          !byteOffset.IsEmpty() && byteOffset.ToLocalChecked()->IsUint32()) {

        const size_t length = byteLength.ToLocalChecked()->Uint32Value();
        void* data = array->GetIndexedPropertiesExternalArrayData();

        if (data) {
          length_ = length / sizeof(T);
          data_   = reinterpret_cast<T*>(data);
        }
      }
    }

#endif
  }

  NAN_INLINE size_t length() const            { return length_; }
  NAN_INLINE T* const operator*()             { return data_;   }
  NAN_INLINE const T* const operator*() const { return data_;   }

 private:
  NAN_DISALLOW_ASSIGN_COPY_MOVE(TypedArrayContents)

  //Disable heap allocation
  void *operator new(size_t size);
  void operator delete(void *, size_t);

  size_t  length_;
  T*      data_;
};

#endif  // NAN_TYPEDARRAY_CONTENTS_H_
