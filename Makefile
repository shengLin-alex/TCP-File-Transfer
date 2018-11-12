.PHONY: clean dirs install

BIN=bin
SRC=src
TEST=test

all: dirs $(BIN)/tftc $(BIN)/tfts $(BIN)/benchmark

$(BIN)/tftc: $(SRC)/client.c $(SRC)/client.h
	gcc $< -o $@

$(BIN)/tfts: $(SRC)/server.c $(SRC)/server.h
	gcc $< -o $@

$(BIN)/benchmark: $(TEST)/benchmark.cpp
	g++ $< -std=c++11 -o $@ -lbenchmark -lpthread

dirs:
	mkdir -p $(SRC) $(TEST) $(BIN)

clean:
	rm -rf $(BIN)

install:
	cp $(BIN)/tftc $(BIN)/tfts /bin