#include "RemoteLan.h"
#include <sio_client.h>

void createSocket() {
    sio::client h;
    auto socket = h.socket("3000");
    h.connect("localhost:3000");
    socket->emit("asdf");
//    h.
}
