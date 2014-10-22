{
  'targets': [
    {
      'target_name': 'libmapboxgl',
      'type': 'none',
      'hard_dependency': 1,
      'direct_dependent_settings': {
        'conditions': [
          ['OS == "linux"', {
            'link_settings': {
              'libraries': [
                '-lX11',
                '-lGL',
                '../vendor/mapbox-gl-native/mapnik-packaging/osx/out/build-cpp11-libstdcpp-gcc-x86_64-linux/lib/libboost_regex.a',
              ],
            },
          }],
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
          }],
        ],
      },
      'actions': [
        {
          'action_name': 'build_mbgl',
          'inputs': [
            'vendor/mapbox-gl-native',
          ],
          'outputs': [
            'vendor/mapbox-gl-native/build/Release',
          ],
          'action': ['./scripts/build_mbgl.sh']
        }
      ]
    },
    {
      'target_name': '<(module_name)',
      'dependencies': [ 'libmapboxgl' ],
      'sources': [
        'src/node_mbgl.cpp',
        'src/render.cpp',
        'src/render_worker.cpp',
        'vendor/mapbox-gl-native/common/headless_display.cpp',
        'vendor/mapbox-gl-native/common/headless_view.cpp',
        'vendor/mapbox-gl-native/common/platform_default.cpp',
        'vendor/mapbox-gl-native/test/fixtures/fixture_log.cpp',
        'vendor/mapbox-gl-native/test/fixtures/fixture_request.cpp',
      ],
      'include_dirs': [
        'include',
        'vendor/mapbox-gl-native/include',
        '<!(node -e \"require(\'nan\')\")',
      ],
      'conditions': [
        ['OS=="linux"', {
          'include_dirs': [
            'vendor/mapbox-gl-native/mapnik-packaging/osx/out/build-cpp11-libstdcpp-gcc-x86_64-linux/include',
          ],
          'libraries': [
            '-L../vendor/mapbox-gl-native/build/Release/obj.target -lmapboxgl',
            '-L../vendor/mapbox-gl-native/mapnik-packaging/osx/out/build-cpp11-libstdcpp-gcc-x86_64-linux/lib -lpng16 -lz',
          ],
        }],
        ['OS=="mac"', {
          'include_dirs': [
            'vendor/mapbox-gl-native/mapnik-packaging/osx/out/build-cpp11-libcpp-universal/include',
          ],
          'libraries': [
            '-L../vendor/mapbox-gl-native/build/Release -lmapboxgl',
            '-L../vendor/mapbox-gl-native/mapnik-packaging/osx/out/build-cpp11-libcpp-universal/lib -lpng16 -lz',
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
