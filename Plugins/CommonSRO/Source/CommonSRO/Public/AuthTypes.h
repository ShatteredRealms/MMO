#pragma once

#include "GenericPlatform/GenericPlatformHttp.h"
#include "CommonSRO.h"

/**
 * Contains URL details for Windows SRO interaction
 */
struct FSROLoginURL
{
	/** The SRO exchange token auth endpoint */
	FString LoginUrl;
	/** The redirect url for SRO to redirect to upon completion */
	FString LoginRedirectUrl;
	/** Port to append to the LoginRedirectURL when communicating with SRO auth services */
	int32 RedirectPort = 9000;
	/** The client id given to us by SRO */
	FString ClientId;
	/** Config based list of permission scopes to use when logging in */
	TArray<FString> ScopeFields;
	/** A value used to verify our response came from our server */
	FString State;
	/** Should request Offline Access. Refresh token is only received on first authorization */
	bool bRequestOfflineAccess = false;

	bool IsValid() const
	{
		// LoginUrl skipped because it's filled in by discovery service
		return !LoginRedirectUrl.IsEmpty() && !ClientId.IsEmpty() && (ScopeFields.Num() > 0);
	}

	FString GenerateNonce()
	{
		// random number to represent client generated state for verification on login
		State = FString::FromInt(FMath::Rand() % 100000);
		return State;
	}

	FString GetRedirectURL() const
	{
		//const FString Redirect = TEXT("urn:ietf:wg:oauth:2.0:oob:auto");
		return FString::Printf(TEXT("%s:%d"), *LoginRedirectUrl, RedirectPort);
	}

	FString GetURL() const
	{
		FString Scopes = FString::Join(ScopeFields, TEXT(" "));
		
		const FString Redirect = GetRedirectURL();
		
		const FString ParamsString = FString::Printf(TEXT("redirect_uri=%s&scope=%s&response_type=code&client_id=%s&state=%s"),
			*Redirect, *FGenericPlatformHttp::UrlEncode(Scopes), *ClientId, *State);

		// auth url to spawn in browser
		const FString URLString = FString::Printf(TEXT("%s?%s%s"),
			*LoginUrl, *ParamsString, bRequestOfflineAccess?TEXT("&access_type=offline"):TEXT(""));
		
		return URLString;
	}
};

/**
 * Types of supported auth tokens
 */
enum class ESROAuthTokenType : uint8
{
	/** Simple single use token meant to be converted to an access token */
	ExchangeToken,
	/** Refresh token meant to be fully converted to an access token */
	RefreshToken,
	/** Allows for access to SRO APIs using verified user account credentials */
	AccessToken,
	/** SRO Client & Secret login used for server backend */
	ClientCredentials,
};

class FJsonWebTokenSRO
{

public:

	/**
	 * Parse a SRO JWT into its constituent parts
	 *
	 * @param InJWTStr JWT id token representations
	 *
	 * @return true if parsing was successful, false otherwise
	 */
	bool Parse(const FString& InJWTStr);

	bool Validate(const FString& Issuer, const TArray<FString>& Aud) const;

	class FHeader : public FJsonSerializable 
	{

	public:
		/** Algorithm used to sign the token */
		FString Alg;
		/** Key Id */
		FString Kid;
		BEGIN_JSON_SERIALIZER
			JSON_SERIALIZE("alg", Alg);
			JSON_SERIALIZE("kid", Kid);
		END_JSON_SERIALIZER
	};

	class FPayload : public FJsonSerializable
	{

	public:

		/** Subscriber */
		FString Sub;
		/** User first name */
		FString FirstName;
		/** User last name */
		FString LastName;
		/** User full name */
		FString RealName;
		/** Issuer */
		FString ISS;
		/** Time of token grant */
		double IAT;
		/** Time of token expiration */
		double EXP;
		FString ATHash;
		/** Audience */
		FString Aud;
		/** Is the email address verified */
		bool bEmailVerified;
		FString AZP;
		/** User email address */
		FString Email;
		/** User profile picture */
		FString Picture;
		/** User locale */
		FString Locale;

		BEGIN_JSON_SERIALIZER
			JSON_SERIALIZE("sub", Sub);
			JSON_SERIALIZE("given_name", FirstName);
			JSON_SERIALIZE("family_name", LastName);
			JSON_SERIALIZE("name", RealName);
			JSON_SERIALIZE("iss", ISS);
			JSON_SERIALIZE("iat", IAT);
			JSON_SERIALIZE("exp", EXP);
			JSON_SERIALIZE("at_hash", ATHash);
			JSON_SERIALIZE("aud", Aud);
			JSON_SERIALIZE("email_verified", bEmailVerified);
			JSON_SERIALIZE("azp", AZP);
			JSON_SERIALIZE("email", Email);
			JSON_SERIALIZE("picture", Picture);
			JSON_SERIALIZE("locale", Locale);
		END_JSON_SERIALIZER
	};

	/** JWT header */
	FHeader Header;
	/** JWT payload */
	FPayload Payload;
};

enum ESROExchangeToken : uint8 { SROExchangeToken };
enum ESRORefreshToken : uint8 { SRORefreshToken };

/**
 * SRO auth token representation, both exchange and access tokens
 */
class FAuthTokenSRO :
	public FJsonSerializable
{
public:

	FAuthTokenSRO()
		: AuthType(ESROAuthTokenType::ExchangeToken)
		, ExpiresIn(0)
		, ExpiresInUTC(0)
	{
	}

	explicit FAuthTokenSRO(const FString& InExchangeToken, ESROExchangeToken)
		: AuthType(ESROAuthTokenType::ExchangeToken)
		, AccessToken(InExchangeToken)
		, ExpiresIn(0)
		, ExpiresInUTC(0)
	{
	}

	explicit FAuthTokenSRO(const FString& InRefreshToken, ESRORefreshToken)
		: AuthType(ESROAuthTokenType::RefreshToken)
		, ExpiresIn(0)
		, RefreshToken(InRefreshToken)
		, ExpiresInUTC(0)
	{
	}

	FAuthTokenSRO(FAuthTokenSRO&&) = default;
	FAuthTokenSRO(const FAuthTokenSRO&) = default;
	FAuthTokenSRO& operator=(FAuthTokenSRO&&) = default;
	FAuthTokenSRO& operator=(const FAuthTokenSRO&) = default;

	/**
	 * Parse a SRO json auth response into an access/refresh token
	 *
	 * @param InJsonStr json response containing the token information
	 *
	 * @return true if parsing was successful, false otherwise
	 */
	bool Parse(const FString& InJsonStr);

	/**
	 * Parse a SRO json auth response into an access/refresh token
	 *
	 * @param InJsonObject json object containing the token information
	 *
	 * @return true if parsing was successful, false otherwise
	 */
	bool Parse(TSharedPtr<FJsonObject> InJsonObject);

	/**
	 * Parse a SRO json auth refresh response into an access/refresh token
	 *
	 * @param InJsonStr json response containing the token information
	 * @param InOldAuthToken previous auth token with refresh token information
	 *
	 * @return true if parsing was successful, false otherwise
	 */
	bool Parse(const FString& InJsonStr, const FAuthTokenSRO& InOldAuthToken);

	/** @return true if this auth token is valid, false otherwise */
	bool IsValid() const
	{
		if (AuthType == ESROAuthTokenType::ExchangeToken)
		{
			return !AccessToken.IsEmpty() && RefreshToken.IsEmpty();
		}
		else if (AuthType == ESROAuthTokenType::RefreshToken)
		{
			return !RefreshToken.IsEmpty();
		}
		else
		{
			return !AccessToken.IsEmpty();
		}
	}

	/** @return true if the token is expired */
	bool IsExpired() const
	{
		if (AuthType == ESROAuthTokenType::AccessToken)
		{
			return (FDateTime::UtcNow() > ExpiresInUTC);
		}

		// For now don't worry about refresh/exchange token expiration
		return false;
	}

	inline bool GetAuthData(const FString& Key, FString& OutVal) const
	{
		const FString* FoundVal = AuthData.Find(Key);
		if (FoundVal != NULL)
		{
			OutVal = *FoundVal;
			return true;
		}
		return false;
	}

	inline void AddAuthData(FString Key, const FString& Value)
	{
		AuthData.Add(MoveTemp(Key), Value);
	}

	/** Type of auth this token represents */
	ESROAuthTokenType AuthType;
	/** Access or exchange token */
	FString AccessToken;
	/** Type of token (valid for AccessToken only) */
	FString TokenType;
	/** Number of seconds until this token expires at time of receipt */
	double ExpiresIn;
	/** Refresh token for generating new AccessTokens */
	FString RefreshToken;
	/** Id token in JWT form */
	FString IdToken;
	/** Parsed IdToken */
	FJsonWebTokenSRO IdTokenJWT;
	/** Absolute time, in UTC, when this token will expire */
	FDateTime ExpiresInUTC;

private:

	void AddAuthAttributes(const TSharedPtr<FJsonObject>& JsonUser);

	/** Any addition auth data associated with the token */
	FJsonSerializableKeyValueMap AuthData;

	BEGIN_JSON_SERIALIZER
		JSON_SERIALIZE("access_token", AccessToken);
		JSON_SERIALIZE("token_type", TokenType);
		JSON_SERIALIZE("expires_in", ExpiresIn);
		JSON_SERIALIZE("refresh_token", RefreshToken);
		JSON_SERIALIZE("id_token", IdToken);
	END_JSON_SERIALIZER
};

/**
 * Container for configuration info related to all SRO API services
 */
class FSROOpenIDConfiguration :
	public FJsonSerializable
{
public:

	/**
	 * Constructor
	 */
	FSROOpenIDConfiguration()
		: bInitialized(false)
		, AuthEndpoint(TEXT("https://sso.shatteredrealmsonline.com/realms/default/protocol/openid-connect/auth"))
		, TokenEndpoint(TEXT("https://sso.shatteredrealmsonline.com/realms/default/protocol/openid-connect/token"))
		, UserInfoEndpoint(TEXT("https://sso.shatteredrealmsonline.com/realms/default/protocol/openid-connect/userinfo")) 
		, RevocationEndpoint(TEXT("https://sso.shatteredrealmsonline.com/realms/default/protocol/openid-connect/revoke"))
	{
	}

	/** @return true if this data is valid, false otherwise */
	bool IsValid() const { return bInitialized; }

	/**
	 * Parse a Json response from SRO into a this data structure
	 *
	 * @return true if parsing was successful, false otherwise
	 */
	bool Parse(const FString& InJsonStr)
	{
		if (!InJsonStr.IsEmpty() && FromJson(InJsonStr))
		{
			bInitialized = true;
		}

		return bInitialized;
	}

	/** Has this data been setup */
	bool bInitialized;
	/** Issuer of the configuration information */
	FString Issuer;
	/** Authentication endpoint for login */
	FString AuthEndpoint;
	/** Token exchange endpoint */
	FString TokenEndpoint;
	/** User profile request endpoint */
	FString UserInfoEndpoint;
	/** Auth revocation endpoint */
	FString RevocationEndpoint;
	/** JWT Cert endpoint */
	FString JWKSURI;
	/** A value used to verify our response came from our server */
	FString DiscoveryUrl = "https://sso.shatteredrealmsonline.com/realms/default/.well-known/openid-configuration";

private:

	BEGIN_JSON_SERIALIZER
		JSON_SERIALIZE("issuer", Issuer);
		JSON_SERIALIZE("authorization_endpoint", AuthEndpoint);
		JSON_SERIALIZE("token_endpoint", TokenEndpoint);
		JSON_SERIALIZE("userinfo_endpoint", UserInfoEndpoint);
		JSON_SERIALIZE("revocation_endpoint", RevocationEndpoint);
		JSON_SERIALIZE("jwks_uri", JWKSURI);
	END_JSON_SERIALIZER
};

/**
 * SRO error from JSON payload
 */
class FErrorSRO :
	public FJsonSerializable
{
public:

	/**
	 * Constructor
	 */
	FErrorSRO()
	{
	}
	
	/** Error type */
	FString Error;
	/** Description of error */
	FString Error_Description;

	FString ToDebugString() const { return FString::Printf(TEXT("%s [Desc:%s]"), *Error, *Error_Description); }

	BEGIN_JSON_SERIALIZER
		JSON_SERIALIZE("error", Error);
		JSON_SERIALIZE("error_description", Error_Description);
	END_JSON_SERIALIZER
};

__forceinline bool FJsonWebTokenSRO::Parse(const FString& InJWTStr)
{
	bool bSuccess = false;

	TArray<FString> Tokens;
	InJWTStr.ParseIntoArray(Tokens, TEXT("."));
	if (Tokens.Num() == 3)
	{
		// Figure out if any Base64 padding adjustment is necessary
		static const TCHAR* const Padding = TEXT("==");
		int32 Padding1 = (4 - (Tokens[0].Len() % 4)) % 4;
		int32 Padding2 = (4 - (Tokens[1].Len() % 4)) % 4;
		int32 Padding3 = (4 - (Tokens[2].Len() % 4)) % 4;
		if (Padding1 < 3 && Padding2 < 3 && Padding3 < 3)
		{
			Tokens[0].AppendChars(Padding, Padding1);
			Tokens[1].AppendChars(Padding, Padding2);
			Tokens[2].AppendChars(Padding, Padding3);

			// Decode JWT header
			FString HeaderStr;
			if (FBase64::Decode(Tokens[0], HeaderStr))
			{
				// Parse header
				if (Header.FromJson(HeaderStr))
				{
					// Decode JWT payload
					FString PayloadStr;
					if (FBase64::Decode(Tokens[1], PayloadStr))
					{
						// Parse payload
						if (Payload.FromJson(PayloadStr))
						{
							// @TODO - Verify that the ID token is properly signed by the issuer.SRO
							// issued tokens are signed using one of the certificates found at the URI specified in the jwks_uri field of the discovery document.
							//https://www.codescience.com/blog/2016/oauth2-server-to-server-authentication-from-salesforce-to-SRO-apis
							// exp	Required	The expiration time of the assertion, specified as seconds since 00:00:00 UTC, January 1, 1970. This value has a maximum of 1 hour after the issued time.
							// iat	Required	The time the assertion was issued, specified as seconds since 00:00:00 UTC, January 1, 1970.

							//Verify that the expiry time(exp) of the ID token has not passed.
							FDateTime ExpiryTime = FDateTime::FromUnixTimestamp(Payload.EXP);
							
							FDateTime IssueTime = FDateTime::FromUnixTimestamp(Payload.IAT);
							if ((ExpiryTime - IssueTime) <= FTimespan(ETimespan::TicksPerHour) && ExpiryTime > FDateTime::UtcNow())
							{
								bSuccess = true;
#if 0
								TArray<uint8> Signature;
								if (FBase64::Decode(Tokens[2], Signature))
								{
									bSuccess = true;
								}
#endif
							}
							else
							{
								UE_LOG(LogCommonSRO, Warning, TEXT("SRO auth: Expiry Time inconsistency"));
								UE_LOG(LogCommonSRO, Warning, TEXT("	Expiry: %s"), *ExpiryTime.ToString());
								UE_LOG(LogCommonSRO, Warning, TEXT("	Issue: %s"), *IssueTime.ToString());
							}
						}
						else
						{
							UE_LOG(LogCommonSRO, Warning, TEXT("SRO auth: Payload data inconsistency"));
						}
					}
					else
					{
						UE_LOG(LogCommonSRO, Warning, TEXT("SRO auth: Payload format inconsistency"));
					}
				}
				else
				{
					UE_LOG(LogCommonSRO, Warning, TEXT("SRO auth: Header data inconsistency"));
				}
			}
			else
			{
				UE_LOG(LogCommonSRO, Warning, TEXT("SRO auth: Header format inconsistency"));
			}
		}
		else
		{
			UE_LOG(LogCommonSRO, Warning, TEXT("SRO auth: JWT format inconsistency"));
		}
	}

	return bSuccess;
}

__forceinline bool FJsonWebTokenSRO::Validate(const FString& Issuer, const TArray<FString>& Aud) const
{
	if (Payload.ISS != Issuer)
	{
		UE_LOG(LogCommonSRO, Warning, TEXT("SRO auth: Issuer inconsistency"));
		UE_LOG(LogCommonSRO, Warning, TEXT("	Required: %s"), *Payload.ISS);
		UE_LOG(LogCommonSRO, Warning, TEXT("	Given: %s"), *Issuer);
		return false;
	}

	for (const FString& Audience : Aud)
	{
		if (Payload.Aud == Audience)
		{
			return true;
		}
	}
	
	UE_LOG(LogCommonSRO, Warning, TEXT("SRO auth: Audience inconsistency"));
	UE_LOG(LogCommonSRO, Warning, TEXT("	Required: %s"), *Payload.Aud);
	for (const FString& Audience : Aud)
	{
		UE_LOG(LogCommonSRO, Warning, TEXT("	Given: %s"), *Audience);
	}
	return false;
}


__forceinline void FAuthTokenSRO::AddAuthAttributes(const TSharedPtr<FJsonObject>& JsonUser)
{
	for (auto It = JsonUser->Values.CreateConstIterator(); It; ++It)
	{
		if (It.Value().IsValid())
		{
			if (It.Value()->Type == EJson::String)
			{
				AuthData.Add(It.Key(), It.Value()->AsString());
			}
			else if (It.Value()->Type == EJson::Boolean)
			{
				AuthData.Add(It.Key(), It.Value()->AsBool() ? TEXT("true") : TEXT("false"));
			}
			else if (It.Value()->Type == EJson::Number)
			{
				AuthData.Add(It.Key(), FString::Printf(TEXT("%f"), (double)It.Value()->AsNumber()));
			}
		}
	}
}

__forceinline bool FAuthTokenSRO::Parse(const FString& InJsonStr, const FAuthTokenSRO& InOldAuthToken)
{
	if (Parse(InJsonStr))
	{
		RefreshToken = InOldAuthToken.RefreshToken;
		AuthData.Add(TEXT("refresh_token"), InOldAuthToken.RefreshToken);
		return true;
	}

	return false;
}

__forceinline bool FAuthTokenSRO::Parse(const FString& InJsonStr)
{
	bool bSuccess = false;

	if (!InJsonStr.IsEmpty())
	{
		TSharedPtr<FJsonObject> JsonAuth;
		TSharedRef< TJsonReader<> > JsonReader = TJsonReaderFactory<>::Create(InJsonStr);

		if (FJsonSerializer::Deserialize(JsonReader, JsonAuth) &&
			JsonAuth.IsValid())
		{
			bSuccess = Parse(JsonAuth);
		}
	}
	else
	{
		UE_LOG(LogCommonSRO, Warning, TEXT("FAuthTokenSRO: Empty Json string"));
	}

	return bSuccess;
}

__forceinline bool FAuthTokenSRO::Parse(TSharedPtr<FJsonObject> InJsonObject)
{
	if (!InJsonObject.IsValid())
	{
		UE_LOG(LogCommonSRO, Warning, TEXT("FAuthTokenSRO: Invalid Json pointer"));
		return false;
	}
	
	if (!FromJson(InJsonObject))
	{
		UE_LOG(LogCommonSRO, Warning, TEXT("FAuthTokenSRO: Json does not match"));
		return false;
	}
	
	AddAuthAttributes(InJsonObject);
	AuthType = ESROAuthTokenType::AccessToken;
	ExpiresInUTC = FDateTime::UtcNow() + FTimespan(ExpiresIn * ETimespan::TicksPerSecond);
	
	if (!IdToken.IsEmpty())
	{
		return IdTokenJWT.Parse(IdToken);
	}
	
	return true;
}
