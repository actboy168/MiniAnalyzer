#include "RepAnalyzer.hpp"
#include "RepAction.hpp"

namespace WRep
{
	void RepAnalyzer::AnalyzerAction(uint8_t playerid, uint32_t time, RepBuffer& buf)
	{
		switch (buf.Peek())
		{
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			/// 0x01 - Pause game                                             [ 1 byte ] [APM-]
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
		case 0x01:
			buf.Read<RepAction01>();
			break;
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			/// 0x02 - Resume game                                            [ 1 byte ] [APM-]
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
		case 0x02:
			buf.Read<RepAction02>();
			break;
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			/// 0x03 - Set game speed in single player game (options menu)    [ 2 byte ] [APM-]
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			///  1 byte  - game speed:
			///             0x00 - slow
			///             0x01 - normal
			///             0x02 - fast
		case 0x03:
			buf.Read<RepAction03>();
			break;
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			/// 0x04 - Increase game speed in single player game (Num+)       [ 1 byte ] [APM-]
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
		case 0x04:
			buf.Read<RepAction04>();
			break;  
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			/// 0x05 - Decrease game speed in single player game (Num-)       [ 1 byte ] [APM-]
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
		case 0x05:
			buf.Read<RepAction05>();
			break;
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			/// 0x06 - Save game                                             [ n bytes ] [APM-]
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			///  n bytes - savegame name (null terminated string)
		case 0x06:
			buf.Read<RepAction06>();
			buf.ReadString();
			break;
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			/// 0x07 - Save game finished                                    [ 5 bytes ] [APM-]
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			///  1 dword - unknown (always 0x00000001 so far)
		case 0x07:
			buf.Read<RepAction07>();
			break;
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			/// 0x10 - Unit/building ability (no additional parameters)     [ 15 bytes ] [APM+]
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			///  1 word  - AbilityFlags (see section 3) ('byte' for patch version < 1.13)
			///  1 dword - ItemID (see section 4)
			///  1 dword - unknownA (0xFFFFFFFF) (only present for patch version >= 1.07)
			///  1 dword - unknownB (0xFFFFFFFF) (only present for patch version >= 1.07)
		case 0x10:
			buf.Read<RepAction10>();
			break;
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			/// 0x11 - Unit/building ability                                [ 22 bytes ] [APM+]
			///        (with target position)
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			///  1 word  - AbilityFlags (see section 3) ('byte' for patch version < 1.13)
			///  1 dword - ItemID (see section 4)
			///  1 dword - unknownA (0xFFFFFFFF) (only present for patch version >= 1.07)
			///  1 dword - unknownB (0xFFFFFFFF) (only present for patch version >= 1.07)
			///  1 dword - target location X
			///  1 dword - target location Y
		case 0x11:
			buf.Read<RepAction11>();
			break;
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			/// 0x12 - Unit/building ability                                [ 31 bytes ] [APM+]
			///        (with target position and target object ID)
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			///  1 word  - AbilityFlags (see section 3) ('byte' for patch version < 1.13)
			///  1 dword - ItemID (see section 4)
			///  1 dword - unknownA (0xFFFFFFFF) (only present for patch version >= 1.07)
			///  1 dword - unknownB (0xFFFFFFFF) (only present for patch version >= 1.07)
			///  1 dword - target position X coordinate
			///  1 dword - target position Y coordinate
			///  1 dword - objectID1
			///  1 dword - objectID2
		case 0x12:
			buf.Read<RepAction12>();
			break;
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			/// 0x13 - Give item to Unit / Drop item on ground              [ 39 bytes ] [APM+]
			///        (with target position, object ID A and B)
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			///  1 word  - AbilityFlags (see section 3) ('byte' for patch version < 1.13)
			///  1 dword - ItemID (see section 4)
			///  1 dword - unknownA (0xFFFFFFFF) (only present for patch version >= 1.07)
			///  1 dword - unknownB (0xFFFFFFFF) (only present for patch version >= 1.07)
			///  1 dword - target location X
			///  1 dword - target location Y
			///  1 dword - Target_objectID_1
			///  1 dword - Target_objectID_2
			///  1 dword - Item_objectID_1
			///  1 dword - Item_objectID_2
		case 0x13:
			buf.Read<RepAction13>();
			break;
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			/// 0x14 - Unit/building ability                                [ 44 bytes ] [APM+]
			///        (with two target positions and two item ID's)
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			///  1 word  - AbilityFlags (see section 3) ('byte' for patch version < 1.13)
			///  1 dword - ItemID_A (see section 4)
			///  1 dword - unknownA (0xFFFFFFFF) (only present for patch version >= 1.07)
			///  1 dword - unknownB (0xFFFFFFFF) (only present for patch version >= 1.07)
			///  1 dword - target location A_X
			///  1 dword - target location A_Y
			///  1 dword - ItemID_B (see section 4)
			///  9 byte  - unknown
			///  1 dword - target location B_X
			///  1 dword - target location B_Y
		case 0x14:
			buf.Read<RepAction14>();
			break;
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			/// 0x16 - Change Selection (Unit, Building, Area)           [ 4+n*8 bytes ] [APM?]
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			///  1 byte  - select mode:
			///             0x01 - add to selection      (select)
			///             0x02 - remove from selection (deselect)
			///  1 word  - number (n) of units/buildings
			/// 
			///  block repeated n times:
			///    1 dword - ObjectID1
			///    1 dword - ObjectID2
		case 0x16:
			{
				RepAction16* pRepAct = buf.Read<RepAction16>();
				for (uint32_t i = 0; i < pRepAct->nUnitsCount; i++)
				{
					buf.Read<uint32_t>();
					buf.Read<uint32_t>();
				}
				break;
			}
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			/// 0x17 - Assign Group Hotkey                               [ 4+n*8 bytes ] [APM+]
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			///  1 byte  - group number (0-9)
			///              the group number is shifted by one:
			///              key '1' is group0, ... , key '9' is group8 and key '0' is group9
			///  1 word  - number (n) of items in selection
			/// 
			///  block repeated n times:
			///    1 dword - ObjectID1
			///    1 dword - ObjectID2
		case 0x17:
			{
				RepAction17* pRepAct = buf.Read<RepAction17>();
				for (uint32_t i = 0; i < pRepAct->nUnitsCount; i++)
				{
					buf.Read<uint32_t>();
					buf.Read<uint32_t>();
				}
				break;
			}
			/// 
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			/// 0x18 - Select Group Hotkey                                   [ 3 bytes ] [APM+]
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			///  1 byte  - group number (0-9)
			///              the group number is shifted by one:
			///              key '1' is group0, ... , key '9' is group8 and key '0' is group9
			///  1 byte  - unknown  (always 0x03)
		case 0x18:
			buf.Read<RepAction18>();
			break;
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			/// 0x19 - Select Subgroup (patch version >= 1.14b)             [ 13 bytes ] [APM?]
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			///  1 dword - ItemID    (see section 4)
			///  1 dword - ObjectID1 (see section 5)
			///  1 dword - ObjectID2
		case 0x19:
			buf.Read<RepAction19>();
			break;
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			/// 0x1A - Pre Subselection                                       [ 1 byte ] [APM-]
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
		case 0x1A:
			buf.Read<RepAction1A>();
			break;
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			/// 0x1B - Unknown                                              [ 10 bytes ] [APM-]
			/// 0x1A for WarCraft III patch version <= 1.14b
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			///  1 byte  - unknown (always 0x01 so far)
			///  1 dword - unknown (ObjectID1?)
			///  1 dword - unknown (ObjectID2?)
		case 0x1B:
			buf.Read<RepAction1B>();
			break;
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			/// 0x1C - Select Ground Item                                   [ 10 bytes ] [APM+]
			/// 0x1B for WarCraft III patch version <= 1.14b
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			///  1 byte  - unknown (flags? always 0x04 so far)
			///  1 dword - ObjectID1
			///  1 dword - ObjectID2
		case 0x1C:
			buf.Read<RepAction1C>();
			break;
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			/// 0x1D - Cancel hero revival                                   [ 9 bytes ] [APM+]
			/// 0x1C for WarCraft III patch version <= 1.14b
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			///  1 dword - UnitID1 (always a hero)
			///  1 dword - UnitID2 (always a hero)
		case 0x1D:
			buf.Read<RepAction1D>();
			break;
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			/// 0x1E - Remove unit from building queue                       [ 6 bytes ] [APM+]
			/// 0x1D for WarCraft III patch version <= 1.14b
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			///  1 byte  - SlotNr (0 = unit currently build,
			///                    1 = first unit in queue,
			///                    2 = second unit in queue,
			///                    ...
			///                    6 = last unit in queue)
			///  1 dword - ItemID (StringID for the canceled unit)
		case 0x1E:
			buf.Read<RepAction1E>();
			break;
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			/// 0x21 - unknown                                               [ 9 bytes ] [APM-]
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			///  1 dword - unknown [A] (0x000064F0 so far)
			///  1 dword - unknown [B] (0x000064F0 so far)
		case 0x21:
			buf.Read<RepAction21>();
			break;
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			/// 0x20, 0x22-0x32 - Single Player Cheats                       [ * bytes ] [APM-]
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			/// 
			///  Action | Size   | Cheat                    | Description
			///  -------+--------+--------------------------+-------------------------------
			///   0x20  | 1 Byte | TheDudeAbides            | Fast cooldown
			///   0x22  | 1 Byte | SomebodySetUpUsTheBomb   | Instant defeat
			///   0x23  | 1 Byte | WarpTen                  | Speeds construction
			///   0x24  | 1 Byte | IocainePowder            | Fast Death/Decay
			///   0x25  | 1 Byte | PointBreak               | Removes food limit
			///   0x26  | 1 Byte | WhosYourDaddy            | God mode
			///   0x27  | 6 Byte | KeyserSoze [amount]      | Gives you X Gold
			///   0x28  | 6 Byte | LeafitToMe [amount]      | Gives you X Lumber
			///   0x29  | 1 Byte | ThereIsNoSpoon           | Unlimited Mana
			///   0x2A  | 1 Byte | StrengthAndHonor         | No defeat
			///   0x2B  | 1 Byte | itvexesme                | Disable victory conditions
			///   0x2C  | 1 Byte | WhoIsJohnGalt            | Enable research
			///   0x2D  | 6 Byte | GreedIsGood [amount]     | Gives you X Gold and Lumber
			///   0x2E  | 5 Byte | DayLightSavings [time]   | Set a time of day
			///   0x2F  | 1 Byte | ISeeDeadPeople           | Remove fog of war
			///   0x30  | 1 Byte | Synergy                  | Disable tech tree requirements
			///   0x31  | 1 Byte | SharpAndShiny            | Research upgrades
			///   0x32  | 1 Byte | AllYourBaseAreBelongToUs | Instant victory
			///         |        |                          |
			///   ?     |        | Motherland [race][level] | Campain level jump
			/// 
			///  Additional Parameter:
			///   o Action 0x27,0x28,0x2D:  Adding Resources
			///      1 byte  - unknown (always 0xFF)
			///      1 dword - (signed) amount of ressources
			/// 
			///   o Action 0x2E: Set time of day
			///      1 float - time (IEEE single precision float, 4Bytes)
		case 0x20:
			buf.Read<RepAction20>();
			break;
		case 0x22:
			buf.Read<RepAction22>();
			break;
		case 0x23:
			buf.Read<RepAction23>();
			break;
		case 0x24:
			buf.Read<RepAction24>();
			break;
		case 0x25:
			buf.Read<RepAction25>();
			break;
		case 0x26:
			buf.Read<RepAction26>();
			break;
		case 0x27:
			buf.Read<RepAction27>();
			break;
		case 0x28:
			buf.Read<RepAction28>();
			break;
		case 0x29:
			buf.Read<RepAction29>();
			break;
		case 0x2A:
			buf.Read<RepAction2A>();
			break;
		case 0x2B:
			buf.Read<RepAction2B>();
			break;
		case 0x2C:
			buf.Read<RepAction2C>();
			break;
		case 0x2D:
			buf.Read<RepAction2D>();
			break;
		case 0x2E:
			buf.Read<RepAction2E>();
			break;
		case 0x2F:
			buf.Read<RepAction2F>();
			break;
		case 0x30:
			buf.Read<RepAction30>();
			break;
		case 0x31:
			buf.Read<RepAction31>();
			break;
		case 0x32:
			buf.Read<RepAction32>();
			break;
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			/// 0x50 - Change ally options                                   [ 6 bytes ] [APM-]
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			///  1 byte  - player slot number (0-11)(including computer AI players)
			///  1 dword - flags
			///              bit 0-4 - set if allied with player (value 0x1F)
			///              bit 5   - set if vision shared with player (value 0x20)
			///              bit 6   - set if unit control is shared with player (value 0x40)
			///              bit 10  - set if "allied victory" is ticked (value 0x0400)
			///                        (for patch version >= 1.07, see note)
		case 0x50:
			buf.Read<RepAction50>();
			break;
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			/// 0x51 - Transfer resources                                   [ 10 bytes ] [APM-]
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			///  1 byte  - player slot number (0-11)(including computer AI players)
			///  1 dword - Gold to transfer
			///  1 dword - Lumber to transfer
		case 0x51:
			buf.Read<RepAction51>();
			break;
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			/// 0x60 - Map trigger chat command (?)                          [ n bytes ] [APM-]
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			///  1 dword - unknownA
			///  1 dword - unknownB
			///  n bytes - null terminated string (chat command or trigger name)
		case 0x60:
			buf.Read<RepAction60>();
			buf.ReadString();
			break;
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			/// 0x61 - ESC pressed                                            [ 1 byte ] [APM+]
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
		case 0x61:
			buf.Read<RepAction61>();
			break;
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			/// 0x62 - Scenario Trigger                                     [ 13 bytes ] [APM-]
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			///  1 dword - unknown [A]
			///  1 dword - unknown [B]
			///  1 dword - unknown (counter) (only present for patch version >= 1.07)
		case 0x62:
			buf.Read<RepAction62>();
			break;
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			/// 0x63 - Unknown                                              [  9 bytes ] [APM-]
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			///  作用未知，但都是和0x6B、0x62一起出现。 
			///  [6B 63 62]  [62 6B 63]
			///  1 dword - unknown [A]
			///  1 dword - unknown [B]
		case 0x63:
			buf.Read<RepAction63>();
			break;
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			/// 0x66 - Enter choose hero skill submenu                        [ 1 byte ] [APM+]
			/// 0x65 for WarCraft III patch version <= 1.06
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
		case 0x66:
			buf.Read<RepAction66>();
			break;
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			/// 0x67 - Enter choose building submenu                          [ 1 byte ] [APM+]
			/// 0x66 for WarCraft III patch version <= 1.06
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
		case 0x67:
			buf.Read<RepAction67>();
			break;
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			/// 0x68 - Minimap signal (ping)                                [ 13 bytes ] [APM-]
			/// 0x67 for WarCraft III patch version <= 1.06
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			///  1 dword - Location X
			///  1 dword - Location Y
			///  1 dword - unknown (00 00 A0 40)
		case 0x68:
			buf.Read<RepAction68>();
			break;
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			/// 0x69 - Continue Game (BlockB)                               [ 17 bytes ] [APM-]
			/// 0x68 for WarCraft III patch version <= 1.06
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			///  1 dword - unknown [C]
			///  1 dword - unknown [D]
			///  1 dword - unknown [A]
			///  1 dword - unknown [B]
		case 0x69:
			buf.Read<RepAction69>();
			break;
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			/// 0x6A - Continue Game (BlockA)                               [ 17 bytes ] [APM-]
			/// 0x69 for WarCraft III patch version <= 1.06
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			///  1 dword  - unknown [A]
			///  1 dword  - unknown [B]
			///  1 dword  - unknown [C]
			///  1 dword  - unknown [D]
		case 0x6A:
			buf.Read<RepAction6A>();
			break;
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			/// 0x6B - SyncStoredInteger actions	     [ n bytes ] [APM-]
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			/// n byte  - unknown1 null terminated string - On the observed replay it seems to be always the same: A trigger name or identifier? -
			/// n byte  - unknown2 null terminated string - Player slot as a string. This info can and will be overridden by action 0x70 in replay of dota 6.39b and 6.39.
			/// n byte  - unknown3 null terminated string - stat identifier (so far "1" : kills - "2" : deaths - "3" : creepkills -  "4" : kills - creepdenies)
			/// 1 dword - statValue - stat value associated to each identifier category.
		case 0x6B:
			{
				buf.Read<uint8_t>();
				std::string szGameCache = buf.ReadString();
				std::string szTable     = buf.ReadString();
				std::string szKey       = buf.ReadString();
				uint32_t nValue         = * buf.Read<uint32_t>();

				if (szGameCache.size() > 4 && szGameCache[2] == '.' && szGameCache[3] == 'x' && szGameCache[4] == '\0')
				{
					AnalyzerAction6B(playerid, time, szTable, szKey, nValue);
				}
			}
			break;
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			/// 0x6F - Unknown                                               [ 1 bytes ] [APM-]
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			/// 偶尔会出现在6B之前，作用未知
			///
		case 0x6F:
			buf.Read<RepAction6F>();
			break;
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			/// 0x75 - Unknown                                               [ 2 bytes ] [APM-]
			/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			///  1 byte  - unknown
		case 0x75:
			buf.Read<RepAction75>();
			break;
		default:
			buf.Read<uint8_t>();
#ifdef _DEBUG
			__asm {int 3}
#endif
			//throw RepException(ERR_UNKNOW_FORMAT, "未知的动作块: 0x%02X", buf.PeekChar());
			break;
		}
	}

	void RepAnalyzer::AnalyzerActionBlock(uint8_t playerid, uint32_t time, RepBuffer&& buf)
	{
		while (!buf.IsEnd())
		{		
			AnalyzerAction(playerid, time, buf);
		}
	}
}
