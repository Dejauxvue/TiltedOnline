#include <Messages/NotifyContainerChange.h>
#include <Serialization.hpp>

void NotifyContainerChange::SerializeRaw(TiltedPhoques::Buffer::Writer& aWriter) const noexcept
{
    Serialization::WriteVarInt(aWriter, itemID);
    Serialization::WriteVarInt(aWriter, itemCount);
    Serialization::WriteVarInt(aWriter, sourceContainer);
    Serialization::WriteVarInt(aWriter, targetContainer);
}

void NotifyContainerChange::DeserializeRaw(TiltedPhoques::Buffer::Reader& aReader) noexcept
{
    ServerMessage::DeserializeRaw(aReader);

    itemID = Serialization::ReadVarInt(aReader);
    itemCount = Serialization::ReadVarInt(aReader);
    sourceContainer = Serialization::ReadVarInt(aReader);
    targetContainer = Serialization::ReadVarInt(aReader);
}
