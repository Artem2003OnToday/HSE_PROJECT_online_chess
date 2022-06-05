cc_library(
    name = "button",
    srcs = [
        "button.cpp",
    ],
    hdrs = [
        "button.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "@sfmllib//:sfml-graphics",
        "@sfmllib//:sfml-system",
        "@sfmllib//:sfml-window",
    ],
)

cc_library(
    name = "user_lobby",
    srcs = [
        "user_lobby.cpp",
    ],
    hdrs = [
        "user_lobby.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":button",
        "@sfmllib//:sfml-graphics",
        "@sfmllib//:sfml-system",
        "@sfmllib//:sfml-window",
    ],
)

cc_library(
    name = "page_lobby",
    srcs = [
        "page_lobby.cpp",
    ],
    hdrs = [
        "page_lobby.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":user_lobby",
        "@sfmllib//:sfml-graphics",
        "@sfmllib//:sfml-system",
        "@sfmllib//:sfml-window",
    ],
)

cc_library(
    name = "player_session_main",
    srcs = ["local_main.cpp"],
    deps = [
        ":page_lobby",
        "@sfmllib//:sfml-graphics",
        "@sfmllib//:sfml-system",
        "@sfmllib//:sfml-window",
    ],
)
