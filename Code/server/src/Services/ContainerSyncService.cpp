

#include <Services/ContainerSyncService.h>
#include <World.h>

#include <Messages/RequestContainerChange.h>

ContainerSyncService::ContainerSyncService(World& aWorld, entt::dispatcher& aDispatcher) : m_world(aWorld)
{
    m_ContainerChangeConnection =
        aDispatcher.sink<PacketEvent<RequestContainerChange>>().connect<&ContainerSyncService::HandleContainerChange>(
            this);
}

void ContainerSyncService::HandleContainerChange(const PacketEvent<RequestContainerChange>& acMessage) noexcept
{
    const auto& message = acMessage.Packet;
    spdlog::info("receiving RequestContainer change: Item({:x}), Amount({:x}), source({:x}), target({:x})",
                 message.itemID, message.itemCount, message.sourceContainer, message.targetContainer);
}
