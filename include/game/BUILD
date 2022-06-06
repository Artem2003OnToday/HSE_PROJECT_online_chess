cc_library(
    name = "game",
    srcs = [
        "game.cpp",
    ],
    hdrs = [
        "game.h",
    ],
    copts = [
        "-std=c++17",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//server:client",
        "//include/piece_upgrade:piece_upgrade",
        "//include/start_menu:start_menu",
        "//include/main_menu:main_menu",
        "//include/local_base:local_base",
        "//include/timer:timer",
        "@sfmllib//:sfml-graphics",
        "@sfmllib//:sfml-window",
        "@sfmllib//:sfml-system",
    ],
)
