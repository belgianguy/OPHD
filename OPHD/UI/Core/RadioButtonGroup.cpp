#include "RadioButtonGroup.h"

void RadioButtonGroup::add(RadioButton& radioButton)
{
	add(radioButton, {0, static_cast<int>(mControls.size() * radioButton.size().y)});
}

void RadioButtonGroup::add(RadioButton& radioButton, NAS2D::Vector<int> offset)
{
	if (std::find(mControls.begin(), mControls.end(), &radioButton) != mControls.end())
	{
		throw std::runtime_error("UIContainer::add(): Duplicate control");
	}

	if (mControls.size() > 0) { mControls.back()->hasFocus(false); }
	mControls.push_back(&radioButton);

	radioButton.position(mRect.startPoint() + offset);
	radioButton.visible(visible());
	radioButton.hasFocus(true);
	radioButton.parentContainer(this);
}
