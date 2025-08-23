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
  "/usr/local/include",
  "/opt/homebrew/include",
  # try to pick up any include flags pkg-config exposes for libraw/libjpeg
  "<!@(pkg-config --cflags-only-I libraw 2>/dev/null | sed -E 's/-I//g' || true)",
  "<!@(pkg-config --cflags-only-I libjpeg 2>/dev/null | sed -E 's/-I//g' || true)"
      ],
      "cflags!": ["-fno-exceptions"],
      "cflags_cc!": ["-fno-exceptions"],
      "conditions": [
        ['OS=="mac"', {
          'xcode_settings': {
            'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
          }
        }]
      ],
      # Use linker flags and pkg-config to discover libs; avoid hardcoded
      # absolute paths that may not exist on runners (e.g. /usr/local on macOS arm).
      "libraries": [
        "-L/opt/homebrew/lib",
        "-lraw_r",
        "-ljpeg",
        "<!@(pkg-config --libs libraw 2>/dev/null || true)",
        "<!@(pkg-config --libs libjpeg 2>/dev/null || true)"
      ],
    }
  ]
}
