{"targets": [
    {
        "target_name" : "symbols"
      , "sources"     : [ "cpp/symbols.cpp" ]
      , "include_dirs": [
            "<!(node -e \"require('..')\")"
        ]
    }
  , {
        "target_name" : "fromv8string"
      , "sources"     : [ "cpp/fromv8string.cpp" ]
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
]}
