#pragma once

#include "UI.h"

#include "../Population/Population.h"

class PopulationPanel: public Control
{
public:
	PopulationPanel();

	void population(Population* pop) { mPopulation = pop; }
	void morale(int* m) { mMorale = m; }
	void old_morale(int* m) { mPreviousMorale = m; }

	void residential_capacity(int m) { mResidentialCapacity = m; }

	/**
	 * \fixme	This class/function is use to store residence capacity
	 *			by the MapViewState. Probably not be an appropriate place.
	 */
	int capacity() const { return mCapacity; }

	void update() override;

protected:

private:
	NAS2D::Image mIcons;
	NAS2D::ImageList mSkin;

	Population* mPopulation = nullptr;

	int mResidentialCapacity = 0;

	int* mMorale = nullptr;
	int* mPreviousMorale = nullptr;

	int mCapacity = 0;
};
