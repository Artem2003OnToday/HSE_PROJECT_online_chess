load("@rules_proto_grpc//cpp:defs.bzl", "cpp_grpc_library", "cpp_proto_library")

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
    visibility = ["//visibility:public"],
)

cc_library(
    name = "client",
    hdrs = ["client.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":grpc_cpp_thing",
    ],
)

cc_binary(
    name = "server",
    srcs = ["server.cpp"],
    visibility = ["//visibility:public"],
    deps = [
        ":grpc_cpp_thing",
    ],
)
