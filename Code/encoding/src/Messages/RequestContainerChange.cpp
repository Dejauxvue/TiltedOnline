#include <Messages/RequestContainerChange.h>
#include <Serialization.hpp>

void RequestContainerChange::SerializeRaw(TiltedPhoques::Buffer::Writer& aWriter) const noexcept
{
    Serialization::WriteVarInt(aWriter, itemID);
    Serialization::WriteVarInt(aWriter, itemCount);
    Serialization::WriteVarInt(aWriter, sourceContainer);
    Serialization::WriteVarInt(aWriter, targetContainer);
}

void RequestContainerChange::DeserializeRaw(TiltedPhoques::Buffer::Reader& aReader) noexcept
{
    ClientMessage::DeserializeRaw(aReader);

    itemID = Serialization::ReadVarInt(aReader);
    itemCount = Serialization::ReadVarInt(aReader);
    sourceContainer = Serialization::ReadVarInt(aReader);
    targetContainer = Serialization::ReadVarInt(aReader);
}
