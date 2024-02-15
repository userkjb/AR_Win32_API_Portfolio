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
/// <param name="_InfoIndex">렌더 순서</param>
void UImageRenderer::SetImage(std::string_view _Name, int _InfoIndex)
{
	Image = UEngineResourcesManager::GetInst().FindImg(_Name);
}

void UImageRenderer::Render(float _DeltaTime)
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

	// 순서 중요!
	FTransform RendererTrans = GetTransform();

	// Actor
	FTransform ActorTrans = GetOwner()->GetTransform();

	// Component의 위치는 부모인 Actor에 상대적이다.
	// 부모 Actor의 위치를 더해줘야 한다.
	RendererTrans.AddPosition(ActorTrans.GetPosition());

	// TransColor 원리
	// 특정 색상과 1비트도 차이가 나지 않는 색상은 출력하지 않는다.
	// TransCopy에서만.
	// PNG일 경우에만.
	EWindowImageType ImageType = Image->GetImageType();

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
		GEngine->MainWindow.GetBackBufferImage()->AlphaCopy(Image, RendererTrans, InfoIndex, TransColor);
		break;
	default:
		MsgBoxAssert("특정 색 및 투명 처리가 불가능한 이미지 입니다.");
		break;
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

	//          12         0
	int Size = static_cast<int>(_Indexs.size());
	Info.Times.reserve(Size);
	for (int i = 0; i <= Size; i++)
	{
		Info.Times.push_back(_Inter);
	}

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
	CurAnimation->CurTime = _Time;
	if (0.0f >= _Time)
	{
		CurAnimation->CurTime = _Time;
	}
	CurAnimation->IsEnd = false;
}

void UImageRenderer::AnimationReset()
{
	CurAnimation = nullptr;
}

void UImageRenderer::BeginPlay()
{
	USceneComponent::BeginPlay();
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
	}

	//  6                 6
	if (Indexs.size() <= CurFrame)
	{
		IsEnd = true;
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
