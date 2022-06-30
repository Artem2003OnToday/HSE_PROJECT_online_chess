load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "main",
    srcs = [
        "main.cpp",
    ],
    copts = [
        "-std=c++17",
        "-lpqxx",
        "-lpq",
        # "-lglfw",
        # "-lGLEW",
        # "-lGLU",
        # "-lGL",
        # "-lglut",
        # "-lsfml-graphics",
        # "-lsfml-window",
        # "-lsfml-system",
        "-pthread",
    ],
    linkopts = [
        "-pthread",
    ],
    data = [
        "//include/512h:png-files",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//include/game:game",
        "//include/page_lobby:page_lobby",
        "//include/timer:timer",
        "@sfmllib//:sfml-graphics",
        "@sfmllib//:sfml-system",
        "@sfmllib//:sfml-window",
        # "@postgresql//:all_postgresql",
    ],
)
