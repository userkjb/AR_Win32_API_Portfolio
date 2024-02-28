#pragma once
#include "EnginePath.h"
#include <list>

class UEngineFile;

class UEngineDirectory : public UEnginePath
{
public :
	UEngineDirectory();
	/// <summary>
	/// 경로를 UEnginePath에 넘기는 생성자.
	/// </summary>
	/// <param name="_Path">폴더 경로</param>
	UEngineDirectory(const UEnginePath& _Path);
	~UEngineDirectory();

	/// <summary>
	/// 외부에서 사용할 파일들을 찾는 함수.
	/// </summary>
	/// <param name="_Ext">확장자들. 대/소문자 가리지 않음.</param>
	/// <param name="_Recursive">하위 폴더 까지 더 볼 것인지?</param>
	/// <returns></returns>
	std::list<UEngineFile> AllFile(
		std::vector<std::string> _Ext = std::vector<std::string>(),
		bool _Recursive = false);

	/// <summary>
	/// 경로 찾아서 올리기.
	/// </summary>
	/// <param name="_Path"></param>
	void MoveToSearchChild(std::string_view _Path);

	// 노가다 방식
	std::list<UEngineDirectory> AllDirectory(bool _Recursive = false);

	UEngineFile NewFile(std::string_view FileName);

private :

	void AllDirectoryRecursive(const std::string_view _Path, std::list<UEngineDirectory>& _Result, bool _Recursive = false);

	/// <summary>
	/// 클래스 내부에서 진짜로 파일을 찾는 함수.
	/// 찾아서 List(UEngineFile)에 담음.
	/// </summary>
	/// <param name="_Path">경로</param>
	/// <param name="_Result">경로 담겨 있는 list</param>
	/// <param name="_Ext">확장자</param>
	/// <param name="_Recursive">하위 폴더 까지 더 볼 것인지?</param>
	void AllFileRecursive(
		const std::string_view _Path, 
		std::list<UEngineFile>& _Result, 
		std::vector<std::string> _Ext = std::vector<std::string>(), 
		bool _Recursive = false);
};