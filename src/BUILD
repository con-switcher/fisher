

cc_binary(
    name = "main",
    srcs = [
        "main.cpp",
    ],
    deps = ["@libuv//:libuv-lib"],
)
cc_binary(
    name = "old",
    srcs = [
        "old.cpp",
    ],
)
cc_binary(
    name = "lwip",
    srcs = [
        "lwip.cpp",
    ],
    deps = [
        "@lwip//:lwip"
    ],
)

cc_binary(
    name = "share",
    srcs = [
        "share.c",
        "share.h",
        "share-main.c",
    ],
    deps = [
    ],
)