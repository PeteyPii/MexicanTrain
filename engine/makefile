# Taken from https://spin.atomicobject.com/2016/08/26/makefile-c-projects/.

BUILD_DIR ?= ./build
SRC_DIR ?= ./src
PROTO_DIR ?= ../common/src
TARGETS_DIR ?= ./src/targets

HEADERS := $(shell find $(SRC_DIR) -name '*.h')
PROTOS := $(shell find $(PROTO_DIR) -name '*.proto')
SRCS := $(shell find $(SRC_DIR) -name '*.cpp' -not -path '$(TARGETS_DIR)/**')
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o) $(PROTOS:$(PROTO_DIR)/%.proto=$(BUILD_DIR)/gen/%.pb.cc.o)
DEPS := $(OBJS:.o=.d)
TARGET_SRCS := $(shell find $(TARGETS_DIR) -name *.cpp)
TARGET_OBJS := $(TARGET_SRCS:%=$(BUILD_DIR)/%.o)
TARGET_EXECS := $(TARGET_SRCS:$(TARGETS_DIR)/%.cpp=$(BUILD_DIR)/%)

INC_DIRS := $(SRC_DIR) $(BUILD_DIR)/gen
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CXX := g++
CPP_FLAGS ?= $(INC_FLAGS) -MMD -MP -std=c++1z -Wall -O2
LD_LIBS := protobuf boost_system
LD_FLAGS ?= -L/usr/lib $(addprefix -l,$(LD_LIBS))

all: $(TARGET_EXECS)

$(TARGET_EXECS): $(BUILD_DIR)/%: $(BUILD_DIR)/$(TARGETS_DIR)/%.cpp.o $(OBJS)
	$(CXX) $^ -o $@ $(LD_FLAGS)

$(BUILD_DIR)/gen/%.pb.cc $(BUILD_DIR)/gen/%.pb.h: $(PROTO_DIR)/%.proto
	$(MKDIR_P) $(dir $@)
	protoc -I$(PROTO_DIR) --cpp_out=$(BUILD_DIR)/gen $<

$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPP_FLAGS) -c $< -o $@

$(BUILD_DIR)/gen/%.pb.cc.o: $(BUILD_DIR)/gen/%.pb.cc
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPP_FLAGS) -c $< -o $@

.PHONY: clean test game_server format

clean:
	$(RM) -r $(BUILD_DIR)

test: $(BUILD_DIR)/test_ais
	$(BUILD_DIR)/test_ais

game_server: $(BUILD_DIR)/game_server
	$(BUILD_DIR)/game_server

format:
	clang-format -i -style=file $(SRCS) $(TARGET_SRCS) $(HEADERS)

-include $(DEPS)

MKDIR_P ?= mkdir -p
