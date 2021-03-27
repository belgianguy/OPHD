#pragma once

#include "UIContainer.h"
#include "TextControl.h"
#include "Label.h"

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <string>

#include <NAS2D/Signal.h>
#include <NAS2D/EventHandler.h>
#include <NAS2D/Resources/Image.h>

class RadioButtonGroup: public UIContainer {
	public:

		void positionChanged(int dX, int dY);
		~RadioButtonGroup();

		template<typename X, typename Y, typename ... Params>
		void add(Y * obj, void (X::*func)(Params...), std::string& label)
		{
			RadioButton* radioButton = new RadioButton(label, this);
			radioButton->click().connect(obj, func);
			add(radioButton, {0, static_cast<int>(mControls.size() * radioButton->size().y)});
		}

	private:
		class RadioButton : public TextControl
		{
			public:
				using ClickCallback = NAS2D::Signals::Signal<>;

				RadioButton(std::string newText, RadioButtonGroup* parentContainer);
				~RadioButton() override;

				void checked(bool toggle);
				bool checked() const;

				void text(const std::string& text);
				const std::string& text() const;

				ClickCallback& click();

				void update() override;
				void parentContainer(RadioButtonGroup* parent);

			protected:
				void onMouseDown(NAS2D::EventHandler::MouseButton button, int x, int y);
				void onSizeChanged() override;
				void onTextChanged() override;

			private:
				const NAS2D::Font& mFont;
				const NAS2D::Image& mSkin;
				Label mLabel;
				ClickCallback mCallback; /**< Object to notify when the Button is activated. */
				RadioButtonGroup* mParentContainer{nullptr};
				bool mChecked{false};
		};
		void add(RadioButton* radioButton, NAS2D::Vector<int> offset);
};
