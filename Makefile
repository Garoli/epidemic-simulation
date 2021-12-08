CC=gcc
CFLAGS=-Wall -Wextra -ansi -pedantic -c
LDFLAGS=-lrt -pthread -lncurses
INCLUDE_PATH=./include
OBJ_PATH=./obj
SRC_PATH=./src
EXE_PATH=./bin
CPPFLAGS=-I$(INCLUDE_PATH)
MKDIR_P= mkdir -p
EXES=$(EXE_PATH)/epidemic_sim $(EXE_PATH)/timer $(EXE_PATH)/citizen_manager $(EXE_PATH)/press_agency

all: $(EXES)

$(EXE_PATH)/%: $(OBJ_PATH)/%.o
	$(MKDIR_P) $(@D)
	$(CC) $^ -o $@ $(LDFLAGS)

$(OBJ_PATH)/city.o: $(INCLUDE_PATH)/citizen.h
$(OBJ_PATH)/utils.o: $(INCLUDE_PATH)/city.h
$(OBJ_PATH)/display.o: $(INCLUDE_PATH)/utils.h
$(OBJ_PATH)/epidemic_sim.o: $(INCLUDE_PATH)/display.h
$(OBJ_PATH)/citizen_manager.o: $(INCLUDE_PATH)/city.h $(INCLUDE_PATH)/utils.h
$(OBJ_PATH)/press_agency.o: $(INCLUDE_PATH)/display.h
$(OBJ_PATH)/timer.o: $(INCLUDE_PATH)/utils.h

$(EXE_PATH)/citizen_manager: $(OBJ_PATH)/city.o $(OBJ_PATH)/citizen.o $(OBJ_PATH)/utils.o
$(EXE_PATH)/epidemic_sim: $(OBJ_PATH)/city.o $(OBJ_PATH)/citizen.o $(OBJ_PATH)/utils.o $(OBJ_PATH)/display.o
$(EXE_PATH)/press_agency: $(OBJ_PATH)/city.o $(OBJ_PATH)/citizen.o $(OBJ_PATH)/utils.o $(OBJ_PATH)/display.o
$(EXE_PATH)/timer: $(OBJ_PATH)/utils.o

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c $(INCLUDE_PATH)/%.h
	$(MKDIR_P) $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ $(LDFLAGS)

clean:
	if test -d $(OBJ_PATH); then rm -r $(OBJ_PATH); fi
	if test -d $(EXE_PATH); then rm -r $(EXE_PATH); fi
	if test -f evolution.txt; then rm -f evolution.txt; fi
ifeq (, $(shell which roundPipe))
	rm roundPipe
endif
ifeq (, $(shell which timerPipe))
	rm timerPipe
endif



mrproper:
	rm $(EXES)