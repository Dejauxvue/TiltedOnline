#include <GameServer.h>

#include <Components.h>

#include <Services/ContainerSyncService.h>
#include <World.h>

#include <Messages/NotifyContainerChange.h>
#include <Messages/RequestContainerChange.h>

ContainerSyncService::ContainerSyncService(World& aWorld, entt::dispatcher& aDispatcher) : m_world(aWorld)
{
    m_ContainerChangeConnection =
        aDispatcher.sink<PacketEvent<RequestContainerChange>>().connect<&ContainerSyncService::HandleContainerChange>(
            this);
}

void ContainerSyncService::HandleContainerChange(const PacketEvent<RequestContainerChange>& acMessage) const noexcept
{
    const auto& message = acMessage.Packet;
    spdlog::info("receiving RequestContainer change: Item({:x}), Amount({:x}), source({:x}), target({:x})",
                 message.itemID, message.itemCount, message.sourceContainer, message.targetContainer);

    NotifyContainerChange notifyChange;
    notifyChange.itemID = message.itemID;
    notifyChange.itemCount = message.itemCount;
    //TODO: is this cast identity?
    notifyChange.sourceContainer = World::ToInteger(static_cast<entt::entity>(message.sourceContainer));
    notifyChange.targetContainer = World::ToInteger(static_cast<entt::entity>(message.targetContainer));
    

    auto view = m_world.view<PlayerComponent>();
    for (auto entity : view)
    {
        auto& player = view.get<PlayerComponent>(entity);
        //TODO: only send to self for testing
        //if (player.ConnectionId != acMessage.ConnectionId)
        {
            spdlog::info("sending NotifyContainerChange");
            GameServer::Get()->Send(player.ConnectionId, notifyChange);
        }
    }
}
