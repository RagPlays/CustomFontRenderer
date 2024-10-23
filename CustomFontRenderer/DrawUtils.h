#ifndef DRAWUTILS_H
#define DRAWUTILS_H

#include <vector>

#include "Structs.h"

class DrawUtils final
{
public:

	// Colors //
	static void SetColor(int r, int g, int b, int a = 255);
	static void SetColor(const Color4f& color);
	static void SetColor(float r, float g, float b, float a = 1.f);
	
	// Point //
	static void SetPointSize(int pointSize);
	static void SetPointSize(float pointSize);
	static void Point(int x, int y);
	static void Point(float x, float y);
	static void Point(const Point2i& point);
	static void Point(const Point2f& point);
	static void Points(const std::vector<Point2i>& points);
	static void Points(const std::vector<Point2f>& points);
	static void Points(const Point2i* pPoints, size_t nrPoints);
	static void Points(Point2f* pPoints, size_t nrPoints);

	// Lines //
	enum class DrawLinesType
	{
		STANDARD, // GL_LINES
		CONTINUES, // GL_LINE_STRIP
		LOOP // GL_LINE_LOOP
	};
	static void SetLineThickness(int thickness);
	static void SetLineThickness(float thickness);
	static void Line(int x1, int y1, int x2, int y2);
	static void Line(float x1, float y1, float x2, float y2);
	static void Line(const Point2i& p1, const Point2i& p2);
	static void Line(const Point2f& p1, const Point2f& p2);
	static void Line(const Linei& line);
	static void Line(const Linef& line);
	static void Lines(const std::vector<Linei>& lines);
	static void Lines(const std::vector<Linef>& lines);
	static void Lines(Linei* pLines, size_t nrLines);
	static void Lines(Linef* pLines, size_t nrLines);
	static void Lines(const std::vector<Point2i>& points, DrawLinesType type);
	static void Lines(const std::vector<Point2f>& points, DrawLinesType type);
	static void Lines(Point2i* pPoints, size_t nrPoints, DrawLinesType type);
	static void Lines(Point2f* pPoints, size_t nrPoints, DrawLinesType type);

	// Arcs //

	// Triangles //
	//static void Triangle(const Point2i& p1, const Point2i& p2, const Point2i& p3);
	//static void Triangle(const Point2f& p1, const Point2f& p2, const Point2f& p3);
	//static void FilledTriangle(const Point2i& p1, const Point2i& p2, const Point2i& p3);
	//static void FilledTriangle(const Point2f& p1, const Point2f& p2, const Point2f& p3);

	// Rectangles //
	//static void Rectangle(int x, int y, int width, int height);
	//static void Rectangle(float x, float y, float width, float height);
	static void Rectangle(const Recti& rect);
	//static void Rectangle(const Rectf& rect);
	//static void FilledRectangle(int x, int y, int width, int height);
	//static void FilledRectangle(float x, float y, float width, float height);
	//static void FilledRectanlge(const Recti& rect);
	//static void FilledRectanlge(const Rectf& rect);

	// Polygon //
	//static void Polygon(const std::vector<Point2i>& points, bool closed = true);
	//static void Polygon(const std::vector<Point2f>& points, bool closed = true);
	//static void Polygon(Point2i* pPoints, size_t nrPoints, bool closed = true);
	//static void Polygon(Point2f* pPoints, size_t nrPoints, bool closed = true);

};

#endif // !DRAWUTILS_H