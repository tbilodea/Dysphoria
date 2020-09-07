// Copyright T Bilodeau 2020


#include "RowCol.h"

void URowCol::SetRow(const int32 NewRow)
{
	Row = NewRow;
}

void URowCol::SetCol(const int32 NewCol)
{
	Col = NewCol;
}

int32 URowCol::GetRow() const
{
	return Row;
}

int32 URowCol::GetCol() const
{
	return Col;
}

URowCol* URowCol::TurnIntoRowCol(FRoomLocation RoomLocation)
{
	URowCol* RowCol = NewObject<URowCol>();
	RowCol->SetRow(RoomLocation.Row);
	RowCol->SetCol(RoomLocation.Col);
	return RowCol;
}
