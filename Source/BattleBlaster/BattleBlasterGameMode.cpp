// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleBlasterGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "Tower.h"
#include "BattleBlasterGameInstance.h"

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

	class APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		ScreenMessageWidget = CreateWidget<UScreenMessage>(PlayerController, ScreenMessageClass);
		if (ScreenMessageWidget)
		{
			ScreenMessageWidget->AddToPlayerScreen();
			ScreenMessageWidget->SetMessageText("Get Ready from C++");
		}
	}

	CountdownSeconds = CountdownDelay;

	GetWorldTimerManager().SetTimer(CountdownTimerHandle, this,
	                                &ABattleBlasterGameMode::OnCountdownTimerTimeout, 1.0f, true);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ABattleBlasterGameMode::ActorDied(AActor* DeadActor)
{
	bool IsGameOver = false;
	// bool IsVictory = false;

	if (DeadActor == Tank)
	{
		// Tank just died
		// UE_LOG(LogTemp, Warning, TEXT("Tank Died, defeat!"));
		Tank->HandleDestruction();
		IsGameOver = true;
	}
	else
	{
		// A Tower just died
		if (ATower* DeadTower = Cast<ATower>(DeadActor))
		{
			// UE_LOG(LogTemp, Warning, TEXT("A Tower just Died!"));
			// DeadTower->Destroy();
			DeadTower->HandleDestruction();


			TowerCount--;
			if (TowerCount == 0)
			{
				// UE_LOG(LogTemp, Warning, TEXT("All Towers are dead, Victory!"));
				IsGameOver = true;
				IsVictory = true;
			}
		}
	}

	if (IsGameOver)
	{
		FString GameOverString = IsVictory ? "Victory!" : "Defeat!";
		// UE_LOG(LogTemp, Warning, TEXT("Game over: %s"), *GameOverString);
		ScreenMessageWidget->SetMessageText(GameOverString);
		ScreenMessageWidget->SetVisibility(ESlateVisibility::Visible);
		

		FTimerHandle GameOverTimerHandle;
		GetWorldTimerManager().SetTimer(GameOverTimerHandle, this,
		                                &ABattleBlasterGameMode::OnGameOverTimerTimeout, GameOverDelay, false);
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ABattleBlasterGameMode::OnGameOverTimerTimeout()
{
	if (class UGameInstance* GameInstance = GetGameInstance())
	{
		if (UBattleBlasterGameInstance* BattleBlasterGameInstance = Cast<UBattleBlasterGameInstance>(GameInstance))
		{
			if (IsVictory)
			{
				// load the next level
				BattleBlasterGameInstance->LoadNextLevel();
			}
			else
			{
				// load current level
				BattleBlasterGameInstance->RestartCurrentLevel();
			}
		}
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ABattleBlasterGameMode::OnCountdownTimerTimeout()
{
	CountdownSeconds -= 1;
	if (CountdownSeconds > 0)
	{
		// UE_LOG(LogTemp, Warning, TEXT("Countdown: %d"), CountdownSeconds);
		ScreenMessageWidget->SetMessageText(FString::FromInt(CountdownSeconds));
	}
	else if (CountdownSeconds == 0)
	{
		// UE_LOG(LogTemp, Warning, TEXT("Go!"));
		ScreenMessageWidget->SetMessageText("Go!!!");
		Tank->SetPlayerEnabled(true);				
	}
	else
	{
		GetWorldTimerManager().ClearTimer(CountdownTimerHandle);
		// UE_LOG(LogTemp, Warning, TEXT("Clear timer!"));
		ScreenMessageWidget->SetVisibility(ESlateVisibility::Hidden);
		
	}
}
