
#CXX = g++

EXE = chess
MAIN_DIR = ./
SOURCES = main.cpp
SOURCES += $(MAIN_DIR)/src/sources/pieces.cpp $(MAIN_DIR)/src/sources/zobristHash.cpp $(MAIN_DIR)/src/sources/repetitionHistory.cpp $(MAIN_DIR)/src/sources/move.cpp $(MAIN_DIR)/src/sources/position.cpp $(MAIN_DIR)/src/sources/psLegalMoveMaskGen.cpp $(MAIN_DIR)/src/sources/moveList.cpp $(MAIN_DIR)/src/sources/legalMoveGen.cpp $(MAIN_DIR)/src/sources/ai.cpp $(MAIN_DIR)/src/sources/openingBook.cpp $(MAIN_DIR)/src/sources/staticEvaluator.cpp $(MAIN_DIR)/src/sources/moveSorter.cpp $(MAIN_DIR)/src/sources/entry.cpp $(MAIN_DIR)/src/sources/transpositionTable.cpp $(MAIN_DIR)/src/sources/logToFile.cpp $(MAIN_DIR)/src/sources/timer.cpp $(MAIN_DIR)/src/sources/game.cpp
OBJS = $(addsuffix .o, $(basename $(notdir $(SOURCES))))
UNAME_S := $(shell uname -s)

CXXFLAGS = -std=c++23 -I$(MAIN_DIR)/src/sources -I$(MAIN_DIR)/src/headers
CXXFLAGS += -g -Wall
LIBS =


##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------

%.o:$(MAIN_DIR)/src/sources/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:$(MAIN_DIR)/src/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: $(EXE)
	@echo Build complete!

$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

clean:
	rm -f $(EXE) $(OBJS)
