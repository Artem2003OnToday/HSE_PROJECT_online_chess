load("@com_github_grpc_grpc//bazel:cc_grpc_library.bzl", "cc_grpc_library")

proto_library(
    name = "what",
    srcs = ["what.proto"],
)

cc_proto_library(
    name = "cc_what",
    deps = [":what"],
)

cc_grpc_library(
    name = "grpc_what",
    srcs = [":what"],
    grpc_only = True,
    deps = [":cc_what"],
)

cc_binary(
    name = "client",
    srcs = ["client.cpp"],
    deps = [
        ":grpc_what",
        "@com_github_grpc_grpc//:grpc++",
    ],
)

cc_binary(
    name = "server",
    srcs = ["server.cpp"],
    deps = [
        ":grpc_what",
        "@com_github_grpc_grpc//:grpc++",
    ],
)

# cc_binary(
#     name = "main",
#     srcs = [
#         "main.cpp",
#     ],
#     visibility = ["//visibility:public"],
#     deps = [
#         "//include/game",
#         "@boost//:smart_ptr",
#         "@sfmllib//:sfml-all",
#     ],
# )
