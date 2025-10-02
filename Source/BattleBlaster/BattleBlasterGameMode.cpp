// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleBlasterGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "Tower.h"

void ABattleBlasterGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> Towers;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATower::StaticClass(), Towers);

	TowerCount = Towers.Num();
	UE_LOG(LogTemp, Warning, TEXT("Number of towers: %d"), TowerCount);

	if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
	{
		Tank = Cast<ATank>(PlayerPawn);
		if (!Tank)
		{
			UE_LOG(LogTemp, Warning, TEXT("GameMode: Failed to find the tank actor!"));
		}
	}

	int32 LoopIndex = 0;
	while (LoopIndex < TowerCount)
	{
		if (AActor* TowerActor = Towers[LoopIndex])
		{
			ATower* Tower = Cast<ATower>(TowerActor);
			if (Tower && Tank)
			{
				Tower->Tank = Tank;
				UE_LOG(LogTemp, Warning, TEXT("%s setting the tank variable"), *Tower->GetActorNameOrLabel());
			}
		}
		LoopIndex++;
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ABattleBlasterGameMode::ActorDied(AActor* DeadActor)
{
	if (DeadActor == Tank)
	{
		// Tank just died
		UE_LOG(LogTemp, Warning, TEXT("Tank Died, defeat!"));
	}
	else
	{
		// A Tower just died
		if (ATower* DeadTower = Cast<ATower>(DeadActor))
		{
			UE_LOG(LogTemp, Warning, TEXT("A Tower just Died!"));
			DeadTower->Destroy();
			
			TowerCount--;
			if (TowerCount == 0)
			{
				UE_LOG(LogTemp, Warning, TEXT("All Towers are dead, Victory!"));
			}
		}
	}
}
