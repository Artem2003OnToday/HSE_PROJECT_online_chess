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
        "-lglfw",
        "-lGLEW",
        "-lGLU",
        "-lGL",
        "-lglut",
        "-lsfml-graphics",
        "-lsfml-window",
        "-lsfml-system",
    ],
    data = [
        ":bb",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//include/game:game",
        "@sfmllib//:sfml-graphics",
        "@sfmllib//:sfml-system",
        "@sfmllib//:sfml-window",
        # "@postgresql//:all_postgresql",
    ],
)

filegroup(
    name = "bb",
    srcs = glob([
        "*.png",
        "*.txt"
    ]),
    visibility = ["//visibility:public"],
)