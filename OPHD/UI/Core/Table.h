#pragma once

#include "Control.h"
#include <NAS2D/Renderer/Point.h>
#include <NAS2D/Utility.h>
#include <NAS2D/Renderer/Renderer.h>

#include <vector>

#include <iostream>

using namespace std;

class Table : public Control
{
public:
	Table(int rows, int columns);

	void add(Control& control, NAS2D::Point<int> location);
	void update() override;
	void print();

protected:
	void positionChanged(int dX, int dY) override;

private:
	int mColumns;
	int mRows;
	std::vector<Control*> mControls;
};
