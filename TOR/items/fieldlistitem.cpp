#include "fieldlistitem.h"

/**
* @brief A Item for the FieldList
*
*
*/
FieldListItem::FieldListItem(FieldProperties *fieldProperties){
	setFlags(flags() | Qt::ItemIsSelectable);
	myFieldProperties = fieldProperties;
	setText(myFieldProperties->getName());
}

/**
* @brief Destructor
*
*
*/
FieldListItem::~FieldListItem(){}



