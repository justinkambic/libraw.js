{
  "targets": [
    {
      "target_name": "libraw_wrapper",
      "cflags!": [ "-fno-exceptions",  ],
      "cflags_cc!": [ "-fno-exceptions"  ],
      "cflags_cc": ["-lraw", "-lm"],
      "sources": [
        "./src/index.cpp",
        "./src/libraw_wrapper.cpp",
        "./src/wraptypes.cpp"
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")"
      ],
      "libraries": ["/usr/local/lib/libraw_r.dylib"],
      "defines": [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
      "conditions": [
        ['OS=="mac"', {
          'xcode_settings': {
            'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
          }
        }]
      ]
    }
  ]
}