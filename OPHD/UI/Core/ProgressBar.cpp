#include "ProgressBar.h"

#include <NAS2D/Renderer/Renderer.h>

#include <NAS2D/Utility.h>


ProgressBar::ProgressBar(int percentageCompleted, int padding)
{
	mPercentageCompleted = percentageCompleted;
	mPadding = padding;
}

void ProgressBar::update()
{
	if (!visible()) { return; }

	auto& renderer = NAS2D::Utility<NAS2D::Renderer>::get();
	renderer.drawBox(NAS2D::Rectangle{mRect.startPoint().x, mRect.startPoint().y, mRect.width, mRect.height}, NAS2D::Color{0, 185, 0});

	if (mPercentageCompleted > 0)
	{
		const auto barWidth = static_cast<int>(static_cast<float>(mRect.width - (mPadding + mPadding)) * mPercentageCompleted / 100);
		renderer.drawBoxFilled(NAS2D::Rectangle{mRect.startPoint().x + mPadding, mRect.startPoint().y + mPadding + 1, barWidth - 1, mRect.height - (mPadding + mPadding) - 1}, NAS2D::Color{0, 100, 0});
	}
}

void ProgressBar::setPercentageCompleted(int percentageCompleted)
{
	mPercentageCompleted = percentageCompleted;
}

