#include "marketcalc_test_common.h"

#include <gtest/gtest.h>


TEST(canPlaceBuilding, RealState) {
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

  // Loop through every possible building placement and make sure it matches expected results
  vector<vector<int>> expected = realStateAllowedBuildingPlacements;
  vector<vector<int>> actual(expected.size(), vector<int>(expected[0].size(), 0));
  for (int row = 0; row < (int)expected.size(); row++) {
    for (int col = 0; col < (int)expected[0].size(); col++) {
      Coord coord{row, col};
      actual[row][col] = (int) canPlaceBuilding(state, coord);
    }
  }

  EXPECT_EQ(expected, actual);
}