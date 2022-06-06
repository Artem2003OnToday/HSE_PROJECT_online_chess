cc_library(
    name = "timer",
    srcs = [
        "timer.cpp",
    ],
    hdrs = [
        "timer.h",
    ],
    copts = [
        "-std=c++17",
        "-pthread"
    ],
    linkopts = ["-pthread"],
    deps = [
        "//include/button:button",
        "//include/text:text",
        "@sfmllib//:sfml-graphics",
        "@sfmllib//:sfml-system",
        "@sfmllib//:sfml-window",
    ],
    visibility = ["//visibility:public"],
)
