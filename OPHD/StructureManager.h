#pragma once

#include "Things/Structures/Structure.h"

#include "ResourcePool.h"
#include "Map/Tile.h"

/**
 * Handles structure updating and resource management for structures.
 *
 * Keeps track of which structures are operational, idle and disabled.
 */
class StructureManager
{
public:
	StructureManager() = default;
	~StructureManager() = default;

public:
	void addStructure(Structure* st, Tile* t);
	void removeStructure(Structure* st);

	StructureList& structureList(Structure::StructureClass st) { return mStructureLists[st]; }
	Tile* tileFromStructure(Structure* st);

	void disconnectAll();
	void dropAllStructures();

	int count() const;

	int getCountInState(Structure::StructureClass st, Structure::StructureState state);

	int disabled();
	int destroyed();

	bool CHAPAvailable();

	void updateEnergyProduction(ResourcePool& resourcePool, PopulationPool& popPool);
	int totalEnergyProduction() const { return mTotalEnergyOutput; }

	void update(ResourcePool& resourcePool, PopulationPool& popPool);

	void serialize(NAS2D::Xml::XmlElement* element);

protected:

private:
	using StructureTileTable = std::map<Structure*, Tile*>;
	using StructureClassTable = std::map<Structure::StructureClass, StructureList>;

private:
	void updateStructures(ResourcePool& resourcePool, PopulationPool& popPool, StructureList& sl);
	void updateFactoryProduction();

	bool structureConnected(Structure* st) { return mStructureTileTable[st]->connected(); }

private:
	StructureTileTable mStructureTileTable; /**< List mapping Structure's to a particular tile. */
	StructureClassTable mStructureLists; /**< Map containing all of the structure list types available. */

	int mTotalEnergyOutput = 0; /**< Total energy output of all energy producers in the structure list. */
};