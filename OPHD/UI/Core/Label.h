// ==================================================================================
// = NAS2D+GUI
// = Copyright © 2008 - 2020, Leeor Dicker
// ==================================================================================
// = This file is part of the NAS2D+GUI library.
// ==================================================================================

#pragma once

#include "Control.h"

#include "NAS2D/Renderer/Color.h"

namespace NAS2D
{
	class Font;
}

/**
 * \class Label
 * \brief A control that contains readonly text.
 * 
 */
class Label : public Control
{
public:
	Label(std::string newText = "");

	void autoSize();
	void font(NAS2D::Font* font);
	bool empty() const { return text().empty(); }
	void clear() { mText.clear(); }
	void update() override;

	int textWidth() const;
	NAS2D::Vector<int> textSize() const;

	void color(const NAS2D::Color& color);

protected:
private:
	NAS2D::Color textColor{NAS2D::Color::White};
};