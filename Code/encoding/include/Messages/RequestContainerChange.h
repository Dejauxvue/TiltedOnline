#pragma once

#include "Message.h"

#include <Buffer.hpp>

using TiltedPhoques::Map;

struct RequestContainerChange final : ClientMessage
{
    RequestContainerChange() : ClientMessage(kRequestContainerChange)
    {
    }

    void SerializeRaw(TiltedPhoques::Buffer::Writer& aWriter) const noexcept override;
    void DeserializeRaw(TiltedPhoques::Buffer::Reader& aReader) noexcept override;

    bool operator==(const RequestContainerChange& acRhs) const noexcept
    {
        return itemID == acRhs.itemID && itemCount == acRhs.itemCount && sourceContainer == acRhs.sourceContainer &&
               targetContainer == acRhs.targetContainer && GetOpcode() == acRhs.GetOpcode();
    }

    uint32_t itemID;
    uint32_t itemCount;
    uint32_t sourceContainer;
    uint32_t targetContainer;
};
