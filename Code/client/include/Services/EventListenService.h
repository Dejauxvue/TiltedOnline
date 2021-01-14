#pragma once
#include <string>

#include <Games/Events.h>

struct World;
struct ImguiService;
struct UpdateEvent;

struct TransportService;

struct Actor;

template <typename EventT> struct EventListenService : BSTEventSink<EventT>
{
    EventListenService(EventDispatcher<EventT> EventDispatcherManager::*dispatcher, const std::string& message) noexcept
        : m_message(message)
    {
        (EventDispatcherManager::Get()->*dispatcher).RegisterSink(this);
    }

    ~EventListenService() noexcept
    {
    }

    TP_NOCOPYMOVE(EventListenService);

  private:
    std::string m_message;
    BSTEventResult OnEvent(const EventT* aEvent, const EventDispatcher<EventT>* aDispatcher) override
    {
        int five = 5;
        std::cout << five << std::endl;
        // auto* containerEvent = RTTI_CAST(aEvent, EventT, TESContainerChangedEvent);
        // if (containerEvent)
        //{

        static std::ofstream of("memory_dump.csv");
        for (int i = 0; i < 5; i++)
        {
            if (i)
                std::cout << ";";
            std::cout << std::hex << *(reinterpret_cast<const uint32_t*>(aEvent) + i);
        }
        std::cout << std::endl;

        //}
        spdlog::log(spdlog::level::info, std::string("event: ") + m_message);
        return BSTEventResult::kOk;
    }
};
