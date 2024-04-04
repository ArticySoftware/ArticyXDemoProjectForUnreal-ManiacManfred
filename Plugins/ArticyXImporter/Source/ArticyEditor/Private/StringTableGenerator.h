//  
// Copyright (c) 2023 articy Software GmbH & Co. KG. All rights reserved.  
//

#pragma once

#include "Containers/UnrealString.h"
#include "Misc/Paths.h"

/**
 * Holds a content string which can be written to a file, specified in the constructor.
 */
// TODO: Share common code with CodeFileGenerator due to similarities
class StringTableGenerator
{
public:

	/**
	 * Executes a lambda. If the Lambda is nullptr (type nullptr_t), nothing bad happens.
	 */
	template<typename Lambda>
	void SafeExecute(Lambda Lamb);

	/**
	 * Creates a new string table generator then executes the ContentGenerator.
	 * At last, WriteToFile is called.
	 */
	template<typename Lambda>
	StringTableGenerator(const FString& TableName, const FString& Culture, Lambda ContentGenerator);

	/** Add a line to the content. */
	void Line(const FString& Key = "", const FString& SourceString = "");

private:

	FString Path;
	FString FileContent = "";

	/** Write the content to file. */
	void WriteToFile() const;
};

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//

template<typename Lambda>
void StringTableGenerator::SafeExecute(Lambda Lamb)
{
	Lamb();
}
template<>
inline void StringTableGenerator::SafeExecute(nullptr_t Lamb)
{
}

template <typename Lambda>
StringTableGenerator::StringTableGenerator(const FString& TableName, const FString& Culture, Lambda ContentGenerator)
{
	const FString FilePath = TEXT("ArticyContent/Generated") / TableName;
	if (Culture.IsEmpty())
	{
		Path = FPaths::ProjectContentDir() / FilePath;
	} else {
		Path = FPaths::ProjectContentDir() / TEXT("L10N") / Culture / FilePath;
	}
	Path += TEXT(".csv");
	
	Line("Key", "SourceString");
	bool bContentWritten = false;
	if(ensure(!std::is_null_pointer<Lambda>::value))
		bContentWritten = ContentGenerator(this) != 0;

	if (bContentWritten)
		WriteToFile();
}
