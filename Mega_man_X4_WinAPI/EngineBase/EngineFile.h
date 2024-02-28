#pragma once
#include "EnginePath.h"
#include "EngineSerializer.h"

/// <summary>
/// Open Mode
/// </summary>
enum class IOOpenMode
{
	None,
	Write,
	Read,
};

/// <summary>
/// Data Type
/// </summary>
enum class IODataType
{
	Binary,
	Text,
};

class UEngineFile : public UEnginePath
{
public :
	UEngineFile();
	UEngineFile(std::filesystem::path _Path);
	~UEngineFile();

	__int64 GetFileSize();

	void Open(IOOpenMode _OpenType, IODataType _DataType);
	void Save(UEngineSerializer& _Data);
	void Load(UEngineSerializer& _Data);

	void Close();

protected :

private :
	IOOpenMode OpenMode = IOOpenMode::None;
	FILE* FileHandle = nullptr;

};