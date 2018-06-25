#pragma once

#include <stdint.h>
#include "RepBlock.hpp"
#include "RepBuffer.hpp"

namespace WRep
{
	class RepAnalyzer
	{
	public:
		static void       Analyze            (RepBuffer&& buf);

	private:
		static RepBuffer  LoadData           (RepBuffer&& buf);
		static void       Load               (RepBuffer&& buf);
		static RepBuffer  LoadBlocksData     (RepBuffer& buf,    uint32_t offset,  uint32_t blocks_count);
		static void       LoadBase           (RepBuffer& buf);
		static void       LoadPlayers        (RepBuffer& buf);
		static void       LoadActions        (RepBuffer& buf,   uint32_t& replay_time);
		static void       LoadTimeSlot       (RepBuffer& buf,   uint32_t n, uint32_t replay_time, bool& is_paused);
		static void       AnalyzerAction6B   (uint8_t playerid, uint32_t time, std::string const& szTable, std::string const& szKey, uint32_t nValue);
		static void       AnalyzerAction     (uint8_t playerid, uint32_t time, RepBuffer& buf);
		static void       AnalyzerActionBlock(uint8_t playerid, uint32_t time, RepBuffer&& buf);
	};
}
