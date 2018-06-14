#include "GameBoard.h"


template<class T>
void GameBoard<T>::clear() {
    _arr.fill({ T(), 0 });    
}

template<class T>
int GameBoard<T>::getPlayer(const Point& pos) const {
    return _arr[getIndex(pos)].player;
}

template<class T>
bool GameBoard<T>::isValid(const Point& pos) const {
    const auto i = getIndex(pos);
    return i >= 0 && i < SIZE;
}

template<class T>
typename GameBoard<T>::Entry&
GameBoard<T>::operator[](const Point& pos) {
    return _arr[getIndex(pos)];
}

template<class T>
const typename GameBoard<T>::Entry&
GameBoard<T>::operator[](const Point& pos) const {
    return _arr[getIndex(pos)];
}

template<class T>
typename GameBoard<T>::Entry&
GameBoard<T>::operator[](const std::pair<int, int> pos) {
    return _arr[getIndex(pos)];
}

template<class T>
const typename GameBoard<T>::Entry&
GameBoard<T>::operator[](const std::pair<int, int> pos) const {
    return _arr[getIndex(pos)];
}

template<class T>
std::size_t GameBoard<T>::getIndex(const Point& pos) {
    return pos.getX() * M + pos.getY();
}