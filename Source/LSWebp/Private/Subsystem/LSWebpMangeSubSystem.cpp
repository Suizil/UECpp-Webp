
#include "LSWebpMangeSubSystem.h"
#include "LSWebpCore.h"
#include "Engine/World.h"
#include "Async/Async.h"
#include "Log/LogLSWebp.h"
bool ULSWebpMangeSubSystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;

}

void ULSWebpMangeSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	GenerateWebpCallBackDelegate.BindUFunction(this, "GenerateWebpCallBack");
	WebpPictureInformation = MakeShareable(new FLSWebpPictureInformation);
}

void ULSWebpMangeSubSystem::Deinitialize()
{
	Super::Deinitialize();
	GenerateWebpCallBackDelegate.Unbind();
	ResetRecord();
}

void ULSWebpMangeSubSystem::Tick(float DeltaTime)
{
	if (ProcessType == ELSWebpProcessType::Recording)
	{
		RecordOneFrame(DeltaTime);
	}

}

TStatId ULSWebpMangeSubSystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(ULSWebpMangeSubSystem, STATGROUP_Tickables);
}

bool ULSWebpMangeSubSystem::IsTickable() const
{
	return !IsTemplate();
}

bool ULSWebpMangeSubSystem::BeginRecord(FString& InGeneratedWebpPicturesPath, TSharedPtr< FLSWebpPictureInformation> InWebpPictureInformation)
{
	if (!GetWorld())
	{
		return false;
	}
	if (ProcessType != ELSWebpProcessType::None)
	{
		return false;
	}
	if (!FLSWebpCore::CheckInWebpPictureSize(GetWorld(), InWebpPictureInformation))
	{
		return false;
	}
	if (!FLSWebpCore::CheckWebpPicturePath(InGeneratedWebpPicturesPath))
	{
		return false;
	}
	ResetRecord();
	ProcessType = ELSWebpProcessType::Recording;
	GeneratedWebpPicturesPath = InGeneratedWebpPicturesPath;
	WebpPictureInformation = InWebpPictureInformation;
	FlushRenderingCommands();

	ScreenHandle = UGameViewportClient::OnScreenshotCaptured().AddUObject(this,
		&ULSWebpMangeSubSystem::ScreenShotCallback);

	return true;
}

void ULSWebpMangeSubSystem::RecordOneFrame(float DeltaTime)
{
	FScreenshotRequest::RequestScreenshot(false);
	if (WebpTimestepMillisecond.Num() == 0)
	{
		WebpTimestepMillisecond.Add(0);
	}
	else
	{
		WebpTimestepMillisecond.Add(DeltaTime * 1000);
	}

}

void ULSWebpMangeSubSystem::EndRecord( FLSWebpFinishGenerateWebp& InFinshWebpBPDelegate)
{
	if (ProcessType != ELSWebpProcessType::Recording)
	{
		return;
	}
	ProcessType = ELSWebpProcessType::Generating;

	FinshWebpBPDelegate = InFinshWebpBPDelegate;



	AsyncTask(ENamedThreads::AnyThread, [&]()
		{
			FPlatformProcess::Sleep(0.2);
			FScopeLock LSLock(&LSWebpMutex);

	bool GenerateWebp = FLSWebpCore::GenerateDynamicWebpPicture(
		GeneratedWebpPicturesPath,
		WebpPictureInformation,
		WebPColor,
		WebpTimestepMillisecond
	);

	AsyncTask(ENamedThreads::GameThread, [&, GenerateWebp]() {

		GenerateWebpCallBackDelegate.ExecuteIfBound(GenerateWebp);

		});
		}
	);
}

void ULSWebpMangeSubSystem::GenerateWebpCallBack(bool bGenerateWebp)
{
	if (ScreenHandle.IsValid())
	{
		UGameViewportClient::OnScreenshotCaptured().Remove(ScreenHandle);
	}

	FinshWebpBPDelegate.ExecuteIfBound(bGenerateWebp);
	ResetRecord();
}

void ULSWebpMangeSubSystem::ResetRecord()
{

	ProcessType = ELSWebpProcessType::None;
	if (ScreenHandle.IsValid())
	{
		UGameViewportClient::OnScreenshotCaptured().Remove(ScreenHandle);
	}
	GeneratedWebpPicturesPath = TEXT("");
	WebpPictureInformation->ResetPictureInformation();
	WebPColor.Empty();
	WebpTimestepMillisecond.Empty();


}



void ULSWebpMangeSubSystem::ScreenShotCallback(int32 InWidth, int32 InHeight, const TArray<FColor>& InColors)
{

	AsyncTask(ENamedThreads::AnyThread, [&, InWidth, InHeight, InColors]() {


		FScopeLock LSLock(&LSWebpMutex);

	TArray<FColor> OutColors;
	for (int32 IndexY = 1; IndexY <= InHeight; IndexY++)
	{
		for (int32 IndexX = 1; IndexX <= InWidth; IndexX++)
		{

			int32 IndexXY = (IndexY - 1) * InWidth + IndexX;

			int32 IndexArray = IndexXY - 1;
			bool  bIndexX = (IndexX >= WebpPictureInformation->X0 + 1) && (IndexX <= WebpPictureInformation->X1 + 1);
			bool  bIndexY = (IndexY >= WebpPictureInformation->Y0 + 1) && (IndexY <= WebpPictureInformation->Y1 + 1);
			if (bIndexX && bIndexY)
			{
				OutColors.Add(InColors[IndexArray]);
			}
		}
	}
	if (OutColors.Num() == (WebpPictureInformation->X1 - WebpPictureInformation->X0 + 1) * (WebpPictureInformation->Y1 - WebpPictureInformation->Y0 + 1))
	{
		WebPColor.Add(OutColors);
		return;
	}


		});



}

