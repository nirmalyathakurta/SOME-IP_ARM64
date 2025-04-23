#include <vsomeip/vsomeip.hpp>
#include <iostream>
#include <memory>
#include <vector>

#define SERVICE_ID  0x1234
#define INSTANCE_ID 0x5678
#define METHOD_ID   0x0421

std::shared_ptr<vsomeip::application> app;

void on_message(const std::shared_ptr<vsomeip::message> &request) {
    auto payload = request->get_payload();
    const vsomeip::byte_t* data_ptr = payload->get_data();
    std::size_t data_len = payload->get_length();
    std::string received(reinterpret_cast<const char*>(data_ptr), data_len);

    std::cout << "Server: Received request: " << received << std::endl;

    // Create response
    std::shared_ptr<vsomeip::message> response = vsomeip::runtime::get()->create_response(request);
    std::shared_ptr<vsomeip::payload> resp_payload = vsomeip::runtime::get()->create_payload();

    std::string reply = "Hello from Server!";
    std::vector<vsomeip::byte_t> resp_data(reply.begin(), reply.end());
    resp_payload->set_data(std::move(resp_data));
    response->set_payload(resp_payload);

    app->send(response);
}

int main() {
    app = vsomeip::runtime::get()->create_application("ServiceApp");

    if (!app->init()) {
        std::cerr << "Server failed to initialize." << std::endl;
        return 1;
    }

    app->register_message_handler(SERVICE_ID, INSTANCE_ID, METHOD_ID, on_message);

    app->offer_service(SERVICE_ID, INSTANCE_ID);

    std::cout << "SOME/IP Server started." << std::endl;
    app->start();

    return 0;
}

