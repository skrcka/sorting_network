all:
	g++ main.cpp sorting_network.cpp -o run -pthread -g -fsanitize=address