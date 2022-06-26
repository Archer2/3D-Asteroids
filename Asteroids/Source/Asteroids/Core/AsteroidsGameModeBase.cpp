// Copyright Epic Games, Inc. All Rights Reserved.


#include "AsteroidsGameModeBase.h"
#include "Actors/ShipBase.h"

AAsteroidsGameModeBase::AAsteroidsGameModeBase()
{
	// Set default classes. These should be overridden to Blueprint Classes in BP extension if necessary
	DefaultPawnClass = AShipBase::StaticClass();
}
