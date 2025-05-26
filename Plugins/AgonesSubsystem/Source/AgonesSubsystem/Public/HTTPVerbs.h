#pragma once

class FHttpVerb
{
public:
	enum EVerb
	{
		Get,
		Post,
		Put,
		Patch
	};

	// ReSharper disable once CppNonExplicitConvertingConstructor
	FHttpVerb(const EVerb Verb) : Verb(Verb)
	{
	}

	FString ToString() const
	{
		switch (Verb)
		{
		case Post:
			return TEXT("POST");
		case Put:
			return TEXT("PUT");
		case Patch:
			return TEXT("PATCH");
		case Get:
		default:
			return TEXT("GET");
		}
	}

private:
	const EVerb Verb;
};