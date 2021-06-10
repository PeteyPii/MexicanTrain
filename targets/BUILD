package(default_visibility = ["//visibility:public"])

cc_binary(
    name = "test_ais",
    srcs = ["test_ais.cpp"],
    deps = [
        "//:game",
        "//:game_settings_wrapper",
        "//:stat_tracker",
        "//ai:greedy_player_ai",
        "//ai:human_player_ai",
        "//ai:random_player_ai",
        "//ai:smart_player_ai",
    ],
)
