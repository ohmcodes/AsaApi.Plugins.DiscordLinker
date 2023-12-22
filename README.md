# AsaApi.Plugins.DiscordLinker
ARK:SA ServerAPI Plugin that support internal Discord bot and Link In-game with Database

# Requirements
- ServerAPI [Download/Install](https://gameservershub.com/forums/resources/ark-survival-ascended-serverapi-crossplay-supported.683/)
- Ofcourse the ARK:SA Server
- Bot Creation
- Discord Channel Integration WebHook

# Features
- User-defined Slash command In-game
- User-defined Slash command Discord
- Discord Webhook (For sending message info)
- Discord Bot (For reading slash commands)
- MySQL and SQLite support
- No third party apps needed
- Token Generation
- Can be use to register players

# Database Fields
```
PlayerName
SteamName
DiscordId (When linked)
EosId
SteamId (Same as EosId until WC fix it)
PlayerId
TribeId
Platform (EPIC, Steam, XBOX, PS)
Token (Generated Token for linking)
IPAddress (IP can be change overtime if the player has dynamic IP)
LinkedDate
```

# Future updates
- Rest API
- Optimization of fetching slash commands
- Permission integration

# KNOWN ISSUES
- Console Log (No Message received)  Doesn't affect anything - Still waiting for ServerAPI update 


# Blank Config
```
{
  "Mysql": {
    "MysqlDB": "",
    "MysqlHost": "",
    "MysqlPass": "",
    "MysqlPort": 3306,
    "MysqlUser": "",
    "UseMysql": false,
    "DbPathOverride": ""
  },
  "DiscordBot": {
    "ChannelID": "",
    "BotToken": "",
    "FetchInterval": 5,
    "WebHook": "",
    "ImageUrl": ""
  },
  "Messages": {
    "Sender": "DSLinker",
    "LinkCMD": "/link",
    "LinkMsg": "Please perform a slashcommand on discord {0} {1}",
    "SuccessMsg": "Your Account has been successfully Linked!",
    "FailedMsg":  "Invalid Token",
    "SlashCMDPrefix": "!",
    "SlashCMD": "link",
    "HelpCMD": "/linkhelp",
    "HelpMsg": "Type {} and write down the Generated token and perform {} space token on Discord",
    "LinkError": "Something wrong with registration please contact an Admin.",
    "ExistsMsg": "You are already registered"
  }
}
```

# Config Commented
```
{
  "Mysql": {
    "MysqlDB": "", /* Database name */
    "MysqlHost": "", /* host or ip address */
    "MysqlPass": "", /* password */
    "MysqlPort": 3306, /* port 3306 default mostly */
    "MysqlUser": "", /* db username */
    "UseMysql": false, /* Mysql credentials is required if enabled */
    "DbPathOverride": "" /* sqlite database location */
  },
  "DiscordBot": {
    "ChannelID": "", /* Discord ChannelID to perform the command */
    "BotToken": "", /* Bot token for reading slash command */
    "FetchInterval": 5, /* Interval for fetching slash command */
    "WebHook": "", /* discord Webhook to send messages if linking is success or not */
    "ImageUrl": "" /* Bot Image url */
  },
  "Messages": {
    "Sender": "DSLinker", /* dont name contains Discord it wont work */
    "LinkCMD": "/link", /* in game slash command */
    "LinkMsg": "Please perform a slashcommand on discord {0} {1}", /* link message when success */
    "SuccessMsg": "Your Account has been successfully Linked!", /* success message when linked */
    "FailedMsg": "Invalid Token", /* if wrong or invalid token */
    "SlashCMDPrefix": "!", /* dont put slash as it is using by many bots */
    "SlashCMD": "link", /* discord command */
    "HelpCMD": "/linkhelp", /* in game command help */
    "HelpMsg": "Type {} and write down the Generated token and perform {} space token on Discord", /* help message */
    "LinkError": "Something wrong with registration please contact an Admin.", /* link error message */
    "ExistsMsg": "You are already registered" /* if registered */
  }
}
```

# Give Love!
[Buy me a coffee](https://www.buymeacoffee.com/ohmcodes)

[Paypal](https://www.paypal.com/donate/?business=8389QZ23QRDPE&no_recurring=0&item_name=Game+Server%2FTools+Community+Donations&currency_code=CAD)

# Source code
As soon as I am comfortable with my coding I will release it to the public :)
