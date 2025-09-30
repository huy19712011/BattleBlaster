// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"

void ATower::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle FireTimerHandle;
	GetWorldTimerManager().SetTimer(FireTimerHandle, this, &ATower::CheckFireCondition, FireRate, true);
}

void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (IsInFireRange())
	{
		RotateTurret(Tank->GetActorLocation());
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ATower::CheckFireCondition()
{
	if (IsInFireRange())
	{
		Fire();
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
bool ATower::IsInFireRange()
{
	return (Tank && (FVector::Dist(GetActorLocation(), Tank->GetActorLocation()) <= FireRange));
}
