{
  'includes': [
    'config.gypi',
  ],
  'targets': [
    {
      'target_name': '<(module_name)',
      'sources': [
        'src/node_mbgl.cpp',
        'src/render.cpp',
        'src/render_worker.cpp',
        'vendor/mapbox-gl-native/test/fixtures/fixture_log.cpp',
        'vendor/mapbox-gl-native/test/fixtures/fixture_request.cpp',
      ],
      'include_dirs': [
        'include',
        '<!(mbgl-config --includedir)',
        '<!(node -e \"require(\'nan\')\")',
      ],
      'conditions': [
        ['OS=="linux"', {
          'include_dirs': [
            '-I/usr/include',
            '-I/usr/local/include',
            '<@(png_cflags)',
            '<@(curl_cflags)',
            '<@(zlib_cflags)',
            '<@(sqlite3_cflags)',
            '<@(glfw3_cflags)',
          ],
          'libraries': [
            '<!(mbgl-config --libs)',
            '<@(png_libraries)',
            '<@(curl_libraries)',
            '<@(zlib_libraries)',
            '<@(sqlite3_libraries)',
            '<@(glfw3_libraries)',
            '-lboost_regex',
          ],
        }],
        ['OS=="mac"', {
          'include_dirs': [
            '<@(png_cflags)',
            '<@(zlib_cflags)',
          ],
          'libraries': [
            '<!(mbgl-config --libs)',
            '<@(png_libraries)',
            '<@(zlib_libraries)',
          ],
        }],
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
