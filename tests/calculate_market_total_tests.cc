#include "marketcalc_test_common.h"

#include <gtest/gtest.h>


TEST(calculateMarketTotal, RealState) {
  

  vector<vector<TileState>> map = {
    {T(0, EMPTY), T(0, EMPTY), T(0, OBSTACLE), T(0, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(0, EMPTY), T(0, CITY), T(0, BUILDING), T(0, EMPTY), T(1, EMPTY), T(1, RESOURCE)},
    {T(0, EMPTY), T(0, USED_RESOURCE), T(0, USED_RESOURCE), T(1, EMPTY), T(1, EMPTY), T(1, EMPTY)},
    {T(2, EMPTY), T(2, USED_RESOURCE), T(2, EMPTY), T(1, OBSTACLE), T(1, CITY), T(1, EMPTY)},
    {T(2, EMPTY), T(2, CITY), T(2, MARKET), T(1, EMPTY), T(1, EMPTY), T(1, EMPTY)},
    {T(2, EMPTY), T(2, USED_RESOURCE), T(2, BUILDING), T(1, EMPTY), T(1, EMPTY), T(1, EMPTY)},
    {T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
  };

  vector<Coord> cityCenters = {
    Coord{1, 1},
    Coord{3, 4},
    Coord{4, 1},
  };

  vector<vector<Coord>> tilesOwnedByCity;
  // 11 tiles
  tilesOwnedByCity.push_back({
    Coord{0, 0}, Coord{0, 1}, Coord{0, 2}, Coord{0, 3}, 
    Coord{1, 0}, Coord{1, 1}, Coord{1, 2}, Coord{1, 3}, 
    Coord{2, 0}, Coord{2, 1}, Coord{2, 2}
  });

  // 14 tiles
  tilesOwnedByCity.push_back({
                Coord{1, 4}, Coord{1, 5}, 
    Coord{2, 3}, Coord{2, 4}, Coord{2, 5}, 
    Coord{3, 3}, Coord{3, 4}, Coord{3, 5}, 
    Coord{4, 3}, Coord{4, 4}, Coord{4, 5},
    Coord{5, 3}, Coord{5, 4}, Coord{5, 5}
  });

  // 13 tiles
  tilesOwnedByCity.push_back({
    Coord{3, 0}, Coord{3, 1}, Coord{3, 2},
    Coord{4, 0}, Coord{4, 1}, Coord{4, 2},
    Coord{5, 0}, Coord{5, 1}, Coord{5, 2},
    Coord{6, 0}, Coord{6, 1}, Coord{6, 2}, Coord{6, 3},
  });

  vector<Coord> curBuildingsInCity = {
    Coord{1, 2},
    Coord{-1, -1},
    Coord{5, 2},
  };

  vector<Coord> curMarketsInCity = {
    Coord{-1, -1},
    Coord{-1, -1},
    Coord{4, 2},
  };

  BacktrackState state = {
    map,
    cityCenters,
    tilesOwnedByCity,
    curBuildingsInCity,
    curMarketsInCity,
    garbageMap, garbageTempLayouts, garbageInts
  };



  int expected = 1;
  int actual = calculateMarketTotal(state);

  EXPECT_EQ(expected, actual);
}
//
TEST(calculateMarketTotal, 2CityBestPlacementInitially) {
  /*
  OFFFFO
  OCWWCO
  OFMMFO
  sexy double level 8 market
  preplaced by user = map is set
  */
  vector<vector<TileState>> map = {
    {T(0, EMPTY), T(0, RESOURCE), T(0, RESOURCE), T(1, RESOURCE), T(1, RESOURCE), T(1, EMPTY)},
    {T(0, EMPTY), T(0, CITY), T(0, BUILDING), T(1, BUILDING), T(1, CITY), T(1, EMPTY)},
    {T(0, EMPTY), T(0, RESOURCE), T(0, MARKET), T(1, MARKET), T(1, RESOURCE), T(1, EMPTY)},
  };
  vector<Coord> cityCenters = {
    Coord{1, 1},
    Coord{1, 4},
  };

  // Create tilesOwnedByCity map
  vector<vector<Coord>> tilesOwnedByCity;
  tilesOwnedByCity.push_back({
    Coord{0, 0}, Coord{0, 1}, Coord{0, 2},
    Coord{1, 0}, Coord{1, 1}, Coord{1, 2},
    Coord{2, 0}, Coord{2, 1}, Coord{2, 2},
  });
  tilesOwnedByCity.push_back({
    Coord{0, 3}, Coord{0, 4}, Coord{0, 5},
    Coord{1, 3}, Coord{1, 4}, Coord{1, 5},
    Coord{2, 3}, Coord{2, 4}, Coord{2, 5},
  });

  vector<Coord> curBuildingsInCity = {
    Coord{1, 2},
    Coord{1, 3},
  };
  vector<Coord> curMarketsInCity = {
    Coord{2, 2},
    Coord{2, 3},
  };

  int expected = 16;
  int actual = calculateMarketTotal(BacktrackState{
    map,
    cityCenters,
    tilesOwnedByCity,

    curBuildingsInCity,
    curMarketsInCity,
    garbageMap, garbageTempLayouts, garbageInts
  });
  EXPECT_EQ(expected, actual);
}

TEST(calculateMarketTotal, 2CityGoodPlacementFromBacktracking) {
  /*
  OOFFFO
  FC^WCO
  OWMFMO
  decent level 5 + level 4 market
  however, the initial map was empty of preplaced buildings and markets
    the buildings and markets were added during backtracking
  */
  vector<vector<TileState>> map = {
    {T(0, EMPTY), T(0, EMPTY), T(0, RESOURCE), T(1, RESOURCE), T(1, RESOURCE), T(1, EMPTY)},
    {T(0, RESOURCE), T(0, CITY), T(0, OBSTACLE), T(1, EMPTY), T(1, CITY), T(1, EMPTY)},
    {T(0, EMPTY), T(0, RESOURCE), T(0, EMPTY), T(1, RESOURCE), T(1, EMPTY), T(1, EMPTY)},
  };
  vector<Coord> cityCenters = {
    Coord{1, 1},
    Coord{1, 4},
  };
  vector<vector<Coord>> tilesOwnedByCity;
  tilesOwnedByCity.push_back({
    Coord{0, 0}, Coord{0, 1}, Coord{0, 2},
    Coord{1, 0}, Coord{1, 1}, Coord{1, 2},
    Coord{2, 0}, Coord{2, 1}, Coord{2, 2},
  });
  tilesOwnedByCity.push_back({
    Coord{0, 3}, Coord{0, 4}, Coord{0, 5},
    Coord{1, 3}, Coord{1, 4}, Coord{1, 5},
    Coord{2, 3}, Coord{2, 4}, Coord{2, 5},
  });

  vector<Coord> curBuildingsInCity = {
    Coord{2, 1},
    Coord{1, 3},
  };
  vector<Coord> curMarketsInCity = {
    Coord{2, 2},
    Coord{2, 4},
  };

  BacktrackState state = {
    map,
    cityCenters,
    tilesOwnedByCity,

    curBuildingsInCity,
    curMarketsInCity,
    garbageMap, garbageTempLayouts, garbageInts
  };

  int expected = 9;
  int actual = calculateMarketTotal(state);
  EXPECT_EQ(expected, actual);
}

TEST(calculateMarketTotal, CapMarketLevel) {
  /*
  OFMFFO
  OCWWCO
  OFFFFO
  Singular lvl 10 market gets capped at MAX_MARKET_LEVEL (typically 8)
  */
  vector<vector<TileState>> map = {
    {T(0, EMPTY), T(0, RESOURCE), T(0, RESOURCE), T(1, RESOURCE), T(1, RESOURCE), T(1, EMPTY)},
    {T(0, EMPTY), T(0, CITY), T(0, RESOURCE), T(1, RESOURCE), T(1, CITY), T(0, EMPTY)},
    {T(0, EMPTY), T(0, RESOURCE), T(0, RESOURCE), T(1, RESOURCE), T(1, RESOURCE), T(1, EMPTY)},
  };
  vector<Coord> cityCenters = {
    Coord{1, 1},
    Coord{1, 4},
  };
  vector<vector<Coord>> tilesOwnedByCity;
  tilesOwnedByCity.push_back({
    Coord{0, 0}, Coord{0, 1}, Coord{0, 2},
    Coord{1, 0}, Coord{1, 1}, Coord{1, 2},
    Coord{2, 0}, Coord{2, 1}, Coord{2, 2},
  });
  tilesOwnedByCity.push_back({
    Coord{0, 3}, Coord{0, 4}, Coord{0, 5},
    Coord{1, 3}, Coord{1, 4}, Coord{1, 5},
    Coord{2, 3}, Coord{2, 4}, Coord{2, 5},
  });
  vector<Coord> curBuildingsInCity = {
    Coord{1, 1},
    Coord{1, 3},
  };
  vector<Coord> curMarketsInCity = {
    Coord{0, 2},
  };
  BacktrackState state = {
    map,
    cityCenters,
    tilesOwnedByCity,

    curBuildingsInCity,
    curMarketsInCity,
    garbageMap, garbageTempLayouts, garbageInts
  };

  int expected = std::min(10, MAX_MARKET_LEVEL);
  int actual = calculateMarketTotal(state);
  EXPECT_EQ(expected, actual);
}

TEST(calculateMarketTotal, Case1) {
  static vector<vector<TileState>> map;
  static vector<Coord> cityCenters;
  static vector<vector<Coord>> tilesOwnedByCity;

  map = {
    {T(0, EMPTY), T(0, OBSTACLE), T(0, EMPTY)},
    {T(0, EMPTY), T(0, CITY), T(0, USED_RESOURCE)},
    {T(0, EMPTY), T(0, MARKET), T(0, BUILDING)},
  };

  cityCenters = {
    Coord{1, 1},
  };

  tilesOwnedByCity.push_back({
    Coord{0, 0}, Coord{0, 1}, Coord{0, 2},
    Coord{1, 0}, Coord{1, 1}, Coord{1, 2},
    Coord{2, 0}, Coord{2, 1}, Coord{2, 2},
  });

  vector<Coord> curBuildingsInCity = {
    Coord{2, 2},
  };
  vector<Coord> curMarketsInCity = {
    Coord{2, 1},
  };

  vector<int> buildingLevelsCurrent = {0};

  BacktrackState state = BacktrackState{
    map,
    cityCenters,
    tilesOwnedByCity,

    curBuildingsInCity,
    curMarketsInCity,
    garbageMap,
    garbageTempLayouts,
    buildingLevelsCurrent,
  };

  int expected = 1;
  int actual = calculateMarketTotal(state);
  EXPECT_EQ(expected, actual);
}