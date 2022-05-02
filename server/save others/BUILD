load("@rules_proto_grpc//cpp:defs.bzl", "cpp_grpc_library")

package(default_visibility = ["//visibility:public"])

proto_library(
    name = "what",
    srcs = ["what.proto"],
    deps = [
        "@com_google_protobuf//:any_proto",
    ],
)

cpp_grpc_library(
    name = "grpc_what",
    protos = [":what"],
)

cc_binary(
    name = "client",
    srcs = ["client.cpp"],
    deps = [
        ":grpc_what",
    ],
)

cc_binary(
    name = "server",
    srcs = ["server.cpp"],
    deps = [
        ":grpc_what",
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
