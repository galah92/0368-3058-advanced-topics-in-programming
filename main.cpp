#include <iostream>
#include <type_traits>
#include "RPSContainers.h"
 

int main() {
    std::cout << "Hello World!" << std::endl;
    static_assert(!std::is_abstract<RPSPoint>(), "RPSPoint is abstract.");
    static_assert(!std::is_abstract<RPSMove>(), "RPSMove is abstract.");
    static_assert(!std::is_abstract<RPSJokerChange>(), "RPSJokerChange is abstract.");
    static_assert(!std::is_abstract<RPSFightInfo>(), "RPSFightInfo is abstract.");
    static_assert(!std::is_abstract<RPSPiecePosition>(), "RPSPiecePosition is abstract.");
    return 0;
}
