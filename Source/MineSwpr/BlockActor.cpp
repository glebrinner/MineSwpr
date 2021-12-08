// Fill out your copyright notice in the Description page of Project Settings.


#include "BlockActor.h"
#include "GridActor.h"
#include "Components/TextRenderComponent.h"


// Sets default values
ABlockActor::ABlockActor() : MineValue(0), IsMarkedAsMineByPlayer(false)
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		ConstructorHelpers::FObjectFinderOptional<UMaterial> BaseMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> BlueMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> OrangeMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> BlackMaterial;
		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/Puzzle/Meshes/PuzzleCube.PuzzleCube"))
			, BaseMaterial(TEXT("/Game/Puzzle/Meshes/BaseMaterial.BaseMaterial"))
			, BlueMaterial(TEXT("/Game/Puzzle/Meshes/BlueMaterial.BlueMaterial"))
			, OrangeMaterial(TEXT("/Game/Puzzle/Meshes/OrangeMaterial.OrangeMaterial"))
			, BlackMaterial(TEXT("/Game/Puzzle/Meshes/BlackMaterial.BlackMaterial"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component
	BlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh0"));
	BlockMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());
	//BlockMesh->SetRelativeScale3D(FVector(1.f,1.f,0.25f));
	BlockMesh->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.25f));
	BlockMesh->SetRelativeLocation(FVector(0.f,0.f,25.f));
	BlockMesh->SetMaterial(0, ConstructorStatics.BlueMaterial.Get());
	BlockMesh->SetupAttachment(DummyRoot);
	BlockMesh->OnClicked.AddDynamic(this, &ABlockActor::BlockClicked);
	BlockMesh->OnInputTouchBegin.AddDynamic(this, &ABlockActor::OnFingerPressedBlock);

	// Save a pointer to the orange material
	BaseMaterial = ConstructorStatics.BaseMaterial.Get();
	BlueMaterial = ConstructorStatics.BlueMaterial.Get();
	OrangeMaterial = ConstructorStatics.OrangeMaterial.Get();
	BlackMaterial = ConstructorStatics.BlackMaterial.Get();

	// CREATE NUMBER.

	// Create static mesh component
	MineText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("MineText"));
	//MineText->SetRelativeLocation(FVector(50.f, 50.f, 100.0f));
	MineText->SetRelativeLocation(FVector(-50.f, -50.f, 0.0f));
	MineText->SetRelativeRotation(FRotator(90.f, 180.f, 0.f));
	MineText->SetXScale(5.0f);
	MineText->SetYScale(5.0f);
	MineText->SetText(SpaceValue); //temp
	MineText->SetupAttachment(DummyRoot);
}

void ABlockActor::BlockClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked)
{
	if (ButtonClicked == EKeys::LeftMouseButton)
		HandleClicked();
	else if (ButtonClicked == EKeys::RightMouseButton && OwningGrid != nullptr && !OwningGrid->IsInGameOver())
	{
		if (IsMarkedAsMineByPlayer)
		{
			BlockMesh->SetMaterial(0, BlueMaterial);
			bIsActive = false;
		}
		else
		{
			BlockMesh->SetMaterial(0, OrangeMaterial);
			bIsActive = true;
		}
		IsMarkedAsMineByPlayer = !IsMarkedAsMineByPlayer;
	}
}


void ABlockActor::OnFingerPressedBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent)
{
	HandleClicked();
}

void ABlockActor::HandleClicked()
{
	// Check we are not already active
	if (!bIsActive && OwningGrid != nullptr && !OwningGrid->IsInGameOver())
	{
		bIsActive = true;

		if (MineValue == -1)
		{
			BlockMesh->SetMaterial(0, BlackMaterial);
			OwningGrid->SetGameOverState();
		}
		else
		{
			BlockMesh->DestroyComponent();
			ShowMineValue();
			OwningGrid->AddScore();
		}
	}
}

void ABlockActor::Highlight(bool bOn)
{
	// Do not highlight if the block has already been activated.
	if (bIsActive)
	{
		return;
	}

	if (bOn)
	{
		BlockMesh->SetMaterial(0, BaseMaterial);
	}
	else
	{
		BlockMesh->SetMaterial(0, BlueMaterial);
	}
}

void ABlockActor::SetMineValue(int newMineValue)
{
	MineValue = newMineValue;
}

void ABlockActor::ShowMineValue()
{
	MineText->SetText(FText::AsNumber(MineValue));
}
