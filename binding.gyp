{
  'targets': [
    {
      'target_name': 'libmapboxgl',
      'type': 'none',
      'hard_dependency': 1,
      'direct_dependent_settings': {
        'conditions': [
          ['OS == "mac"', {
            'link_settings': {
              'libraries': [
                '-framework OpenGL',
              ],
            },
            'xcode_settings': {
              'OTHER_LDFLAGS': [
                '-framework OpenGL',
              ],
            },
          }, {
            'link_settings': {
              'libraries': [
                '-lX11',
                '-lGL',
                '../vendor/mapbox-gl-native/mapnik-packaging/osx/out/build-cpp11-libstdcpp-gcc-x86_64-linux/lib/libboost_regex.a',
              ],
            },
          }],
        ],
      },
      'actions': [
        {
          'action_name': 'make_mbgl',
          'inputs': [
            'vendor/mapbox-gl-native',
          ],
          'outputs': [
            'vendor/mapbox-gl-native/build/Release',
          ],
          'action': ['./scripts/make_mbgl.sh']
        }
      ]
    },
    {
      'target_name': '<(module_name)',
      'dependencies': [ 'libmapboxgl' ],
      'sources': [
        'src/node_mbgl.cpp',
        'src/render_tile.cpp',
        'vendor/mapbox-gl-native/common/headless_view.cpp',
        'vendor/mapbox-gl-native/common/platform_default.cpp',
        'vendor/mapbox-gl-native/test/fixtures/fixture_log.cpp',
        'vendor/mapbox-gl-native/test/fixtures/fixture_request.cpp',
      ],
      'include_dirs': [
        'include',
        'vendor/mapbox-gl-native/include',
        'vendor/mapbox-gl-native/mapnik-packaging/osx/out/build-cpp11-libcpp-universal/include',
        '<!(node -e \"require(\'nan\')\")',
      ],
      'libraries': [
        '<!@(pkg-config vendor/mapbox-gl-native/mapnik-packaging/osx/out/build-cpp11-libcpp-universal/lib/pkgconfig/libpng.pc --libs --static)',
      ],
      'conditions': [
        ['OS=="linux"', {
          'libraries': [
            '-L../vendor/mapbox-gl-native/build/Release/obj.target -lmapboxgl',
          ],
        }],
        ['OS=="mac"', {
          'libraries': [
            '-L../vendor/mapbox-gl-native/build/Release -lmapboxgl',
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
