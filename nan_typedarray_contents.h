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

      const size_t    byte_length = array->ByteLength();
      const ptrdiff_t byte_offset = array->ByteOffset();
      v8::Local<v8::ArrayBuffer> buffer = array->Buffer();

      char* data = static_cast<char*>(buffer->GetContents().Data());

      length_ = byte_length / sizeof(T);
      data_   = reinterpret_cast<T*>(data + byte_offset);
    }

#else

    if (from->IsObject() && !from->IsNull()) {

      v8::Local<v8::Object> array = v8::Local<v8::Object>::Cast(from);

      MaybeLocal<v8::Value> byte_length = Get(array,
        New<v8::String>("byteLength").ToLocalChecked());
      MaybeLocal<v8::Value> byte_offset = Get(array,
        New<v8::String>("byteOffset").ToLocalChecked());

      if (!byte_length.IsEmpty() && byte_length.ToLocalChecked()->IsUint32() &&
          !byte_offset.IsEmpty() && byte_offset.ToLocalChecked()->IsUint32()) {

        const size_t length = byte_length.ToLocalChecked()->Uint32Value();
        void* data = array->GetIndexedPropertiesExternalArrayData();

        if (data) {
          length_ = length / sizeof(T);
          data_   = static_cast<T*>(data);
        }
      }
    }

#endif

    assert(reinterpret_cast<uintptr_t>(data_) % sizeof(T) == 0);
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
