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
      "libraries": ["/usr/local/lib/libraw_r.dylib"],
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