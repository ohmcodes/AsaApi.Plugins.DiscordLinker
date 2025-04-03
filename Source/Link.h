

void LinkDiscordCallback(AShooterPlayerController* pc, FString* param, int send_mode, int sender_platform)
{
	Log::GetLog()->warn("Function: {}", __FUNCTION__);

	if (AsaApi::GetApiUtils().IsPlayerDead(pc)) return;

	TArray<FString> player_perm = GetPlayerPermissions(pc->GetEOSId());

	FString included = FString(DiscordLinker::config.value("IncludedPermission", "Default"));
	if (!player_perm.Contains(included)) return;

	FString excluded = FString(DiscordLinker::config.value("ExcludedPermission", "Registered"));
	if (player_perm.Contains(excluded)) return;

	if (CheckEos(pc->GetEOSId()))
	{
		FString token = GetPlayerToken(pc->GetEOSId());

		pc->CopyStringToClipboard(&token);

		AsaApi::GetApiUtils().SendChatMessage(pc, DiscordLinker::config["DiscordBot"].value("DiscordName", "DiscordLinker").c_str(), DiscordLinker::config["Messages"].value("LinkedMSG", "Please perform a (/link {0}) on Discord").c_str(), token.ToString());
	}
	else
	{
		// add player
		AddPlayer(pc, sender_platform);
	}
}

void GetPlayerIDCallback(AShooterPlayerController* pc, FString* param, int, int)
{

}

void GetEosIDCallback(AShooterPlayerController* pc, FString* param, int, int)
{

}

void GetDiscordIDCallback(AShooterPlayerController* pc, FString* param, int, int)
{

}
// for player
// tribeid



// Get Someone's
// Player ID by player name
// Eos ID by player name
// Tribe ID by player name
// Discord ID by player name
// get ip
// get platform