{
  "targets": [
    {
      "target_name": "<(module_name)",
      "sources": [
        "src/node_mbgl.cpp",
        "src/map.cpp",
        "src/render_worker.cpp",
      ],
      "include_dirs": [
        "include",
        "<!(node -e \"require('nan')\")",
        "<!(mbgl-config --includedir)",
      ],
      'libraries': [
        '<!@(mbgl-config --libs)',
      ],
      'xcode_settings': {
          'MACOSX_DEPLOYMENT_TARGET': '10.8',
          'OTHER_CPLUSPLUSFLAGS': [
              '-Wshadow',
              '-std=c++11',
              '-stdlib=libc++',
              '-Wno-unused-variable',
          ],
          'OTHER_LDFLAGS': [
              '-Wl, -bind_at_load'
          ],
          'GCC_ENABLE_CPP_RTTI': 'YES',
          'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
      },
      'cflags_cc!': ['-fno-rtti', '-fno-exceptions'],
      'cflags_cc' : ['-std=c++11','-Wshadow'],
      'cflags_c' : ['-std=c99']
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
