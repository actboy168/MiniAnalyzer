#include "RepAnalyzer.hpp"
#include <cstdarg>
#include <vector>
#include <iostream>

namespace WRep
{
	/// | 6.66b
	/// | * Added replay data for creep kills/denies (at time intervals,
	/// | format: "CSK"+I2S(PlayerID),value and "CSD"+I2S(PlayerID),value)
	/// | 
	/// | 6.65
	/// | * Replay Data: host playerid is 0 if the game was created using HCL automode
	/// | 
	/// | * New replay data available
	/// | Roshan Death: "Roshan",0/1
	/// | Aegis Received: "AegisOn", PlayerID
	/// | Aegis Dropped: "AegisOff", PlayerID
	/// | Game Mode: "Mode"+String,HostPlayerID
	/// | Hero Levelup: "Level"+Level,PlayerID
	/// | Rune Store: "RuneStore"+RuneID, PlayerID. RuneID=1/2/3/4/5
	/// | Rune Use: "RuneUse"+RuneID, PlayerID. RuneID=1/2/3/4/5 (Haste/Regen/DD/Illusion/Invis)
	/// | 
	/// | 6.59
	/// | 
	/// | * When players swap heroes, the new player/heroid data for replay parsers are updated
	/// | * When a player leaves, the state of his inventory is now recorded in the replay for easier parsing (Same 8_# actions)
	/// | * When players swap heroes, the new player/heroid data for replay parsers are updated
	/// | 
	/// | 6.58
	/// | 
	/// | * Added lots of new replay data features for game parsers (see details below)
	/// | * Endgame replay data actions now automatically done a few seconds after 3 players leave
	/// | * Fixed some bugs with replay data when host leaves the game
	/// | 
	/// | Replay Data Specs:
	/// | ==================
	/// | - "Ban",playerid,heroid: is sent for each time a hero is banned in cm/xl
	/// | - "id" packets are sent after the mode phase (supports -sp)
	/// | - "GameStart",1: synced to time 0 ingame (creep spawn time)
	/// | - "9" packets are now sent each time a hero is picked
	/// | 
	/// | 6.55
	/// | 
	/// | * Added post creepspawn gametime to replay data for parsers
	/// | * Added Hero ID data to the replay
	/// | 
	/// | 6.54
	/// | 
	/// | * Added extra dynamic replay data to help replay parsers present in-game data (see below)
	/// | * Added extra data to end of replay (Player Gold, Assists, Neutral CS, Items)
	/// | 
	/// | ++ New real-time replay data:
	/// | - When a hero dies: Hero<d>,<k> where d=dead player id and k=kill player id
	/// | - When a chicken dies: Courier<d>,<k> where d=dead player id and k=kill player id
	/// | - When tower dies: Tower<a><l><s>,<k> where a=alliance (0,1 0=sent, 1=scourge) l=level (1,2,3,4) s=side (0=top,1=mid,2=bot) and k=kill player id. for level 4 towers, s=1
	/// | - When rax dies: rax<a><s><t>,<k> where a=alliance (0,1 0=sent, 1=scourge) s=side (0=top,1=mid,2=bot) t=rax type (0,1 0=melee, 1=range) k=kill player id
	/// | - When Throne gets hurt: Frozen Throne,<p> where p=current percentage hp (75/50/25/10)
	/// | - When Tree gets hurt: World Tree,<p> where p=current percentage hp (75/50/25/10)
	/// | - When a player gets disconnected: CK<k>D<d>N<n>,<p> where k=creep kills, d=creep denies, n=neutral kills, p=player id of leaver
	/// | 
	/// | 6.49
	/// | 
	/// | * Added player index flags to replays

	namespace detail
	{
		std::string Format(std::string format, ...)
		{
			if (format.empty())
			{
				return std::move(std::string(""));
			}

			std::string formattedStr("");

			va_list argsList = NULL; 

			va_start(argsList, format); 

			size_t formattedStrLen = _vscprintf(format.c_str(), argsList) + 1;

			std::vector<char> buffer(formattedStrLen, '\0');

			int nWritten = _vsnprintf_s(&buffer[0], buffer.size(), formattedStrLen, format.c_str(), argsList);    

			if (nWritten > 0)
			{
				formattedStr = &buffer[0];
			}

			va_end(argsList); 

			return std::move(formattedStr); 
		}

		std::string Print(uint32_t time, std::string const& szTable, std::string const& szKey, uint32_t nValue)
		{
			if (nValue > 'A000')
			{
				return std::move(Format("%02d:%02d:%02d.%03d| Table = %s, Key = %s, Value = '%c%c%c%c'", 
					time/1000/60/60,
					time/1000/60%60,
					time/1000%60,
					time%1000,
					szTable.c_str(), 
					szKey.c_str(), 
					((char*)&nValue)[3], 
					((char*)&nValue)[2], 
					((char*)&nValue)[1], 
					((char*)&nValue)[0]));
			}
			else
			{
				return std::move(Format("%02d:%02d:%02d.%03d| Table = %s, Key = %s, Value = %d", 
					time/1000/60/60,
					time/1000/60%60,
					time/1000%60,
					time%1000,
					szTable.c_str(), 
					szKey.c_str(), 
					nValue));
			}
		}
	}

	void RepAnalyzer::AnalyzerAction6B(uint8_t /*playerid*/, uint32_t time, std::string const& szTable, std::string const& szKey, uint32_t nValue)
	{
		std::cout << detail::Print(time, szTable, szKey, nValue) <<std::endl;
	}

}
