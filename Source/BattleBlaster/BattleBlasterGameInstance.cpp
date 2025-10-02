// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleBlasterGameInstance.h"

#include "Kismet/GameplayStatics.h"

// ReSharper disable once CppMemberFunctionMayBeConst
void UBattleBlasterGameInstance::LoadNextLevel()
{
	if (CurrentLevelIndex < LastLevelIndex)
	{
		ChangeLevel(CurrentLevelIndex + 1);
	}
	else
	{
		RestartGame();
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UBattleBlasterGameInstance::RestartCurrentLevel()
{
	ChangeLevel(CurrentLevelIndex);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UBattleBlasterGameInstance::RestartGame()
{
	ChangeLevel(1);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UBattleBlasterGameInstance::ChangeLevel(int32 Index)
{
	if (Index > 0 && Index <= LastLevelIndex)
	{
		CurrentLevelIndex = Index;

		FString LevelNameString = FString::Printf(TEXT("Level_%d"), CurrentLevelIndex);

		UGameplayStatics::OpenLevel(GetWorld(), *LevelNameString);
	}
}
