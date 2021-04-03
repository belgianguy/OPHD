#include "RadioButton.h"

class RadioButtonGroup
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
        mRadioButtons.emplace_back(name);
        mRadioButtons.back().stateChanged().connect(this, &RadioButtonGroup::radioButtonStateChanged);
    }

private:
    void radioButtonStateChanged(RadioButton& rb)
    {
        // Do stuff here, reference to RadioButton is perhaps optional?
    }

    std::vector<RadioButton> mRadioButtons;
};
