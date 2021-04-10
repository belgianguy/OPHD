#pragma once

#include "TextControl.h"
#include "Label.h"
#include "RadioButtonGroup.h"

#include <NAS2D/Signal.h>
#include <NAS2D/EventHandler.h>
#include <NAS2D/Resources/Image.h>
#include <NAS2D/Delegate.h>

#include "../../Cache.h"
#include "../../Constants.h"

#include <NAS2D/Utility.h>
#include <NAS2D/Renderer/Renderer.h>
#include <NAS2D/MathUtils.h>

#include <algorithm>

#include <string>

#include <iostream>

using namespace NAS2D;

class RadioButtonGroup : public Control
{
public:
	~RadioButtonGroup();
    template<typename X, typename Y, typename ... Params>
    void add(Y * obj, void (X::*func)(Params...), const std::string& name, bool checked = false)
    {
        //RadioButton* rb = new RadioButton(name, this, NAS2D::MakeDelegate(obj, func));
        NAS2D::Vector<int> offset = {0, 13};
        offset.y = mRadioButtons.size() * offset.y;

    	mRadioButtons.emplace_back(name, this, NAS2D::MakeDelegate(obj, func));
    	//mRadioButtons.push_back(rb);
    	mRadioButtons.back().visible(visible());
    	mRadioButtons.back().position(mRect.startPoint() + offset);
    	mRadioButtons.back().checked(checked);
		if(checked) { mRadioButtons.back().click(); }

    }

    void positionChanged(int dX, int dY) override;
    void update() override;

    void clearSelection();
    bool hasOneSelectedItem();

private:
    class RadioButton : public TextControl
    {
    public:
    	using ClickCallback = NAS2D::Signals::Signal<>;

    	RadioButton(std::string newText, RadioButtonGroup* parentContainer, NAS2D::DelegateX<void> delegate);
    	~RadioButton() override;

    	void checked(bool toggle);
    	bool checked() const;

    	void text(const std::string& text);
    	const std::string& text() const;

    	ClickCallback& click();

    	void update() override;

    protected:

    	void onSizeChanged() override;
    	void onTextChanged() override;

    	void parentContainer(RadioButtonGroup* parent);

    private:
    	const NAS2D::Font& mFont;
    	const NAS2D::Image& mSkin;
    	Label mLabel;
    	ClickCallback mCallback; /**< Object to notify when the Button is activated. */
    	RadioButtonGroup* mParentContainer{nullptr};
    	bool mChecked{false};

    	void onMouseDown(NAS2D::EventHandler::MouseButton button, int x, int y);

    	NAS2D::DelegateX<void> mRbgDelegate;
    };

	std::vector<RadioButton> mRadioButtons;
};
