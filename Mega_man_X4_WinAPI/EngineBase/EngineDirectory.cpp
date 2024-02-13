#include "EngineDirectory.h"
#include "EngineString.h"
#include "EngineFile.h"

UEngineDirectory::UEngineDirectory()
{
}

UEngineDirectory::UEngineDirectory(const UEnginePath& _Path)
	: UEnginePath(_Path.GetFullPath())
{
}

UEngineDirectory::~UEngineDirectory()
{
}

std::list<UEngineFile> UEngineDirectory::AllFile(std::vector<std::string> _Ext, bool _Recursive)
{
	// ���� ���� ��� ���� �ش� List�� �ִ´�.
	std::list<UEngineFile> Result;
	
	for (size_t i = 0; i < _Ext.size(); i++)
	{
		_Ext[i] = UEngineString::ToUpper(_Ext[i]);
	}

	// Path : EnginePath -> std::filesystem::path Path.
	AllFileRecursive(Path.string(), Result, _Ext, _Recursive);

	return Result;
}


void UEngineDirectory::AllFileRecursive(const std::string_view _Path, std::list<UEngineFile>& _Result, std::vector<std::string> _Ext, bool _Recursive)
{
	std::filesystem::directory_iterator DirIter = std::filesystem::directory_iterator(_Path);

	// ���� ����� ���丮�� ��ȸ.
	// ���丮�� ����. - entry
	for (const std::filesystem::directory_entry& Entry : DirIter)
	{
		std::filesystem::path Path = Entry.path();
		std::filesystem::path Ext = Entry.path().extension();
		std::string UpperExt = UEngineString::ToUpper(Ext.string());

		// ������ �ִٸ�,
		if (true == Entry.is_directory())
		{
			if (true == _Recursive)
			{
				AllFileRecursive(Path.string(), _Result, _Ext, _Recursive);
			}
			continue;
		}

		if (0 == _Ext.size())
		{
			_Result.push_back(UEngineFile(Path.string()));
			continue;
		}

		
		bool Check = false;

		for (size_t i = 0; i < _Ext.size(); i++)
		{
			if (_Ext[i] == UpperExt)
			{
				Check = true;
			}
		}

		if (true == Check)
		{
			_Result.push_back(UEngineFile(Path.string()));
		}
	}
}
