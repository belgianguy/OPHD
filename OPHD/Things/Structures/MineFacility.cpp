#include "MineFacility.h"

#include "../../Constants.h"


const int MineFacilityStorageCapacity = 500;


/**
 * Computes how many units of ore should be pulled.
 */
static int pull_count(MineFacility* _mf, size_t index)
{
	const int storageCapacity = (_mf->storageCapacity() / 4);
	const int remainingCapacity = storageCapacity - _mf->production().resources[index];

	const int total = std::clamp(constants::BASE_MINE_PRODUCTION_RATE, 0, remainingCapacity);

	return total;
}


MineFacility::MineFacility(Mine* mine) : Structure(constants::MINE_FACILITY,
	"structures/mine_facility.sprite",
	StructureClass::Mine,
	StructureID::SID_MINE_FACILITY),
	mMine(mine)
{
	sprite().play(constants::STRUCTURE_STATE_CONSTRUCTION);
	maxAge(1200);
	turnsToBuild(2);

	requiresCHAP(false);
	selfSustained(true);
	storageCapacity(500);
}


void MineFacility::activated()
{
	mMine->increaseDepth();
	mMine->active(true);
}


void MineFacility::think()
{
	if (forceIdle()) { return; }

	if (mDigTurnsRemaining > 0)
	{
		--mDigTurnsRemaining;

		if (mDigTurnsRemaining == 0)
		{
			mMine->increaseDepth();
			mExtensionComplete(this);
		}

		return;
	}

	static const StorableResources capacity{ MineFacilityStorageCapacity / 4,
		MineFacilityStorageCapacity / 4,
		MineFacilityStorageCapacity / 4,
		MineFacilityStorageCapacity / 4
	};

	if (isIdle() && mMine->active())
	{
		if (storage() < capacity)
		{
			enable();
		}
	}

	if (mMine->exhausted())
	{
		idle(IdleReason::MineExhausted);
		return;
	}

	if (mMine->active())
	{
		if (storage() >= capacity)
		{
			idle(IdleReason::MineExhausted);
			return;
		}

		StorableResources ore;

		if (mMine->miningCommonMetals())
		{
			ore.resources[0] = mMine->pull(Mine::OreType::ORE_COMMON_METALS, pull_count(this, 0));
		}

		if (mMine->miningCommonMinerals())
		{
			ore.resources[1] = mMine->pull(Mine::OreType::ORE_COMMON_MINERALS, pull_count(this, 1));
		}

		if (mMine->miningRareMetals())
		{
			ore.resources[2] = mMine->pull(Mine::OreType::ORE_RARE_METALS, pull_count(this, 2));
		}

		if (mMine->miningRareMinerals())
		{
			ore.resources[3] = mMine->pull(Mine::OreType::ORE_RARE_MINERALS, pull_count(this, 3));
		}

		storage() += ore;
	}
	else if (!isIdle())
	{
		idle(IdleReason::MineInactive);
	}
}


bool MineFacility::canExtend() const
{
	return (mMine->depth() < mMaxDepth) && (mDigTurnsRemaining == 0);
}


void MineFacility::extend()
{
	if (!canExtend()) { return; }
	mDigTurnsRemaining = constants::BASE_MINE_SHAFT_EXTENSION_TIME;
}


bool MineFacility::extending() const
{
	return mDigTurnsRemaining > 0;
}


int MineFacility::digTimeRemaining() const
{
	return mDigTurnsRemaining;
}
