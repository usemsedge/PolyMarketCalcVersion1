#pragma once

#include "marketcalc.h"
#include "chrono"

#include <vector>

using std::vector;

inline vector<vector<TileState>> garbageMap;
inline vector<int> garbageInts;


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
inline BacktrackState makeRealState() {
  vector<vector<TileState>> map;
  vector<Coord> cityCenters;
  unordered_map<int, vector<Coord>> tilesOwnedByCity;
  vector<Coord> curBuildingsInCity;
  vector<Coord> curMarketsInCity;
  vector<vector<TileState>> bestLayoutReturn;
  vector<vector<TileState>> bestLayoutCurrent;
  vector<int> buildingLevelsCurrent;

  map = {
    {T(0, EMPTY), T(0, EMPTY), T(0, OBSTACLE), T(0, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(0, EMPTY), T(0, CITY), T(0, BUILDING), T(0, EMPTY), T(1, EMPTY), T(1, RESOURCE)},
    {T(0, EMPTY), T(0, USED_RESOURCE), T(0, USED_RESOURCE), T(1, EMPTY), T(1, EMPTY), T(1, EMPTY)},
    {T(2, EMPTY), T(2, USED_RESOURCE), T(2, EMPTY), T(1, OBSTACLE), T(1, CITY), T(1, EMPTY)},
    {T(2, EMPTY), T(2, CITY), T(2, MARKET), T(1, EMPTY), T(1, EMPTY), T(1, EMPTY)},
    {T(2, EMPTY), T(2, USED_RESOURCE), T(2, BUILDING), T(1, EMPTY), T(1, EMPTY), T(1, EMPTY)},
    {T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
  };
  cityCenters = {
      Coord{1, 1},
      Coord{3, 4},
      Coord{4, 1},
  };

  tilesOwnedByCity.clear();
  // 11 tiles
  tilesOwnedByCity[0] = {
    Coord{0, 0}, Coord{0, 1}, Coord{0, 2}, Coord{0, 3}, 
    Coord{1, 0}, Coord{1, 1}, Coord{1, 2}, Coord{1, 3}, 
    Coord{2, 0}, Coord{2, 1}, Coord{2, 2}
  };

  // 14 tiles
  tilesOwnedByCity[1] = {
                Coord{1, 4}, Coord{1, 5}, 
    Coord{2, 3}, Coord{2, 4}, Coord{2, 5}, 
    Coord{3, 3}, Coord{3, 4}, Coord{3, 5}, 
    Coord{4, 3}, Coord{4, 4}, Coord{4, 5},
    Coord{5, 3}, Coord{5, 4}, Coord{5, 5}
  };

  // 13 tiles
  tilesOwnedByCity[2] = {
    Coord{3, 0}, Coord{3, 1}, Coord{3, 2},
    Coord{4, 0}, Coord{4, 1}, Coord{4, 2},
    Coord{5, 0}, Coord{5, 1}, Coord{5, 2},
    Coord{6, 0}, Coord{6, 1}, Coord{6, 2}, Coord{6, 3},
  };

  // 2 buildings placed — per city ID
  curBuildingsInCity = {
      Coord{1, 2},
      Coord{-1, -1},
      Coord{5, 2},
  };

  // 1 market placed
  curMarketsInCity = {
      Coord{-1, -1},
      Coord{-1, -1},
      Coord{4, 2},
  };

  return BacktrackState{
      map,
      cityCenters,
      tilesOwnedByCity,
      curBuildingsInCity,
      curMarketsInCity,
      bestLayoutReturn,
      bestLayoutCurrent,
      buildingLevelsCurrent,
  };
}

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

