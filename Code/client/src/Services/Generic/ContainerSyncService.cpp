#include <Messages/RequestContainerChange.h>

#include <Services/ContainerSyncService.h>

ContainerSyncService::ContainerSyncService(World& aWorld, entt::dispatcher& aDispatcher) : m_world(aWorld)
{
    EventDispatcherManager::Get()->containerChangedEvent.RegisterSink(this);
}

BSTEventResult ContainerSyncService::OnEvent(const TESContainerChangedEvent* event,
                                             const EventDispatcher<TESContainerChangedEvent>*)
{
    spdlog::info("transfering {:x} amount: {:x} from {:x} to {:x}", event->itemID, event->itemCount,
                 event->sourceContainer, event->targetContainer);
    RequestContainerChange requestChange;
    requestChange.itemID = event->itemID;
    requestChange.itemCount = event->itemCount;
    requestChange.sourceContainer = event->sourceContainer;
    requestChange.targetContainer = event->targetContainer;
    m_world.GetTransport().Send(requestChange);
    return BSTEventResult::kOk;
}
