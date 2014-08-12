{
  "targets": [
    {
      'target_name': 'action_before_build',
      'type': 'none',
      'copies': [
          {
            'files': [ 'vendor/mapbox-gl-native/build/Release/libmapboxgl.a' ],
            'destination': '<(SHARED_INTERMEDIATE_DIR)'
          }
      ]
    },
    {
      'target_name': '<(module_name)',
      'dependencies': [ 'action_before_build' ],
      "sources": [
        'src/node_mbgl.cpp',
        'src/render_tile.cpp',
        'src/tile_worker.cpp',
      ],
      "include_dirs": [
        'include',
        'vendor/mapbox-gl-native/include',
        "<!(node -e \"require('nan')\")",
      ],
      'libraries': [
        '<(SHARED_INTERMEDIATE_DIR)/libmapboxgl.a',
      ],
      'xcode_settings': {
          'MACOSX_DEPLOYMENT_TARGET': '10.8',
          'OTHER_CPLUSPLUSFLAGS': [
              '-Wshadow',
              '-std=c++11',
              '-stdlib=libc++',
              '-Wno-unused-variable',
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
