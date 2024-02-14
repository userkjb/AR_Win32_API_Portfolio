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
	for (std::pair<const std::string, UWindowImage*>& Pair : Images)
	{
		delete Pair.second;
		Pair.second = nullptr;
	}

	Images.clear();
}

UWindowImage* UEngineResourcesManager::LoadImg(std::string_view _Path)
{
	UEnginePath NewPath = UEnginePath(std::filesystem::path(_Path));
	std::string FileName = NewPath.GetFileName();
	return LoadImg(_Path, FileName);
}

UWindowImage* UEngineResourcesManager::LoadImg(std::string_view _Path, std::string_view _Name)
{
	// 받은 string 을 대문자로 변환
	std::string UpperName = UEngineString::ToUpper(_Name);

	if (true == Images.contains(UpperName))
	{
		// 에디터 모드나 디버그에서는 속력을 따질 필요가 없다.
		// 게임에서 실제 실행되지 않을 내용에 대해서는 속력을 따지지 않는다.
		MsgBoxAssert(std::string("경로 : ") + std::string(_Path) + "파일명 : " + std::string(_Name) + "이미 로드한 파일을 또 로드하려고 했습니다");
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
		MsgBoxAssert("파일명 : " + std::string(_Name) + "은 존재하지 않는 이미지입니다");
		return nullptr;
	}

	return Images[UpperName];
}

void UEngineResourcesManager::CuttingImage(std::string_view _Name, int _X, int _Y)
{
	// 자를 대상 이미지를 찾는다.
	UWindowImage* FindImage = FindImg(_Name);

	if (nullptr == FindImage)
	{
		MsgBoxAssert("파일명 : " + std::string(_Name) + " 이 존재하지 않습니다.");
	}

	//FindImage->Cutting(_X, _Y);
}