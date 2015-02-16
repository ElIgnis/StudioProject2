#pragma once

#include "SP2.h"

class CItem
{
public:
	//Vars
	string ItemName;
	double ItemPrice;
	Vector3 ItemShape;

	//States
	bool IsDestroyed;
	bool IsCheckedOut;

	void SetDetails(string SetName, double SetPrice, Vector3 SetShape);

	CItem(void);
	~CItem(void);
};