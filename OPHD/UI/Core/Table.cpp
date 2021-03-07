#include "Table.h"

using namespace NAS2D;

Table::Table(int columns, int rows)
{
	mColumns = columns;
	mRows = rows;
}

void Table::add(Control& control, NAS2D::Point<int> location)
{
	// 0,0   1,0
	// 0,1   1,1
	// 0,2   1,2
	// 0,3   1,3

	/*
	 * Say we want to have icons in the left column, the widest icon is 32px
	 * And we have a text in the right column, the widest is 140px
	 */

	if (location.x >= 0 && location.x < mColumns && location.y >= 0 && location.y < mRows) {
		control.position(mRect.startPoint());
		control.visible(visible());
		control.hasFocus(true);
		mControls.push_back(std::make_pair(&control, location));
	}
}

void Table::update()
{
	if (!visible()) { return; }

	for (auto pair : mControls)
	{
		pair.first->update();
	}
}

void Table::positionChanged(int dX, int dY)
{
	Control::positionChanged(dX, dY);

	for (auto pair : mControls)
	{
		pair.first->position(pair.first->position() + NAS2D::Vector{dX, dY});
	}
}

void Table::print()
{
	std::cout << "Table Rectangle start" << std::endl;
	std::cout << mRect.x << std::endl;
	std::cout << mRect.y << std::endl;
	std::cout << mRect.width << std::endl;
	std::cout << mRect.height << std::endl;
	std::cout << "Table Rectangle end" << std::endl;

	for (auto control : mControls)
	{
		std::cout << "Control Rectangle start" << std::endl;
		std::cout << control.first->rect().x << std::endl;
		std::cout << control.first->rect().y << std::endl;
		std::cout << control.first->size().x << std::endl;
		std::cout << control.first->size().y << std::endl;
		std::cout << control.second.x << std::endl;
		std::cout << control.second.y << std::endl;
		std::cout << "Control Rectangle end" << std::endl;
	}
}
