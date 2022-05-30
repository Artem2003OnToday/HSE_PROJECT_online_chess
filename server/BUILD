load("@rules_proto_grpc//cpp:defs.bzl", "cpp_grpc_library", "cpp_proto_library")

# cc_library(
#     name = "player_info",
#     srcs = "chess_match.cpp",
#     hdrs = "chess_match.h",
# )

proto_library(
    name = "thing_proto",
    srcs = ["thing.proto"],
    visibility = ["//visibility:public"],
    deps = ["@com_google_protobuf//:any_proto"],
)

cpp_proto_library(
    name = "cpp_thing_proto",
    protos = [":thing_proto"],
    visibility = ["//visibility:public"],
)

cpp_grpc_library(
    name = "grpc_cpp_thing",
    protos = [":thing_proto"],
)

cc_binary(
    name = "client",
    srcs = ["client.cpp"],
    deps = [
        ":grpc_cpp_thing",
    ],
)

cc_binary(
    name = "server",
    srcs = ["server.cpp"],
    deps = [
        ":grpc_cpp_thing",
    ],
)
