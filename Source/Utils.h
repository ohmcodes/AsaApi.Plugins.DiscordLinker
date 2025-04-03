
#include <fstream>
#include <random>

bool CheckToken(FString token)
{
	std::string escaped_token = DiscordLinker::pluginTemplateDB->escapeString(token.ToString());

	std::string query = fmt::format("SELECT * FROM {} WHERE Token='{}'", DiscordLinker::config["PluginDBSettings"]["TableName"].get<std::string>(), escaped_token);

	std::vector<std::map<std::string, std::string>> results;
	DiscordLinker::pluginTemplateDB->read(query, results);

	if (results.size() <= 0)
	{
		return true;
	}
	else
	{
		return results[0]["Token"] == token.ToString() ? false : true;
	}

	
}

FString GenerateToken(int digit)
{
	std::random_device rd;
	std::mt19937 gen(rd());

	const std::string charset = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	std::uniform_int_distribution<int> distribution(0, charset.size() - 1);
	FString strToken;
	int j = 0;

	do
	{
		for (int i = 0; i < 6; ++i)
		{
			int randomIndex = distribution(gen);
			char randomChar = charset[randomIndex];
			strToken += randomChar;

		}
		// TODO: temp
		Log::GetLog()->warn("IterCount {}", j++);

	} while (!CheckToken(strToken));

	return strToken;
}



bool CheckEos(FString eos_id)
{
	std::string escaped_eosid = DiscordLinker::pluginTemplateDB->escapeString(eos_id.ToString());

	std::string query = fmt::format("SELECT * FROM {} WHERE EosId='{}'", DiscordLinker::config["PluginDBSettings"]["TableName"].get<std::string>(), escaped_eosid);

	std::vector<std::map<std::string, std::string>> results;
	DiscordLinker::pluginTemplateDB->read(query, results);

	Log::GetLog()->warn("RESULTS COUNT {}",results.size());

	return results.size() <= 0 ? false : true;
}

FString GetPlatform(int platform)
{
	FString pf;
	switch (platform)
	{
	case 0:
		pf = "Epic";
		break;
	case 1:
		pf = "Steam";
	case 2:
		pf = "XBOX";
	case 3:
		pf = "Playstation";
	default:
		break;
	}
	return pf;
}



// Give points

// Set Permission

#if 0
static bool startsWith(const std::string& str, const std::string& prefix)
{
	return str.size() >= prefix.size() && str.compare(0, prefix.size(), prefix) == 0;
}

void FetchMessageFromDiscordCallback(bool success, std::string results)
{
	//Log::GetLog()->warn("Function: {}", __FUNCTION__);

	if (success)
	{
		if(results.empty()) return;

		try
		{
			nlohmann::json resObj = nlohmann::json::parse(results)[0];

			if (resObj.is_null())
			{
				Log::GetLog()->warn("resObj is null");
				return;
			}

			auto globalName = resObj["author"]["global_name"];

			// if not sent by bot
			if (resObj.contains("bot") && globalName.is_null())
			{
				Log::GetLog()->warn("the sender is bot");
				return;
			}

			std::string msg = resObj["content"].get<std::string>();
			
			if (!startsWith(msg, "!"))
			{
				Log::GetLog()->warn("message not startswith !");
				return;
			}

			if (DiscordLinker::lastMessageID == resObj["id"].get<std::string>()) return;
			
			std::string sender = fmt::format("Discord: {}", globalName.get<std::string>());

			AsaApi::GetApiUtils().SendChatMessageToAll(FString(sender), msg.c_str());

			DiscordLinker::lastMessageID = resObj["id"].get<std::string>();
		}
		catch (std::exception& error)
		{
			Log::GetLog()->error("Error parsing JSON results. Error: {}",error.what());
		}
	}
	else
	{
		Log::GetLog()->warn("Failed to fetch messages. success: {}", success);
	}
}

void FetchMessageFromDiscord()
{
	//Log::GetLog()->warn("Function: {}", __FUNCTION__);

	std::string botToken = DiscordLinker::config["DiscordBot"].value("BotToken","");

	std::string channelID = DiscordLinker::config["DiscordBot"].value("ChannelID", "");

	std::string apiURL = FString::Format("https://discord.com/api/v10/channels/{}/messages?limit=1", channelID).ToString();

	std::vector<std::string> headers = {
		"Content-Type: application/json",
		"User-Agent: DiscordLinker/1.0",
		"Connection: keep-alive",
		"Accept: */*",
		"Content-Length: 0",
		"Authorization: Bot " + botToken
	};

	try
	{
		bool req = DiscordLinker::req.CreateGetRequest(apiURL, FetchMessageFromDiscordCallback, headers);

		if (!req)
			Log::GetLog()->error("Failed to perform Get request. req = {}", req);
	}
	catch (const std::exception& error)
	{
		Log::GetLog()->error("Failed to perform Get request. Error: {}", error.what());
	}
}

void SendMessageToDiscordCallback(bool success, std::string results, std::unordered_map<std::string, std::string> responseHeaders)
{
	if (!success)
	{
		Log::GetLog()->error("Failed to send Post request. {} {} {}", __FUNCTION__, success, results);
	}
	else
	{
		Log::GetLog()->info("Success. {} {} {}", __FUNCTION__, success, results);
	}
}

void SendMessageToDiscord(std::string msg)
{

	Log::GetLog()->warn("Function: {}", __FUNCTION__);

	
	std::string webhook = DiscordLinker::config["DiscordBot"].value("Webhook", "");
	std::string botImgUrl = DiscordLinker::config["DiscordBot"].value("BotImageURL", "");

	if (webhook == "" || webhook.empty()) return;

	FString msgFormat = L"{{\"content\":\"{}\",\"username\":\"{}\",\"avatar_url\":\"{}\"}}";

	FString msgOutput = FString::Format(*msgFormat, msg, "ArkBot", botImgUrl);

	std::vector<std::string> headers = {
		"Content-Type: application/json",
		"User-Agent: DiscordLinker/1.0",
		"Connection: keep-alive",
		"Accept: */*"
	};

	try
	{
		bool req = DiscordLinker::req.CreatePostRequest(webhook, SendMessageToDiscordCallback, msgOutput.ToStringUTF8(), "application/json", headers);

		if(!req)
			Log::GetLog()->error("Failed to send Post request. req = {}", req);
	}
	catch (const std::exception& error)
	{
		Log::GetLog()->error("Failed to send Post request. Error: {}", error.what());
	}
}

bool Points(FString eos_id, int cost, bool check_points = false)
{
	if (cost == -1)
	{
		if (DiscordLinker::config["Debug"].value("Points", false) == true)
		{
			Log::GetLog()->warn("Cost is -1");
		}
		return false;
	}

	if (cost == 0)
	{
		if (DiscordLinker::config["Debug"].value("Points", false) == true)
		{
			Log::GetLog()->warn("Cost is 0");
		}

		return true;
	}

	nlohmann::json config = DiscordLinker::config["PointsDBSettings"];

	if (config.value("Enabled", false) == false)
	{
		if (DiscordLinker::config["Debug"].value("Points", false) == true)
		{
			Log::GetLog()->warn("Points system is disabled");
		}

		return true;
	}

	std::string tablename = config.value("TableName", "ArkShopPlayers");
	std::string unique_id = config.value("UniqueIDField", "EosId");
	std::string points_field = config.value("PointsField", "Points");
	std::string totalspent_field = config.value("TotalSpentField", "TotalSpent");

	if (tablename.empty() || unique_id.empty() || points_field.empty())
	{
		if (DiscordLinker::config["Debug"].value("Points", false) == true)
		{
			Log::GetLog()->warn("DB Fields are empty");
		}
		return false;
	}

	std::string escaped_eos_id = DiscordLinker::pointsDB->escapeString(eos_id.ToString());

	std::string query = fmt::format("SELECT * FROM {} WHERE {}='{}'", tablename, unique_id, escaped_eos_id);

	std::vector<std::map<std::string, std::string>> results;

	if (!DiscordLinker::pointsDB->read(query, results))
	{
		if (DiscordLinker::config["Debug"].value("Points", false) == true)
		{
			Log::GetLog()->warn("Error reading points db");
		}

		return false;
	}

	if (results.size() <= 0)
	{
		if (DiscordLinker::config["Debug"].value("Points", false) == true)
		{
			Log::GetLog()->warn("No record found");
		}
		return false;
	}

	int points = std::atoi(results[0].at(points_field).c_str());

	if (check_points)
	{
		if (DiscordLinker::config["Debug"].value("Points", false) == true)
		{
			Log::GetLog()->warn("Player got {} points", points);
		}

		if (points >= cost) return true;
	}
	else
	{
		int amount = points - cost;

		std::vector<std::pair<std::string, std::string>> data;

		data.push_back({ points_field, std::to_string(amount) });

		if (totalspent_field != "")
		{
			int total_spent = std::atoi(results[0].at(totalspent_field).c_str());
			std::string total_ts = std::to_string(total_spent + cost);

			data.push_back({totalspent_field, total_ts});
		}

		std::string condition = fmt::format("{}='{}'", unique_id, escaped_eos_id);

		if (DiscordLinker::pointsDB->update(tablename, data, condition))
		{
			if (DiscordLinker::config["Debug"].value("Points", false) == true)
			{
				Log::GetLog()->info("{} Points DB updated", amount);
			}

			return true;
		}
	}

	return false;
}

nlohmann::json GetCommandString(const std::string permission, const std::string command)
{
	if (permission.empty()) return {};
	if (command.empty()) return {};

	nlohmann::json config_obj = DiscordLinker::config["PermissionGroups"];
	nlohmann::json perm_obj = config_obj[permission];
	nlohmann::json command_obj = perm_obj["Commands"];
	nlohmann::json setting_obj = command_obj[command];

	return setting_obj;
}

#endif

TArray<FString> GetPlayerPermissions(FString eos_id)
{
	TArray<FString> PlayerPerms = { "Default" };

	std::string escaped_eos_id = DiscordLinker::permissionsDB->escapeString(eos_id.ToString());

	std::string tablename = DiscordLinker::config["PermissionsDBSettings"].value("TableName", "Players");

	std::string condition = DiscordLinker::config["PermissionsDBSettings"].value("UniqueIDField", "EOS_Id");

	std::string query = fmt::format("SELECT * FROM {} WHERE {}='{}';", tablename, condition, escaped_eos_id);

	std::vector<std::map<std::string, std::string>> results;
	if (!DiscordLinker::permissionsDB->read(query, results))
	{
		if (DiscordLinker::config["Debug"].value("Permissions", false) == true)
		{
			Log::GetLog()->warn("Error reading permissions DB");
		}

		return PlayerPerms;
	}

	if (results.size() <= 0) return PlayerPerms;

	std::string permsfield = DiscordLinker::config["PermissionsDBSettings"].value("PermissionGroupField","PermissionGroups");

	FString playerperms = FString(results[0].at(permsfield));

	if (DiscordLinker::config["Debug"].value("Permissions", false) == true)
	{
		Log::GetLog()->info("current player perms {}", playerperms.ToString());
	}

	playerperms.ParseIntoArray(PlayerPerms, L",", true);

	return PlayerPerms;
}

#if 0
FString GetPriorPermByEOSID(FString eos_id)
{
	TArray<FString> player_groups = GetPlayerPermissions(eos_id);

	const nlohmann::json permGroups = DiscordLinker::config["PermissionGroups"];

	std::string defaultGroup = "Default";
	int minPriority = INT_MAX;
	nlohmann::json result;
	FString selectedPerm = "Default";

	for (const FString& param : player_groups)
	{
		if (permGroups.contains(param.ToString()))
		{
			int priority = static_cast<int>(permGroups[param.ToString()]["Priority"]);
			if (priority < minPriority)
			{
				minPriority = priority;
				result = permGroups[param.ToString()];
				selectedPerm = param;
			}
		}
	}

	if (result.is_null() && permGroups.contains(defaultGroup))
	{
		if(!permGroups[defaultGroup].is_null())
			result = permGroups[defaultGroup];

		result = {};
	}

	if (DiscordLinker::config["Debug"].value("Permissions", false) == true)
	{
		Log::GetLog()->info("Selected Permission {}", selectedPerm.ToString());
	}

	return selectedPerm;
}
#endif

// using
bool AddPlayer(AShooterPlayerController* pc, int sender_platform)
{
	const FString token = GenerateToken(DiscordLinker::config["General"].value("TokenDigits",6));

	FString steam_id;
	AsaApi::GetApiUtils().GetShooterGameMode()->GetSteamIDStringForPlayerID(&steam_id, pc->GetLinkedPlayerID());

	FString ip_addr;
	pc->GetPlayerNetworkAddress(&ip_addr);

	std::vector<std::pair<std::string, std::string>> data = {
		{"PlayerName", pc->GetCharacterName().ToString()},
		{"SteamName", pc->PlayerStateField()->PlayerNamePrivateField().ToString()},
		{"EosId", pc->GetEOSId().ToString()},
		{"SteamId", steam_id.ToString()},
		{"PlayerId", std::to_string(pc->GetLinkedPlayerID())},
		{"TribeId", std::to_string(pc->TargetingTeamField())},
		{"Platform", GetPlatform(sender_platform).ToString()},
		{"Token", token.ToString()},
		{"IpAddress", ip_addr.ToString()}
	};

	return DiscordLinker::pluginTemplateDB->create(DiscordLinker::config["PluginDBSettings"]["TableName"].get<std::string>(), data);
}

FString GetPlayerToken(FString eos_id)
{
	std::string escaped_id = DiscordLinker::pluginTemplateDB->escapeString(eos_id.ToString());

	std::string query = fmt::format("SELECT * FROM {} WHERE EosId='{}'", DiscordLinker::config["PluginDBSettings"]["TableName"].get<std::string>(), escaped_id);

	std::vector<std::map<std::string, std::string>> results;
	DiscordLinker::pluginTemplateDB->read(query, results);

	if (results.size() > 0)
	{
		FString token = FString(results[0]["Token"]);
		return token;
	}
	else
	{
		Log::GetLog()->error("Something went wrong.");
		return "";
	}
}

#if 0
bool ReadPlayer(FString eosID)
{
	std::string escaped_id = DiscordLinker::pluginTemplateDB->escapeString(eosID.ToString());

	std::string query = fmt::format("SELECT * FROM {} WHERE EosId='{}'", DiscordLinker::config["PluginDBSettings"]["TableName"].get<std::string>(), escaped_id);

	std::vector<std::map<std::string, std::string>> results;
	DiscordLinker::pluginTemplateDB->read(query, results);

	return results.size() <= 0 ? false : true;
}

bool UpdatePlayer(FString eosID, FString playerName)
{
	std::string unique_id = "EosId";

	std::string escaped_id = DiscordLinker::pluginTemplateDB->escapeString(eosID.ToString());

	std::vector<std::pair<std::string, std::string>> data = {
		{"PlayerName", playerName.ToString() + "123"}
	};

	std::string condition = fmt::format("{}='{}'", unique_id, escaped_id);

	return DiscordLinker::pluginTemplateDB->update(DiscordLinker::config["PluginDBSettings"]["TableName"].get<std::string>(), data, condition);
}

bool DeletePlayer(FString eosID)
{
	std::string escaped_id = DiscordLinker::pluginTemplateDB->escapeString(eosID.ToString());

	std::string condition = fmt::format("EosId='{}'", escaped_id);

	return DiscordLinker::pluginTemplateDB->deleteRow(DiscordLinker::config["PluginDBSettings"]["TableName"].get<std::string>(), condition);
}

#endif

void ReadConfig()
{
	try
	{
		const std::string config_path = AsaApi::Tools::GetCurrentDir() + "/ArkApi/Plugins/" + PROJECT_NAME + "/config.json";
		std::ifstream file{config_path};
		if (!file.is_open())
		{
			throw std::runtime_error("Can't open config file.");
		}
		file >> DiscordLinker::config;

		Log::GetLog()->info("{} config file loaded.", PROJECT_NAME);

		DiscordLinker::isDebug = DiscordLinker::config["Debug"]["DiscordLinker"].get<bool>();

		Log::GetLog()->warn("Debug {}", DiscordLinker::isDebug);

	}
	catch(const std::exception& error)
	{
		Log::GetLog()->error("Config load failed. ERROR: {}", error.what());
		throw;
	}
}

void LoadDatabase()
{
	Log::GetLog()->warn("LoadDatabase");
	DiscordLinker::pluginTemplateDB = DatabaseFactory::createConnector(DiscordLinker::config["PluginDBSettings"]);

	nlohmann::ordered_json tableDefinition = {};
	if (DiscordLinker::config["PluginDBSettings"].value("UseMySQL", true) == true)
	{
		tableDefinition = {
			{"Id", "INT NOT NULL AUTO_INCREMENT"},
			{"PlayerName", "VARCHAR(50) NOT NULL"},
			{"SteamName", "VARCHAR(50)"},
			{"DiscordId", "VARCHAR(50)"},
			{"EosId", "VARCHAR(50) NOT NULL"},
			{"SteamId", "VARCHAR(50)"},
			{"PlayerId", "VARCHAR(50) NOT NULL"},
			{"TribeId", "VARCHAR(50)"},
			{"Platform", "VARCHAR(50)"},
			{"Token", "VARCHAR(255)"},
			{"IpAddress", "VARCHAR(50)"},
			{"CreateAt", "DATETIME DEFAULT CURRENT_TIMESTAMP"},
			{"PRIMARY", "KEY(Id)"},
			{"UNIQUE", "INDEX EosId_UNIQUE (EosId ASC)"}
		};
	}
	else
	{
		tableDefinition = {
			{"Id","INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT"},
			{"PlayerName","TEXT"},
			{"SteamName","TEXT"},
			{"DiscordId","TEXT"},
			{"EosId","TEXT NOT NULL UNIQUE"},
			{"SteamdId","TEXT"},
			{"PlayerId","TEXT"},
			{"TribeId","TEXT"},
			{"Platform","TEXT"},
			{"Token","TEXT"},
			{"IpAddress","TEXT"},
			{"CreateAt","TIMESTAMP DEFAULT CURRENT_TIMESTAMP"}
		};
	}

	DiscordLinker::pluginTemplateDB->createTableIfNotExist(DiscordLinker::config["PluginDBSettings"].value("TableName", ""), tableDefinition);


	// PermissionsDB
	if (DiscordLinker::config["PermissionsDBSettings"].value("Enabled", true) == true)
	{
		DiscordLinker::permissionsDB = DatabaseFactory::createConnector(DiscordLinker::config["PermissionsDBSettings"]);
	}

	// PointsDB (ArkShop)
	if (DiscordLinker::config["PointsDBSettings"].value("Enabled", true) == true)
	{
		DiscordLinker::pointsDB = DatabaseFactory::createConnector(DiscordLinker::config["PointsDBSettings"]);
	}
	
}