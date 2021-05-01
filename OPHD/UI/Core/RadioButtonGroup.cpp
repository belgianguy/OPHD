#include "RadioButtonGroup.h"

#include "../../Cache.h"
#include "../../Constants.h"

#include <NAS2D/Utility.h>
#include <NAS2D/Renderer/Renderer.h>
#include <NAS2D/MathUtils.h>

#include <algorithm>


using namespace NAS2D;


void RadioButtonGroup::add(NAS2D::DelegateX<void> delegate, const std::string& name, bool checked)
{
	//RadioButton* rb = new RadioButton(name, this, NAS2D::MakeDelegate(obj, func));
	NAS2D::Vector<int> offset = {0, 13};
	offset.y = mRadioButtons.size() * offset.y;

	mRadioButtons.emplace_back(name, this, delegate);
	//mRadioButtons.push_back(rb);
	mRadioButtons.back().visible(visible());
	mRadioButtons.back().position(mRect.startPoint() + offset);
	mRadioButtons.back().checked(checked);
	if(checked) { mRadioButtons.back().click(); }
}

void RadioButtonGroup::onMove(NAS2D::Vector<int> displacement)
{
	Control::onMove(displacement);

	for (auto &control : mRadioButtons)
	{
		//control.position(control.position() + NAS2D::Vector{dX, dY});
		control.position(control.position() + displacement);
	}
}

void RadioButtonGroup::update()
{
	if (!visible()) { return; }

	//const auto displayRect = Rectangle<int>::Create(position(), mRect.size());
	//Utility<Renderer>::get().drawBoxFilled(displayRect, {255, 0, 0});

	if( mRadioButtons.size() > 0 && !hasOneSelectedItem() ) {
		clearSelection();
		mRadioButtons[0].click();
	}

	for (auto &control : mRadioButtons)
	{
		control.update(); /*if (control->hasFocus()) { Utility<Renderer>::get().drawBox(control->rect(), 255, 0, 255); }*/
	}
}

void RadioButtonGroup::clearSelection()
{
	if (!visible()) { return; }

	for (auto &control : mRadioButtons)
	{
		control.checked(false);
	}
}

bool RadioButtonGroup::hasOneSelectedItem()
{
	int count = 0;
	for (auto &control : mRadioButtons)
	{
		if(control.checked()) { count++; }
	}
	return count == 1;
}


RadioButtonGroup::~RadioButtonGroup()
{

}

void RadioButtonGroup::setSelected(std::size_t index)
{
	mRadioButtons[index].checked(true);
}
















