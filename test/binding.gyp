{"targets": [
    {
        "target_name" : "symbols"
      , "sources"     : [ "cpp/symbols.cpp" ]
      , "include_dirs": [
            "<!(node -e \"require('..')\")"
        ]
    }
  , {
        "target_name" : "strings"
      , "sources"     : [ "cpp/strings.cpp" ]
      , "include_dirs": [
            "<!(node -e \"require('..')\")"
        ]
    }
  , {
        "target_name" : "optionvalues"
      , "sources"     : [ "cpp/optionvalues.cpp" ]
      , "include_dirs": [
            "<!(node -e \"require('..')\")"
        ]
    }
  , {
        "target_name" : "multifile"
      , "sources"     : [ "cpp/multifile1.cpp", "cpp/multifile2.cpp" ]
      , "include_dirs": [
            "<!(node -e \"require('..')\")"
        ]
    }
  , {
        "target_name" : "news"
      , "sources"     : [ "cpp/news.cpp" ]
      , "include_dirs": [
            "<!(node -e \"require('..')\")"
        ]
    }
  , {
        "target_name" : "morenews"
      , "sources"     : [ "cpp/morenews.cpp" ]
      , "include_dirs": [
            "<!(node -e \"require('..')\")"
        ]
    }
  , {
        "target_name" : "returnvalue"
      , "sources"     : [ "cpp/returnvalue.cpp" ]
      , "include_dirs": [
            "<!(node -e \"require('..')\")"
        ]
    }
  , {
        "target_name" : "returnundefined"
      , "sources"     : [ "cpp/returnundefined.cpp" ]
      , "include_dirs": [
            "<!(node -e \"require('..')\")"
        ]
    }
  , {
        "target_name" : "returnnull"
      , "sources"     : [ "cpp/returnnull.cpp" ]
      , "include_dirs": [
            "<!(node -e \"require('..')\")"
        ]
    }
  , {
        "target_name" : "returnemptystring"
      , "sources"     : [ "cpp/returnemptystring.cpp" ]
      , "include_dirs": [
            "<!(node -e \"require('..')\")"
        ]
    }
  , {
        "target_name" : "asyncworker"
      , "sources"     : [ "cpp/asyncworker.cpp" ]
      , "include_dirs": [
            "<!(node -e \"require('..')\")"
        ]
    }
  , {
        "target_name" : "settergetter"
      , "sources"     : [ "cpp/settergetter.cpp" ]
      , "include_dirs": [
            "<!(node -e \"require('..')\")"
        ]
    }
  , {
        "target_name" : "persistent"
      , "sources"     : [ "cpp/persistent.cpp" ]
      , "include_dirs": [
            "<!(node -e \"require('..')\")"
        ]
    }
  , {
        "target_name" : "weak"
      , "sources"     : [ "cpp/weak.cpp" ]
      , "include_dirs": [
            "<!(node -e \"require('..')\")"
        ]
    }
  , {
        "target_name" : "bufferworkerpersistent"
      , "sources"     : [ "cpp/bufferworkerpersistent.cpp" ]
      , "include_dirs": [
            "<!(node -e \"require('..')\")"
        ]
    }
  , {
        "target_name" : "asyncworkererror"
      , "sources"     : [ "cpp/asyncworkererror.cpp" ]
      , "include_dirs": [
            "<!(node -e \"require('..')\")"
        ]
    }, {
        "target_name" : "settemplate"
      , "sources"     : [ "cpp/settemplate.cpp" ]
      , "include_dirs": [
            "<!(node -e \"require('..')\")"
        ]
    }
    , {
        "target_name" : "makecallback"
      , "sources"     : [ "cpp/makecallback.cpp" ]
      , "include_dirs": [
            "<!(node -e \"require('..')\")"
        ]
    }
    , {
        "target_name" : "isolatedata"
      , "sources"     : [ "cpp/isolatedata.cpp" ]
      , "include_dirs": [
            "<!(node -e \"require('..')\")"
        ]
    }
    , {
        "target_name" : "gc"
      , "sources"     : [ "cpp/gc.cpp" ]
      , "include_dirs": [
            "<!(node -e \"require('..')\")"
        ]
    }
    , {
        "target_name" : "nannew"
      , "sources"     : [ "cpp/nannew.cpp" ]
      , "include_dirs": [
            "<!(node -e \"require('..')\")"
        ]
    }
	, {
	   "target_name" : "asyncprogressworker"
	  , "sources"     : [ "cpp/asyncprogressworker.cpp" ]
	  , "include_dirs": [
	        "<!(node -e \"require('..')\")"
	  ]
	}
]}
