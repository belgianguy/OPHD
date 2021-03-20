#include "RadioButtonGroup.h"
#include "../../Cache.h"
#include "../../Constants.h"

#include <NAS2D/Utility.h>
#include <NAS2D/Renderer/Renderer.h>
#include <NAS2D/MathUtils.h>

#include <algorithm>


using namespace NAS2D;
using ClickCallback = NAS2D::Signals::Signal<>;

RadioButtonGroup::~RadioButtonGroup()
{
	for (auto control : mControls)
	{
		delete control;
	}
}

void RadioButtonGroup::add(RadioButton* radioButton, NAS2D::Vector<int> offset)
{
	if (std::find(mControls.begin(), mControls.end(), radioButton) != mControls.end())
	{
		throw std::runtime_error("RadioButtonGroup::add(): Duplicate control");
	}

	if (mControls.size() > 0) { mControls.back()->hasFocus(false); }
	mControls.push_back(radioButton);

	radioButton->position(mRect.startPoint() + offset);
	radioButton->visible(visible());
	radioButton->hasFocus(true);
	radioButton->parentContainer(this);
}

void RadioButtonGroup::positionChanged(int dX, int dY)
{
	Control::positionChanged(dX, dY);

	for (auto control : mControls)
	{
		control->position(control->position() + NAS2D::Vector{dX, dY});
	}
}

RadioButtonGroup::RadioButton::RadioButton(std::string newText) :
	mFont{fontCache.load(constants::FONT_PRIMARY, constants::FONT_PRIMARY_NORMAL)},
	mSkin{imageCache.load("ui/skin/checkbox.png")},
	mLabel{newText}
{
	Utility<EventHandler>::get().mouseButtonDown().connect(this, &RadioButton::onMouseDown);
}

RadioButtonGroup::RadioButton::RadioButton(std::string newText, RadioButtonGroup* parentContainer):
	mFont{fontCache.load(constants::FONT_PRIMARY, constants::FONT_PRIMARY_NORMAL)},
	mSkin{imageCache.load("ui/skin/checkbox.png")},
	mLabel{newText},
	mParentContainer{parentContainer}
{
	Utility<EventHandler>::get().mouseButtonDown().connect(this, &RadioButton::onMouseDown);
	size({13,13});
	onTextChanged();
}

RadioButtonGroup::RadioButton::~RadioButton()
{
	Utility<EventHandler>::get().mouseButtonDown().disconnect(this, &RadioButton::onMouseDown);
}

/**
 * Sets checked state.
 *
 * \note	Setting check state with this method will not fire
 *			a clicked event.
 */
void RadioButtonGroup::RadioButton::checked(bool toggle)
{
	mChecked = toggle;
}


bool RadioButtonGroup::RadioButton::checked() const
{
	return mChecked;
}

void RadioButtonGroup::RadioButton::parentContainer(RadioButtonGroup* parent)
{
	mParentContainer = parent;
}

void RadioButtonGroup::RadioButton::text(const std::string& text)
{
	mLabel.text(text);
	onTextChanged();
}

const std::string& RadioButtonGroup::RadioButton::text() const
{
	return mLabel.text();
}

RadioButtonGroup::RadioButton::ClickCallback& RadioButtonGroup::RadioButton::click()
{
	for (auto* sibling : mParentContainer->controls())
	{
		if (auto* asRadioButton = dynamic_cast<RadioButton*>(sibling))
		{
			std::cout << "RadioButton " << asRadioButton->text() << " " << asRadioButton->hasFocus() << std::endl;
			asRadioButton->checked(false);
		}
	}

	checked(true);
	return mCallback;
}


void RadioButtonGroup::RadioButton::onMouseDown(EventHandler::MouseButton button, int x, int y)
{
	if (!enabled() || !visible() || !hasFocus()) { return; }

	if (button == EventHandler::MouseButton::BUTTON_LEFT && mRect.contains(Point{x, y}))
	{
		std::cout << "Clicked " << text() << std::endl;

		click();
		mCallback();
	}
}


void RadioButtonGroup::RadioButton::onTextChanged()
{
	const auto textWidth = mFont.width(text());
	width((textWidth > 0) ? 20 + textWidth : 13);
}


/**
 * Enforces minimum and maximum sizes.
 */
void RadioButtonGroup::RadioButton::onSizeChanged()
{
	mRect.size({std::max(mRect.width, 13), 13});
}


void RadioButtonGroup::RadioButton::update()
{
	auto& renderer = Utility<Renderer>::get();

	const auto unselectedIconRect = NAS2D::Rectangle{0, 0, 13, 13};
	const auto selectedIconRect = NAS2D::Rectangle{13, 0, 13, 13};

	renderer.drawCircleFilled(position() + NAS2D::Vector{6.5,6.5}, 6.5, Color::Black, 10);

	renderer.drawSubImage(mSkin, position(), (mChecked ? selectedIconRect : unselectedIconRect));
	renderer.drawText(mFont, text(), position() + NAS2D::Vector{20, 0}, NAS2D::Color::White);
}
