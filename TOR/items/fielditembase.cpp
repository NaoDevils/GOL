#include "fielditembase.h"

/**
* @brief Base class for all Items on the Field
*
*
*/
FieldItemBase::FieldItemBase() {
	itemType = misc;
	name = QString::number(rand());
}
