#include "ProgressBar.h"

#include <NAS2D/Renderer/Renderer.h>

#include <NAS2D/Utility.h>


ProgressBar::ProgressBar(int width, int height, int percentageCompleted, int padding)
{
	mWidth = width;
	mHeight = height;
	mPercentageCompleted = percentageCompleted;
	mPadding = padding;
}

void ProgressBar::update()
{
	if (!visible()) { return; }

	auto& renderer = NAS2D::Utility<NAS2D::Renderer>::get();
	renderer.drawBox(NAS2D::Rectangle{mRect.startPoint().x, mRect.startPoint().y, mWidth, mHeight}, NAS2D::Color{0, 185, 0});

	if (mPercentageCompleted > 0)
	{
		int bar_width = static_cast<int>(static_cast<float>(mWidth - (mPadding + mPadding)) * mPercentageCompleted / 100);
		renderer.drawBoxFilled(NAS2D::Rectangle{mRect.startPoint().x + mPadding, mRect.startPoint().y + mPadding + 1, bar_width - 1, mHeight - (mPadding + mPadding) - 1}, NAS2D::Color{0, 100, 0});
	}
}

void ProgressBar::setPercentageCompleted(int percentageCompleted)
{
	mPercentageCompleted = percentageCompleted;
}

