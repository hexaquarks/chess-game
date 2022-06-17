#include "../../include/Utilities/StockfishConnector.hpp"


StockfishConnector& StockfishConnector::get()
{
    static StockfishConnector instance;
    return instance;
}


void StockfishConnector::fetchResult()
{
    char* data = buffer.data();
    if (fgets(data, MAX_DATA_LENGTH, pipe))
        result = data;
}


void StockfishConnector::connectToEngine()
{
    std::cout << "Opening reading pipe" << std::endl;
    pipe = popen("stockfish", "r+");
    if (!pipe)
    {
        std::cerr << "Couldn't start stockfish" << std::endl;
        return;
    }

    fetchResult();
    connected = true;
    checkIfReady();
}


void StockfishConnector::checkIfReady()
{
    if (!connected)
    {
        std::cerr << "Cannot call checkIfReady since stockfish is not connected" << std::endl;
        isready = false;
        return;
    }

    std::string command = "isready\n";
    fwrite(command.c_str(), sizeof(char), command.length(), pipe);
    fetchResult();
    isready = (result == "readyok\n");
}


std::string StockfishConnector::getNextMove(std::string& position, int depth)
{
    if (!isready)
    {
        std::cerr << "Cannot call getNextMove since stockfish is not ready" << std::endl;
        return "error";
    }

    std::string command = "position startpos moves " + position + "\ngo depth " + std::to_string(depth) + "\n";
    fwrite(command.c_str(), sizeof(char), command.length(), pipe);

    int bestmoveIndex;
    do
    {
        fetchResult();
        bestmoveIndex = result.find("bestmove");
    } while (bestmoveIndex == -1);

    return result.substr(bestmoveIndex + 9, 4);
}


void StockfishConnector::closeConnection()
{
    if (!connected) return;
    connected = pclose(pipe);
    isready = connected;
}
