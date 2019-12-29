/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2018 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#ifndef NAN_H_
# error You cannot include nan_worker_cpp17.tcc outside of nan.h!
#endif

#if !NAN_ENABLE_CPP17_WORKERS
# error You need to include nan_worker_legacy.tcc if you want to use legacy workers!
#endif

/* abstract */ 
class AsyncWorker {
public:
    explicit AsyncWorker(
        Callback *callback_,
        const char* resource_name = "nan::AsyncWorker"
    ) : 
        callback(callback_), 
        errmsg_(nullptr) 
    {
        request.data = this;

        HandleScope scope;
        v8::Local<v8::Object> obj = New<v8::Object>();
        persistentHandle.Reset(obj);
        async_resource = new AsyncResource(resource_name, obj);
    }

    virtual ~AsyncWorker() {
        HandleScope scope;

        if (!persistentHandle.IsEmpty()) {
            persistentHandle.Reset();
        }
        delete callback;
        delete[] errmsg_;
        delete async_resource;
    }

    virtual void WorkComplete() {
        HandleScope scope;

        if (errmsg_ == nullptr) {
            HandleOKCallback();
        } else {
            HandleErrorCallback();
        }

        delete callback;
        callback = nullptr;
    }

    inline void SaveToPersistent(
        const char *key, 
        const v8::Local<v8::Value>& value
    ) {
        HandleScope scope;
        Set(New(persistentHandle), New(key).ToLocalChecked(), value).FromJust();
    }

    inline void SaveToPersistent(
        const v8::Local<v8::String>& key, 
        const v8::Local<v8::Value>& value
    ) {
        HandleScope scope;
        Set(New(persistentHandle), key, value).FromJust();
    }

    inline void SaveToPersistent(
        uint32_t index, 
        const v8::Local<v8::Value>& value
    ) {
        HandleScope scope;
        Set(New(persistentHandle), index, value).FromJust();
    }

    inline v8::Local<v8::Value> GetFromPersistent(const char *key) const {
        EscapableHandleScope scope;
        return scope.Escape(
            Get(New(persistentHandle), New(key).ToLocalChecked())
            .FromMaybe(v8::Local<v8::Value>()));
    }

    inline v8::Local<v8::Value>
    GetFromPersistent(const v8::Local<v8::String> &key) const {
        EscapableHandleScope scope;
        return scope.Escape(
            Get(New(persistentHandle), key)
            .FromMaybe(v8::Local<v8::Value>()));
    }

    inline v8::Local<v8::Value> GetFromPersistent(uint32_t index) const {
        EscapableHandleScope scope;
        return scope.Escape(
            Get(New(persistentHandle), index)
            .FromMaybe(v8::Local<v8::Value>()));
    }

    virtual void Execute() = 0;

    uv_work_t request;

    virtual void Destroy() {
        delete this;
    }

protected:
    Persistent<v8::Object> persistentHandle;
    Callback *callback;
    AsyncResource *async_resource;

    virtual void HandleOKCallback() {
        HandleScope scope;

        callback->Call(0, nullptr, async_resource);
    }

    virtual void HandleErrorCallback() {
        HandleScope scope;

        v8::Local<v8::Value> argv[] = {
            v8::Exception::Error(New<v8::String>(ErrorMessage()).ToLocalChecked())
        };
        callback->Call(1, argv, async_resource);
    }

    void SetErrorMessage(const char *msg) {
        delete[] errmsg_;

        size_t size = strlen(msg) + 1;
        errmsg_ = new char[size];
        memcpy(errmsg_, msg, size);
    }

    const char* ErrorMessage() const {
        return errmsg_;
    }

private:
    NAN_DISALLOW_ASSIGN_COPY_MOVE(AsyncWorker)
    char *errmsg_;
};

/* abstract */ 
class AsyncBareProgressWorkerBase : public AsyncWorker {
public:
    explicit AsyncBareProgressWorkerBase(
        Callback *callback_,
        const char* resource_name = "nan::AsyncBareProgressWorkerBase"
    ) : 
        AsyncWorker(callback_, resource_name) 
    {
      uv_async_init(
          GetCurrentEventLoop(),
          &async, 
          AsyncProgress_
      );
      async.data = this;
    }

    virtual ~AsyncBareProgressWorkerBase() = default;

    virtual void WorkProgress() = 0;

    virtual void Destroy() {
        uv_close(reinterpret_cast<uv_handle_t*>(&async), AsyncClose_);
    }

 private:
    inline static NAUV_WORK_CB(AsyncProgress_) {
        AsyncBareProgressWorkerBase *worker =
                static_cast<AsyncBareProgressWorkerBase*>(async->data);
        worker->WorkProgress();
    }

    inline static void AsyncClose_(uv_handle_t* handle) {
        AsyncBareProgressWorkerBase *worker =
                static_cast<AsyncBareProgressWorkerBase*>(handle->data);
        delete worker;
    }

protected:
    uv_async_t async;
};

template<class T>
/* abstract */
class AsyncBareProgressWorker : public AsyncBareProgressWorkerBase {
public:
    explicit AsyncBareProgressWorker(
        Callback *callback_,
        const char* resource_name = "nan::AsyncBareProgressWorker"
    ) : 
        AsyncBareProgressWorkerBase(callback_, resource_name) 
    {
        uv_mutex_init(&async_lock);
    }

    virtual ~AsyncBareProgressWorker() {
        uv_mutex_destroy(&async_lock);
    }

    class ExecutionProgress {
    friend class AsyncBareProgressWorker;
    public:
        void Signal() const {
            uv_mutex_lock(&that_->async_lock);
            uv_async_send(&that_->async);
            uv_mutex_unlock(&that_->async_lock);
        }

        void Send(const T* data, size_t count) const {
            that_->SendProgress_(data, count);
        }

    private:
        explicit ExecutionProgress(AsyncBareProgressWorker *that) : that_(that) {}
        NAN_DISALLOW_ASSIGN_COPY_MOVE(ExecutionProgress)
        AsyncBareProgressWorker* const that_;
    };

    virtual void Execute(const ExecutionProgress& progress) = 0;
    virtual void HandleProgressCallback(const T *data, size_t size) = 0;

 protected:
    uv_mutex_t async_lock;

 private:
    void Execute() /*final override*/ {
        ExecutionProgress progress(this);
        Execute(progress);
    }

    virtual void SendProgress_(const T *data, size_t count) = 0;
};

template<class T>
/* abstract */
class AsyncProgressWorkerBase : public AsyncBareProgressWorker<T> {
public:
    explicit AsyncProgressWorkerBase(
        Callback *callback_,
        const char* resource_name = "nan::AsyncProgressWorkerBase"
    ) : 
        AsyncBareProgressWorker<T>(callback_, resource_name), 
        asyncdata_(nullptr),
        asyncsize_(0) 
    {
    }

    virtual ~AsyncProgressWorkerBase() {
        delete[] asyncdata_;
    }

    void WorkProgress() {
        uv_mutex_lock(&this->async_lock);
        T *data = asyncdata_;
        size_t size = asyncsize_;
        asyncdata_ = nullptr;
        asyncsize_ = 0;
        uv_mutex_unlock(&this->async_lock);

        // Don't send progress events after we've already completed.
        if (this->callback) {
            this->HandleProgressCallback(data, size);
        }
        delete[] data;
    }

 private:
    void SendProgress_(const T *data, size_t count) {
        T *new_data = new T[count];
        std::copy(data, data + count, new_data);

        uv_mutex_lock(&this->async_lock);
        T *old_data = asyncdata_;
        asyncdata_ = new_data;
        asyncsize_ = count;
        uv_async_send(&this->async);
        uv_mutex_unlock(&this->async_lock);

        delete[] old_data;
    }

    T *asyncdata_;
    size_t asyncsize_;
};

// This ensures compatibility to the previous un-templated AsyncProgressWorker
// class definition.
typedef AsyncProgressWorkerBase<char> AsyncProgressWorker;

template<class T>
/* abstract */
class AsyncBareProgressQueueWorker : public AsyncBareProgressWorkerBase {
public:
    explicit AsyncBareProgressQueueWorker(
        Callback *callback_,
        const char* resource_name = "nan::AsyncBareProgressQueueWorker"
    ) : 
        AsyncBareProgressWorkerBase(callback_, resource_name) 
    {
    }

    virtual ~AsyncBareProgressQueueWorker() = default;

    class ExecutionProgress {
    friend class AsyncBareProgressQueueWorker;

    public:
        void Send(std::unique_ptr<T[]> messages, size_t nMessages) const {
            that_->SendProgress_(std::move(messages), nMessages);
        }

        NAN_DEPRECATED void Send(const T* messages, size_t nMessages) const {
            that_->SendProgress_(messages, nMessages);
        }

    private:
        explicit ExecutionProgress(AsyncBareProgressQueueWorker *that)
            : that_(that) {}

        NAN_DISALLOW_ASSIGN_COPY_MOVE(ExecutionProgress)
        AsyncBareProgressQueueWorker* const that_;
    };

    virtual void Execute(const ExecutionProgress& progress) = 0;
    virtual void HandleProgressCallback(const T *data, size_t size) = 0;

 private:
    void Execute() final {
        ExecutionProgress progress(this);
        Execute(progress);
    }

    virtual void SendProgress_(std::unique_ptr<T[]> messages, size_t nMessages) = 0;

    NAN_DEPRECATED virtual void SendProgress_(const T *messages, size_t nMessages) = 0;
};

template<class T>
/* abstract */
class AsyncProgressQueueWorker : public AsyncBareProgressQueueWorker<T> {
public:
    using ProgressMessageQueueType = 
    std::queue<
        std::pair<
            std::unique_ptr<T[], std::function<void(T*)>>,
            size_t
        >
    >;

    explicit AsyncProgressQueueWorker(
        Callback *callback_,
        const char* resource_name = "nan::AsyncProgressQueueWorker"
    ) : 
        AsyncBareProgressQueueWorker<T>(callback_) 
    {

    }

    virtual ~AsyncProgressQueueWorker() {
        //Just to be sure
        std::lock_guard lockGuard(queueLock_);

        //Copy and swap, the held data is automatically deleted by the smart pointers
        ProgressMessageQueueType().swap(progressMessageQueue_);
    }

    virtual void WorkComplete() override {
        WorkProgress();
        AsyncWorker::WorkComplete();
    }

    virtual void WorkProgress() override {
        std::unique_lock lock(queueLock_);

        while (!progressMessageQueue_.empty()) {
            auto& datapair = progressMessageQueue_.front();

            auto buf = std::move(datapair.first);
            auto nMessages = datapair.second;

            progressMessageQueue_.pop();
            lock.unlock();

            // Don't send progress events after we've already completed.
            if (this->callback) {
                this->HandleProgressCallback(buf.get(), nMessages);
            }
            
            //Delete the array held by the smtart pointer before we reacquire the lock in 
            //order to keep the time the queue is locked as low as possibles
            buf.reset();

            lock.lock();
        }

        //Lock will be automatically released
    }

private:
    virtual void SendProgress_(std::unique_ptr<T[]> messages, size_t nMessages) override {
        {
        std::lock_guard lockGuard(queueLock_);
        progressMessageQueue_.emplace(std::move(messages), nMessages);
        }

        uv_async_send(&this->async);
    }

    NAN_DEPRECATED virtual void SendProgress_(const T * const messages, const size_t nMessages) override {

        auto buffer = [&]() -> std::unique_ptr<T[], std::function<void(T*)>> {
            if(messages != nullptr) {
                //Allocate memory
                const size_t bufferSize = sizeof(T) * nMessages;
                void* rawBuf = std::aligned_alloc(alignof(T), bufferSize);
                if(!rawBuf) {
                    //No need to deallocate anything
                    throw std::bad_alloc();
                }

                auto messageBuf = static_cast<T*>(rawBuf);
                //Wrap the buffer into a unique_ptr to prevent memory leaks
                //We do not need to worry about std::unique_ptr's constructor to throw
                //and produce a memory leak, because it is noexcept!
                std::unique_ptr<T[], std::function<void(T*)>> messageBufferPtr(
                    messageBuf,
                    //Custom deleter!
                    [nMessages](T* ptr) {
                        //No need to wrap destroy_n in a try catch block
                        //If something goes wrong, std::terminate is called anyways
                        std::destroy_n(ptr, nMessages);
                        std::free(ptr);
                    }
                );

                //Copy the data to the uninitialised memory
                if constexpr(std::is_trivially_copyable_v<T>) {
                    //Use fast memcpy path
                    std::memcpy(rawBuf, messages, bufferSize);
                } else {
                    //Use the generic way which does not require T to be default constructible
                    std::uninitialized_copy_n(messages, nMessages, messageBuf);
                }

                return messageBufferPtr;
            } else {
                //Allow the user to send nullptrs if he desires to do so...
                //We need to take special care because of memcpy and stuff
                return std::unique_ptr<T[]>(nullptr);
            }
        }();
        
        {
        std::lock_guard lockGuard(queueLock_);
        progressMessageQueue_.emplace(std::move(buffer), nMessages);
        }

        //Wake up the event loop!
        uv_async_send(&this->async);
    }

    std::mutex queueLock_;
    ProgressMessageQueueType progressMessageQueue_;
};

inline void AsyncExecute(uv_work_t* req) {
    AsyncWorker *worker = static_cast<AsyncWorker*>(req->data);
    worker->Execute();
}

inline void AsyncExecuteComplete(uv_work_t* req) {
    AsyncWorker* worker = static_cast<AsyncWorker*>(req->data);
    worker->WorkComplete();
    worker->Destroy();
}

inline void AsyncQueueWorker(AsyncWorker* worker) {
    uv_queue_work(
        GetCurrentEventLoop(), 
        &worker->request, 
        AsyncExecute, 
        reinterpret_cast<uv_after_work_cb>(AsyncExecuteComplete)
    );
}

inline void LaunchAsyncWorker(AsyncWorker* worker) {
    AsyncQueueWorker(worker);
}