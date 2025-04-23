#include <vsomeip/vsomeip.hpp>
#include <iostream>
#include <memory>
#include <chrono>
#include <thread>

#define SERVICE_ID     0x1234
#define INSTANCE_ID    0x5678
#define EVENT_ID       0x4321
#define EVENTGROUP_ID  0x0020

std::shared_ptr<vsomeip::application> app;

void publish_event() {
    while (true) {
        std::shared_ptr<vsomeip::payload> payload = vsomeip::runtime::get()->create_payload();
        std::string message = "Periodic Update from Server";
        std::vector<vsomeip::byte_t> data(message.begin(), message.end());
        payload->set_data(data);

        app->notify(SERVICE_ID, INSTANCE_ID, EVENT_ID, payload);
        std::cout << "Server: Event Published." << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}

int main() {
    app = vsomeip::runtime::get()->create_application("PublisherApp");

    if (!app->init()) {
        std::cerr << "Server failed to initialize." << std::endl;
        return 1;
    }

    app->offer_service(SERVICE_ID, INSTANCE_ID);
    app->offer_event(SERVICE_ID, INSTANCE_ID, EVENT_ID, EVENTGROUP_ID);
    app->start_offer_service();

    std::thread(publish_event).detach();

    std::cout << "SOME/IP Publisher started." << std::endl;
    app->start();

    return 0;
}
