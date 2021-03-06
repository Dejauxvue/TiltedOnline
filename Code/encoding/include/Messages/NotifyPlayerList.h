#pragma once

#include "Message.h"
#include <Stl.hpp>

using TiltedPhoques::String;
using TiltedPhoques::Map;

struct NotifyPlayerList final : ServerMessage
{
    NotifyPlayerList() : 
        ServerMessage(kNotifyPlayerList)
    {
    }

    virtual ~NotifyPlayerList() = default;

    void SerializeRaw(TiltedPhoques::Buffer::Writer& aWriter) const noexcept override;
    void DeserializeRaw(TiltedPhoques::Buffer::Reader& aReader) noexcept override;

    bool operator==(const NotifyPlayerList& acRhs) const noexcept
    {
        return Players == acRhs.Players &&
            GetOpcode() == acRhs.GetOpcode();
    }

    Map<uint32_t, String> Players{};
};
