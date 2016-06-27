#pragma once

#include "Structure.h"

class AirShaft: public Structure
{
public:
	AirShaft():	Structure(constants::AIR_SHAFT, "structures/air_shaft.sprite", STRUCTURE_TUBE)
	{
		sprite().play(constants::STRUCTURE_STATE_OPERATIONAL);
		maxAge(400);

		connectorDirection(CONNECTOR_VERTICAL);

		requiresCHAP(false);
		defineResourceCostToBuild();
	}


	~AirShaft()
	{
	}

	void ug() { sprite().play(constants::STRUCTURE_STATE_OPERATIONAL_UG); }

protected:
private:

};
