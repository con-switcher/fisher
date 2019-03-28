package(default_visibility = ["//visibility:public"])

cc_import(
    name = "lwip-lib",
    static_library = "liblwip.a",
)

cc_library(
    name = "lwip",
    hdrs = glob([
        "include/**/*.h",
    ]),
    strip_include_prefix= "include",
    deps = [
        ":lwip-lib" ,
    ],
)