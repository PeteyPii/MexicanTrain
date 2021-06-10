package(default_visibility = ["//visibility:public"])

cc_library(
    name = "greedy_player_ai",
    srcs = ["GreedyPlayerAI.cpp"],
    hdrs = ["GreedyPlayerAI.h"],
    deps = [
        ":logging_player_ai",
        "//:board",
        "//:common",
        "//:player",
        "//:rng",
        "//:stat_tracker",
    ],
)

cc_library(
    name = "human_player_ai",
    srcs = ["HumanPlayerAI.cpp"],
    hdrs = ["HumanPlayerAI.h"],
    deps = [
        ":random_player_ai",
        "//:board",
        "//:common",
        "//:player",
        "//:rng",
        "//:stat_tracker",
    ],
)

cc_library(
    name = "logging_player_ai",
    srcs = ["LoggingPlayerAI.cpp"],
    hdrs = ["LoggingPlayerAI.h"],
    deps = [
        "//:board",
        "//:common",
        "//:player",
        "//:player_ai",
    ],
)

cc_library(
    name = "random_player_ai",
    srcs = ["RandomPlayerAI.cpp"],
    hdrs = ["RandomPlayerAI.h"],
    deps = [
        ":logging_player_ai",
        "//:board",
        "//:common",
        "//:player",
        "//:rng",
        "//:stat_tracker",
    ],
)

cc_library(
    name = "smart_player_ai",
    srcs = ["SmartPlayerAI.cpp"],
    hdrs = ["SmartPlayerAI.h"],
    deps = [
        ":random_player_ai",
        "//:board",
        "//:common",
        "//:player",
        "//:rng",
        "//:stat_tracker",
        "//:train",
    ],
)
