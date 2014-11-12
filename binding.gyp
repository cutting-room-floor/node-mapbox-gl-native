{
  "targets": [
    {
      "target_name": "<(module_name)",
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
    {
      'target_name': 'action_after_build',
      'type': 'none',
      'dependencies': [ '<(module_name)' ],
      'copies': [
          {
            'files': [ '<(PRODUCT_DIR)/<(module_name).node' ],
            'destination': '<(module_path)'
          }
      ]
    }
  ]
}
