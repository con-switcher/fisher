package(default_visibility = ["//visibility:public"])

cc_library(
    name = "libuv-lib",
    srcs = [
        "out/libuv_a.a",
    ],
    hdrs = glob([
        "include/**/*.h",
    ]),
    strip_include_prefix = "include",
    linkopts = [
    ],
)