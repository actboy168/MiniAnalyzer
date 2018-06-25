#pragma once

#include <stdint.h>

namespace WRep
{
#pragma pack(push, 1) 

	struct RepAction01
	{
		uint8_t  nActionID;
	};

	struct RepAction02
	{
		uint8_t  nActionID;
	};

	struct RepAction03
	{
		uint8_t  nActionID;
		uint8_t  nGameSpeed;
	};

	struct RepAction04
	{
		uint8_t  nActionID;
	};

	struct RepAction05
	{
		uint8_t  nActionID;
	};

	struct RepAction06
	{
		uint8_t  nActionID;
		//ZSTRING szSavegameName;
	};

	struct RepAction07
	{
		uint8_t  nActionID;
		uint32_t nUnk;
	};

	struct RepAction10
	{
		uint8_t  nActionID;
		uint16_t nAbilityFlags;
		uint32_t nItemID;
		uint32_t nUnkA;
		uint32_t nUnkB;
	};

	struct RepAction11
	{
		uint8_t  nActionID;
		uint16_t nAbilityFlags;
		uint32_t nItemID;
		uint32_t nUnkA;
		uint32_t nUnkB;
		uint32_t nTargetLocX;
		uint32_t nTargetLocY;
	};

	struct RepAction12
	{
		uint8_t  nActionID;
		uint16_t nAbilityFlags;
		uint32_t nItemID;
		uint32_t nUnkA;
		uint32_t nUnkB;
		uint32_t nTargetLocX;
		uint32_t nTargetLocY;
		uint64_t nObjID;
	};

	struct RepAction13
	{
		uint8_t  nActionID;
		uint16_t nAbilityFlags;
		uint32_t nItemID;
		uint32_t nUnkA;
		uint32_t nUnkB;
		uint32_t nTargetLocX;
		uint32_t nTargetLocY;
		uint64_t nTargetObjID;
		uint64_t nItemObjID;
	};

	struct RepAction14
	{
		uint8_t  nActionID;
		uint16_t nAbilityFlags;
		uint32_t nItemIDA;
		uint32_t nUnkA;
		uint32_t nUnkB;
		uint32_t nTargetLocAX;
		uint32_t nTargetLocAY;
		uint32_t nItemIDB;
		uint8_t  nUnkC;
		uint32_t nUnkD;
		uint32_t nUnkE;
		uint32_t nTargetLocBX;
		uint32_t nTargetLocBY;
	};

	struct RepAction16
	{
		uint8_t  nActionID;
		uint8_t  nSelectMode;
		uint16_t nUnitsCount;
		//uint64_t ObjectID[nUnitsCount];
	};

	struct RepAction17
	{
		uint8_t  nActionID;
		uint8_t  nGroupNo;
		uint16_t nUnitsCount;
		//uint64_t ObjectID[nUnitsCount];
	};

	struct RepAction18
	{
		uint8_t  nActionID;
		uint8_t  nGroupNo;
		uint8_t  nUnk;
	};

	struct RepAction19
	{
		uint8_t  nActionID;
		uint32_t nItemID;
		uint64_t nObjectID;
	};

	struct RepAction1A
	{
		uint8_t  nActionID;
	};

	struct RepAction1B
	{
		uint8_t  nActionID;
		uint8_t  nUnk;
		uint64_t nObjectID;
	};

	struct RepAction1C
	{
		uint8_t  nActionID;
		uint8_t  nUnk;
		uint64_t nObjectID;
	};

	struct RepAction1D
	{
		uint8_t  nActionID;
		uint64_t nObjectID;
	};

	struct RepAction1E
	{
		uint8_t  nActionID;
		uint8_t  nSlotNr;
		uint32_t nItemID;
	};

	struct RepAction21
	{
		uint8_t  nActionID;
		uint64_t nUnk;
	};

	struct RepAction20
	{
		uint8_t  nActionID;
	};

	struct RepAction22
	{
		uint8_t  nActionID;
	};
	struct RepAction23
	{
		uint8_t  nActionID;
	};
	struct RepAction24
	{
		uint8_t  nActionID;
	};
	struct RepAction25
	{
		uint8_t  nActionID;
	};
	struct RepAction26
	{
		uint8_t  nActionID;
	};
	struct RepAction27
	{
		uint8_t  nActionID;
		uint8_t  nUnk;
		uint32_t nRessources;
	};
	struct RepAction28
	{
		uint8_t  nActionID;
		uint8_t  nUnk;
		uint32_t nRessources;
	};
	struct RepAction29
	{
		uint8_t  nActionID;
	};
	struct RepAction2A
	{
		uint8_t  nActionID;
	};
	struct RepAction2B
	{
		uint8_t  nActionID;
	};
	struct RepAction2C
	{
		uint8_t  nActionID;
	};
	struct RepAction2D
	{
		uint8_t  nActionID;
		uint8_t  nUnk;
		uint32_t nRessources;
	};
	struct RepAction2E
	{
		uint8_t  nActionID;
		uint32_t nTime;
	};
	struct RepAction2F
	{
		uint8_t  nActionID;
	};
	struct RepAction30
	{
		uint8_t  nActionID;
	};
	struct RepAction31
	{
		uint8_t  nActionID;
	};
	struct RepAction32
	{
		uint8_t  nActionID;
	};

	struct RepAction50
	{
		uint8_t  nActionID;
		uint8_t  nPlayerSlot;
		uint32_t nFlag;
	};
	struct RepAction51
	{
		uint8_t  nActionID;
		uint8_t  nPlayerSlot;
		uint32_t nGoldToTransfer;
		uint32_t nLumberToTransfer;
	};

	struct RepAction60
	{
		uint8_t  nActionID;
		uint32_t nUnkA;
		uint32_t nUnkB;
		//ZSTRING szUnk;
	};

	struct RepAction61
	{
		uint8_t  nActionID;
	};

	struct RepAction62
	{
		uint8_t  nActionID;
		uint32_t nUnkA;
		uint32_t nUnkB;
		uint32_t nCounter;
	};

	struct RepAction63
	{
		uint8_t  nActionID;
		uint32_t nUnkA;
		uint32_t nUnkB;
	};

	struct RepAction66
	{
		uint8_t  nActionID;
	};

	struct RepAction67
	{
		uint8_t  nActionID;
	};

	struct RepAction68
	{
		uint8_t  nActionID;
		uint32_t nLocX;
		uint32_t nLocY;
		uint32_t nUnk;
	};

	struct RepAction69
	{
		uint8_t  nActionID;
		uint32_t nUnkC;
		uint32_t nUnkD;
		uint32_t nUnkA;
		uint32_t nUnkB;
	};

	struct RepAction6A
	{
		uint8_t  nActionID;
		uint32_t nUnkA;
		uint32_t nUnkB;
		uint32_t nUnkC;
		uint32_t nUnkD;
	};

	struct RepAction6B
	{
		uint8_t  nActionID;
		//ZSTRING szGCName;
		//ZSTRING szTable;
		//ZSTRING szKey;
		uint32_t nValue;
	};

	struct RepAction6F
	{
		uint8_t  nActionID;
	};

	struct RepAction75
	{
		uint8_t  nActionID;
		uint8_t  nUnk;
	};

}

#pragma pack(pop)
