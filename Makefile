.PHONY: app clean cleanall run test

CMD := g++
LIB := -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lboost_unit_test_framework
FLAGS := -std=c++17 -g
RM := rm -rf
SRC := src/
OBJ := obj/
BIN := ./
SRCS := $(shell find $(SRC) -type f -name "*.cpp")
OBJS := $(patsubst $(SRC)%.cpp,$(OBJ)%.o,$(SRCS))
APP := $(BIN)Chess
MKDIR = mkdir

TEST_SRC := tests/
TEST_SRCS := $(wildcard $(TEST_SRC)*.cpp)
TEST_OBJS := $(patsubst $(TEST_SRC)%.cpp,$(OBJ)%.o,$(TEST_SRCS))
TEST_APP := $(BIN)Test
TEST_OBJS += $(filter-out $(OBJ)Application/main.o, $(OBJS))


app: OBJS := $(filter-out $(OBJ)main_test.o, $(OBJS))  # Exclude main_test.o for app
app: $(APP)

$(APP): $(OBJS) | $(BIN)
	$(CMD) -o $@ $^ $(LIB) $(FLAGS)
	@echo "Finished app compilation"

$(OBJ)%.o: $(SRC)%.cpp | $(OBJ)
	$(MKDIR) -p $(@D)
	$(CMD) -o $@ -c $< $(FLAGS)
	@echo "Finished building object file for $<"

$(BIN) $(OBJ):
	$(MKDIR) $@

clean:
	$(RM) $(OBJ)*.o
	$(RM) $(OBJ)*/*.o
	@echo "Removed object files"

cleanall: clean
	$(RM) $(APP)
	@echo "Removed compiled file"

run: app
	@echo "Starting program"
	$(APP)

test: $(TEST_APP)
	@echo "Running tests"
	$(TEST_APP)

$(TEST_APP): $(TEST_OBJS)
	$(CMD) -o $@ $^ $(LIB) $(FLAGS)
	@echo "Finished test compilation"

$(OBJ)%.o: $(TEST_SRC)%.cpp | $(OBJ)
	$(MKDIR) -p $(@D)
	$(CMD) -o $@ -c $< $(FLAGS)
	@echo "Finished building object file for $<"
