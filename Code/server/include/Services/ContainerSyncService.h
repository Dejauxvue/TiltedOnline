#pragma once

#include <Events/PacketEvent.h>

struct World;
struct RequestContainerChange;

class ContainerSyncService
{
public:
    ContainerSyncService(World& aWorld, entt::dispatcher& aDispatcher);

  
private:
    void HandleContainerChange(const PacketEvent<RequestContainerChange>& aChanges) const noexcept;

    entt::scoped_connection m_ContainerChangeConnection;

    World& m_world;
};
