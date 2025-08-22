{
  "targets": [
    {
      "target_name": "node_libraw_binding",
      "sources": [
        "./src/index.cpp",
        "./src/libraw_wrapper.cpp",
        "./src/wraptypes.cpp"
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        "<!@(node -e \"process.stdout.write((process.env.PREFIX || '/usr/local') + '/include')\")"
      ],
      "cxxflags": ["-std=c++17", "-stdlib=libc++"],
      "cflags!": ["-fno-exceptions"],
      "cflags_cc!": ["-fno-exceptions"],
      "conditions": [
        ['OS=="mac"', {
          'xcode_settings': {
            'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
            'CLANG_CXX_LANGUAGE_STANDARD': 'c++17',
            'CLANG_CXX_LIBRARY': 'libc++'
          }
        }]
      ],
      "libraries": [
        "<!@(node -e \"process.stdout.write((process.env.PREFIX || '/usr/local') + '/lib/libraw_r.a')\")",
        "<!@(node -e \"process.stdout.write((process.env.PREFIX || '/usr/local') + '/lib/libjpeg.a')\")"
      ],
    }
  ]
}
