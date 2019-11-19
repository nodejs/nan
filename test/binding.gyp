{
  "target_defaults":
    {
        "cflags" : ["-Wall", "-Wextra", "-Wno-unused-parameter"],
        "defines": [ "V8_DEPRECATION_WARNINGS=1" ],
        "include_dirs": ["<!(node -e \"require('..')\")"]
    },
  "targets": [
    {
        "target_name" : "symbols"
      , "sources"     : [ "cpp/symbols.cpp" ]
    }
  , {
        "target_name" : "strings"
      , "sources"     : [ "cpp/strings.cpp" ]
    }
  , {
        "target_name" : "multifile"
      , "sources"     : [ "cpp/multifile1.cpp", "cpp/multifile2.cpp" ]
    }
  , {
        "target_name" : "news"
      , "sources"     : [ "cpp/news.cpp" ]
    }
  , {
        "target_name"         : "morenews"
      , "win_delay_load_hook" : "false"
      , "sources"             : [ "cpp/morenews.cpp" ]
    }
  , {
        "target_name" : "returnvalue"
      , "sources"     : [ "cpp/returnvalue.cpp" ]
    }
  , {
        "target_name" : "returnundefined"
      , "sources"     : [ "cpp/returnundefined.cpp" ]
    }
  , {
        "target_name" : "returnnull"
      , "sources"     : [ "cpp/returnnull.cpp" ]
    }
  , {
        "target_name" : "returnemptystring"
      , "sources"     : [ "cpp/returnemptystring.cpp" ]
    }
  , {
        "target_name" : "asyncprogressqueueworker"
      , "sources"     : [ "cpp/asyncprogressqueueworker.cpp" ]
    }
  , {
        "target_name" : "asyncprogressqueueworkerstream"
      , "sources"     : [ "cpp/asyncprogressqueueworkerstream.cpp" ]
    }
  , {
        "target_name" : "asyncworker"
      , "sources"     : [ "cpp/asyncworker.cpp" ]
    }
  , {
        "target_name" : "accessors"
      , "sources"     : [ "cpp/accessors.cpp" ]
    }
  , {
        "target_name" : "accessors2"
      , "sources"     : [ "cpp/accessors2.cpp" ]
    }
  , {
        "target_name" : "persistent"
      , "sources"     : [ "cpp/persistent.cpp" ]
    }
  , {
        "target_name" : "weak"
      , "sources"     : [ "cpp/weak.cpp" ]
    }
  , {
        "target_name" : "weak2"
      , "sources"     : [ "cpp/weak2.cpp" ]
    }
  , {
        "target_name" : "bufferworkerpersistent"
      , "sources"     : [ "cpp/bufferworkerpersistent.cpp" ]
    }
  , {
        "target_name" : "asyncworkererror"
      , "sources"     : [ "cpp/asyncworkererror.cpp" ]
    }, {
        "target_name" : "settemplate"
      , "sources"     : [ "cpp/settemplate.cpp" ]
    }
    , {
        "target_name" : "makecallback"
      , "sources"     : [ "cpp/makecallback.cpp" ]
    }
    , {
        "target_name" : "maybe"
      , "sources"     : [ "cpp/maybe.cpp" ]
    }
    , {
        "target_name" : "asyncresource"
      , "sources"     : [ "cpp/asyncresource.cpp" ]
    }
    , {
        "target_name" : "callbackcontext"
      , "sources"     : [ "cpp/callbackcontext.cpp" ]
    }
    , {
        "target_name" : "isolatedata"
      , "sources"     : [ "cpp/isolatedata.cpp" ]
    }
    , {
        "target_name" : "gc"
      , "sources"     : [ "cpp/gc.cpp" ]
    }
    , {
        "target_name" : "nannew"
      , "sources"     : [ "cpp/nannew.cpp" ]
    }
    , {
        "target_name" : "asyncprogressworker"
      , "sources"     : [ "cpp/asyncprogressworker.cpp" ]
    }
    , {
        "target_name" : "asyncprogressworkerstream"
      , "sources"     : [ "cpp/asyncprogressworkerstream.cpp" ]
    }
    , {
        "target_name" : "asyncprogressworkersignal"
      , "sources"     : ["cpp/asyncprogressworkersignal.cpp"]
    }
    , {
        "target_name" : "nancallback"
      , "sources"     : [ "cpp/nancallback.cpp" ]
    }
    , {
        "target_name" : "threadlocal"
      , "sources"     : [ "cpp/threadlocal.cpp" ]
    }
    , {
        "target_name" : "objectwraphandle"
      , "sources"     : [ "cpp/objectwraphandle.cpp" ]
    }
    , {
        "target_name" : "error"
      , "sources"     : [ "cpp/error.cpp" ]
    }
    , {
        "target_name" : "namedinterceptors"
      , "sources"     : [ "cpp/namedinterceptors.cpp" ]
    }
    , {
        "target_name" : "indexedinterceptors"
      , "sources"     : [ "cpp/indexedinterceptors.cpp" ]
    }
    , {
        "target_name" : "converters"
      , "sources"     : [ "cpp/converters.cpp" ]
    }, {
        "target_name" : "buffer"
      , "sources"     : [ "cpp/buffer.cpp" ]
    }
    , {
        "target_name" : "trycatch"
      , "sources"     : [ "cpp/trycatch.cpp" ]
    }
    , {
        "target_name" : "wrappedobjectfactory"
      , "sources"     : [ "cpp/wrappedobjectfactory.cpp" ]
    }
    , {
        "target_name" : "setcallhandler"
      , "sources"     : [ "cpp/setcallhandler.cpp" ]
    }
    , {
        "target_name" : "typedarrays"
      , "sources"     : [ "cpp/typedarrays.cpp" ]
    }
    , {
        "target_name" : "private"
      , "sources"     : [ "cpp/private.cpp" ]
    }
    , {
        "target_name" : "parse"
      , "sources"     : [ "cpp/json-parse.cpp" ]
    }
    , {
        "target_name" : "stringify"
      , "sources"     : [ "cpp/json-stringify.cpp" ]
    }
    , {
        "target_name":"methodswithdata"
      , "sources"     :["cpp/methodswithdata.cpp"]
    }
]}
