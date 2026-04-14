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
      "cflags_cc": ["-std=c++17"],
      "xcode_settings": {
        "CLANG_CXX_LANGUAGE_STANDARD": "c++17",
        "CLANG_CXX_LIBRARY": "libc++",
        "MACOSX_DEPLOYMENT_TARGET": "11.0"
      },
      "conditions": [
        ["OS==\"mac\"", {
          "xcode_settings": {
            "GCC_ENABLE_CPP_EXCEPTIONS": "YES"
          },
          "include_dirs": [
            "/usr/local/include",
            "/opt/homebrew/include",
            "<!@(pkg-config --cflags-only-I libraw 2>/dev/null | sed -E 's/-I//g' || true)",
            "<!@(pkg-config --cflags-only-I libjpeg 2>/dev/null | sed -E 's/-I//g' || true)"
          ],
          "libraries": [
            "-L/opt/homebrew/lib",
            "-lraw_r",
            "-ljpeg",
            "<!@(pkg-config --libs libraw 2>/dev/null || true)",
            "<!@(pkg-config --libs libjpeg 2>/dev/null || true)"
          ]
        }],
        ["OS==\"linux\"", {
          "include_dirs": [
            "/usr/local/include",
            "<!@(pkg-config --cflags-only-I libraw 2>/dev/null | sed -E 's/-I//g' || true)",
            "<!@(pkg-config --cflags-only-I libjpeg 2>/dev/null | sed -E 's/-I//g' || true)"
          ],
          "libraries": [
            "-lraw_r",
            "-ljpeg",
            "<!@(pkg-config --libs libraw 2>/dev/null || true)",
            "<!@(pkg-config --libs libjpeg 2>/dev/null || true)"
          ]
        }],
        ["OS==\"win\"", {
          "msvs_settings": {
            "VCCLCompilerTool": {
              "ExceptionHandling": 1,
              "AdditionalOptions": ["/std:c++17"]
            }
          },
          "include_dirs": [
            "<!@(node -e \"var p=process.env.LIBRAW_PREFIX||'';if(p)console.log(p+'/include')\")"
          ],
          "libraries": [
            "<!@(node -e \"var p=process.env.LIBRAW_PREFIX||'';if(p)console.log(p+'/lib/raw.lib')\")",
            "<!@(node -e \"var p=process.env.LIBRAW_PREFIX||'';if(p)console.log(p+'/lib/jpeg.lib')\")"
          ]
        }]
      ]
    }
  ]
}
