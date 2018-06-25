#pragma once

#include <stdint.h>

#pragma pack(push, 1) 

namespace WRep
{
	struct SlotRecord
	{
		uint8_t player_id;
		uint8_t unk;
		uint8_t slot_state;
		uint8_t controller;
		uint8_t team;
		uint8_t color;
		uint8_t race;
		uint8_t strength;
		uint8_t handicap;
	};

	struct RepBlock00
	{
		uint8_t  nBlockID;
	};

	struct RepBlock17
	{
		uint8_t  nBlockID;
		uint32_t nReason;
		uint8_t  nPlayerId;
		uint32_t nResult;
		uint32_t nUnk;
	};

	struct RepBlock1A
	{
		uint8_t  nBlockID;
		uint32_t nUnk;
	};

	struct RepBlock1B
	{
		uint8_t  nBlockID;
		uint32_t nUnk;
	};

	struct RepBlock1C
	{
		uint8_t  nBlockID;
		uint32_t nUnk;
	};

	struct RepBlock1F
	{
		uint8_t  nBlockID;
		uint16_t nCount;
		uint16_t nIncrementTime;
		// RepAction[nCount];
	};

	struct RepBlock20
	{
		uint8_t  nBlockID;
		uint8_t  nPlayerID;
		uint16_t nCount;
		uint8_t  nFlag;
		//uint32_t nChatMode; (if nFlag != 0x10)
		//ZSTRING szChatMsg;
	};

	struct RepBlock22
	{
		uint8_t  nBlockID;
		uint8_t  nSize;
		uint32_t nUnk;
	};

	struct RepBlock23
	{
		uint8_t  nBlockID;
		uint32_t nUnkA;
		uint8_t  nUnkB;
		uint32_t nUnkC;
		uint8_t  nUnkD;
	};

	struct RepBlock2F
	{
		uint8_t  nBlockID;
		uint32_t nMode;
		uint32_t nCountdownTime;
	};
}

#pragma pack(pop)
