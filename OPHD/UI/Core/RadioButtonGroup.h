#pragma once

#include "UIContainer.h"
#include "RadioButton.h"

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

class RadioButtonGroup: public UIContainer {
	public:
		void add(RadioButton& radioButton);
	private:
		void add(RadioButton& radioButton, NAS2D::Vector<int> offset);
};
