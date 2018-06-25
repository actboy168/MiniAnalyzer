#include "RepAnalyzer.hpp"
#include "Exception.hpp"
#include <Zlib/zlib.h>
#include <memory>

namespace WRep
{
#pragma pack(push, 1) 
	struct RepHeader
	{
		char     szSignString[28];
		uint32_t nHeaderSize;
		uint32_t nCompressedFileSize;
		uint32_t nReplayHeaderVersion;
		uint32_t nDecompressedFileSize;
		uint32_t nBlocks;
	};
	struct RepHeaderV0
	{
		RepHeader tHeader;
		uint16_t  nUnk;
		uint16_t  nVersion;
		uint16_t  nBuild;
		uint16_t  nFlag;
		uint32_t  nReplayLength;
		uint32_t  nCrc32;
	};
	struct RepHeaderV1
	{
		RepHeader tHeader;
		uint32_t  nSign;
		uint32_t  nVersion;
		uint16_t  nBuild;
		uint16_t  nFlag;
		uint32_t  nReplayLength;
		uint32_t  nCrc32;
	};
#pragma pack(pop)

	RepBuffer RepAnalyzer::LoadData(RepBuffer&& buf)
	{
		//2.0 [Header]
		//offset | size/type | Description
		//-------+-----------+-----------------------------------------------------------
		//0x0000 | 28 chars  | zero terminated string "Warcraft III recorded game\0x1A\0"
		//0x001c |  1 dword  | fileoffset of first compressed data block (header size)
		//       |           |  0x40 for WarCraft III with patch <= v1.06
		//       |           |  0x44 for WarCraft III patch >= 1.07 and TFT replays
		//0x0020 |  1 dword  | overall size of compressed file
		//0x0024 |  1 dword  | replay header version:
		//       |           |  0x00 for WarCraft III with patch <= 1.06
		//       |           |  0x01 for WarCraft III patch >= 1.07 and TFT replays
		//0x0028 |  1 dword  | overall size of decompressed data (excluding header)
		//0x002c |  1 dword  | number of compressed data blocks in file
		//0x0030 |  n bytes  | SubHeader (see section 2.1 and 2.2)

		RepHeader* pHeader = buf.Read<RepHeader>();

		if (0 != memcmp(pHeader->szSignString, "Warcraft III recorded game\x1A\x00", sizeof(pHeader->szSignString)))
		{
			throw Exception("指定的文件不是合法的Warcraft III replay文件。");
		}

		if (pHeader->nReplayHeaderVersion == 0)
		{
			throw Exception("不支持1.06及更低版本录像.");
		}
		else if (pHeader->nReplayHeaderVersion == 1)
		{
			//2.2 [SubHeader] for header version 1
			//offset | size/type | Description
			//-------+-----------+-----------------------------------------------------------
			//0x0000 |  1 dword  | version identifier string reading:
			//       |           |  'WAR3' for WarCraft III Classic
			//       |           |  'W3XP' for WarCraft III Expansion Set 'The Frozen Throne'
			//0x0004 |  1 dword  | version number (corresponds to patch 1.xx so far)
			//0x0008 |  1  word  | build number (see section 2.3)
			//0x000A |  1  word  | flags
			//       |           |   0x0000 for single player games
			//       |           |   0x8000 for multiplayer games (LAN or Battle.net)
			//0x000C |  1 dword  | replay length in msec
			//0x0010 |  1 dword  | CRC32 checksum for the header
			//       |           | (the checksum is calculated for the complete header
			//       |           |  including this field which is set to zero)

			buf.Seek(0);
			RepHeaderV1* pHeader1 = buf.Read<RepHeaderV1>();

			if (pHeader1->nSign != 'W3XP')
				throw Exception("只支持冰封王座录像,不支持混乱之治录像.");
		}

		return LoadBlocksData(buf, pHeader->nHeaderSize, pHeader->nBlocks);
	}

	RepBuffer RepAnalyzer::LoadBlocksData(RepBuffer& buf, uint32_t offset, uint32_t blocks_count)
	{
		buf.Seek(offset);

		RepBuffer blocks_data;

		uint32_t compressedSize, decompressedSize;

		int ret;
		unsigned have;
		z_stream strm;

		for (uint32_t i = 0; i < blocks_count; i++)
		{
			//offset | size/type | Description
			//-------+-----------+-----------------------------------------------------------
			//0x0000 |  1  word  | size n of compressed data block (excluding header)
			//0x0002 |  1  word  | size of decompressed data block (currently 8k)
			//0x0004 |  1 dword  | unknown (probably checksum)
			//0x0008 |  n bytes  | compressed data (decompress using zlib)

			compressedSize    = * buf.Read<uint16_t>();
			decompressedSize  = * buf.Read<uint16_t>();
			buf.Read<uint32_t>();

			std::string in = buf.ReadBytes(compressedSize);

			std::unique_ptr<uint8_t[]> out(new uint8_t[decompressedSize]);

			/* allocate inflate state */
			strm.zalloc = Z_NULL;
			strm.zfree = Z_NULL;
			strm.opaque = Z_NULL;
			strm.avail_in = 0;
			strm.next_in = Z_NULL;
			ret = inflateInit(&strm);
			if (ret != Z_OK)
			{
				throw Exception("录像文件损坏，zlib解压错误。");
			}

			strm.avail_in = compressedSize;
			strm.next_in = (uint8_t*)in.data();
			strm.avail_out = decompressedSize;
			strm.next_out = out.get();

			ret = inflate(&strm, Z_SYNC_FLUSH);
			//assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
			switch (ret) 
			{
			case Z_NEED_DICT:
				ret = Z_DATA_ERROR;     /* and fall through */
			case Z_DATA_ERROR:
			case Z_MEM_ERROR:
				(void)inflateEnd(&strm);
				throw Exception("录像文件损坏，zlib解压错误。");
			}

			have = decompressedSize - strm.avail_out;

			blocks_data.Write((char*)out.get(), have);

			(void)inflateEnd(&strm);
		}

		blocks_data.Seek(0);

		return std::move(blocks_data);
	}

	std::pair<uint8_t, std::string> LoadPlayerRecord(RepBuffer& reader)
	{
		// 4.1 [PlayerRecord]
		// offset | size/type | Description
		// -------+-----------+-----------------------------------------------------------
		// 0x0000 |  1 byte   | RecordID:
		//        |           |  0x00 for game host
		//        |           |  0x16 for additional players (see 4.9)
		// 0x0001 |  1 byte   | PlayerID
		// 0x0002 |  n bytes  | PlayerName (null terminated string)
		//    n+2 |  1 byte   | size of additional data:
		//        |           |  0x01 = custom
		//        |           |  0x08 = ladder
		//
		// o For custom games:
		// 
		// offset | size/type | Description
		// -------+-----------+---------------------------------------------------------
		// 0x0000 | 1 byte    | null byte (1 byte)
		// 
		// o For ladder games:
		// 
		// offset | size/type | Description
		// -------+-----------+---------------------------------------------------------
		// 0x0000 | 4 bytes   | runtime of players Warcraft.exe in milliseconds
		// 0x0004 | 4 bytes   | player race flags:
		//        |           |   0x01=human
		//        |           |   0x02=orc
		//        |           |   0x04=nightelf
		//        |           |   0x08=undead
		//        |           |  (0x10=daemon)
		//        |           |   0x20=random
		//        |           |   0x40=race selectable/fixed (see notes in section 4.11)

		reader.Read<uint8_t>();
		uint8_t player_id = * reader.Read<uint8_t>();
		std::string name = reader.ReadString();
		reader.ReadBytes(* reader.Read<uint8_t>());

		return std::move(std::make_pair(player_id, name));
	}

	void RepAnalyzer::LoadBase(RepBuffer& buf)
	{
		//4.0 [Decompressed data]
		// # |   Size   | Name
		//---+----------+--------------------------
		// 1 |   4 byte | Unknown (0x00000110 - another record id?)
		// 2 | variable | PlayerRecord (see 4.1)
		// 3 | variable | GameName (null terminated string) (see 4.2)
		// 4 |   1 byte | Nullbyte
		// 5 | variable | Encoded String (null terminated) (see 4.3)
		//   |          |  - GameSettings (see 4.4)
		//   |          |  - Map&CreatorName (see 4.5)
		// 6 |   4 byte | PlayerCount (see 4.6)
		// 7 |   4 byte | GameType (see 4.7)
		// 8 |   4 byte | LanguageID (see 4.8)
		// 9 | variable | PlayerList (see 4.9)
		//10 | variable | GameStartRecord (see 4.11)

		buf.Read<uint32_t>();
		LoadPlayerRecord(buf);
		buf.ReadString();
		buf.Read<uint8_t>();

		RepBuffer decoded(buf.GetDecodedBytes());

		decoded.Read<uint32_t>();
		decoded.Read<uint8_t>();
		decoded.Read<uint8_t>();
		decoded.Read<uint8_t>();
		decoded.Read<uint8_t>();
		decoded.Read<uint8_t>();
		decoded.Read<uint32_t>();
		decoded.ReadString();


		//4.0 [Decompressed data]
		// # |   Size   | Name
		//---+----------+--------------------------
		// 6 |   4 byte | PlayerCount (see 4.6)
		// 7 |   4 byte | GameType (see 4.7)
		// 8 |   4 byte | LanguageID (see 4.8)
		// 9 | variable | PlayerList (see 4.9)
		//10 | variable | GameStartRecord (see 4.11)

		//PlayerCount, GameType, LanguageID
		buf.Read<uint32_t>();
		buf.Read<uint32_t>();
		buf.Read<uint32_t>();

		while (buf.Peek() == 0x16)
		{
			//Player List
			//offset | size/type | Description
			//-------+-----------+-----------------------------------------------------------
			//0x0000 | 4/11 byte | PlayerRecord (see 4.1)
			//0x000? |    4 byte | unknown
			//       |           |  (always 0x00000000 for patch version >= 1.07
			//       |           |   always 0x00000001 for patch version <= 1.06)

			LoadPlayerRecord(buf);
			buf.Read<uint32_t>();
		}
	}

	void RepAnalyzer::LoadPlayers(RepBuffer& buf)
	{
		//4.10 [GameStartRecord]
		//offset | size/type | Description
		//-------+-----------+-----------------------------------------------------------
		//0x0000 |  1 byte   | RecordID - always 0x19
		//0x0001 |  1 word   | number of data bytes following
		//0x0003 |  1 byte   | nr of SlotRecords following (== nr of slots on startscreen)
		//0x0004 |  n bytes  | nr * SlotRecord (see 4.11)
		//   n+4 |  1 dword  | RandomSeed (see 4.12)
		//   n+8 |  1 byte   | SelectMode
		//       |           |   0x00 - team & race selectable (for standard custom games)
		//       |           |   0x01 - team not selectable
		//       |           |          (map setting: fixed alliances in WorldEditor)
		//       |           |   0x03 - team & race not selectable
		//       |           |          (map setting: fixed player properties in WorldEditor)
		//       |           |   0x04 - race fixed to random
		//       |           |          (extended map options: random races selected)
		//       |           |   0xcc - Automated Match Making (ladder)
		//   n+9 |  1 byte   | StartSpotCount (nr. of start positions in map)

		buf.Read<uint8_t>();
		buf.Read<uint16_t>();
		uint8_t nSlots = * buf.Read<uint8_t>();
		for (uint8_t index = 0; index < nSlots; index++)
		{
			//4.11 [SlotRecord]
			//offset | size/type | Description
			//-------+-----------+-----------------------------------------------------------
			//0x0000 |  1 byte   | player id (0x00 for computer players)
			//0x0001 |  1 byte   | map download percent: 0x64 in custom, 0xff in ladder
			//0x0002 |  1 byte   | slotstatus:
			//       |           |   0x00 empty slot
			//       |           |   0x01 closed slot
			//       |           |   0x02 used slot
			//0x0003 |  1 byte   | computer player flag:
			//       |           |   0x00 for human player
			//       |           |   0x01 for computer player
			//0x0004 |  1 byte   | team number:0 - 11
			//       |           | (team 12 == observer or referee)
			//0x0005 |  1 byte   | color (0-11):
			//       |           |   value+1 matches player colors in world editor:
			//       |           |   (red, blue, cyan, purple, yellow, orange, green,
			//       |           |    pink, gray, light blue, dark green, brown)
			//       |           |   color 12 == observer or referee
			//0x0006 |  1 byte   | player race flags (as selected on map screen):
			//       |           |   0x01=human
			//       |           |   0x02=orc
			//       |           |   0x04=nightelf
			//       |           |   0x08=undead
			//       |           |   0x20=random
			//       |           |   0x40=race selectable/fixed (see notes below)
			//0x0007 |  1 byte   | computer AI strength: (only present in v1.03 or higher)
			//       |           |   0x00 for easy
			//       |           |   0x01 for normal
			//       |           |   0x02 for insane
			//       |           | for non-AI players this seems to be always 0x01
			//0x0008 |  1 byte   | player handicap in percent (as displayed on startscreen)
			//       |           | valid values: 0x32, 0x3C, 0x46, 0x50, 0x5A, 0x64
			//       |           | (field only present in v1.07 or higher)

			buf.Read<SlotRecord>();
		}

		buf.Read<uint32_t>(); //random seed
		buf.Read<uint8_t>();  //select mode
		buf.Read<uint8_t>();  //startspotcount
	}

	void RepAnalyzer::LoadTimeSlot(RepBuffer& buf, uint32_t n, uint32_t replay_time, bool& is_paused)
	{
		uint8_t playerid;
		uint32_t blockn;

		while (n > 0)
		{
			playerid = * buf.Read<uint8_t>();
			blockn   = * buf.Read<uint16_t>();
			n -= 3;

			switch(buf.Peek())
			{
				//pause game
			case 0x01:
				is_paused = true;
				break;
				//resume game
			case 0x02:
				is_paused = false;
				break;
			}
			AnalyzerActionBlock(playerid, replay_time, RepBuffer(buf.ReadBytes(blockn)));
			n -= blockn;
		}
	}

	void RepAnalyzer::LoadActions(RepBuffer& buf, uint32_t& replay_time)
	{
		bool is_paused = false;

		while (!buf.IsEnd())
		{
			switch(buf.Peek())
			{
			case 0x00:
				buf.Read<RepBlock00>();
				break;
			case 0x17:
				//leave game
				buf.Read<RepBlock17>();
				break;
			case 0x1A:
				buf.Read<RepBlock1A>();
				break;
			case 0x1B:
				buf.Read<RepBlock1B>();
				break;
			case 0x1C:
				buf.Read<RepBlock1C>();
				break;
			case 0x1E:
			case 0x1F:
				{
					//time slot
					RepBlock1F* pBlock = buf.Read<RepBlock1F>();
					if (!is_paused)
					{
						replay_time += pBlock->nIncrementTime;
					}
					LoadTimeSlot(buf, pBlock->nCount-2, replay_time, is_paused);
					break;
				}
			case 0x20:
				{
					//chat
					RepBlock20* pBlock = buf.Read<RepBlock20>();
					if (pBlock->nFlag != 0x10)
						buf.Read<uint32_t>();
					buf.ReadString();
					break;
				}
			case 0x22:
				buf.Read<RepBlock22>();
				break;
			case 0x23:
				buf.Read<RepBlock23>();
				break;
			case 0x2F:
				buf.Read<RepBlock2F>();
				break;
			default:
				throw Exception("未知的数据块: 0x%02X", buf.Peek());
				break;
			}
		}
	}

	void RepAnalyzer::Load(RepBuffer&& buf)
	{
		uint32_t time = 0;
		LoadBase(buf);
		LoadPlayers(buf);
		LoadActions(buf, time);
	}

	void RepAnalyzer::Analyze(RepBuffer&& buf)
	{
		Load(LoadData(std::forward<RepBuffer>(buf)));
	}
}
