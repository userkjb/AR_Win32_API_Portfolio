#pragma once
#include "EnginePath.h"
#include "EngineSerializer.h"

/// <summary>
/// Open Mode
/// </summary>
enum class IOOpenMode
{
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

	void Open(IOOpenMode _OpenType, IODataType _DataType);
	void Save(UEngineSerializer& _Data);
	void Close();

protected :

private :
	FILE* FileHandle = nullptr;

};