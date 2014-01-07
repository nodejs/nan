{"targets": [
    {
        "target_name" : "symbols"
      , "sources"     : [ "cpp/symbols.cpp" ]
      , "cflags"      : [ "-Wno-unused-local-typedefs" ]
      , "include_dirs": [
            "<!(node -e \"require('..')\")"
        ]
    }
  , {
        "target_name" : "fromv8string"
      , "sources"     : [ "cpp/fromv8string.cpp" ]
      , "cflags"      : [ "-Wno-unused-local-typedefs" ]
      , "include_dirs": [
            "<!(node -e \"require('..')\")"
        ]
    }
  , {
        "target_name" : "optionvalues"
      , "sources"     : [ "cpp/optionvalues.cpp" ]
      , "cflags"      : [ "-Wno-unused-local-typedefs" ]
      , "include_dirs": [
            "<!(node -e \"require('..')\")"
        ]
    }
  , {
        "target_name" : "multifile"
      , "sources"     : [ "cpp/multifile1.cpp", "cpp/multifile2.cpp" ]
      , "cflags"      : [ "-Wno-unused-local-typedefs" ]
      , "include_dirs": [
            "<!(node -e \"require('..')\")"
        ]
    }
  , {
        "target_name" : "returnvalue"
      , "sources"     : [ "cpp/returnvalue.cpp" ]
      , "cflags"      : [ "-Wno-unused-local-typedefs" ]
      , "include_dirs": [
            "<!(node -e \"require('..')\")"
        ]
    }
  , {
        "target_name" : "returnundefined"
      , "sources"     : [ "cpp/returnundefined.cpp" ]
      , "cflags"      : [ "-Wno-unused-local-typedefs" ]
      , "include_dirs": [
            "<!(node -e \"require('..')\")"
        ]
    }
  , {
        "target_name" : "returnnull"
      , "sources"     : [ "cpp/returnnull.cpp" ]
      , "cflags"      : [ "-Wno-unused-local-typedefs" ]
      , "include_dirs": [
            "<!(node -e \"require('..')\")"
        ]
    }
  , {
        "target_name" : "returnemptystring"
      , "sources"     : [ "cpp/returnemptystring.cpp" ]
      , "cflags"      : [ "-Wno-unused-local-typedefs" ]
      , "include_dirs": [
            "<!(node -e \"require('..')\")"
        ]
    }
  , {
        "target_name" : "asyncworker"
      , "sources"     : [ "cpp/asyncworker.cpp" ]
      , "cflags"      : [ "-Wno-unused-local-typedefs" ]
      , "include_dirs": [
            "<!(node -e \"require('..')\")"
        ]
    }
  , {
        "target_name" : "settergetter"
      , "sources"     : [ "cpp/settergetter.cpp" ]
      , "cflags"      : [ "-Wno-unused-local-typedefs" ]
      , "include_dirs": [
            "<!(node -e \"require('..')\")"
        ]
    }
  , {
        "target_name" : "persistent"
      , "sources"     : [ "cpp/persistent.cpp" ]
      , "cflags"      : [ "-Wno-unused-local-typedefs" ]
      , "include_dirs": [
            "<!(node -e \"require('..')\")"
        ]
    }
]}
