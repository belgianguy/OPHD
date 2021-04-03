#pragma once

#include "TextControl.h"
#include "Label.h"

#include <NAS2D/Signal.h>
#include <NAS2D/EventHandler.h>
#include <NAS2D/Resources/Image.h>

#include <string>


class RadioButtonGroup : public Control
{
public:
    RadioButtonGroup() = default;
    ~RadioButtonGroup()
    {
        for(auto& radioButton : mRadioButtons)
        {
            radioButton.stateChanged().disconnect(this, &RadioButtonGroup::radioButtonStateChanged);
        }
    }

    void add(const std::string& name)
    {
        mRadioButtons.emplace_back(name, this);
        mRadioButtons.back().stateChanged().connect(this, &RadioButtonGroup::radioButtonStateChanged);
    }

private:
    class RadioButton : public TextControl
    {
    public:
    	//using ClickCallback = NAS2D::Signals::Signal<>;
    	using StateChanged = NAS2D::Signals::Signal<RadioButton&>;

    	RadioButton(std::string newText, RadioButtonGroup* parentContainer);
    	~RadioButton() override;

    	void checked(bool toggle);
    	bool checked() const;

    	void text(const std::string& text);
    	const std::string& text() const;

    	//ClickCallback& click();

    	void update() override;


    	StateChanged& stateChanged() { return mStateChanged; }

    protected:
    	//void onMouseDown(NAS2D::EventHandler::MouseButton button, int x, int y);

    	void onSizeChanged() override;
    	void onTextChanged() override;

    	void parentContainer(RadioButtonGroup* parent);

    private:
    	const NAS2D::Font& mFont;
    	const NAS2D::Image& mSkin;
    	Label mLabel;
    	//ClickCallback mCallback; /**< Object to notify when the Button is activated. */
    	RadioButtonGroup* mParentContainer{nullptr};
    	bool mChecked{false};

    	void onMouseDown(NAS2D::EventHandler::MouseButton button, int x, int y);

    	StateChanged mStateChanged;

    	friend class UIContainer;
    };

    void radioButtonStateChanged(RadioButton& rb)
	{
		// Do stuff here, reference to RadioButton is perhaps optional?
	}

	std::vector<RadioButton> mRadioButtons;
};
