#include <iostream>
#include <type_traits>
#include "GameContainers.h"
#include "GameBoard.h"
 

int main() {
    std::cout << "Hello World!" << std::endl;
    static_assert(!std::is_abstract<RPSPoint>(), "RPSPoint is abstract.");
    static_assert(!std::is_abstract<RPSMove>(), "RPSMove is abstract.");
    static_assert(!std::is_abstract<RPSJokerChange>(), "RPSJokerChange is abstract.");
    static_assert(!std::is_abstract<RPSFightInfo>(), "RPSFightInfo is abstract.");
    static_assert(!std::is_abstract<RPSPiecePosition>(), "RPSPiecePosition is abstract.");
    //static_assert(!std::is_abstract<GameBoard>(), "RPSBoard is abstract.");
    return 0;
}
