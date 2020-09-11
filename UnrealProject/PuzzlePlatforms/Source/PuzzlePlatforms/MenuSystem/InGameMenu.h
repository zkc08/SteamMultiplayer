// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuSystem/MenuInterface.h"
#include "PuzzlePlatformGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "InGameMenu.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UInGameMenu : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;

public:
	void SetMenuInterface(IMenuInterface* MenuInterface);

	void Setup();
	void TearDown();
	
	UFUNCTION()
	void BackButtonPressed();

	UFUNCTION()
	void QuitButtonPressed();

	IMenuInterface* MenuInterface;

private:

	UPROPERTY(meta = (BindWidget))
	class UButton* BackButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* QuitButton;

	UPuzzlePlatformGameInstance* GameInstance;
};