#include "Precompile.h"
#include "Item.h"

Item::Item()
	: Name_("")
	, Grade_(ItemGrade::None)
	, Type_(ItemType::None)
	, CoolTime_(0.f)
{
}

Item::~Item()
{
}
