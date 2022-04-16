load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "main",
    srcs = [
        "main.cpp",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//include/game:game",
        "@boost//:smart_ptr",
        "@sfmllib//:sfml-all",
    ],
)
