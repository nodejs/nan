{
  "targets": [
    {
      "target_name": "addon",
      "sources": [
        "addon.cc",
        "pi_est.cc",
        "sync.cc",
        "async.cc"
      ],
      "include_dirs": ["<!(node -e \"require('nan')\")"]
    }
  ]
}
