CXX := g++
TARGET := schedule
HALIDEPATH := /mathworks/hub/3rdparty/R2022a/7412566/glnxa64/Halide
HEADER := -I $(HALIDEPATH)/include
HEADERTOOLS := -I $(HALIDEPATH)/tools
LIBPNG := -I/usr/include/libpng16
BIN = -L $(HALIDEPATH)/bin/

all: 1

1: $(TARGET).cpp
	$(CXX) $(TARGET).cpp -o $(TARGET) $(HEADER) $(HEADERTOOLS) $(LIBPNG) \
    $(BIN) -lHalide -lpng16 -ljpeg -lpthread -ldl -std=c++17

.phony: clean

clean:
	rm $(TARGET) || echo -n ""


