#include "RadioButtonGroup.h"

using namespace NAS2D;

RadioButtonGroup::RadioButton::RadioButton(std::string newText, RadioButtonGroup* parentContainer, NAS2D::DelegateX<void> delegate) :
	mFont{fontCache.load(constants::FONT_PRIMARY, constants::FONT_PRIMARY_NORMAL)},
	mSkin{imageCache.load("ui/skin/radio.png")},
	mLabel{newText},
	mParentContainer{parentContainer}
{
	hasFocus(true);
	text(newText);
	mRbgDelegate = delegate;
	Utility<EventHandler>::get().mouseButtonDown().connect(this, &RadioButtonGroup::RadioButton::onMouseDown);
	onTextChange();
}

RadioButtonGroup::RadioButton::~RadioButton()
{
	Utility<EventHandler>::get().mouseButtonDown().disconnect(this, &RadioButtonGroup::RadioButton::onMouseDown);
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
	onTextChange();
}

const std::string& RadioButtonGroup::RadioButton::text() const
{
	return mLabel.text();
}


RadioButtonGroup::RadioButton::ClickCallback& RadioButtonGroup::RadioButton::click()
{
	mParentContainer->clearSelection();
	std::cout << "In click " << std::endl;

	mRbgDelegate();

	checked(true);
	return mCallback;
}



void RadioButtonGroup::RadioButton::onMouseDown(EventHandler::MouseButton button, int x, int y)
{

	std::cout << "Before onMouseDown " << std::endl;

	if (!enabled() || !visible() || !hasFocus()) { return; }

	if (button == EventHandler::MouseButton::Left && mRect.contains(Point{x, y}))
	{
		std::cout << "In onMouseDown " << mLabel.text() << std::endl;

		click();
	}
}


void RadioButtonGroup::RadioButton::onTextChange()
{
	const auto textWidth = mFont.width(text());
	width((textWidth > 0) ? 20 + textWidth : 13);
}


/**
 * Enforces minimum and maximum sizes.
 */
void RadioButtonGroup::RadioButton::onResize()
{
	mRect.size({std::max(mRect.width, 13), 13});
}


void RadioButtonGroup::RadioButton::update()
{
	auto& renderer = Utility<Renderer>::get();

	const auto unselectedIconRect = NAS2D::Rectangle{0, 0, 13, 13};
	const auto selectedIconRect = NAS2D::Rectangle{13, 0, 13, 13};

	renderer.drawSubImage(mSkin, position(), (mChecked ? selectedIconRect : unselectedIconRect));
	renderer.drawText(mFont, text(), position() + NAS2D::Vector{20, 0}, NAS2D::Color::White);
}
