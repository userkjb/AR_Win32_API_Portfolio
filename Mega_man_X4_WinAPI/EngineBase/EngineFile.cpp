#include "EngineFile.h"
#include "EngineDebug.h"
#include "EngineSerializer.h" //


UEngineFile::UEngineFile()
{
}

UEngineFile::UEngineFile(std::filesystem::path _Path)
	: UEnginePath(_Path)
{
}

UEngineFile::~UEngineFile()
{
	Close();
}

void UEngineFile::Open(IOOpenMode _OpenType, IODataType _DataType)
{
	std::string Path = GetFullPath();

	std::string Mode;

	switch (_OpenType)
	{
	case IOOpenMode::Write:
		Mode += "w";
		break;
	case IOOpenMode::Read:
		Mode += "r";
		break;
	default:
		break;
	}

	switch (_DataType)
	{
	case IODataType::Binary:
		Mode += "b";
		break;
	case IODataType::Text:
		Mode += "t";
		break;
	default:
		break;
	}

	fopen_s(&FileHandle, Path.c_str(), Mode.c_str());

	if (nullptr == FileHandle)
	{
		MsgBoxAssert("파일 오픈에 실패했습니다" + Path);
	}
}

void UEngineFile::Save(UEngineSerializer& _Data)
{
	std::vector<char>& SaveData = _Data.Data;

	char* StartPtr = &SaveData[0];
	fwrite(StartPtr, SaveData.size(), 1, FileHandle);
}

void UEngineFile::Close()
{
	if (nullptr != FileHandle)
	{
		fclose(FileHandle);
	}
}
