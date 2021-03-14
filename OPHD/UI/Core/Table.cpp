#include "Table.h"

using namespace NAS2D;

Table::Table(int rows, int columns)
{
	mControls.resize(rows * columns);
	mRows = rows;
	mColumns = columns;
}

void Table::add(Control &control, NAS2D::Point<int> location)
{
	// 0,0   1,0
	// 0,1   1,1
	// 0,2   1,2
	// 0,3   1,3

	/*
	 * Say we want to have icons in the left column, the widest icon is 32px
	 * And we have a text in the right column, the widest is 140px
	 */

	float cellWidth = size().x / mColumns;
	float cellHeight = size().y / mRows;

	if (location.x >= 0 && location.x < mColumns && location.y >= 0 && location.y < mRows) {
		std::cout << "Pos before: (" << control.position().x << ", " << control.position().y << ")" << std::endl;
		control.position(mRect.startPoint() + NAS2D::Vector{location.x * cellWidth, location.y * cellHeight});
		std::cout << "Pos after: (" << control.position().x << ", " << control.position().y << ")" << std::endl;
		control.visible(visible());
		control.hasFocus(true);
		int index = mColumns * location.y + location.x;
		mControls[index] = &control;
	}
}

void Table::update()
{
	if (!visible()) { return; }


	const auto displayRect = Rectangle<int>::Create(position(), mRect.size());
	Utility<Renderer>::get().drawBoxFilled(displayRect, {255, 0, 0});

	for(auto control : mControls)
	{
		if (control != nullptr)
		{
			control->update();
		}
	}
}

void Table::positionChanged(int dX, int dY)
{
	Control::positionChanged(dX, dY);

	for(auto control : mControls)
	{
		if (control != nullptr)
		{
			control->position(control->position() + NAS2D::Vector{dX, dY});
		}
	}
}

void Table::print()
{
	std::cout << "Table Rectangle start" << std::endl;
	std::cout << "Start (" << mRect.x << ", " << mRect.y << ")" << std::endl;
	std::cout << "Width: " << mRect.width << std::endl;
	std::cout << "Height: " << mRect.height << std::endl;
	std::cout << "Table Rectangle end" << std::endl;

	for(auto control : mControls)
	{
		if (control != nullptr)
		{
			std::cout << "Control Rectangle start" << std::endl;
			std::cout << control->rect().x << std::endl;
			std::cout << control->rect().y << std::endl;
			std::cout << control->size().x << std::endl;
			std::cout << control->size().y << std::endl;
			std::cout << "Control Rectangle end" << std::endl;
		}
	}
}
