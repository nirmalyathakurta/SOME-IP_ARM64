#include <vsomeip/vsomeip.hpp>
#include <iostream>
#include <memory>
#include <cstring>

#define SERVICE_ID  0x1234
#define INSTANCE_ID 0x5678
#define METHOD_ID   0x0421

std::shared_ptr<vsomeip::application> app;

void on_message(const std::shared_ptr<vsomeip::message> &response) {
    auto payload = response->get_payload();
    std::string received(reinterpret_cast<const char*>(payload->get_data()), payload->get_length());
    std::cout << "Client: Received response: " << received << std::endl;
}

void on_availability(vsomeip::service_t, vsomeip::instance_t, bool is_available) {
    if (is_available) {
        std::shared_ptr<vsomeip::message> request = vsomeip::runtime::get()->create_request();
        request->set_service(SERVICE_ID);
        request->set_instance(INSTANCE_ID);
        request->set_method(METHOD_ID);

        // Use runtime to create payload
        std::shared_ptr<vsomeip::payload> payload = vsomeip::runtime::get()->create_payload();

        std::vector<vsomeip::byte_t> data = {'H', 'i'};
        payload->set_data(std::move(data)); // Use move for performance
        request->set_payload(payload);

        app->send(request);
    }
}

int main() {
    app = vsomeip::runtime::get()->create_application("ClientApp");

    if (!app->init()) {
        std::cerr << "Failed to initialize app" << std::endl;
        return 1;
    }

    app->register_availability_handler(SERVICE_ID, INSTANCE_ID, on_availability);
    app->register_message_handler(SERVICE_ID, INSTANCE_ID, METHOD_ID, on_message);

    app->request_service(SERVICE_ID, INSTANCE_ID);

    std::cout << "SOME/IP Client started." << std::endl;
    app->start();

    return 0;
}

