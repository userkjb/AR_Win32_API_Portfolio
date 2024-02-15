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
	// ���� Actor ����
	AActor* Owner = GetOwner();
	// Actor �� �ִ� Level ����.
	ULevel* Level = Owner->GetWorld();

	std::map<int, std::list<UImageRenderer*>>& Renderers = Level->Renderers;

	// ���� ����(Actor)�� �ٲ�� ���� Ȥ�ó� ���� �ٸ� ���� ���ߴٸ�
	// �� ���� ���� ���´�.
	Renderers[GetOrder()].remove(this);

	// ���� ����(Actor)�� �ٲ��.
	UTickObject::SetOrder(_Order);

	Renderers[GetOrder()].push_back(this);

}

/// <summary>
/// ���ڷ� ���� �̸��� �̹����� ã�Ƽ� �����Ѵ�.
/// UEngineResourcesManager���� �̹����� ã�� �����´�.
/// </summary>
/// <param name="_Name">�̹��� �̸�</param>
/// <param name="_InfoIndex">���� ����</param>
void UImageRenderer::SetImage(std::string_view _Name, int _InfoIndex)
{
	Image = UEngineResourcesManager::GetInst().FindImg(_Name);
}

void UImageRenderer::Render(float _DeltaTime)
{
	if (nullptr == Image)
	{
		MsgBoxAssert("�̹����� �������� �ʴ� Renderer �Դϴ�.");
	}

	if (nullptr != CurAnimation)
	{
		Image = CurAnimation->Image;
		InfoIndex = CurAnimation->Update(_DeltaTime);
	}

	// ���� �߿�!
	FTransform RendererTrans = GetTransform();

	// Actor
	FTransform ActorTrans = GetOwner()->GetTransform();

	// Component�� ��ġ�� �θ��� Actor�� ������̴�.
	// �θ� Actor�� ��ġ�� ������� �Ѵ�.
	RendererTrans.AddPosition(ActorTrans.GetPosition());

	// TransColor ����
	// Ư�� ����� 1��Ʈ�� ���̰� ���� �ʴ� ������ ������� �ʴ´�.
	// TransCopy������.
	// PNG�� ��쿡��.
	EWindowImageType ImageType = Image->GetImageType();

	switch (ImageType)
	{
	case EWindowImageType::IMG_NONE:
	{
		MsgBoxAssert("�̹��� Ÿ���� NONE �Դϴ�.");
		break;
	}
	case EWindowImageType::IMG_BMP: // BMP �� ��� �Ϲ������� TransCopy�� ���� ó���� �Ѵ�.
		GEngine->MainWindow.GetBackBufferImage()->TransCopy(Image, RendererTrans, InfoIndex, TransColor);
		break;
	case EWindowImageType::IMG_PNG:
		GEngine->MainWindow.GetBackBufferImage()->AlphaCopy(Image, RendererTrans, InfoIndex, TransColor);
		break;
	default:
		MsgBoxAssert("Ư�� �� �� ���� ó���� �Ұ����� �̹��� �Դϴ�.");
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
		MsgBoxAssert(std::string(_ImageName) + "�̹����� �������� �ʽ��ϴ�.");
		return;
	}

	std::string UpperAniName = UEngineString::ToUpper(_AnimationName);

	if (true == AnimationInfos.contains(UpperAniName))
	{
		MsgBoxAssert(std::string(UpperAniName) + "��� �̸��� �ִϸ��̼��� �̹� �����մϴ�.");
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
		MsgBoxAssert(std::string(UpperAniName) + "��� �̸��� �ִϸ��̼��� �������� �ʽ��ϴ�.");
		return;
	}

	// ���� �������� �ִϸ��̼ǰ� ������ �Ȱ��� �ִϸ��̼��� �����϶���ϸ� �װ� �������� �ʴ´�.
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
