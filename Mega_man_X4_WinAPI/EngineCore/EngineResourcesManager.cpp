#include "EngineResourcesManager.h"
#include <EnginePlatform\WindowImage.h> // UEnginePath
#include <EngineBase/EngineString.h>
#include <EngineBase/EngineDebug.h>
#include <EngineCore/EngineCore.h> // GEngine

UEngineResourcesManager::UEngineResourcesManager()
{
}

UEngineResourcesManager::~UEngineResourcesManager()
{
}

UWindowImage* UEngineResourcesManager::LoadImg(std::string_view _Path)
{
	UEnginePath NewPath = UEnginePath(std::filesystem::path(_Path));
	std::string FileName = NewPath.GetFileName();
	return LoadImg(_Path, FileName);
}

UWindowImage* UEngineResourcesManager::LoadImg(std::string_view _Path, std::string_view _Name)
{
	// ���� string �� �빮�ڷ� ��ȯ
	std::string UpperName = UEngineString::ToUpper(_Name);

	if (true == Images.contains(UpperName))
	{
		// ������ ��峪 ����׿����� �ӷ��� ���� �ʿ䰡 ����.
		// ���ӿ��� ���� ������� ���� ���뿡 ���ؼ��� �ӷ��� ������ �ʴ´�.
		MsgBoxAssert(std::string("��� : ") + std::string(_Path) + "���ϸ� : " + std::string(_Name) + "�̹� �ε��� ������ �� �ε��Ϸ��� �߽��ϴ�");
		return nullptr;
	}

	UWindowImage* NewImage = new UWindowImage();
	NewImage->SetName(UpperName);
	NewImage->SetPath(_Path);
	NewImage->Load(GEngine->MainWindow.GetWindowImage());

	Images[UpperName] = NewImage;

	return nullptr;
}

UWindowImage* UEngineResourcesManager::FindImg(std::string_view _Name)
{
	std::string UpperName = UEngineString::ToUpper(_Name);

	if (false == Images.contains(UpperName))
	{
		MsgBoxAssert("���ϸ� : " + std::string(_Name) + "�� �������� �ʴ� �̹����Դϴ�");
		return nullptr;
	}

	return Images[UpperName];
}
