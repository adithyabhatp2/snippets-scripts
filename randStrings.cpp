

#include <iostream>

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include <vector>

#include "../gen-cpp/Echo.h"

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

#define DEBUGLEVEL 1

void clientUsage(void) {
    printf("usage: client -s <server name> -p <port> -n <num pkts> -l <msg length>\n");
    exit(1);
}

string generateRandStr(size_t length) {
    static string charset = "0123456789"
            "abcdefghijklmnopqrstuvwxyz"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    string result;
    result.resize(length);
    for (size_t i = 0; i < length; i++) {
        result[i] = charset[rand() % charset.length()];
    }
    return result;
}

vector <string> generateRandomStrings(unsigned long numStrings, unsigned long strLen) {
    vector <string> result;
    for (unsigned long i = 0; i < numStrings; i++) {
        result.push_back(generateRandStr(strLen));
    }
    return result;
}

void communicatewithServer(char *hostName, int port, unsigned long numPkts, unsigned long msgLen) {

    vector <string> msgPackets = generateRandomStrings(numPkts, msgLen);

    boost::shared_ptr <TTransport> socket(new TSocket(hostName, port));
    boost::shared_ptr <TTransport> transport(new TBufferedTransport(socket));
    boost::shared_ptr <TProtocol> protocol(new TBinaryProtocol(transport));
    EchoClient client(protocol);

    try {
        transport->open();

        for (string msg : msgPackets) {
            string returnVal;
            client.echo(returnVal, msg);
            if(DEBUGLEVEL) {
                cout << "Echoing: " << returnVal << endl;
            }
        }
        transport->close();

    } catch (TException &tx) {
        cout << "ERROR: " << tx.what() << endl;
    }

}

int main(int argc, char *argv[]) {

    if (argc < 9) {
        printf("Insufficient parameters!\n");
        clientUsage();
    }

    unsigned long int serverPort = UINT_MAX;
    unsigned long numPkts = 10;
    unsigned long msgLen = 10;

    char *hostName;
    int ch;

    // Read input params from cmd line
    while ((ch = getopt(argc, argv, "s:p:n:l:")) != -1) {
        switch (ch) {
            case 's':
                hostName = strdup(optarg);
                break;
            case 'p':
                serverPort = strtoul(optarg, NULL, 10);
                if (serverPort <= 1024 || serverPort > 65536) {
                    printf("Invalid Port\n");
                    clientUsage();
                }
                break;
            case 'n':
                numPkts = strtoul(optarg, NULL, 10);
                if (numPkts == 0 && errno == EINVAL) {
                    clientUsage();
                }
                break;
            case 'l':
                msgLen = strtoul(optarg, NULL, 10);
                if (msgLen == 0 && errno == EINVAL) {
                    clientUsage();
                }
                break;
            case '?':
            default:
                clientUsage();
                return 0;
        }
    }


    communicatewithServer(hostName, serverPort, numPkts, msgLen);

    char exitCh = 0;
    cin >> exitCh;


}