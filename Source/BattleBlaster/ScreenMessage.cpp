// Fill out your copyright notice in the Description page of Project Settings.


#include "ScreenMessage.h"

// ReSharper disable once CppMemberFunctionMayBeConst
void UScreenMessage::SetMessageText(const FString& Message)
{
	MessageTextBlock->SetText(FText::FromString(Message));
}
