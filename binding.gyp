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
        "<!@(node -p \"require('node-addon-api').include\")"
      ],
      "cflags!": ["-fno-exceptions"],
      "cflags_cc!": ["-fno-exceptions"],
      "conditions": [
        ['OS=="mac"', {
          "libraries": ["/usr/local/lib/libraw_r.dylib"],
          'xcode_settings': {
            'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
          }
        }],
        ['OS=="linux"', {
          "libraries": ["/usr/local/lib/libraw_r.so.19"],
        }]
      ]
    }
  ]
}