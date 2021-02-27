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
	ProgressBar(int percentageCompleted, int padding);

	void update() override;
	void setPercentageCompleted(int percentageCompleted);
private:
	int mPercentageCompleted;
	int mPadding;
};
