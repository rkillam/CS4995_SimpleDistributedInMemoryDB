#include <libconfig.h++>
#include <zmqpp/zmqpp.hpp>


int main (int argc, char *argv[]) {
    //  Prepare our context and sockets
    zmqpp::context context;
    zmqpp::socket frontend(context, zmqpp::socket_type::router);
    zmqpp::socket backend(context, zmqpp::socket_type::dealer);

    libconfig::Config conf;
    conf.readFile("db.conf");

    const libconfig::Setting& root = conf.getRoot();

    std::string masterPort = root["MASTER_PORT"];
    frontend.bind("tcp://*:" + masterPort);

    std::string slavePort = root["SLAVE_PORT"];
    backend.bind("tcp://*:" + slavePort);

    // Initialize poll set
    zmqpp::poller poller;
    poller.add(frontend);
    poller.add(backend);

    // Switch messages between sockets
    while(true) {
        zmqpp::message message;
        poller.poll();

        if(poller.has_input(frontend)) {
            frontend.receive(message);
            backend.send(message);
        }

        if(poller.has_input(backend)) {
            backend.receive(message);
            frontend.send(message);
        }
    }

    return 0;
}
