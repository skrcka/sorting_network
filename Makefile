all:
	g++ main.cpp sorting_network.cpp sorting_network_omp.cpp sorting_network_seq.cpp -o run -fopenmp -pthread -g -fsanitize=address
nosan:
	g++ main.cpp sorting_network.cpp sorting_network_omp.cpp sorting_network_seq.cpp -o run -fopenmp -pthread -g
optimized:
	g++ main.cpp sorting_network.cpp sorting_network_omp.cpp sorting_network_seq.cpp -o run -fopenmp -pthread