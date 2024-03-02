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
/// <param name="_InfoIndex">���� �⺻�� 0</param>
void UImageRenderer::SetImage(std::string_view _Name, int _InfoIndex)
{
	Image = UEngineResourcesManager::GetInst().FindImg(_Name);

	if (nullptr == Image)
	{
		MsgBoxAssert(std::string(_Name) + "�̹����� �������� �ʽ��ϴ�.");
		return;
	}

	InfoIndex = _InfoIndex;
}

void UImageRenderer::Render(float _DeltaTime)
{
	// �̹����� �ؽ�Ʈ�� Render �ؾ� �ϱ� ������ ������ Render�� ImageRender�Լ��� �̵�.

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
	Info.Times = _Inters;

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
		MsgBoxAssert("�̹����� �������� �ʴ� Renderer �Դϴ�.");
	}

	if (nullptr != CurAnimation)
	{
		Image = CurAnimation->Image;
		InfoIndex = CurAnimation->Update(_DeltaTime);
	}

	FTransform RendererTrans = GetRenderTransForm();

	// TransColor ����
	// Ư�� ����� 1��Ʈ�� ���̰� ���� �ʴ� ������ ������� �ʴ´�.
	// TransCopy������.
	// PNG�� ��쿡��.
	EWindowImageType ImageType = Image->GetImageType();

	// �̹��� ũ�� �ڵ� ����.
	const UImageInfo& Info = Image->ImageInfo(InfoIndex);

	// �̹��� ����(���� / ������ / �߾�)
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

	// �̹��� ũ�� �ڵ�����.
	if (true == AutoImageScaleValue)
	{
		RendererTrans.SetScale(Info.CuttingTrans.GetScale());
	}

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
		MsgBoxAssert("Ư�� �� �� ���� ó���� �Ұ����� �̹��� �Դϴ�.");
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
