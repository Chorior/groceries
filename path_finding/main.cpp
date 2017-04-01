#include <iostream>
#include <fstream>
#include "path_finding.h"

inline void print_vector(const std::vector<octomap::point3d> &vec);

int main(int argc, char *argv[])
{
	if (2 != argc)
	{
		std::cout << "sorry, you should input a filename.";
		exit(EXIT_FAILURE);
	}

	using namespace path_finding_;

	point3d start(0.0f, 0.0f, 0.0f), goal(-1.812685013f, 0.0f, 2.187417746f);
	//point3d start(2.0f, 2.0f, 2.0f), goal(2.98f, 2.24f, 2.0f);
	path_finding pf(argv[1], start, goal, 0.3);
	//path_finding pf(std::make_shared<octomap::OcTree>(argv[1]), start, goal, 0.3);

	std::vector<point3d> paths[4];

	// state : 6-XY, 5-XZ, 3-YZ, 7-XYZ 
	const unsigned state = 5;
	std::cout << "breadth_first_search: ";
	swap(paths[0], pf.breadth_first_search(state));
	std::cout << "Dijkstra_algorithm: ";
	swap(paths[1], pf.Dijkstra_algorithm(state));
	std::cout << "greedy_best_first_search: ";
	swap(paths[2], pf.greedy_best_first_search(state));
	std::cout << "a_star_algorithm: ";
	swap(paths[3], pf.a_star_algorithm(state));

	std::cout << "breadth_first_search: \n";
	print_vector(paths[0]);
	std::cout << "Dijkstra_algorithm: \n";
	print_vector(paths[1]);
	std::cout << "greedy_best_first_search: \n";
	print_vector(paths[2]);
	std::cout << "a_star_algorithm: \n";
	print_vector(paths[3]);
}

void print_vector(const std::vector<octomap::point3d> &vec)
{
	std::ofstream out("result.txt", std::ofstream::binary);
	for (auto &e : vec)
	{
		out << e << std::ends;
	}
	out << std::endl;
}

