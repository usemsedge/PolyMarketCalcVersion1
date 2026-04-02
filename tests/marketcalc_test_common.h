#pragma once

#include "marketcalc.h"
#include "chrono"

#include <vector>

using std::vector;

inline vector<vector<TileState>> garbageMap;
inline vector<vector<vector<TileState>>> garbageTempLayouts;
inline vector<int> garbageInts = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};



inline constexpr TileState T(int owner, int type) {
  return TileState{owner, type};
}

inline vector<vector<TileState>> makeNoSpecialMap() {
  return {
    {T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, CITY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, CITY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, CITY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
  };
}

inline vector<Coord> makeNoSpecialCityCenters() {
  return {
    Coord{1, 1},
    Coord{3, 4},
    Coord{4, 1},
  };
}


/*
 // Create a simple map and state for testing 
  // Contains a mix of everything
  // 3 cities, 2 buildings, 1 market
  // Market total 1

  |_________
  |  ∧   |
  | 0*  ■|
  | ⊞⊞   |
  | ⊞ ∧1 |
  | 2⊕   |
  | ⊞*   |
  |      |
  |      |
   Legend:
   ∧ = OBSTACLE
   0, 1, 2 = CITY
   ⊞ = USED_RESOURCE
   ■ = RESOURCE
   ⊕ = MARKET
   * = BUILDING
   (empty) = EMPTY
  */


inline const vector<vector<int>> realStateAllowedMarketPlacements = {
  {0, 1, 0, 1, 0, 0},
  {0, 0, 0, 1, 0, 0},
  {0, 0, 0, 1, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 1, 0, 0},
  {0, 0, 0, 1, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
};
inline const vector<vector<int>> realStateAllowedBuildingPlacements = {
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 1, 0},
  {0, 0, 0, 1, 1, 1},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
};

