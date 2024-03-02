#include "ImageRenderer.h"
#include "EngineResourcesManager.h"
#include <EngineBase/EngineDebug.h>
#include "Actor.h"
#include "EngineCore.h"

UImageRenderer::UImageRenderer()
{
}

UImageRenderer::~UImageRenderer()
{
}

void UImageRenderer::SetOrder(int _Order)
{
	// 속한 Actor 설정
	AActor* Owner = GetOwner();
	// Actor 가 있는 Level 설정.
	ULevel* Level = Owner->GetWorld();

	std::map<int, std::list<UImageRenderer*>>& Renderers = Level->Renderers;

	// 나의 오더(Actor)가 바뀌기 전에 혹시나 내가 다른 곳에 속했다면
	// 그 곳을 빠져 나온다.
	Renderers[GetOrder()].remove(this);

	// 나의 오더(Actor)가 바뀐다.
	UTickObject::SetOrder(_Order);

	Renderers[GetOrder()].push_back(this);

}

/// <summary>
/// 인자로 받은 이름의 이미지를 찾아서 설정한다.
/// UEngineResourcesManager에서 이미지를 찾아 가져온다.
/// </summary>
/// <param name="_Name">이미지 이름</param>
/// <param name="_InfoIndex">순서 기본값 0</param>
void UImageRenderer::SetImage(std::string_view _Name, int _InfoIndex)
{
	Image = UEngineResourcesManager::GetInst().FindImg(_Name);

	if (nullptr == Image)
	{
		MsgBoxAssert(std::string(_Name) + "이미지가 존재하지 않습니다.");
		return;
	}

	InfoIndex = _InfoIndex;
}

void UImageRenderer::Render(float _DeltaTime)
{
	// 이미지와 텍스트를 Render 해야 하기 때문에 기존의 Render는 ImageRender함수로 이동.

	if (false == Text.empty())
	{
		TextRender(_DeltaTime);
	}
	else
	{
		ImageRender(_DeltaTime);
	}
}

void UImageRenderer::CreateAnimation(
	std::string_view _AnimationName, 
	std::string_view _ImageName, 
	int _Start, 
	int _End, 
	float _Inter, 
	bool _Loop)
{
	std::vector<int> Indexs;
	int Size = _End - _Start;

	for (int i = _Start; i <= _End; i++)
	{
		Indexs.push_back(i);
	}

	CreateAnimation(_AnimationName, _ImageName, Indexs, _Inter, _Loop);
}

void UImageRenderer::CreateAnimation(
	std::string_view _AnimationName,
	std::string_view _ImageName,
	std::vector<int> _Indexs, 
	float _Inter,
	bool _Loop)
{
	std::vector<float> Inters;
	//          12         0
	int Size = static_cast<int>(_Indexs.size());
	Inters.reserve(Size);
	for (int i = 0; i <= Size; i++)
	{
		Inters.push_back(_Inter);
	}

	CreateAnimation(_AnimationName, _ImageName, _Indexs, Inters, _Loop);
}

void UImageRenderer::CreateAnimation(
	std::string_view _AnimationName, 
	std::string_view _ImageName, 
	std::vector<int> _Indexs,
	std::vector<float> _Inters, 
	bool _Loop)
{
	UWindowImage* FindImage = UEngineResourcesManager::GetInst().FindImg(_ImageName);

	if (nullptr == FindImage)
	{
		MsgBoxAssert(std::string(_ImageName) + "이미지가 존재하지 않습니다.");
		return;
	}

	std::string UpperAniName = UEngineString::ToUpper(_AnimationName);

	if (true == AnimationInfos.contains(UpperAniName))
	{
		MsgBoxAssert(std::string(UpperAniName) + "라는 이름의 애니메이션이 이미 존재합니다.");
		return;
	}

	UAnimationInfo& Info = AnimationInfos[UpperAniName];
	Info.Name = UpperAniName;
	Info.Image = FindImage;
	Info.CurFrame = 0;
	Info.CurTime = 0.0f;
	Info.Loop = _Loop;
	Info.Times = _Inters;

	Info.Indexs = _Indexs;
}

void UImageRenderer::ChangeAnimation(std::string_view _AnimationName, bool _IsForce, int _StartIndex, float _Time)
{
	std::string UpperAniName = UEngineString::ToUpper(_AnimationName);

	if (false == AnimationInfos.contains(UpperAniName))
	{
		MsgBoxAssert(std::string(UpperAniName) + "라는 이름의 애니메이션이 존재하지 않습니다.");
		return;
	}

	// 지금 진행중인 애니메이션과 완전히 똑같은 애니메이션을 실행하라고하면 그걸 실행하지 않는다.
	if (false == _IsForce && nullptr != CurAnimation && CurAnimation->Name == UpperAniName)
	{
		return;
	}

	UAnimationInfo& Info = AnimationInfos[UpperAniName];
	CurAnimation = &Info;
	CurAnimation->CurFrame = _StartIndex;
	//CurAnimation->CurTime = _Time;
	CurAnimation->CurTime = CurAnimation->Times[_StartIndex];
	if (0.0f < _Time)
	{
		CurAnimation->CurTime = _Time;
	}
	CurAnimation->IsEnd = false;
}

void UImageRenderer::AnimationReset()
{
	CurAnimation = nullptr;
}

void UImageRenderer::TextRender(float _DeltaTime)
{
	FTransform RendererTrans = GetRenderTransForm();

	switch (TextEffect)
	{
	case 1:
		GEngine->MainWindow.GetBackBufferImage()->TextCopy(Text, Font, Size, RendererTrans, TextColor, TextColor2);
		break;
	case 2:
		GEngine->MainWindow.GetBackBufferImage()->TextCopyBold(Text, Font, Size, RendererTrans, TextColor);
		break;
	default:
		GEngine->MainWindow.GetBackBufferImage()->TextCopy(Text, Font, Size, RendererTrans, TextColor);
		break;
	}
}

void UImageRenderer::ImageRender(float _DeltaTime)
{
	if (nullptr == Image)
	{
		MsgBoxAssert("이미지가 존재하지 않는 Renderer 입니다.");
	}

	if (nullptr != CurAnimation)
	{
		Image = CurAnimation->Image;
		InfoIndex = CurAnimation->Update(_DeltaTime);
	}

	FTransform RendererTrans = GetRenderTransForm();

	// TransColor 원리
	// 특정 색상과 1비트도 차이가 나지 않는 색상은 출력하지 않는다.
	// TransCopy에서만.
	// PNG일 경우에만.
	EWindowImageType ImageType = Image->GetImageType();

	// 이미지 크기 자동 설정.
	const UImageInfo& Info = Image->ImageInfo(InfoIndex);

	// 이미지 정렬(왼쪽 / 오른쪽 / 중앙)
	switch (SortType) 
	{
	case EImageSortType::Left:
	{
		//const UImageInfo& Info = Image->ImageInfo(InfoIndex);
		//RendererTrans.AddPosition({ Info.CuttingTrans.GetScale().hX() , 0.0f });
		RendererTrans.AddPosition({ RendererTrans.GetScale().hX() , 0.0f });
		break;
	}
	default:
		break;
	}

	// 이미지 크기 자동설정.
	if (true == AutoImageScaleValue)
	{
		RendererTrans.SetScale(Info.CuttingTrans.GetScale());
	}

	switch (ImageType)
	{
	case EWindowImageType::IMG_NONE:
	{
		MsgBoxAssert("이미지 타입이 NONE 입니다.");
		break;
	}
	case EWindowImageType::IMG_BMP: // BMP 인 경우 일반적으로 TransCopy로 투명 처리를 한다.
		GEngine->MainWindow.GetBackBufferImage()->TransCopy(Image, RendererTrans, InfoIndex, TransColor);
		break;
	case EWindowImageType::IMG_PNG:
		if (0.0f == Angle)
		{
			GEngine->MainWindow.GetBackBufferImage()->AlphaCopy(Image, RendererTrans, InfoIndex, TransColor);
		}
		else
		{
			GEngine->MainWindow.GetBackBufferImage()->PlgCopy(Image, RendererTrans, InfoIndex, Angle * UEngineMath::DToR);
		}
		
		break;
	default:
		MsgBoxAssert("특정 색 및 투명 처리가 불가능한 이미지 입니다.");
		break;
	}
}

FTransform UImageRenderer::GetRenderTransForm()
{
	FTransform RendererTrans = GetActorBaseTransform();

	if (true == CameraEffect)
	{
		AActor* Actor = GetOwner();
		ULevel* World = Actor->GetWorld();
		FVector CameraPos = World->GetCameraPos();
		CameraPos *= CameraRatio;
		RendererTrans.AddPosition(-CameraPos);
	}

	return RendererTrans;
}

void UImageRenderer::BeginPlay()
{
	USceneComponent::BeginPlay();
}

void UImageRenderer::Tick(float _DeltaTime)
{
	USceneComponent::Tick(_DeltaTime);

	if (nullptr != CurAnimation)
	{
		Image = CurAnimation->Image;
		InfoIndex = CurAnimation->Update(_DeltaTime);
	}
}


// ===== AnimationInfo Class =====

int UAnimationInfo::Update(float _DeltaTime)
{
	IsEnd = false;
	CurTime -= _DeltaTime;

	if (0.0f >= CurTime)
	{
		CurTime = Times[CurFrame];
		++CurFrame;

		if (1 == Indexs.size())
		{
			IsEnd = true;
		}
	}

	//  6                 6
	if (Indexs.size() <= CurFrame)
	{
		if (1 < Indexs.size())
		{
			IsEnd = true;
		}

		if (true == Loop)
		{
			// //            0  1  2  3  4  5 
			//    Indexs => 20 21 22 23 24 25
			CurFrame = 0;
		}
		else
		{
			//                               
			//               0  1  2  3  4  5 
			//    Indexs => 20 21 22 23 24 25
			--CurFrame;
		}
	}

	int Index = Indexs[CurFrame];

	return Index;
}
