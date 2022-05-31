#include "iostream"
#include "array"


class StockfishConnector
{
public:
    static StockfishConnector& get();
    ~StockfishConnector() { closeConnection(); }

    void connectToEngine();
    void checkIfReady();

    std::string getNextMove(std::string& position, int = 10);

    void closeConnection();

    bool isConnected() const { return connected; }
    bool isReady() const { return isready; }
    std::string getLastResult() const { return result; }

private:
    inline static constexpr int MAX_DATA_LENGTH = 1024;

    FILE* pipe = nullptr;
    std::string result;
    std::array<char, MAX_DATA_LENGTH> buffer;
    bool connected = false;
    bool isready = false;

    StockfishConnector() {};
    void fetchResult();
};
