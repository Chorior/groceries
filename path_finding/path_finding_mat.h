#pragma once

#include <cmath>
#include <queue>
#include <chrono>
#include <vector>
#include <memory>
#include <climits>
#include <cassert>
#include <algorithm>
#include <functional>
#include <unordered_set>
#include <unordered_map>
#include <opencv2\opencv.hpp>

// create hasher for cv::Point
namespace std {
	template <>
	struct hash<cv::Point>
	{
		typedef size_t result_type;
		typedef cv::Point argument_type;
		size_t operator()(const cv::Point& n) const;
	};

	inline size_t hash<cv::Point>::operator()(const cv::Point& n) const
	{
		return hash<int>()(n.x) ^ hash<int>()(n.y);
	}
}

namespace path_finding_mat_
{
	struct noncopyable
	{
	protected:
		noncopyable() {};
		virtual ~noncopyable() {};

		noncopyable(const noncopyable&);
		noncopyable& operator=(const noncopyable&);
	};

	struct myGrid: noncopyable
	{
		// constructor
		explicit myGrid(std::shared_ptr<cv::Mat> map_)
			:m_map(map_)
		{}

		// get id's neighbors
		std::vector<cv::Point>
			neighbors(const cv::Point &id, bool free = true, const unsigned level = 1) const
		{
			std::vector<cv::Point> ret;

			if (level) {
				get_hv_neighbors(id, ret, free, level);
				get_slope_neighbors(id, ret, free, level);
			}
			else {
				ret.push_back(id);
			}

			return ret;
		}

		// id's minimal distance to obstacle
		inline unsigned
			min_dist2obs(const cv::Point &id, const unsigned radius) const
		{
			cv::Point p1(id.x - radius, id.y - radius);
			cv::Point p2(id.x + radius, id.y + radius);
			if (static_cast<int>(4 * radius * radius) == 
				cv::countNonZero((*m_map)(cv::Rect(p1, p2))))
			{
				return radius;
			}
			else {
				for (unsigned i = 1; i <= radius; ++i)
				{
					if (!no_obstacle(id, i)) {
						return i;
					}
				}
				return radius;
			}
		}
	protected:
		std::shared_ptr<cv::Mat> m_map;

		inline bool
			valid(const cv::Point &id) const
		{
			return id.x > 0 && id.y > 0
				&& id.x < m_map->cols && id.y < m_map->rows;
		}

		inline bool
			is_free(const cv::Point &id) const
		{			
			return valid(id) && 255 == m_map->at<uchar>(id);
		}

		inline void
			add_element(const cv::Point &id,
				std::vector<cv::Point> &ret,
				bool free) const
		{
			if (!free || is_free(id))
			{
				ret.push_back(id);
			}
		}

		inline void
			add_element(const cv::Point &id,
				const unsigned& offset_x,
				const unsigned& offset_y,
				std::vector<cv::Point> &ret,
				bool free) const
		{
			cv::Point neighbor1(id);
			cv::Point neighbor2(id);
			cv::Point neighbor3(id);
			cv::Point neighbor4(id);
			neighbor1.x += offset_x; neighbor1.y += offset_y;
			neighbor2.x -= offset_x; neighbor2.y -= offset_y;
			neighbor3.x += offset_x; neighbor3.y -= offset_y;
			neighbor4.x -= offset_x; neighbor4.y += offset_y;
			add_element(neighbor1, ret, free);
			add_element(neighbor2, ret, free);
			add_element(neighbor3, ret, free);
			add_element(neighbor4, ret, free);
		}

		inline void
			get_hv_neighbors(const cv::Point &id,
				std::vector<cv::Point> &ret,
				bool free,
				const unsigned level) const
		{			
			cv::Point neighbor1(id);
			cv::Point neighbor2(id);
			cv::Point neighbor3(id);
			cv::Point neighbor4(id);
			
			neighbor1.x += level;
			neighbor2.x -= level;
			neighbor3.y += level;
			neighbor4.y -= level;
			add_element(neighbor1, ret, free);
			add_element(neighbor2, ret, free);
			add_element(neighbor3, ret, free);
			add_element(neighbor4, ret, free);
		}

		inline void
			get_slope_neighbors(const cv::Point &id,
				std::vector<cv::Point> &ret,
				bool free,
				const unsigned level) const
		{			
			for (unsigned k = 1; k < level; ++k)
			{
				add_element(id, k, level, ret, free);
				add_element(id, level, k, ret, free);
			}
			add_element(id, level, level, ret, free);
		}

		inline bool
			no_hv_obstacle(const cv::Point &id,
				const unsigned level) const
		{			
			cv::Point neighbor1(id);
			cv::Point neighbor2(id);
			cv::Point neighbor3(id);
			cv::Point neighbor4(id);

			neighbor1.x += level;
			neighbor2.x -= level;
			neighbor3.y += level;
			neighbor4.y -= level;

			return is_free(neighbor1) && is_free(neighbor2)
				&& is_free(neighbor3) && is_free(neighbor4);
		}

		inline bool
			no_slope_obstacle(const cv::Point &id,
				const unsigned& offset_x,
				const unsigned& offset_y) const
		{
			cv::Point neighbor1(id);
			cv::Point neighbor2(id);
			cv::Point neighbor3(id);
			cv::Point neighbor4(id);
			neighbor1.x += offset_x; neighbor1.y += offset_y;
			neighbor2.x -= offset_x; neighbor2.y -= offset_y;
			neighbor3.x += offset_x; neighbor3.y -= offset_y;
			neighbor4.x -= offset_x; neighbor4.y += offset_y;

			return is_free(neighbor1) && is_free(neighbor2)
				&& is_free(neighbor3) && is_free(neighbor4);
		}

		inline bool
			no_slope_obstacle(const cv::Point &id,
				const unsigned level) const
		{			
			for (unsigned k = 1; k < level; ++k)
			{
				if (!no_slope_obstacle(id, k, level) ||
					!no_slope_obstacle(id, level, k))
				{
					return false;
				}
			}
			return no_slope_obstacle(id, level, level);
		}

		inline bool
			no_obstacle(const cv::Point &id,
				const unsigned level) const
		{
			return no_hv_obstacle(id, level) && no_slope_obstacle(id, level);
		}
	};

	struct GridWithWeight : myGrid // default public
	{
		// constructor
		explicit GridWithWeight(std::shared_ptr<cv::Mat> map_)
			: myGrid(map_) 
		{}

		inline float
			cost(const cv::Point &from, const cv::Point &to) const
		{			
			if(is_free(to))
			{
				cv::Point tmp(to - from);
				
				int total = abs(tmp.x) + abs(tmp.y);
				if (0 == total) { return 0.0f; }
				else if (1 == total) { return 1.0f; }
				else if (2 == total) { return 1.414f; }
			}
			return std::numeric_limits<float>::infinity();
		}

		inline float
			distance(const cv::Point &from, const cv::Point &to) const
		{
			int diff_x = abs(to.x - from.x);
			int diff_y = abs(to.y - from.y);

			int minus = std::min(diff_x, diff_y);
			return static_cast<float>(abs(diff_x - diff_y) + 1.414f * minus);
		}
	};

	struct path_finding_base : noncopyable
	{
	protected:
		std::shared_ptr<cv::Mat> m_map;
		cv::Mat m_dist_map;
		cv::Point m_start;
		cv::Point m_goal;

		std::chrono::time_point<std::chrono::high_resolution_clock> m_time_start;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_time_stop;

		// set start
		inline void
			set_start(const cv::Point &s)
		{
			m_start = s;
		}

		// set goal
		inline void
			set_goal(const cv::Point &g)
		{
			m_goal = g;
		}

		// print took times
		inline void
			print_took_times()
		{
			m_time_stop = std::chrono::high_resolution_clock::now();
			std::cout << "took " << std::chrono::duration<double, std::milli>(
					m_time_stop - m_time_start).count() << " ms\n";
		}

		explicit path_finding_base(const std::shared_ptr<cv::Mat> &map_)
		{
			set_map(map_);
		}

		explicit path_finding_base(const cv::Mat &map_)
		{
			set_map(map_);
		}

	public:
		inline void
			set_map(const std::shared_ptr<cv::Mat> &map_)
		{
#ifdef DEBUG
			assert(map_ && map_->type() == CV_8UC1);
#endif
			m_map = map_;
			cv::distanceTransform(*m_map, m_dist_map, CV_DIST_C, 3);
		}

		inline void
			set_map(const cv::Mat &map_)
		{
#ifdef DEBUG
			assert(!map_.empty() && map_.type() == CV_8UC1);
#endif
			m_map = std::make_shared<cv::Mat>(map_);
			cv::distanceTransform(*m_map, m_dist_map, CV_DIST_C, 3);
		}
	};

	class path_finding_mat : path_finding_base
	{
	private:
		cv::Point m_valid_goal;
		int m_goal_deviation;
		float m_safe_radius;

		// check equality with deviation
		inline bool
			equal_with_deviation(const cv::Point &a, const cv::Point &b)
		{
			cv::Point tmp(a - b);
			return abs(tmp.x) <= m_goal_deviation &&
				abs(tmp.y) <= m_goal_deviation;
		}

		// get path from map
		inline std::vector<cv::Point>
			get_path(const std::unordered_map<cv::Point, cv::Point> &came_from)
		{
			std::vector<cv::Point> path;

			// check
			if (came_from.cend() == came_from.find(m_valid_goal))
			{
				std::cerr << m_start << " to " << m_goal << " path not found.\n";
			}
			else
			{
				path.push_back(m_valid_goal);
				auto current = m_valid_goal;
				while (current != m_start)
				{
					current = came_from.find(current)->second;
					path.emplace_back(current);
				}
				std::reverse(path.begin(), path.end());
			}

			print_took_times();
			return path;
		}

	public:
		path_finding_mat(const std::shared_ptr<cv::Mat> &map_,			
			const int &goal_deviation_,
			const float safe_radius_ = 5.0f)
			: path_finding_base(map_), m_goal_deviation(goal_deviation_),
			m_safe_radius(safe_radius_)
		{}

		path_finding_mat(const cv::Mat &map_,
			const int &goal_deviation_,
			const float safe_radius_ = 5.0f)
			: path_finding_base(map_), m_goal_deviation(goal_deviation_),
			m_safe_radius(safe_radius_)
		{}

		inline cv::Mat
			get_dist_map() const
		{
			return m_dist_map;
		}

		inline void
			set_goal_deviation(const unsigned &goal_deviation_)
		{
			m_goal_deviation = goal_deviation_;
		}

		inline void
			set_safe_radius(const float &radius)
		{
			m_safe_radius = radius;
		}

		std::vector<cv::Point> breadth_first_search(const cv::Point &start_,const cv::Point &goal_);
		std::vector<cv::Point> Dijkstra_algorithm(const cv::Point &start_, const cv::Point &goal_);
		std::vector<cv::Point> greedy_best_first_search(const cv::Point &start_, const cv::Point &goal_);
		std::vector<cv::Point> a_star_algorithm(const cv::Point &start_, const cv::Point &goal_);
	};

	inline std::vector<cv::Point> 
		path_finding_mat::breadth_first_search(const cv::Point &start_, const cv::Point &goal_)
	{
		set_start(start_);
		set_goal(goal_);

		m_time_start = std::chrono::high_resolution_clock::now();
		const myGrid graph(m_map);
		std::queue<cv::Point> frontiers;
		std::unordered_map<cv::Point, cv::Point> came_from;

		//came_from.emplace(m_start, m_start);
		frontiers.push(m_start);
		m_valid_goal = m_goal;

		//scan graph
		while (!frontiers.empty())
		{
			auto current = frontiers.front();
			frontiers.pop();

			if (equal_with_deviation(current, m_goal))
			{
				m_valid_goal = current;
				break;
			}

			auto neighbors = graph.neighbors(current);
			for (const auto &next : neighbors)
			{
				auto min_dist2obs = m_dist_map.at<float>(next);
				if (m_safe_radius <= min_dist2obs &&
					came_from.cend() == came_from.find(next))
				{
					frontiers.push(next);
					came_from.emplace(next, current);
				}
			}
		}

		// get path
		return get_path(came_from);
	}

	inline std::vector<cv::Point>
		path_finding_mat::Dijkstra_algorithm(const cv::Point &start_, const cv::Point &goal_)
	{
		set_start(start_);
		set_goal(goal_);

		m_time_start = std::chrono::high_resolution_clock::now();
		const GridWithWeight graph(m_map);
		std::priority_queue< std::pair<float, cv::Point>, std::vector<std::pair<float, cv::Point> >,
			std::greater<std::pair<float, cv::Point> > > frontiers;
		std::unordered_map<cv::Point, cv::Point> came_from;
		std::unordered_map<cv::Point, float> cost_so_far;

		//came_from.emplace(m_start, m_start);
		cost_so_far.emplace(m_start, 0.0f);
		frontiers.emplace(0.0f, m_start);
		m_valid_goal = m_goal;

		//scan graph
		while (!frontiers.empty())
		{
			auto current = frontiers.top().second;
			frontiers.pop();

			if (equal_with_deviation(current, m_goal))
			{
				m_valid_goal = current;
				break;
			}

			auto neighbors = graph.neighbors(current);
			for (const auto &next : neighbors)
			{
				if (cost_so_far.cend() != cost_so_far.find(next)) {
					continue;
				}
				auto min_dist2obs = m_dist_map.at<float>(next);
				auto new_cost = cost_so_far[current] + graph.cost(current, next);
				if (m_safe_radius <= min_dist2obs)
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

	inline std::vector<cv::Point>
		path_finding_mat::greedy_best_first_search(const cv::Point &start_, const cv::Point &goal_)
	{
		set_start(start_);
		set_goal(goal_);

		m_time_start = std::chrono::high_resolution_clock::now();
		const GridWithWeight graph(m_map);
		std::priority_queue< std::pair<float, cv::Point>, std::vector<std::pair<float, cv::Point> >,
			std::greater<std::pair<float, cv::Point> > > frontiers;
		std::unordered_map<cv::Point, cv::Point> came_from;

		//came_from.emplace(m_start, m_start);
		frontiers.emplace(0.0f, m_start);
		m_valid_goal = m_goal;

		// scan map
		while (!frontiers.empty())
		{
			auto current = frontiers.top().second;
			frontiers.pop();

			if (equal_with_deviation(current, m_goal))
			{
				m_valid_goal = current;
				break;
			}

			auto neighbors = graph.neighbors(current);
			for (const auto &next : neighbors)
			{
				auto min_dist2obs = m_dist_map.at<float>(next);
				if (m_safe_radius <= min_dist2obs &&
					came_from.cend() == came_from.find(next))
				{
					float distance = graph.distance(next, m_goal);
					frontiers.emplace(distance, next);
					came_from.emplace(next, current);
				}
			}
		}

		// get path
		return get_path(came_from);
	}

	inline std::vector<cv::Point>
		path_finding_mat::a_star_algorithm(const cv::Point &start_, const cv::Point &goal_)
	{
		set_start(start_);
		set_goal(goal_);

		m_time_start = std::chrono::high_resolution_clock::now();
		const GridWithWeight graph(m_map);
		std::priority_queue< std::pair<float, cv::Point>, std::vector<std::pair<float, cv::Point> >,
			std::greater<std::pair<float, cv::Point> > > frontiers;
		std::unordered_map<cv::Point, cv::Point> came_from;
		std::unordered_map<cv::Point, float> cost_so_far;

		//came_from.emplace(m_start, m_start);
		cost_so_far.emplace(m_start, 0.0f);
		frontiers.emplace(0.0f, m_start);
		m_valid_goal = m_goal;

		// scan map
		while (!frontiers.empty())
		{
			auto current = frontiers.top().second;
			frontiers.pop();

			if (equal_with_deviation(current, m_goal))
			{
				m_valid_goal = current;
				break;
			}

			auto neighbors = graph.neighbors(current);
			for (const auto &next : neighbors)
			{
				if (cost_so_far.cend() != cost_so_far.find(next)) {
					continue;
				}
				auto min_dist2obs = m_dist_map.at<float>(next);
				auto new_cost = cost_so_far[current] + graph.cost(current, next);
				if (m_safe_radius <= min_dist2obs)
				{
					frontiers.emplace(
						new_cost + graph.distance(next, m_goal),
						next);
					came_from[next] = current;
					cost_so_far[next] = new_cost;
				}
			}
		}

		// get path
		return get_path(came_from);
	}

	class temp_goal : noncopyable
	{
		std::shared_ptr<cv::Mat> m_map;
		cv::Mat m_dist_map;
		std::unordered_set<cv::Point> m_history;

		void transform()
		{
			cv::distanceTransform(*m_map, m_dist_map, CV_DIST_C, 3);
		}

		std::pair<bool, cv::Point>
			neareast_temp_goal(const cv::Point &current, const cv::Point &goal)
		{	
			m_history.emplace(current);
			const GridWithWeight graph(m_map);
			std::priority_queue< std::pair<float, cv::Point>, std::vector<std::pair<float, cv::Point> >,
				std::greater<std::pair<float, cv::Point> > > options;

			auto neighbors = graph.neighbors(current);
			std::for_each(neighbors.begin(), neighbors.end(),
				[&](const cv::Point& p)
			{
				if (m_history.end() == m_history.find(p)) {
					auto min_dist2obs = m_dist_map.at<float>(p);
					auto cost = graph.distance(p, goal) + 1.0f / min_dist2obs * 100.0f;
					options.emplace(cost, p);
				}
			});

			// get result
			std::pair<bool, cv::Point> ret;
			ret.first = !options.empty();
			if (ret.first) {
				ret.second = options.top().second;
			}

			return ret;
		}

	public:
		explicit temp_goal(const std::shared_ptr<cv::Mat> &map_)
			: m_map(nullptr)
		{
			set_map(map_);
		}

		explicit temp_goal(const cv::Mat &map_)
			: m_map(nullptr)
		{
			set_map(map_);
		}

		temp_goal(const std::shared_ptr<cv::Mat> &map_, 
			const cv::Mat &dist_map_)
			: m_map(nullptr), m_dist_map(dist_map_)
		{
			set_map(map_, 0);
		}

		temp_goal(const cv::Mat &map_,
			const cv::Mat &dist_map_)
			: m_map(nullptr), m_dist_map(dist_map_)
		{
			set_map(map_, 0);
		}

		inline void
			set_map(const std::shared_ptr<cv::Mat> &map_, bool trans = 1)
		{
#ifdef DEBUG
			assert(map_ && map_->type() == CV_8UC1);
#endif
			m_map = map_;

			if (trans) {
				transform();
			}
		}

		inline void
			set_map(const cv::Mat &map_, bool trans = 1)
		{
#ifdef DEBUG
			assert(!map_.empty() && map_.type() == CV_8UC1);
#endif
			m_map = std::make_shared<cv::Mat>(map_);
			
			if (trans) {
				transform();
			}
		}

		std::pair<bool, cv::Point>
			operator()(const cv::Point &current, const cv::Point &goal, float valid_dist)
		{
			if (valid_dist < 0.0f) {
				return std::pair<bool, cv::Point>(false, cv::Point());
			}

			std::pair<bool, cv::Point> ret;			
			auto m_time_start = std::chrono::high_resolution_clock::now();

			const GridWithWeight graph(m_map);
			cv::Point temp_goal = current;
			float distance = 0.0f;
			while(distance < valid_dist)
			{
				auto neighbor_goal = neareast_temp_goal(temp_goal, goal);
				if (neighbor_goal.first)
				{
					temp_goal = neighbor_goal.second;

					distance = graph.distance(current, temp_goal);
				}
				else {					
					ret.first = false;
					return ret;
				}					
			}

			// print took times
			auto m_time_stop = std::chrono::high_resolution_clock::now();
			std::cout << "took "
				<< std::chrono::duration<double, std::milli>(
					m_time_stop - m_time_start).count() << " ms\n";

			// return result
			ret.first = true;
			ret.second = temp_goal;
			return ret;
		}
	};
}
