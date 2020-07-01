// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "ListBoxBase.h"

#include "../../Constants.h"

#include "NAS2D/Utility.h"
#include "NAS2D/Renderer/Renderer.h"
#include "NAS2D/MathUtils.h"

#include <algorithm>


using namespace NAS2D;


/**
 * C'tor
 */
ListBoxBase::ListBoxBase()
{
	_init();
}


/**
 * D'tor
 */
ListBoxBase::~ListBoxBase()
{
	mSlider.change().disconnect(this, &ListBoxBase::slideChanged);

	Utility<EventHandler>::get().mouseWheel().disconnect(this, &ListBoxBase::onMouseWheel);
	Utility<EventHandler>::get().mouseButtonDown().disconnect(this, &ListBoxBase::onMouseDown);
	Utility<EventHandler>::get().mouseMotion().disconnect(this, &ListBoxBase::onMouseMove);

	for (auto item : mItems) { delete item; }
}


/**
 * Internal initializer function.
 */
void ListBoxBase::_init()
{
	Utility<EventHandler>::get().mouseWheel().connect(this, &ListBoxBase::onMouseWheel);
	Utility<EventHandler>::get().mouseButtonDown().connect(this, &ListBoxBase::onMouseDown);
	Utility<EventHandler>::get().mouseMotion().connect(this, &ListBoxBase::onMouseMove);

	mSlider.displayPosition(false);
	mSlider.length(0);
	mSlider.thumbPosition(0);
	mSlider.change().connect(this, &ListBoxBase::slideChanged);

	_update_item_display();
}


/**
 * 
 */
void ListBoxBase::visibilityChanged(bool)
{
	_update_item_display();
}


/**
 * Updates values required for properly displaying list items.
 */
void ListBoxBase::_update_item_display()
{
	mItemWidth = static_cast<unsigned int>(width());

	if ((mItemHeight * mItems.size()) > static_cast<std::size_t>(height()))
	{
		mLineCount = static_cast<int>(height() / mItemHeight);

		if (mLineCount < mItems.size())
		{
			mSlider.position({rect().x() + mRect.width() - 14, mRect.y()});
			mSlider.size({14, mRect.height()});
			mSlider.length((mItemHeight * mItems.size()) - height());
			mCurrentOffset = static_cast<unsigned int>(mSlider.thumbPosition());
			mItemWidth -= static_cast<unsigned int>(mSlider.width());
			mSlider.visible(true);
		}
	}
	else
	{
		mCurrentOffset = 0;
		mSlider.length(0);
		mSlider.visible(false);
	}
}


/**
 * Resized event handler.
 */
void ListBoxBase::onSizeChanged()
{
	_update_item_display();
}


/**
 * Mouse Down event handler.
 */
void ListBoxBase::onMouseDown(EventHandler::MouseButton button, int x, int y)
{
	if (!visible() || !hasFocus()) { return; }

	if (empty() || button == EventHandler::MouseButton::BUTTON_MIDDLE) { return; }

	if (button == EventHandler::MouseButton::BUTTON_RIGHT && mRect.to<int>().contains(Point{x, y}))
	{
		setSelection(constants::NO_SELECTION);
		return;
	}

	// A few basic checks
	if (!rect().contains(mMousePosition) || mCurrentHighlight == constants::NO_SELECTION) { return; }
	if (mSlider.visible() && mSlider.rect().contains(Point{x, y})) { return; }
	if (static_cast<std::size_t>(mCurrentHighlight) >= mItems.size()) { return; }

	setSelection(mCurrentHighlight);
}


/**
 * Mouse Motion event handler.
 */
void ListBoxBase::onMouseMove(int x, int y, int /*relX*/, int /*relY*/)
{
	if (!visible() || empty()) { return; }

	mMousePosition = {x, y};

	// Ignore mouse motion events if the pointer isn't within the menu rect.
	if (!rect().contains(mMousePosition))
	{
		mCurrentHighlight = constants::NO_SELECTION;
		return;
	}

	// if the mouse is on the slider then the slider should handle that
	if (mSlider.visible() && mSlider.rect().contains(mMousePosition))
	{
		mCurrentHighlight = constants::NO_SELECTION;
		return;
	}

	mCurrentHighlight = (y - static_cast<int>(positionY()) + mCurrentOffset) / mItemHeight;

	if (static_cast<std::size_t>(mCurrentHighlight) >= mItems.size())
	{
		mCurrentHighlight = constants::NO_SELECTION;
	}
}


/**
 * Mouse Wheel event handler.
 * 
 * \todo	Make the scroll step configurable. Legacy from the ListBox.
 */
void ListBoxBase::onMouseWheel(int /*x*/, int y)
{
	if (!visible()) { return; }
	if (!rect().contains(mMousePosition)) { return; }

	float change = static_cast<float>(mItemHeight);

	mSlider.changeThumbPosition((y < 0 ? change : -change));
}


/**
 * Slider changed event handler.
 */
void ListBoxBase::slideChanged(float newPosition)
{
	_update_item_display();
	// Intentional truncation of fractional value
	int pos = static_cast<int>(newPosition);
	if (static_cast<float>(pos) != newPosition)
	{
		mSlider.thumbPosition(static_cast<float>(pos));
	}
}


/**
 * Adds a ListBoxItem.
 * 
 * \warning	Requires a pointer to a ListBoxItem -- memory is owned
 *			and managed by ListBoxBase.
 */
void ListBoxBase::addItem(ListBoxItem* item)
{
	auto it = std::find(mItems.begin(), mItems.end(), item);
	if (it == mItems.end()) { mItems.push_back(item); }
}


/**
 * Removes a ListBoxItem.
 * 
 * \warning	Frees memory allocated for a ListBoxItem. All pointers
 *			and/or references will become invalidated.
 */
void ListBoxBase::removeItem(ListBoxItem* item)
{
	auto it = std::find(mItems.begin(), mItems.end(), item);
	if (it == mItems.end()) { return; }
	delete (*it);
	mItems.erase(it);
}


/**
 * Clears all items from the list.
 */
void ListBoxBase::clearItems()
{
	for (auto item : mItems) { delete item; }
	mItems.clear();
	mCurrentSelection = constants::NO_SELECTION;
	mCurrentHighlight = constants::NO_SELECTION;
	_update_item_display();
}


/**
 * Number of items in the ListBoxBase.
 */
std::size_t ListBoxBase::count() const
{
	return mItems.size();
}


/**
 * True if no items are in the list.
 */
bool ListBoxBase::empty() const
{
	return mItems.empty();
}


/**
 * Index of the current mouse hover highlight.
 */
unsigned int ListBoxBase::currentHighlight() const
{
	return mCurrentHighlight;
}


/**
 * Index of the current selection.
 */
unsigned int ListBoxBase::currentSelection() const
{
	return mCurrentSelection;
}


/**
 * Sets the current selection index.
 * 
 * \note	Out of range selection indicies will set the ListBoxBase to no selection.
 */
void ListBoxBase::setSelection(int selection)
{
	mItems.empty() ? mCurrentSelection == constants::NO_SELECTION : mCurrentSelection = selection;
	mSelectionChanged();
}


const std::string& ListBoxBase::selectionText() const
{
	if (currentSelection() == constants::NO_SELECTION) { return constants::EMPTY_STR; }
	return mItems[currentSelection()]->Text;
}


/**
 * Clears the current selection.
 */
void ListBoxBase::clearSelection()
{
	mCurrentSelection = constants::NO_SELECTION;
}


/**
 * Sets item height.
 * 
 * \note	Internal function for specialized types.
 */
void ListBoxBase::item_height(int h)
{
	mItemHeight = h;
}


/**
 * Draws the ListBox
 */
void ListBoxBase::update()
{
	if (!visible()) { return; }

	auto& renderer = Utility<Renderer>::get();

	// CONTROL EXTENTS
	const auto backgroundRect = NAS2D::Rectangle{mRect.x(), mRect.y(), static_cast<float>(mItemWidth), mRect.height()};
	renderer.drawBoxFilled(backgroundRect, NAS2D::Color::Black);

	hasFocus() ? renderer.drawBox(backgroundRect, NAS2D::Color{0, 185, 0}) : renderer.drawBox(backgroundRect, NAS2D::Color{75, 75, 75});

	renderer.clipRect(mRect);

	// MOUSE HIGHLIGHT
	float highlight_y = positionY() + static_cast<float>((mCurrentHighlight * mItemHeight) - mCurrentOffset);
	renderer.drawBoxFilled(positionX(), highlight_y, static_cast<float>(mItemWidth), static_cast<float>(mItemHeight), 0, 185, 0, 50);

	mSlider.update();

	renderer.clipRectClear();
}
