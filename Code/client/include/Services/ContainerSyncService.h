#pragma once

#include <Events/EventDispatcher.h>
#include <Games/Events.h>
#include <World.h>

class ContainerSyncService final : public BSTEventSink<TESContainerChangedEvent>
{
  public:
    ContainerSyncService(World&, entt::dispatcher&);
    ~ContainerSyncService() override = default;

  private:
    BSTEventResult OnEvent(const TESContainerChangedEvent*, const EventDispatcher<TESContainerChangedEvent>*) override;

    entt::scoped_connection m_containerSyncConnection;

    World& m_world;
};
