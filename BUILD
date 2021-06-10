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

cc_binary(
    name = "godot_app",
    srcs = ["GodotInit.cpp"],
    linkshared = 1,
    linkstatic = 1,
    deps = [
        ":godot_cpp",
    ],
)

cc_library(
    name = "board",
    srcs = ["Board.cpp"],
    hdrs = ["Board.h"],
    deps = [
        ":common",
        ":game_settings_wrapper",
        ":identity_generator",
        ":player",
        ":rng",
        ":tile",
        ":train",
    ],
)

cc_library(
    name = "common",
    hdrs = ["Common.h"],
)

cc_library(
    name = "game_settings_wrapper",
    srcs = ["GameSettingsWrapper.cpp"],
    hdrs = ["GameSettingsWrapper.h"],
    deps = [
        ":common",
    ],
)

cc_library(
    name = "identity_generator",
    srcs = ["IdentityGenerator.cpp"],
    hdrs = ["IdentityGenerator.h"],
    deps = [
        ":common",
    ],
)

cc_library(
    name = "game",
    srcs = ["Game.cpp"],
    hdrs = ["Game.h"],
    deps = [
        ":common",
        ":game_settings_wrapper",
        ":player",
        ":player_ai",
        ":rng",
        ":round",
    ],
)

cc_library(
    name = "player",
    srcs = [
        "EnemyPlayer.cpp",
        "Player.cpp",
    ],
    hdrs = [
        "EnemyPlayer.h",
        "Player.h",
    ],
    deps = [
        ":common",
        ":identity_generator",
        ":tile",
    ],
)

cc_library(
    name = "player_ai",
    srcs = ["PlayerAI.cpp"],
    hdrs = ["PlayerAI.h"],
    deps = [
        ":board",
        ":common",
        ":player",
        ":stat_tracker",
    ],
)

cc_library(
    name = "rng",
    srcs = ["RNG.cpp"],
    hdrs = ["RNG.h"],
)

cc_library(
    name = "round",
    srcs = ["Round.cpp"],
    hdrs = ["Round.h"],
    deps = [
        ":board",
        ":common",
        ":game_settings_wrapper",
        ":player",
        ":player_ai",
        ":rng",
    ],
)

cc_library(
    name = "stat_tracker",
    srcs = ["StatTracker.cpp"],
    hdrs = ["StatTracker.h"],
    deps = [
        ":common",
    ],
)

cc_library(
    name = "tile",
    srcs = ["Tile.cpp"],
    hdrs = ["Tile.h"],
    deps = [
        ":common",
        ":identity_generator",
    ],
)

cc_library(
    name = "train",
    srcs = ["Train.cpp"],
    hdrs = ["Train.h"],
    deps = [
        ":common",
        ":train_tile",
    ],
)

cc_library(
    name = "train_tile",
    srcs = ["TrainTile.cpp"],
    hdrs = ["TrainTile.h"],
    deps = [
        ":common",
        ":tile",
    ],
)
