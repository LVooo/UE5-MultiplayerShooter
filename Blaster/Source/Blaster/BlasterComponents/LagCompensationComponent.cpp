// Fill out your copyright notice in the Description page of Project Settings.


#include "LagCompensationComponent.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Components/BoxComponent.h"

ULagCompensationComponent::ULagCompensationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void ULagCompensationComponent::BeginPlay()
{
	Super::BeginPlay();
}

void ULagCompensationComponent::SaveFramePackage(FramePackage& Package)
{
	Character = Character == nullptr ? Cast<ABlasterCharacter>(GetOwner()) : Character;
	if (Character)
	{
		Package.Time = GetWorld()->GetTimeSeconds();
		for (auto& BoxPairs : Character->HitCollisionBoxes)
		{
			FBoxInformation BoxInformation;
			BoxInformation.Location = BoxPairs.Value->GetComponentLocation();
			BoxInformation.Rotation = BoxPairs.Value->GetComponentRotation();
			BoxInformation.BoxExtent = BoxPairs.Value->GetScaledBoxExtent();
			Package.HitBoxInfo.Add(BoxPairs.Key, BoxInformation);
		}
	}
}

FramePackage ULagCompensationComponent::InterpBetweenFrames(const FramePackage& OlderFrame,
	const FramePackage& YoungerFrame, float HitTime)
{
	const float Distance = YoungerFrame.Time - OlderFrame.Time;
	const float InterpFraction = FMath::Clamp((HitTime - OlderFrame.Time) / Distance, 0.f, 1.f);

	FramePackage InterpFramePackage;
	InterpFramePackage.Time = HitTime;

	for (auto& YoungerPair : YoungerFrame.HitBoxInfo)
	{
		const FName& BoxInfoName = YoungerPair.Key;

		const FBoxInformation& OlderBox = OlderFrame.HitBoxInfo[BoxInfoName];
		const FBoxInformation& YoungerBox = YoungerFrame.HitBoxInfo[BoxInfoName];

		FBoxInformation InterpBoxInfo;

		InterpBoxInfo.Location = FMath::VInterpTo(OlderBox.Location, YoungerBox.Location, 1.f, InterpFraction);
		InterpBoxInfo.Rotation = FMath::RInterpTo(OlderBox.Rotation, YoungerBox.Rotation, 1.f, InterpFraction);
		InterpBoxInfo.BoxExtent = YoungerBox.BoxExtent;

		InterpFramePackage.HitBoxInfo.Add(BoxInfoName, InterpBoxInfo);
	}

	return InterpFramePackage;
}

void ULagCompensationComponent::ShowFramePackage(const FramePackage& Package, const FColor& Color)
{
	for (auto& BoxInfo : Package.HitBoxInfo)
	{
		DrawDebugBox(
			GetWorld(),
			BoxInfo.Value.Location,
			BoxInfo.Value.BoxExtent,
			FQuat(BoxInfo.Value.Rotation),
			Color,
			false,
			4.f
			);
	}
}

void ULagCompensationComponent::ServerSideRewind(ABlasterCharacter* HitCharacter, const FVector_NetQuantize& TraceStart,
	const FVector_NetQuantize& HitLocation, float HitTime)
{
	bool bReturn =
		HitCharacter == nullptr ||
			HitCharacter->GetLagCompensation() == nullptr ||
				HitCharacter->GetLagCompensation()->FrameHistory.GetHead() == nullptr ||
					HitCharacter->GetLagCompensation()->FrameHistory.GetTail() == nullptr;
	if (bReturn) return;

	// 我们用来验证是否命中的frame package
	FramePackage FrameToCheck;
	bool bShouldInterpolate = true;
	// 被击中角色的frame history
	const TDoubleLinkedList<FramePackage>& History = HitCharacter->GetLagCompensation()->FrameHistory;
	const float OldestHistoryTime = History.GetHead()->GetValue().Time;
	const float NewestHistoryTime = History.GetTail()->GetValue().Time;
	if (OldestHistoryTime > HitTime)
	{
		// 时间过于久远，客户端延迟过高，不使用服务器倒带算法
		return;
	}
	if (OldestHistoryTime == HitTime)
	{
		FrameToCheck = History.GetHead()->GetValue();
		bShouldInterpolate = false;
	}
	if (NewestHistoryTime <= HitTime)
	{
		FrameToCheck = History.GetTail()->GetValue();
		bShouldInterpolate = false;
	}

	TDoubleLinkedList<FramePackage>::TDoubleLinkedListNode* Younger = History.GetTail();
	TDoubleLinkedList<FramePackage>::TDoubleLinkedListNode* Older = History.GetTail();
	// 判断是否older时间比命中时间更近
	while (Older->GetValue().Time > HitTime) 
	{
		// 找到 OlderTime < HitTime < YoungerTime的范围区间
		if (Older->GetPrevNode() == nullptr) break;
		Older = Older->GetPrevNode();
		if (Older->GetValue().Time > HitTime)
		{
			Younger = Older;
		}
	}
	// 高度不可能，但以防万一
	if (Older->GetValue().Time == HitTime)
	{
		FrameToCheck = Older->GetValue();
		bShouldInterpolate = false;
	}
	if (bShouldInterpolate)
	{
		// 在Older和Younger时间之间Interpolate
	}
	
}

void ULagCompensationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (FrameHistory.Num() <= 1)
	{
		FramePackage ThisFrame;
		SaveFramePackage(ThisFrame);
		FrameHistory.AddHead(ThisFrame);
	}
	else
	{
		float HistoryLength = FrameHistory.GetTail()->GetValue().Time - FrameHistory.GetHead()->GetValue().Time;
		while (HistoryLength > MaxRecordTime)
		{
			FrameHistory.RemoveNode(FrameHistory.GetHead());
			HistoryLength = FrameHistory.GetTail()->GetValue().Time - FrameHistory.GetHead()->GetValue().Time;
		}
		FramePackage ThisFrame;
		SaveFramePackage(ThisFrame);
		FrameHistory.AddHead(ThisFrame);

		ShowFramePackage(ThisFrame, FColor::Red);
	}
}
