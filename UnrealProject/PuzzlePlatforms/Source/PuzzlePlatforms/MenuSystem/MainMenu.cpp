// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"

bool UMainMenu::Initialize() 
{
    bool Success = Super::Initialize();
    if (!Success) return false;

    if (!ensure(HostButton != nullptr)) return false;
    HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

    if (!ensure(JoinButton != nullptr)) return false;
    JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);

    if (!ensure(BackButton != nullptr)) return false;
    BackButton->OnClicked.AddDynamic(this, &UMainMenu::BackToMainMenu);

    if (!ensure(JoinGameButton != nullptr)) return false;
    JoinGameButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);
    
    if (!ensure(QuitGameButton != nullptr)) return false;
    QuitGameButton->OnClicked.AddDynamic(this, &UMainMenu::QuitGame);

    return true;
}

void UMainMenu::HostServer() 
{
    if (MenuInterface != nullptr)
    {
        MenuInterface->Host();
    }
}

void UMainMenu::OpenJoinMenu() 
{
    if (!ensure(MenuSwitcher != nullptr)) return;
    if (!ensure(JoinMenu != nullptr)) return;
    MenuSwitcher->SetActiveWidget(JoinMenu);
}

void UMainMenu::BackToMainMenu() 
{
    if (!ensure(MenuSwitcher != nullptr)) return;
    if (!ensure(MainMenu != nullptr)) return;
    MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenu::JoinServer() 
{
    if (MenuInterface != nullptr)
    {
        if (!ensure(IPAdressField != nullptr)) return;
        const FString Address = IPAdressField->GetText().ToString();
        MenuInterface->Join(Address);
    }
}

void UMainMenu::QuitGame() 
{
    if (MenuInterface != nullptr)
    {
        MenuInterface->QuitGame();
    }
}

void UMainMenu::SetMenuInterface(IMenuInterface* MenuInterface) 
{
    this->MenuInterface = MenuInterface;
}

void UMainMenu::Setup() 
{
    this->AddToViewport();

    UWorld* World = GetWorld();
    if (!ensure(World != nullptr)) return;

    APlayerController* PlayerController = World->GetFirstPlayerController();
    if (!ensure(PlayerController != nullptr)) return;


    FInputModeUIOnly InputModeData;

    InputModeData.SetWidgetToFocus(this->TakeWidget());
    InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

    PlayerController->SetInputMode(InputModeData);

    PlayerController->bShowMouseCursor = true;
}

void UMainMenu::TearDown() 
{
	this->RemoveFromViewport();

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	FInputModeGameOnly InputModeData;
	PlayerController->SetInputMode(InputModeData);

	PlayerController->bShowMouseCursor = false;
}
