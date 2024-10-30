#include "DrawUtils.h"

#include <SDL2/SDL_opengl.h>

// COLORS //

void DrawUtils::SetColor(int r, int g, int b, int a)
{
	SetColor(Color4f{ r, g, b, a });
}

void DrawUtils::SetColor(const Color4f& color)
{
	SetColor(color.r, color.g, color.b, color.a);
}

void DrawUtils::SetColor(float r, float g, float b, float a)
{
	glColor4f(r, g, b, a);
}

// POINTS //

void DrawUtils::SetPointSize(int pointSize)
{
	glPointSize(static_cast<GLfloat>(pointSize));
}

void DrawUtils::SetPointSize(float pointSize)
{
	glPointSize(pointSize);
}

void DrawUtils::Point(int x, int y)
{
	glBegin(GL_POINT);
	{
		glVertex2i(x, y);
	}
	glEnd();
}

void DrawUtils::Point(float x, float y)
{
	glBegin(GL_POINT);
	{
		glVertex2f(x, y);
	}
	glEnd();
}

void DrawUtils::Point(const Point2i& point)
{
	glBegin(GL_POINT);
	{
		glVertex2i(point.x, point.y);
	}
	glEnd();
}

void DrawUtils::Point(const Point2f& point)
{
	glBegin(GL_POINT);
	{
		glVertex2f(point.x, point.y);
	}
	glEnd();
}

void DrawUtils::Points(const std::vector<Point2i>& points)
{
	glBegin(GL_POINTS);
	{
		for (size_t idx{ 0 }; idx < points.size(); ++idx)
		{
			const Point2i& point{ points[idx] };
			glVertex2i(point.x, point.y);
		}
	}
	glEnd();
}

void DrawUtils::Points(const std::vector<Point2f>& points)
{
	glBegin(GL_POINTS);
	{
		for (size_t idx{ 0 }; idx < points.size(); ++idx)
		{
			const Point2f& point{ points[idx] };
			glVertex2f(point.x, point.y);
		}
	}
	glEnd();
}

void DrawUtils::Points(const Point2i* pPoints, size_t nrPoints)
{
	glBegin(GL_POINTS);
	{
		for (int idx{ 0 }; idx < nrPoints; ++idx)
		{
			const Point2i& point{ pPoints[idx] };
			glVertex2i(point.x, point.y);
		}
	}
	glEnd();

	//// Enable the vertex array client state
	//glEnableClientState(GL_VERTEX_ARRAY);

	//// Tell OpenGL where the point data is
	//glVertexPointer(2, GL_FLOAT, 0, pPoints);  // Each vertex is 2 floats (x, y)

	//// Draw the points in one go using glDrawArrays
	//glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(nrPoints));

	//// Disable the vertex array state
	//glDisableClientState(GL_VERTEX_ARRAY);
}

void DrawUtils::Points(Point2f* pPoints, size_t nrPoints)
{
	glBegin(GL_POINTS);
	{
		for (int idx{ 0 }; idx < nrPoints; ++idx)
		{
			glVertex2f(pPoints[idx].x, pPoints[idx].y);
		}
	}
	glEnd();

	//// Enable the vertex array client state
	//glEnableClientState(GL_VERTEX_ARRAY);

	//// Tell OpenGL where the point data is
	//glVertexPointer(2, GL_INT, 0, pPoints);

	//// Draw the points in one go using glDrawArrays
	//glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(nrPoints));

	//// Disable the vertex array state
	//glDisableClientState(GL_VERTEX_ARRAY);
}

void DrawUtils::SetLineThickness(int thickness)
{
	glLineWidth(static_cast<GLfloat>(thickness));
}

void DrawUtils::SetLineThickness(float thickness)
{
	glLineWidth(thickness);
}

void DrawUtils::Line(int x1, int y1, int x2, int y2)
{
	glBegin(GL_LINES);
	{
		glVertex2i(x1, y1);
		glVertex2i(x2, y2);
	}
	glEnd();
}

void DrawUtils::Line(float x1, float y1, float x2, float y2)
{
	glBegin(GL_LINES);
	{
		glVertex2f(x1, y1);
		glVertex2f(x2, y2);
	}
	glEnd();
}

void DrawUtils::Line(const Point2i& p1, const Point2i& p2)
{
	glBegin(GL_LINES);
	{
		glVertex2i(p1.x, p1.y);
		glVertex2i(p2.x, p2.y);
	}
	glEnd();
}

void DrawUtils::Line(const Point2f& p1, const Point2f& p2)
{
	glBegin(GL_LINES);
	{
		glVertex2f(p1.x, p1.y);
		glVertex2f(p2.x, p2.y);
	}
	glEnd();
}

void DrawUtils::Line(const Linei& line)
{
	glBegin(GL_LINES);
	{
		const Point2i& p1{ line.pointOne };
		const Point2i& p2{ line.pointTwo };
		glVertex2i(p1.x, p1.y);
		glVertex2i(p2.x, p2.y);
	}
	glEnd();
}

void DrawUtils::Line(const Linef& line)
{
	glBegin(GL_LINES);
	{
		const Point2f& p1{ line.pointOne };
		const Point2f& p2{ line.pointTwo };
		glVertex2f(p1.x, p1.y);
		glVertex2f(p2.x, p2.y);
	}
	glEnd();
}

void DrawUtils::Lines(const std::vector<Linei>& lines)
{
	glBegin(GL_LINES);
	{
		for (const auto& line : lines)
		{
			const Point2i& p1{ line.pointOne };
			const Point2i& p2{ line.pointTwo };
			glVertex2i(p1.x, p1.y);
			glVertex2i(p2.x, p2.y);
		}
	}
	glEnd();
}

void DrawUtils::Lines(const std::vector<Linef>& lines)
{
	glBegin(GL_LINES);
	{
		for (const auto& line : lines)
		{
			const Point2f& p1{ line.pointOne };
			const Point2f& p2{ line.pointTwo };
			glVertex2f(p1.x, p1.y);
			glVertex2f(p2.x, p2.y);
		}
	}
	glEnd();
}

void DrawUtils::Lines(Linei* pLines, size_t nrLines)
{
	glBegin(GL_LINES);
	{
		for (int idx{ 0 }; idx < nrLines; ++idx)
		{
			const Linei& line{ pLines[idx] };
			const Point2i& p1{ line.pointOne };
			const Point2i& p2{ line.pointTwo };
			glVertex2i(p1.x, p1.y);
			glVertex2i(p2.x, p2.y);
		}
	}
	glEnd();
}

void DrawUtils::Lines(Linef* PLines, size_t nrLines)
{
	glBegin(GL_LINES);
	{
		for (int idx{ 0 }; idx < nrLines; ++idx)
		{
			const Linef& line{ PLines[idx] };
			const Point2f& p1{ line.pointOne };
			const Point2f& p2{ line.pointTwo };
			glVertex2f(p1.x, p1.y);
			glVertex2f(p2.x, p2.y);
		}
	}
	glEnd();
}

void DrawUtils::Lines(const std::vector<Point2i>& points, DrawLinesType type)
{
	switch (type)
	{
	case DrawUtils::DrawLinesType::STANDARD:
		break;
	case DrawUtils::DrawLinesType::CONTINUES:
		break;
	case DrawUtils::DrawLinesType::LOOP:
		break;
	default:
		break;
	}
}

void DrawUtils::Lines(const std::vector<Point2f>& points, DrawLinesType type)
{
	switch (type)
	{
	case DrawUtils::DrawLinesType::STANDARD:
		break;
	case DrawUtils::DrawLinesType::CONTINUES:
		break;
	case DrawUtils::DrawLinesType::LOOP:
		break;
	default:
		break;
	}
}

void DrawUtils::Lines(Point2i* pPoints, size_t nrPoints, DrawLinesType type)
{
	switch (type)
	{
	case DrawUtils::DrawLinesType::STANDARD:
		break;
	case DrawUtils::DrawLinesType::CONTINUES:
		break;
	case DrawUtils::DrawLinesType::LOOP:
		break;
	default:
		break;
	}
}

void DrawUtils::Lines(Point2f* pPoints, size_t nrPoints, DrawLinesType type)
{
	switch (type)
	{
	case DrawUtils::DrawLinesType::STANDARD:
		break;
	case DrawUtils::DrawLinesType::CONTINUES:
		break;
	case DrawUtils::DrawLinesType::LOOP:
		break;
	default:
		break;
	}
}

void DrawUtils::Rectangle(const Recti& rect)
{
	// Bottom left
	const Point2i point1{ rect.x, rect.y };
	// Top left
	const Point2i point2{ rect.x, rect.y + rect.height };
	// Top right
	const Point2i point3{rect.x + rect.width, rect.y + rect.height };
	// Bottom right
	const Point2i point4{ rect.x + rect.width, rect.y };

	Line(point1, point2);
	Line(point2, point3);
	Line(point3, point4);
	Line(point4, point1);
}
