#include "ProgressBar.h"

#include <NAS2D/Renderer/Renderer.h>

#include <NAS2D/Utility.h>


ProgressBar::ProgressBar(int value, int maxValue, int padding)
{
	mValue = value;
	mMaxValue = maxValue;
	mPadding = padding;
}

void ProgressBar::update()
{
	if (!visible()) { return; }

	auto& renderer = NAS2D::Utility<NAS2D::Renderer>::get();
	renderer.drawBox(NAS2D::Rectangle{mRect.startPoint().x, mRect.startPoint().y, mRect.width, mRect.height}, NAS2D::Color{0, 185, 0});

	if (mValue > 0)
	{
		const auto barWidth = static_cast<int>(static_cast<float>(mRect.width - (mPadding + mPadding)) * (mValue / mMaxValue));
		renderer.drawBoxFilled(NAS2D::Rectangle{mRect.startPoint().x + mPadding, mRect.startPoint().y + mPadding + 1, barWidth - 1, mRect.height - (mPadding + mPadding) - 1}, NAS2D::Color{0, 100, 0});
	}
}

void ProgressBar::setValue(int value)
{
	mValue = value;
}

