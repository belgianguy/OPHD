#include "FactoryProduction.h"

#include "StringTable.h"
#include "../Things/Structures/Factory.h"

#include <NAS2D/Utility.h>
#include <NAS2D/Renderer/Renderer.h>


using namespace NAS2D;


FactoryProduction::FactoryProduction() :
	Window{constants::WINDOW_FACTORY_PRODUCTION}
{
	size({320, 162});

	// Set up GUI Layout
	add(mProductGrid, {constants::MARGIN, 25});
	mProductGrid.size({140, 110});
	mProductGrid.showTooltip(true);
	mProductGrid.hide();
	mProductGrid.selectionChanged().connect(this, &FactoryProduction::productSelectionChanged);

	add(btnOkay, {233, 138});
	btnOkay.size({40, 20});
	btnOkay.click().connect(this, &FactoryProduction::btnOkayClicked);

	add(btnCancel, {276, 138});
	btnCancel.size({40, 20});
	btnCancel.click().connect(this, &FactoryProduction::btnCancelClicked);

	add(btnClearSelection, {5, 138});
	btnClearSelection.size({mProductGrid.size().x, 20});
	btnClearSelection.click().connect(this, &FactoryProduction::btnClearSelectionClicked);

	add(btnApply, {mProductGrid.size().x + 12, btnClearSelection.positionY()});
	btnApply.size({40, 20});
	btnApply.click().connect(this, &FactoryProduction::btnApplyClicked);

	add(chkIdle, {mProductGrid.size().x + 12, 115});
	chkIdle.size({50, 20});
	chkIdle.click().connect(this, &FactoryProduction::chkIdleClicked);

	rbOne.size({40,13});
	rbTwo.size({40,13});
	rbThree.size({40,13});

	add(rbgOptions, {mProductGrid.size().x + 12, 25});
	rbgOptions.size({50,80});
	rbgOptions.add(rbOne);
	rbgOptions.add(rbTwo);
	rbgOptions.add(rbThree);

	/*
	add(mTable, {mProductGrid.size().x + 12, 25}); //{mRect.startPoint() + NAS2D::Vector{constants::MARGIN * 2 + mProductGrid.size().x, 25}
	mTable.size({50,80});

	chkOne.size({50, 20});
	mTable.add(chkOne, {0,0});

	chkTwo.size({50, 20});
	mTable.add(chkTwo, {0,1});

	chkThree.size({50, 20});
	mTable.add(chkThree, {0,2});

	btnOne.size({50, 20});
	mTable.add(btnOne, {1,0});

	btnTwo.size({50, 20});
	mTable.add(btnTwo, {1,1});

	btnThree.size({50, 20});
	mTable.add(btnThree, {1,2});

	mTable.print();
	*/
}


void FactoryProduction::clearProduct()
{
	mProduct = ProductType::PRODUCT_NONE;
	mProductCost.clear();
	mProductGrid.clearSelection();
}


void FactoryProduction::hide()
{
	Control::hide();
	mFactory = nullptr;
	clearProduct();
}


void FactoryProduction::productSelectionChanged(const IconGrid::IconGridItem* _item)
{
	if (!mFactory) { return; }

	if (!_item)
	{
		mProductCost.clear();
		return;
	}

	mProduct = static_cast<ProductType>(_item->meta);
	mProductCost = productCost(mProduct);
}


void FactoryProduction::btnOkayClicked()
{
	mFactory->productType(mProduct);
	hide();
}


void FactoryProduction::btnApplyClicked()
{
	mFactory->productType(mProduct);
}


void FactoryProduction::btnCancelClicked()
{
	hide();
}


void FactoryProduction::btnClearSelectionClicked()
{
	clearProduct();
	btnApplyClicked();
}


void FactoryProduction::chkIdleClicked()
{
	if (!mFactory) { return; }

	mFactory->forceIdle(chkIdle.checked());
}


void FactoryProduction::factory(Factory* newFactory)
{
	mFactory = newFactory;

	if (!mFactory) { return; }

	mProductGrid.clear();
	clearProduct();

	// destroyed factories can't produce anything at all ever.
	if (mFactory->destroyed())
	{
		chkIdle.enabled(false);
		chkIdle.checked(false);
		return;
	}

	chkIdle.checked(mFactory->isIdle());

	const Factory::ProductionTypeList& ptlist = mFactory->productList();

	// The production type list from the factory should never, ever be empty. If it is there is
	// a clear logic error somewhere so fail very loudly and conspicuously here.
	if (ptlist.empty())
	{
		throw std::runtime_error("FactoryProduction::factory(): Factory provided with an empty production type list.");
	}

	for (std::size_t i = 0; i < ptlist.size(); ++i)
	{
		mProductGrid.addItem(productDescription(ptlist[i]), ptlist[i], ptlist[i]);
	}

	if (mFactory->productType() == ProductType::PRODUCT_NONE) { mProductGrid.clearSelection(); }
	else { mProductGrid.selection_meta(static_cast<int>(mFactory->productType())); }

	mProduct = mFactory->productType();
	mProductCost = productCost(mFactory->productType());
}


void FactoryProduction::update()
{
	if (!visible()) { return; }

	Window::update();

	/*
	StringTable stringTable(2, 5);
	stringTable.position(mRect.startPoint() + NAS2D::Vector{constants::MARGIN * 2 + mProductGrid.size().x, 25});
	stringTable.setColumnJustification(1, StringTable::Justification::Right);

	stringTable.setColumnText(0,
		{
			"Turns Completed:",
			"Common Metals:",
			"Common Minerals:",
			"Rare Metals:",
			"Rare Minerals:"
		});

	stringTable.setColumnText(1,
		{
			std::to_string(mFactory->productionTurnsCompleted()) + " of " + std::to_string(mProductCost.turnsToBuild()),
			mProductCost.commonMetals()* mProductCost.turnsToBuild(),
			mProductCost.commonMinerals()* mProductCost.turnsToBuild(),
			mProductCost.rareMetals()* mProductCost.turnsToBuild(),
			mProductCost.rareMinerals()* mProductCost.turnsToBuild()
		});

	stringTable.computeRelativeCellPositions();
	stringTable.draw(Utility<Renderer>::get());
	*/
}
