#pragma once

#include <string>
#include <vector>
#include <queue>
#include <cmath>
#include <unordered_map>
#include <functional>
#include <algorithm>
#include <octomap/octomap.h>
#include <chrono>
#include <memory>
#include <cstdlib> // EXIT_SUCCESS EXIT_FAILURE

// create hasher for point3d
namespace std {

	using namespace octomap;

	template <>
	struct hash<point3d>
	{
		typedef size_t result_type;
		typedef point3d argument_type;
		size_t operator()(const point3d& n) const;
	};

	size_t hash<point3d>::operator()(const point3d& n) const
	{
		return hash<float>()(n(0)) ^ hash<float>()(n(1)) ^ hash<float>()(n(2));
	}
}

namespace path_finding_
{
	using namespace octomap;

	class noncopyable
	{
	protected:
		noncopyable() {}
		~noncopyable() {}

	private:  // emphasize the following members are private
		noncopyable(const noncopyable&);
		noncopyable& operator=(const noncopyable&);
	};

	struct myGrid : noncopyable
	{
		// constructor
		explicit myGrid(std::shared_ptr<OcTree> p, unsigned s = 5)
			: ptree(p), state(s) {}

		// get id's neighbors
		std::vector<point3d>
			neighbors(const point3d &id) const
		{
			std::vector<point3d> result;

			if (state & 4) { get_neighbors(0, id, result); }
			if (state & 2) { get_neighbors(1, id, result); }
			if (state & 1) { get_neighbors(2, id, result); }

			return result;
		}
	protected:
		// 110: XY, 101: XZ, 011: YZ, 111: XYZ 
		unsigned state;
		std::shared_ptr<OcTree> ptree;

		inline void
			get_neighbors(const unsigned i, const point3d &id, std::vector<point3d> &ret) const
		{
			auto resolution = ptree->getResolution();
			point3d neighbor1(id);
			point3d neighbor2(id);
			neighbor1(i) += static_cast<float>(resolution);
			neighbor2(i) -= static_cast<float>(resolution);
			auto node1 = ptree->search(neighbor1);
			auto node2 = ptree->search(neighbor2);
			if (node1 && !ptree->isNodeOccupied(node1))
			{
				ret.push_back(neighbor1);
			}
			if (node2 && !ptree->isNodeOccupied(node2))
			{
				ret.push_back(neighbor2);
			}
		}
	};

	struct GridWithWeight : myGrid
	{
		// constructor
		explicit GridWithWeight(std::shared_ptr<OcTree> p, unsigned s = 5)
			: myGrid(p, s) {}

		inline float
			cost(const point3d &from, const point3d &to) const
		{
			auto node = ptree->search(to);
			if (node && !ptree->isNodeOccupied(node))
			{
				return 1.0f;
			}
			return 10.0f;
		}

		inline float
			distance(const point3d &from, const point3d &to) const
		{
			return static_cast<float>(
				point3d(to - from).norm_sq()
				);
		}
	};

	class path_finding : noncopyable
	{
	private:
		std::shared_ptr<OcTree> ptree;
		point3d start;
		point3d goal;
		point3d valid_goal;
		double deviation_ratio;
		float goal_deviation;
		std::chrono::time_point<std::chrono::steady_clock> time_start;
		std::chrono::time_point<std::chrono::steady_clock> time_stop;

		// check filename
		inline bool
			check_filename(const std::string &filename)
		{
			auto size = filename.size();
			if (size <= 3)
			{
				std::cout << "sorry, your .bt filename is incorrect.";
				exit(EXIT_FAILURE);
			}

			auto point_index = size - 3;
			if ('t' != filename[point_index + 2] ||
				'b' != filename[point_index + 1] ||
				'.' != filename[point_index] ||
				point_index != filename.find('.'))
			{
				std::cout << "sorry, your .bt filename is incorrect.";
				exit(EXIT_FAILURE);
			}

			return true;
		}

		// check equality with deviation
		inline bool
			point3d_equal(const point3d &a, const point3d &b)
		{
			point3d tmp = a - b;
			if (abs(tmp(0)) < goal_deviation &&
				abs(tmp(1)) < goal_deviation &&
				abs(tmp(2)) < goal_deviation)
			{
				return true;
			}
			return false;
		}

		// print took times
		inline void
			print_took_times()
		{
			time_stop = std::chrono::high_resolution_clock::now();
			std::cout << "took " << std::chrono::duration<double, std::milli>(time_stop - time_start).count() << " ms\n";
		}

		inline std::vector<point3d>
			get_path(const std::unordered_map<point3d, point3d> &came_from)
		{
			std::vector<point3d> path;

			// check
			if (came_from.cend() == came_from.find(valid_goal))
			{
				std::cout << start << " to " << goal << " path not found.\n";
			}
			else
			{
				path.push_back(valid_goal);
				auto current = valid_goal;
				while (!(current == start))
				{
					current = came_from.find(current)->second;
					path.push_back(current);
				}
				std::reverse(path.begin(), path.end());
			}

			print_took_times();
			return path;
		}
	public:
		path_finding(const std::string &filename,
			const point3d &point_start,
			const point3d &point_goal,
			const double &resolution_ratio)
			: start(point_start), goal(point_goal), 
			valid_goal(point_goal), deviation_ratio(resolution_ratio)
		{
			ptree = nullptr;
			set_filename(filename);
		}

		// some value set
		inline void set_filename(const std::string &filename)
		{
			if (check_filename(filename))
			{
				ptree = std::make_shared<OcTree>(filename);
				if (ptree)
				{
					goal_deviation = static_cast<float>(
						ptree->getResolution() * deviation_ratio
						);
				}
				else
				{
					std::cout << "create OcTree from " << filename << " failed.";
					exit(EXIT_FAILURE);
				}
			}
		}

		inline void set_deviation_ratio(const double &ratio)
		{
			if ((ratio != deviation_ratio) && ptree)
			{
				deviation_ratio = ratio;
				goal_deviation = static_cast<float>(
					ptree->getResolution() * deviation_ratio
					);
			}
		}

		inline void set_start(const point3d &s)
		{
			start = s;
		}

		inline void set_goal(const point3d &g)
		{
			goal = g;
			valid_goal = g;
		}

		// state : 110-XY, 101-XZ, 011-YZ, 111-XYZ 
		std::vector<point3d> breadth_first_search(unsigned state = 5);
		std::vector<point3d> Dijkstra_algorithm(unsigned state = 5);
		std::vector<point3d> greedy_best_first_search(unsigned state = 5);
		std::vector<point3d> a_star_algorithm(unsigned state = 5);
	};

	std::vector<point3d>
		path_finding::breadth_first_search(unsigned state)
	{
		using namespace std;
		time_start = std::chrono::high_resolution_clock::now();
		myGrid graph(ptree, state);
		queue<point3d> frontiers;
		unordered_map<point3d, point3d> came_from{ { start, start } };

		frontiers.push(start);
		valid_goal = goal;

		//scan graph
		while (!frontiers.empty())
		{
			auto current = frontiers.front();
			frontiers.pop();

			if (point3d_equal(current, goal))
			{
				valid_goal = current;
				break;
			}

			for (auto next : graph.neighbors(current))
			{
				if (came_from.cend() == came_from.find(next))
				{
					frontiers.push(next);
					came_from.emplace(next, current);
				}
			}
		}

		// get path
		return get_path(came_from);
	}

	std::vector<point3d>
		path_finding::Dijkstra_algorithm(unsigned state)
	{
		using namespace std;
		time_start = std::chrono::high_resolution_clock::now();
		GridWithWeight graph(ptree, state);
		priority_queue < pair<float, point3d>, std::vector<pair<float, point3d> >,
			std::greater<pair<float, point3d> > > frontiers;
		unordered_map<point3d, point3d> came_from{ { start, start } };
		unordered_map<point3d, float> cost_so_far{ { start, 0.0f } };

		frontiers.emplace(0.0f, start);
		point3d valid_goal(goal);

		//scan graph
		while (!frontiers.empty())
		{
			auto current = frontiers.top().second;
			frontiers.pop();

			if (point3d_equal(current, goal))
			{
				valid_goal = current;
				break;
			}

			for (auto next : graph.neighbors(current))
			{
				auto new_cost = cost_so_far[current] + graph.cost(current, next);
				if (cost_so_far.cend() == cost_so_far.find(next) ||
					new_cost < cost_so_far[next])
				{
					frontiers.emplace(new_cost, next);
					came_from[next] = current;
					cost_so_far[next] = new_cost;
				}
			}
		}

		// get path
		return get_path(came_from);
	}

	std::vector<point3d>
		path_finding::greedy_best_first_search(unsigned state)
	{
		using namespace std;
		time_start = std::chrono::high_resolution_clock::now();
		GridWithWeight graph(ptree, state);
		priority_queue < pair<float, point3d>, std::vector<pair<float, point3d> >,
			std::greater<pair<float, point3d> > > frontiers;
		unordered_map<point3d, point3d> came_from{ { start, start } };

		frontiers.emplace(0.0f, start);
		point3d valid_goal(goal);

		// scan map
		while (!frontiers.empty())
		{
			auto current = frontiers.top().second;
			frontiers.pop();

			if (point3d_equal(current, goal))
			{
				valid_goal = current;
				break;
			}

			for (auto next : graph.neighbors(current))
			{
				if (came_from.cend() == came_from.find(next))
				{
					frontiers.emplace(graph.distance(next, goal), next);
					came_from.insert(make_pair(next, current));
				}
			}
		}

		// get path
		return get_path(came_from);
	}

	std::vector<point3d>
		path_finding::a_star_algorithm(unsigned state)
	{
		using namespace std;
		time_start = std::chrono::high_resolution_clock::now();
		GridWithWeight graph(ptree, state);
		priority_queue < pair<float, point3d>, std::vector<pair<float, point3d> >,
			std::greater<pair<float, point3d> > > frontiers;
		unordered_map<point3d, point3d> came_from{ { start, start } };
		unordered_map<point3d, float> cost_so_far{ { start, 0.0f } };

		frontiers.emplace(0.0f, start);
		point3d valid_goal(goal);

		// scan map
		while (!frontiers.empty())
		{
			auto current = frontiers.top().second;
			frontiers.pop();

			if (point3d_equal(current, goal))
			{
				valid_goal = current;
				break;
			}

			for (auto next : graph.neighbors(current))
			{
				auto new_cost = cost_so_far[current] + graph.cost(current, next);
				if (came_from.cend() == came_from.find(next) || new_cost < cost_so_far[next])
				{
					frontiers.emplace(new_cost + graph.distance(next, goal), next);
					came_from[next] = current;
					cost_so_far[next] = new_cost;
				}
			}
		}

		// get path
		return get_path(came_from);
	}
}