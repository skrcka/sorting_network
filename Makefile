all:
	g++ main.cpp sorting_network.cpp -o run -pthread -g -fsanitize=address
nosan:
	g++ main.cpp sorting_network.cpp -o run -pthread -g
optimized:
	g++ main.cpp sorting_network.cpp -o run -pthread