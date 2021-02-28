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
	ProgressBar(int value, int maxValue);

	void update() override;
	void setValue(int value);
	void setMaxValue(int maxValue);
	void setEnabled(bool enabled);
private:
	int mValue;
	int mMaxValue;
	int mPadding = 1;
	bool mEnabled = false;
};
