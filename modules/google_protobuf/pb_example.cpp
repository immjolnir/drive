#include "proto/common/timestamp.pb.h"
#include "proto/tutorial/addressbook.pb.h"

int main() {
    // Verify that the version of the library that we linked against is
    // compatible with the version of the headers we compiled against.
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    proto::common::Timestamp timestamp;
    timestamp.set_seconds(1);
    timestamp.set_nanos(1);

    // Optional:  Delete all global objects allocated by libprotobuf.
    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}