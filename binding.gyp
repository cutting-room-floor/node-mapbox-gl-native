{
  "targets": [
    {
      "target_name": "addon",
      "sources": [
        "src/addon.cc",
        "src/myobject.cc",
        "src/addworker.cc",
      ],
      "include_dirs": [
        "include",
        "<!(node -e \"require('nan')\")",
      ],
    },
  ]
}
