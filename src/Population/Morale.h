#pragma once

#include <cstdint>

/**
 * Morale modifier values.
 */
struct MoraleModifier
{
public:
	MoraleModifier() = default;

	int researchBonus{0};
	int productionBonus{0};
	int fertilityRate{0};
	int mortalityRate{0};
};
