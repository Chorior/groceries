// opencv2.core.types.hpp
template<typename _Tp> static inline
bool operator == (const Point_<_Tp>& a, const Point_<_Tp>& b)
{
    return a.x == b.x && a.y == b.y;
}

template<typename _Tp> static inline
bool operator > (const Point_<_Tp>& a, const Point_<_Tp>& b)
{
	return (a.x > b.x) ||
		(a.x == b.x && a.y > b.y);
}

template<typename _Tp> static inline
bool operator < (const Point_<_Tp>& a, const Point_<_Tp>& b)
{
	return !(a > b || a == b);
}
