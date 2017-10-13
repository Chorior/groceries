#pragma once

#include "path_finding_mat.h"

class draw_path
{
	std::shared_ptr<cv::Mat> m_map;
	cv::Mat m_map_tmp;

	std::string m_win_name;

	static void on_mouse(int event, int x, int y, int flags, void* this_);
	void path_drawer(std::vector<cv::Point> paths);	
protected:
	// not defined
	draw_path();
	draw_path(const draw_path&);
	draw_path& operator=(const draw_path&);
public:
	explicit draw_path(const std::shared_ptr<cv::Mat> &map_)
		: m_map(nullptr)
	{
		set_map(map_);
	}

	explicit draw_path(const cv::Mat &map_)
		: m_map(nullptr)
	{
		set_map(map_);
	}

	inline void
		set_map(const std::shared_ptr<cv::Mat> &map_)
	{
		assert(map_ && map_->type() == CV_8UC1);
		m_map = map_;
	}

	inline void
		set_map(const cv::Mat &map_)
	{
		assert(!map_.empty() && map_.type() == CV_8UC1);
		m_map = std::make_shared<cv::Mat>(map_);		
	}

	inline void 
		show_map()
	{
		m_win_name = "map";
		cvNamedWindow(m_win_name.c_str());
		cv::setMouseCallback(m_win_name, on_mouse, this);

		if (m_map) {
			cv::imshow(m_win_name, *m_map);
			cv::waitKey(0);
		}
	}
};

inline void draw_path::on_mouse(int event, int x, int y, int flags, void* this_)
{
	static int path_finding_type;
	static unsigned safe_radius = 5;

	static cv::Point start;
	static cv::Point goal;

	switch (event)
	{
	case cv::EVENT_MOUSEWHEEL:
	{
		if (flags < 0) {
			path_finding_type = (++path_finding_type) % 4;
			std::cout << "path_finding_type: " << path_finding_type << std::endl;
		}
		else {
			safe_radius = (++safe_radius) % 10;
			std::cout << "safe_radius: " << safe_radius << std::endl;
		}
		break;
	}
	case cv::EVENT_LBUTTONDBLCLK:
	{
		start.x = x;
		start.y = y;
		break;
	}
	case cv::EVENT_RBUTTONDBLCLK:
	{
		goal.x = x;
		goal.y = y;

		draw_path *p = static_cast<draw_path*>(this_);
		assert(p);

		path_finding_mat_::path_finding_mat pf(p->m_map, 0);
		pf.set_safe_radius(static_cast<float>(safe_radius));

		std::vector<cv::Point> paths;
		switch (path_finding_type)
		{
		case 0:
			std::cout << "breadth_first_search ";
			std::swap(paths, pf.breadth_first_search(start, goal)); break;
		case 1:
			std::cout << "Dijkstra_algorithm ";
			std::swap(paths, pf.Dijkstra_algorithm(start, goal)); break;
		case 2:
			std::cout << "greedy_best_first_search ";
			std::swap(paths, pf.greedy_best_first_search(start, goal)); break;
		case 3:
			std::cout << "a_star_algorithm ";
			std::swap(paths, pf.a_star_algorithm(start, goal)); break;
		default:
			break;
		}

		if (paths.empty())
		{
			path_finding_mat_::temp_goal tg(p->m_map, pf.get_dist_map());
			auto ret = tg(start, goal, 10);
			if (ret.first) {
				std::swap(paths, pf.a_star_algorithm(start, ret.second));
			}			
		}

		if (paths.empty()) {
			std::cerr << "no path." << std::endl;
		}
		else {
			p->path_drawer(paths);
		}
		break;
	}
	default:
		break;
	}
}

inline void draw_path::path_drawer(std::vector<cv::Point> paths)
{
	cv::cvtColor(*m_map, m_map_tmp, CV_GRAY2BGR);
	for (int i = 0; i < static_cast<int>(paths.size()) - 1; i++)
	{
		cv::line(m_map_tmp, paths[i], paths[i + 1], cv::Scalar(0, 0, 255), 4);
	}

	cv::imshow(m_win_name, m_map_tmp);
}
