package(default_visibility = ["//visibility:public"])

cc_library(
    name = "godot_headers",
    hdrs = glob(["godot-cpp/godot-headers/**/*.h"]),
    includes = ["godot-cpp/godot-headers/"],
)

cc_library(
    name = "godot_cpp",
    srcs = glob(["godot-cpp/src/**/*.cpp"]),
    hdrs = glob(["godot-cpp/include/**/*.hpp"]),
    includes = [
        "godot-cpp/include",
        "godot-cpp/include/core",
        "godot-cpp/include/gen",
    ],
    linkstatic = 1,
    deps = [
        ":godot_headers",
    ],
)

# cc_library(
#     name = "godot_app",
#     srcs = ["GodotInit.cpp"],
#     deps = [
#         ":godot_cpp",
#         ":godot_headers",
#     ],
# )

# genrule(
#     name = "gdignore",
#     outs = [".gdignore"],
#     cmd = "touch $(location .gdignore)",
# )

cc_binary(
    name = "godot_app",
    srcs = ["GodotInit.cpp"],
    data = [
        # ":gdignore",
    ],
    linkshared = 1,
    linkstatic = 1,
    deps = [
        ":godot_cpp",
    ],
)
