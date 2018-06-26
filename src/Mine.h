#pragma once

#include "Common.h"

#include "NAS2D/NAS2D.h"

class Mine
{
public:
	Mine(MineProductionRate _rate);
	~Mine();

	bool active() const { return mActive; }
	void active(bool _b) { mActive = _b; }

	bool exhausted() const;

	bool yieldsCommonMetals() const { return commonMetalsRate() > 0; }
	bool yieldsRareMetals() const { return rareMetalsRate() > 0; }
	bool yieldsCommonMinerals() const { return commonMineralsRate() > 0; }
	bool yieldsRareMinerals() const { return rareMineralsRate() > 0; }

	MineProductionRate productionRate() { return mProductionRate; }

	int depth() const { return mDepth; }
	void depth(int _d) { mDepth = _d; }

	int commonMetalsRate() const { return mCommonMetalYield; }
	int rareMetalsRate() const { return mRareMetalYield; };
	int commonMineralsRate() const { return mCommonMineralYield; };
	int rareMineralsRate() const { return mRareMineralYield; };

	void update();

	void serialize(NAS2D::Xml::XmlElement* _ti);

private:
	void productionRate(MineProductionRate _rate);

	void commonMetalsRate(int _rate) { mCommonMetalYield = _rate; }
	void rareMetalsRate(int _rate) { mRareMetalYield = _rate; };
	void commonMineralsRate(int _rate) { mCommonMineralYield = _rate; };
	void rareMineralsRate(int _rate) { mRareMineralYield = _rate; };

private:
	Mine() = delete;
	Mine(const Mine&) = delete;
	Mine& operator=(const Mine&) = delete;

private:
	int					mDepth = 0;								/**< Depth of the Mine. */

	int					mCommonMetalYield = 0;					/**< Total per level of the mine. */
	int					mRareMetalYield = 0;					/**< Total per level of the mine. */
	int					mCommonMineralYield = 0;				/**< Total per level of the mine. */
	int					mRareMineralYield = 0;					/**< Total per level of the mine. */

	MineProductionRate	mProductionRate = PRODUCTION_RATE_LOW;	/**< Mine's production rate. */

	bool				mActive = false;						/**< Flag indicating whether this mine is active or not. */
};
