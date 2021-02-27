// ==================================================================================
// = NAS2D+GUI
// = Copyright © 2008 - 2020, Leeor Dicker
// ==================================================================================
// = This file is part of the NAS2D+GUI library.
// ==================================================================================

#pragma once

#include "Control.h"

/**
 * \class ProgressBar
 * \brief A control that show the percentual progress of an operation.
 *
 */
class ProgressBar : public Control
{
public:
	ProgressBar(int width, int height, int percentageCompleted, int padding);

	void update() override;
	void setPercentageCompleted(int percentageCompleted);
private:
	int mWidth;
	int mHeight;
	int mPercentageCompleted;
	int mPadding;
};
