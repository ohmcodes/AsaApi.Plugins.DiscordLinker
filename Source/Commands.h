

void AddOrRemoveCommands(bool addCmd = true)
{
	const FString LinkDiscord = DiscordLinker::config["Commands"]["LinkPlayerCMD"].get<std::string>().c_str();
	if (!LinkDiscord.IsEmpty())
	{
		if (addCmd)
		{
			AsaApi::GetCommands().AddChatCommand(LinkDiscord, &LinkDiscordCallback);
		}
		else
		{
			AsaApi::GetCommands().RemoveChatCommand(LinkDiscord);
		}
	}
}