load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

_RULES_BOOST_COMMIT = "652b21e35e4eeed5579e696da0facbe8dba52b1f"

http_archive(
    name = "com_github_nelhage_rules_boost",
    sha256 = "c1b8b2adc3b4201683cf94dda7eef3fc0f4f4c0ea5caa3ed3feffe07e1fb5b15",
    strip_prefix = "rules_boost-%s" % _RULES_BOOST_COMMIT,
    urls = [
        "https://github.com/nelhage/rules_boost/archive/%s.tar.gz" % _RULES_BOOST_COMMIT,
    ],
)

load("@com_github_nelhage_rules_boost//:boost/boost.bzl", "boost_deps")

boost_deps()

http_archive(
    name = "rules_foreign_cc",
    sha256 = "bcd0c5f46a49b85b384906daae41d277b3dc0ff27c7c752cc51e43048a58ec83",
    strip_prefix = "rules_foreign_cc-0.7.1",
    url = "https://github.com/bazelbuild/rules_foreign_cc/archive/0.7.1.tar.gz",
)

load("@rules_foreign_cc//foreign_cc:repositories.bzl", "rules_foreign_cc_dependencies")

rules_foreign_cc_dependencies()

local_repository(
    name = "sfmllib",
    path = "third-party/SFML",
)
