CXX=g++
CXXFLAGS=-lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_gfx -std=c++17 -Wall -pedantic -O3
MAIN = main
SRCS = fe_textbox.cpp fe_menu.cpp fe_keyboard_cursor.cpp fe_input_system.cpp fe_font.cpp fe_game.cpp fe_hexboard.cpp fe_hexcell.cpp main.cpp be_graph.cpp be_hex_ai.cpp be_hexboard.cpp be_kernel.cpp
OBJS = $(SRCS:.cpp=.o)

#all: $(MAIN)
#	@echo compiled
# $@ = output
# $< = the first one
%.o:%.cpp $(SRCS)
	$(CXX) -c -o $@ $< -O3

$(MAIN): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS)

clean:
	rm -f $(MAIN) *.o
	



