// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "UObject/ConstructorHelpers.h"

#include "ServerRow.h"

UMainMenu::UMainMenu(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/MenuSysytem/UI/WBP_ServerRow"));
	if (!ensure(ServerRowBPClass.Class != nullptr)) return;

	ServerRowClass = ServerRowBPClass.Class;
}

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
    if (MenuInterface != nullptr)
    {
        MenuInterface->RefreshServerList();
    }
}

void UMainMenu::BackToMainMenu() 
{
    if (!ensure(MenuSwitcher != nullptr)) return;
    if (!ensure(MainMenu != nullptr)) return;
    MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenu::SetServerList(TArray<FString> ServerNames) 
{
    UWorld* World = this->GetWorld();
    if (!ensure(World != nullptr)) return;
        
    ServerList->ClearChildren();
        
    for (const FString& ServerName : ServerNames)
    {
	    UServerRow* Row = CreateWidget<UServerRow>(World, ServerRowClass);
		if (!ensure(Row != nullptr)) return;

        uint32 i = 0;
        Row->ServerName->SetText(FText::FromString(ServerName));
        Row->Setup(this, i);
        ++i;

		ServerList->AddChild(Row);    
    }
}

void UMainMenu::SelectIndex(uint32 Index) 
{
    SelectedIndex = Index;
    UpdateChildren();   
}

void UMainMenu::UpdateChildren() 
{
    for (int32 i = 0; i < ServerList->GetChildrenCount(); ++i)
    {
        auto Row = Cast<UServerRow>(ServerList->GetChildAt(i));
        if (Row != nullptr)
		{
            if (SelectedIndex.IsSet())
            {            
                Row->Selected = (SelectedIndex.GetValue() == i);
            }
		}
    }
}

void UMainMenu::JoinServer() 
{
    if (SelectedIndex.IsSet() && MenuInterface != nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("Selected index %d"), SelectedIndex.GetValue());
        MenuInterface->Join(SelectedIndex.GetValue());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Selected index not found"));
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


