
#include "Forms/TESForm.h"
#include "Services/PapyrusService.h"

#include <Components.h>
#include <Messages/NotifyContainerChange.h>
#include <Messages/RequestContainerChange.h>

#include <Services/ContainerSyncService.h>

ContainerSyncService::ContainerSyncService(World& aWorld, entt::dispatcher& aDispatcher) : m_world(aWorld)
{
    aDispatcher.sink<NotifyContainerChange>().connect<&ContainerSyncService::OnContainerSync>(this);
    spdlog::info("started listener to Notifycontainerchange");
    EventDispatcherManager::Get()->containerChangedEvent.RegisterSink(this);
}

void ContainerSyncService::OnContainerSync(const NotifyContainerChange& acEvent)
{
    spdlog::info("receiving notify changes: item({:x}), amount({:x}), source({:x}), target({:x})", acEvent.itemID,
                 acEvent.itemCount, acEvent.sourceContainer, acEvent.targetContainer);

    auto view = m_world.view<RemoteComponent, FormIdComponent>();

    const auto source =
        std::find_if(std::begin(view), std::end(view), [id = acEvent.sourceContainer, view](entt::entity entity) {
            return view.get<RemoteComponent>(entity).Id == id;
        });

    const auto target =
        std::find_if(std::begin(view), std::end(view), [id = acEvent.targetContainer, view](entt::entity entity) {
            return view.get<RemoteComponent>(entity).Id == id;
        });

    if (source == std::end(view))
        spdlog::info("could not fit id for source");

    if (target == std::end(view))
        spdlog::info("could not fit id for target");

    if (source != std::end(view) && target != std::end(view))
    {
        auto& sourceFormIdComponent = view.get<FormIdComponent>(*source);
        auto& sourceRemoteComponent = view.get<RemoteComponent>(*source);
        auto& targetFormIdComponent = view.get<FormIdComponent>(*target);
        auto& targetRemoteComponent = view.get<RemoteComponent>(*target);

        using ObjectReference = TESObjectREFR;
        PAPYRUS_FUNCTION(void, ObjectReference, AddItem, uint32_t, uint32_t, boolean);

        auto* pForm = TESForm::GetById(targetFormIdComponent.Id);
        auto* pObject = RTTI_CAST(pForm, TESForm, TESObjectREFR);
        s_pAddItem(pObject, acEvent.itemID, acEvent.itemCount, true);

        spdlog::info("source form id: {:x}, remote id: {:x}", sourceFormIdComponent.Id, sourceRemoteComponent.Id);
        spdlog::info("target form id: {:x}, remote id: {:x}", targetFormIdComponent.Id, targetRemoteComponent.Id);
    }
}

BSTEventResult ContainerSyncService::OnEvent(const TESContainerChangedEvent* event,
                                             const EventDispatcher<TESContainerChangedEvent>*)
{
    spdlog::info("transfering {:x} amount: {:x} from {:x} to {:x}", event->itemID, event->itemCount,
                 event->sourceFormId, event->targetFormId);

    auto view = m_world.view<FormIdComponent, LocalComponent>();

    RequestContainerChange requestChange;
    requestChange.itemID = event->itemID;
    requestChange.itemCount = event->itemCount;
    for (auto entity : view)
    {

        auto& formIdComponent = view.get<FormIdComponent>(entity);
        if (formIdComponent.Id == event->sourceFormId)
        {
            auto& localComponent = view.get<LocalComponent>(entity);

            requestChange.sourceContainer = localComponent.Id;
        }

        if (formIdComponent.Id == event->targetFormId)
        {
            auto& localComponent = view.get<LocalComponent>(entity);

            requestChange.targetContainer = localComponent.Id;
        }
    }

    m_world.GetTransport().Send(requestChange);

    return BSTEventResult::kOk;
}
