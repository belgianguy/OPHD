#include "ToolTip.h"

#include "../../Cache.h"
#include "../../Constants.h"

#include <NAS2D/EventHandler.h>
#include <NAS2D/Utility.h>
#include <NAS2D/Renderer/Renderer.h>


using namespace NAS2D;


ToolTip::ToolTip():
	mFont{ fontCache.load(constants::FONT_PRIMARY, constants::FONT_PRIMARY_NORMAL) }
{
	Utility<EventHandler>::get().mouseMotion().connect(this, &ToolTip::onMouseMove);
}


ToolTip::~ToolTip()
{
	Utility<EventHandler>::get().mouseMotion().disconnect(this, &ToolTip::onMouseMove);
}


void ToolTip::add(Control& c, const std::string& str)
{
	for (auto& item : mControls)
	{
		if (item.first == &c)
		{
			item.second = str;
			return;
		}
	}

	mControls.push_back(std::make_pair(&c, str));
}


void ToolTip::buildDrawParams(std::pair<Control*, std::string>& item, int mouseX)
{
	constexpr int padding = constants::MARGIN_TIGHT * 2;

	const int tooltipWidth = mFont.width(item.second) + padding;
	const int tooltipHeight = mFont.height() + padding;
	
	auto tooltipPosition = item.first->position();
	tooltipPosition.x = mouseX;

	auto offset = Vector{ 0, -tooltipHeight - constants::MARGIN };

	if (tooltipPosition.y + offset.y < 0)
	{
		offset.y = tooltipHeight + constants::MARGIN;
	}


	auto& renderer = Utility<Renderer>::get();
	if (tooltipPosition.x + tooltipWidth > renderer.size().x)
	{
		offset.x -= (tooltipPosition.x + tooltipWidth) - (renderer.size().x - constants::MARGIN);
	}

	position(tooltipPosition + offset);
	size({ tooltipWidth, tooltipHeight });
}


void ToolTip::onMouseMove(int x, int y, int dX, int dY)
{
	if (dX != 0 || dY != 0)
	{
		if (mFocusedControl)
		{
			if (mFocusedControl->first->rect().contains({ x, y })) { return; }
			else { mFocusedControl = nullptr; }
		}

		mTimer.reset();
	}

	for (auto& item : mControls)
	{
		if (mFocusedControl) { break; }
		if (item.first->rect().contains({ x, y }))
		{
			mFocusedControl = &item;
			buildDrawParams(item, x);
			return;
		}
	}

	mFocusedControl = nullptr;
}


void ToolTip::update()
{
	if (mTimer.accumulator() < 1000)
	{
		return;
	}

	if (mFocusedControl)
	{
		auto& renderer = Utility<Renderer>::get();
		renderer.drawBoxFilled(rect(), Color::DarkGray);
		renderer.drawBox(rect(), Color::Black);
		renderer.drawText(mFont, mFocusedControl->second, Point{ positionX() + constants::MARGIN_TIGHT, positionY() + constants::MARGIN_TIGHT });
	}
}
