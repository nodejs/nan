{
  "target_defaults":
    {
        "cflags" : ["-Wall", "-Wextra"],
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
        "target_name" : "optionvalues"
      , "sources"     : [ "cpp/optionvalues.cpp" ]
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
        "target_name" : "morenews"
      , "sources"     : [ "cpp/morenews.cpp" ]
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
        "target_name" : "asyncworker"
      , "sources"     : [ "cpp/asyncworker.cpp" ]
    }
  , {
        "target_name" : "settergetter"
      , "sources"     : [ "cpp/settergetter.cpp" ]
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
        "target_name" : "nancallback"
      , "sources"     : [ "cpp/nancallback.cpp" ]
    }
    , {
        "target_name" : "threadlocal"
      , "sources"     : [ "cpp/threadlocal.cpp" ]
    }
    , {
        "target_name" : "converters"
      , "sources"     : [ "cpp/converters.cpp" ]
    }
]}
