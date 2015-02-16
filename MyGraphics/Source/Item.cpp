#include "Item.h"

CItem::CItem(void)
{

}

CItem::~CItem(void)
{

}

void CItem::SetDetails(string SetName, double SetPrice, Vector3 SetShape)
{
	ItemName = SetName;
	ItemPrice = SetPrice;
	ItemShape = SetShape;
}