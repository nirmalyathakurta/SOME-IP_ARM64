#include <vsomeip/vsomeip.hpp>
#include <iostream>
#include <memory>

#define SERVICE_ID     0x1234
#define INSTANCE_ID    0x5678
#define EVENT_ID       0x4321
#define EVENTGROUP_ID  0x0020

std::shared_ptr<vsomeip::application> app;

void on_event(const std::shared_ptr<vsomeip::message> &msg) {
    auto payload = msg->get_payload();
    std::string data(reinterpret_cast<const char*>(payload->get_data()), payload->get_length());
    std::cout << "Client: Received Event - " << data << std::endl;
}

int main() {
    app = vsomeip::runtime::get()->create_application("SubscriberApp");

    if (!app->init()) {
        std::cerr << "Client failed to initialize." << std::endl;
        return 1;
    }

    app->register_message_handler(SERVICE_ID, INSTANCE_ID, EVENT_ID, on_event);

    app->request_service(SERVICE_ID, INSTANCE_ID);
    app->subscribe(SERVICE_ID, INSTANCE_ID, EVENTGROUP_ID);

    std::cout << "SOME/IP Subscriber started." << std::endl;
    app->start();

    return 0;
}
