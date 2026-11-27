#pragma once

#include <cstdint>

namespace Shared::Structures
{
	struct EffectBlock
	{
		uint16_t id = 0;
		uint16_t properties = 0;
		uint16_t flags = 0;
		uint8_t targets[10] = {};
		uint32_t eventInfo = 0;

		void BuildId(uint16_t cardIntID)
		{
			this->id = cardIntID;
		}
		void BuildProperties(uint8_t side, uint8_t location, uint8_t responseWindow, uint8_t kind, uint8_t alreadyUsed)
		{
			side &= 1;
			location &= 0x1f;
			responseWindow &= 0x3f;
			kind &= 3;
			alreadyUsed &= 1;
			this->properties = (alreadyUsed << 14) | (kind << 12) | (responseWindow << 6) | (location << 1) | (side);
		}
		void BuildFlags(uint8_t leavesAfter, uint8_t finishedResolving, uint8_t instance, uint8_t targetCount)
		{
			leavesAfter &= 1;
			finishedResolving &= 1;
			instance &= 0xff;
			targetCount &= 7;
			this->flags = (targetCount << 13) | (instance << 5) | (finishedResolving << 2) | (leavesAfter << 1);
		}
		void BuildEventInfo(uint8_t eventSide, uint8_t eventLocation)
		{
			eventSide &= 1;
			eventLocation &= 0x1f;
			this->eventInfo = (eventLocation << 8) | (eventSide);
		}
		void AddInstanceTarget(uint16_t inst)
		{
			if (this->GetTargetCount() >= 5) return;
			((uint16_t*)targets)[this->GetTargetCount()] = inst;
		}
		void AddFullTarget(uint32_t dword)
		{
			if (this->GetTargetCount() >= 2) return;
			((uint32_t*)targets)[this->GetTargetCount()] = dword;
		}
		uint16_t GetIntID()
		{
			return id & 0xFFF;
		}
		uint8_t GetSide()
		{
			return properties & 1;
		}
		uint8_t GetLocation()
		{
			return (properties >> 1) & 0x1F;
		}
		uint8_t GetResponseWindow()
		{
			return (properties >> 6) & 0x3F;
		}
		uint8_t GetActivationType()
		{
			return (properties >> 12) & 3;
		}
		bool GetAlreadyUsed()
		{
			return (properties >> 14) & 1;
		}
		bool GetLeavesAfter()
		{
			return (flags >> 1) & 1;
		}
		bool GetFinishedResolving()
		{
			return (flags >> 2) & 1;
		}
		uint8_t  GetInstance()
		{
			return (flags >> 5) & 0xff;
		}
		uint8_t GetTargetCount()
		{
			return (flags >> 13) & 7;
		}
		uint16_t GetInstanceTarget(uint8_t index)
		{
			if (index >= this->GetTargetCount()) return 0;
			return ((uint16_t*)targets)[index];
		}
		uint32_t GetFullTarget(uint8_t index)
		{
			if (index >= this->GetTargetCount()) return 0;
			return ((uint32_t*)targets)[index];
		}
		uint8_t GetEventSide()
		{
			return eventInfo & 1;
		}
		uint8_t GetEventLocation()
		{
			return (eventInfo >> 8) & 0x1F;
		}
		void SetFinishedResolving(bool finished)
		{
			if (finished)
				flags |= (1 << 2);
			else
				flags &= ~(1 << 2);
		}
	};
}