// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "FactoryReport.h"

#include "../../Constants.h"
#include "../../FontManager.h"


static int SORT_BY_PRODUCT_POSITION = 0;

static Rectangle_2d	FACTORY_LISTBOX;


static Font* FONT;


/**
 * C'tor
 */
FactoryReport::FactoryReport()
{
	init();
}


/**
 * D'tor
 */
FactoryReport::~FactoryReport()
{}


/**
 * Sets up UI positions.
 */
void FactoryReport::init()
{
	FONT = Utility<FontManager>::get().font(constants::FONT_PRIMARY, 10);

	addControl("btnShowAll", &btnShowAll, 10, 10);
	btnShowAll.font(*FONT);
	btnShowAll.size(75, 20);
	btnShowAll.type(Button::BUTTON_TOGGLE);
	btnShowAll.toggle(true);
	btnShowAll.text("All");
	btnShowAll.click().connect(this, &FactoryReport::btnShowAllClicked);

	addControl("btnShowSurface", &btnShowSurface, 87, 10);
	btnShowSurface.font(*FONT);
	btnShowSurface.size(75, 20);
	btnShowSurface.type(Button::BUTTON_TOGGLE);
	btnShowSurface.text("Surface");
	btnShowSurface.click().connect(this, &FactoryReport::btnShowSurfaceClicked);

	addControl("btnShowUnderground", &btnShowUnderground, 164, 10);
	btnShowUnderground.font(*FONT);
	btnShowUnderground.size(75, 20);
	btnShowUnderground.type(Button::BUTTON_TOGGLE);
	btnShowUnderground.text("Underground");
	btnShowUnderground.click().connect(this, &FactoryReport::btnShowUndergroundClicked);

	addControl("btnShowActive", &btnShowActive, 10, 33);
	btnShowActive.font(*FONT);
	btnShowActive.size(75, 20);
	btnShowActive.type(Button::BUTTON_TOGGLE);
	btnShowActive.text("Active");
	btnShowActive.click().connect(this, &FactoryReport::btnShowActiveClicked);

	addControl("btnShowIdle", &btnShowIdle, 87, 33);
	btnShowIdle.font(*FONT);
	btnShowIdle.size(75, 20);
	btnShowIdle.type(Button::BUTTON_TOGGLE);
	btnShowIdle.text("Idle");
	btnShowIdle.click().connect(this, &FactoryReport::btnShowIdleClicked);

	addControl("btnShowDisabled", &btnShowDisabled, 164, 33);
	btnShowDisabled.font(*FONT);
	btnShowDisabled.size(75, 20);
	btnShowDisabled.type(Button::BUTTON_TOGGLE);
	btnShowDisabled.text("Disabled");
	btnShowDisabled.click().connect(this, &FactoryReport::btnShowDisabledClicked);

	addControl("cboTestBox", &cboFilterByProduct, 250, 33);
	cboFilterByProduct.font(*FONT);
	cboFilterByProduct.size(200, 20);

	cboFilterByProduct.addItem("None");
	cboFilterByProduct.addItem("Clothing");
	cboFilterByProduct.addItem("Maintenance Supplies");
	cboFilterByProduct.addItem("Medicine");
	cboFilterByProduct.addItem("Robodigger");
	cboFilterByProduct.addItem("Robodozer");
	cboFilterByProduct.addItem("Roboexplorer");
	cboFilterByProduct.addItem("Robominer");
	cboFilterByProduct.addItem("Road Materials");
	cboFilterByProduct.addItem("Truck");

	SORT_BY_PRODUCT_POSITION = cboFilterByProduct.rect().x() + cboFilterByProduct.rect().width() - FONT->width("Filter by Product");

	Control::resized().connect(this, &FactoryReport::resized);
}


void FactoryReport::resized(Control* c)
{
	int h = height();
	FACTORY_LISTBOX.x(positionX() + 10);
	FACTORY_LISTBOX.y(cboFilterByProduct.positionY() + cboFilterByProduct.height() + 10);
	FACTORY_LISTBOX.width(cboFilterByProduct.positionX() + cboFilterByProduct.width() - 10);
	FACTORY_LISTBOX.height(height() - 74);
}


void FactoryReport::filterButtonClicked()
{
	btnShowAll.toggle(false);
	btnShowSurface.toggle(false);
	btnShowUnderground.toggle(false);
	btnShowActive.toggle(false);
	btnShowIdle.toggle(false);
	btnShowDisabled.toggle(false);
}


void FactoryReport::btnShowAllClicked()
{
	filterButtonClicked();
	btnShowAll.toggle(true);
}


void FactoryReport::btnShowSurfaceClicked()
{
	filterButtonClicked();
	btnShowSurface.toggle(true);
}


void FactoryReport::btnShowUndergroundClicked()
{
	filterButtonClicked();
	btnShowUnderground.toggle(true);
}


void FactoryReport::btnShowActiveClicked()
{
	filterButtonClicked();
	btnShowActive.toggle(true);
}

void FactoryReport::btnShowIdleClicked()
{
	filterButtonClicked();
	btnShowIdle.toggle(true);
}


void FactoryReport::btnShowDisabledClicked()
{
	filterButtonClicked();
	btnShowDisabled.toggle(true);
}


void FactoryReport::update()
{
	Renderer& r = Utility<Renderer>::get();
	//r.drawBox(FACTORY_LISTBOX, 255, 255, 255);

	r.drawLine(cboFilterByProduct.rect().x() + cboFilterByProduct.rect().width() + 10, rect().y() + 10, cboFilterByProduct.rect().x() + cboFilterByProduct.rect().width() + 10, rect().y() + rect().height() - 10, 255, 255, 255);
	r.drawText(*FONT, "Filter by Product", SORT_BY_PRODUCT_POSITION, rect().y() + 10, 0, 185, 0);

	UIContainer::update();
}