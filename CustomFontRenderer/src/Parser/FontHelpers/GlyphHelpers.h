#ifndef GLYPHHELPERS_H
#define GLYPHHELPERS_H

#include <memory>
#include <vector>
#include <glm/glm.hpp>

#include "Parser/FontHelpers/FontStructs.h"

std::vector<std::vector<GlyphPoint>> CreateContoursWithImpliedPoints(const GlyphData& glyph, float scale = 1.f)
{
	std::vector<std::vector<GlyphPoint>> contours{};
	contours.reserve(glyph.contourEndIndices.size());

	int contourStart{ 0 };

	for (uint16_t contourEnd : glyph.contourEndIndices)
	{
		// Extract the current contour's points
		const std::vector<GlyphPoint>& points{ glyph.points };
		const int contourLength{ contourEnd - contourStart + 1 };

		// Find the first on-curve point to use as the starting point
		int firstOnCurvePointIdx{};
		for (; firstOnCurvePointIdx < contourLength; ++firstOnCurvePointIdx)
		{
			if (points[contourStart + firstOnCurvePointIdx].onCurve) break;
		}

		// Create a new contour with implied points
		std::vector<GlyphPoint> newContour{};
		newContour.reserve(contourLength);
		for (int idx{}; idx <= contourLength; ++idx)
		{
			// Access the current and the next points in the contour
			const GlyphPoint& currentPoint{ points[contourStart + (idx + firstOnCurvePointIdx + 0) % contourLength] };
			const GlyphPoint& nextPoint{ points[contourStart + (idx + firstOnCurvePointIdx + 1) % contourLength] };

			// Add the current point to the new contour
			newContour.emplace_back(GlyphPoint{ static_cast<int>(currentPoint.x * scale), static_cast<int>(currentPoint.y * scale) , currentPoint.onCurve });

			// If consecutive points are both on or both off the curve, insert a midpoint
			if (currentPoint.onCurve == false && nextPoint.onCurve == false && idx < contourLength)
			{
				const glm::ivec2 midPoint{ glm::vec2{ (currentPoint.x + nextPoint.x) / 2.f, (currentPoint.y + nextPoint.y) / 2.f } * scale };
				newContour.emplace_back(GlyphPoint{ midPoint.x, midPoint.y, !currentPoint.onCurve });
			}
		}

		// Store the processed contour
		contours.emplace_back(newContour);
		contourStart = contourEnd + 1;
	}

	return contours;
}

GlyphBounds GetGlyphBounds(const GlyphData& glyph, float scale = 1.f)
{
	constexpr float antiAliasPadding{ 0.005f }; // If needed

	const float left{ glyph.xMin * scale };
	const float right{ glyph.xMax * scale };
	const float top{ glyph.yMax * scale };
	const float bottom{ glyph.yMin * scale };

	const glm::vec2 center{ glm::vec2{ left + right, top + bottom } * 0.5f };
	const glm::vec2 size{ glm::vec2{ right - left, top - bottom } };

	return GlyphBounds{ center, size };
}

#endif //!GLYPHHELPERS_H 