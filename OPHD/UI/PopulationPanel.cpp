#include "PopulationPanel.h"

#include "../Cache.h"
#include "../Common.h"
#include "../Constants.h"
#include "../Population/Population.h"

#include <NAS2D/Utility.h>
#include <NAS2D/Renderer/Renderer.h>

#include <algorithm>


using namespace NAS2D;


static constexpr int IconSize = 32;


static const auto trendIndex = [](int value)
{
	return (value >= 0) ? 0 : 1;
};

static const std::array trend
{
	Color{ 0, 185, 0 },
	Color::Red
};


static const auto moraleIndex = [](int val)
{
	return std::clamp(val, 0, 999) / 200;
};


static const std::array moraleStringColor
{
	Color::Red,
	Color::Orange,
	Color::Yellow,
	Color::White,
	Color{ 0, 185, 0 }
};


PopulationPanel::PopulationPanel() :
	mFont{ fontCache.load(constants::FONT_PRIMARY, constants::FONT_PRIMARY_NORMAL) },
	mFontBold{ fontCache.load(constants::FONT_PRIMARY_BOLD, constants::FONT_PRIMARY_NORMAL) },
	mIcons{ imageCache.load("ui/icons.png") },
	mSkin
	{
		imageCache.load("ui/skin/window_top_left.png"),
		imageCache.load("ui/skin/window_top_middle.png"),
		imageCache.load("ui/skin/window_top_right.png"),
		imageCache.load("ui/skin/window_middle_left.png"),
		imageCache.load("ui/skin/window_middle_middle.png"),
		imageCache.load("ui/skin/window_middle_right.png"),
		imageCache.load("ui/skin/window_bottom_left.png"),
		imageCache.load("ui/skin/window_bottom_middle.png"),
		imageCache.load("ui/skin/window_bottom_right.png")
	}
{
	constexpr int linesOfText = 14;
	constexpr int edgeBuffer = constants::MARGIN * 2;
	
	const int windowHeight = mFontBold.height() + (mFont.height() * linesOfText) + edgeBuffer;

	int largestStringLength = mFontBold.width(constants::MoraleBreakdown);
	for (int i = 0; i < moraleStringTableCount(); ++i)
	{
		const int lengthCompare = mFont.width(moraleString(i));
		if (lengthCompare > largestStringLength)
		{
			largestStringLength = lengthCompare;
		}
	}

	largestStringLength += edgeBuffer + mFont.width("999999");

	mPopulationPanelWidth = mFontBold.width(constants::PopulationBreakdown) + edgeBuffer;
	const int windowWidth = largestStringLength + edgeBuffer + mPopulationPanelWidth;


	size({ windowWidth, windowHeight });
}


void PopulationPanel::update()
{
	auto& renderer = Utility<Renderer>::get();
	mSkin.draw(renderer, mRect);

	const int fontHeight = mFont.height();
	const int fontBoldHeight = mFontBold.height();

	auto position = NAS2D::Point{ positionX() + constants::MARGIN, positionY() + constants::MARGIN };

	// POPULATION
	renderer.drawText(mFontBold, constants::PopulationBreakdown, position);
	const std::array populationData
	{
		std::tuple{NAS2D::Rectangle{0, 96, IconSize, IconSize}, mPopulation->size(Population::PersonRole::ROLE_CHILD), std::string("Children")},
		std::tuple{NAS2D::Rectangle{32, 96, IconSize, IconSize}, mPopulation->size(Population::PersonRole::ROLE_STUDENT), std::string("Students")},
		std::tuple{NAS2D::Rectangle{64, 96, IconSize, IconSize}, mPopulation->size(Population::PersonRole::ROLE_WORKER), std::string("Workers")},
		std::tuple{NAS2D::Rectangle{96, 96, IconSize, IconSize}, mPopulation->size(Population::PersonRole::ROLE_SCIENTIST), std::string("Scientists")},
		std::tuple{NAS2D::Rectangle{128, 96, IconSize, IconSize}, mPopulation->size(Population::PersonRole::ROLE_RETIRED), std::string("Retired")},
	};

	position.y += fontBoldHeight + constants::MARGIN;
	const auto textOffset = NAS2D::Vector{ IconSize + constants::MARGIN, (IconSize / 2) - (fontHeight / 2) };
	for (const auto& [imageRect, personCount, personRole] : populationData)
	{
		renderer.drawSubImage(mIcons, position, imageRect);
	
		const auto roleCount = std::to_string(personCount);
		renderer.drawText(mFont, personRole + ": ", position + textOffset);

		const NAS2D::Point<int> labelPosition = { positionX() + mPopulationPanelWidth - mFont.width(roleCount) - constants::MARGIN , position.y + textOffset.y };
		renderer.drawText(mFont, roleCount, labelPosition);
		position.y += IconSize + constants::MARGIN;
	}

	// DIVIDER LINE
	position = NAS2D::Point{ positionX() + mPopulationPanelWidth, positionY() + constants::MARGIN };
	renderer.drawLine(position, position + NAS2D::Vector<int>{ 0, rect().height - 10 }, Color::DarkGray);

	// MORALE
	position.x += constants::MARGIN;
	renderer.drawText(mFontBold, constants::MoraleBreakdown, position);

	position.y += fontBoldHeight;
	const auto moraleLevel = moraleIndex(mMorale);
	renderer.drawText(mFont, moraleString(Morale::Description) + moraleString(moraleLevel), position, moraleStringColor[moraleLevel]);

	position.y += fontHeight;
	renderer.drawText(mFont, "Current: " + std::to_string(mMorale) + " / Previous: " + std::to_string(mPreviousMorale), position);

	position.y += fontHeight;
	int capacityPercent = (mResidentialCapacity > 0) ? (mPopulation->size() * 100 / mResidentialCapacity) : 0;
	const auto housingText = "Housing: " + std::to_string(mPopulation->size()) + " / " + std::to_string(mResidentialCapacity) + "  (" + std::to_string(capacityPercent) + "%)";
	renderer.drawText(mFont, housingText, position, NAS2D::Color::White);

	position.y += fontHeight + fontHeight / 2;

	renderer.drawLine(position, position + NAS2D::Vector<int>{ rect().width - mPopulationPanelWidth - constants::MARGIN * 2, 0 }, Color::DarkGray);

	position.y += fontHeight / 2;	
	
	for (auto& item : mMoraleChangeReasons)
	{
		renderer.drawText(mFont, item.first, position);

		const auto text = formatDiff(item.second);
		const NAS2D::Point<int> labelPosition = { rect().x + rect().width - mFont.width(text) - 5 , position.y };

		renderer.drawText(mFont, text, labelPosition, trend[trendIndex(item.second)]);
		position.y += fontHeight;
	}
}
