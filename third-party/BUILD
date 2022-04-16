load("@rules_foreign_cc//foreign_cc:defs.bzl", "cmake")

filegroup(
    name = "sfmllib",
    srcs = glob(["**"]),
    visibility = ["//visibility:public"],
)

cmake(
    name = "sfml",
    lib_source = ":sfmllib",
    working_directory = "@SFML//include/SFML",
)
