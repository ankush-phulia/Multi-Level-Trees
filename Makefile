CC = g++
CXXFLAGS = -std=c++11 -fpermissive
GLFLAGS = -lGL -lm -lglfw -lGLEW -lassimp

BUILD_TYPE = test
ifeq ($(BUILD_TYPE), debug)
	CXXFLAGS += -g
else
	CXXFLAGS += -O3
endif

compile :
	$(CC) main.cpp $(CXXFLAGS) $(GLFLAGS) -o main 

test :
	$(CC) test.cpp $(CXXFLAGS) $(GLFLAGS) -o test 

clean :
	rm main
