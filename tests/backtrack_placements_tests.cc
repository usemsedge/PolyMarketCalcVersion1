#include "marketcalc_test_common.h"

#include <gtest/gtest.h>


void prettyPrintOwners(const vector<vector<TileState>>& map) {
  for (const auto& row : map) {
    for (const auto& tile : row) {
      std::cout << tile.owner << " ";
    }
    std::cout << std::endl;
  }
}



TEST(backtrackPlacements, Tiny1Building1Market_BaseCase_DoNothing) {
  /*
  Creates a very simple map with 1 city 1 buildings 1 market
    |____
    | ∧ |
    | 0⊞|
    | ⊕*|
  Expected result (do nothing)
    |____
    | ∧ |
    | 0⊞|
    | ⊕*| 
  */
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
  vector<vector<TileState>> bestLayoutReturn = map;
  vector<vector<vector<TileState>>> tempLayouts = {map, map};

  BacktrackState state = BacktrackState{
    map,
    cityCenters,
    tilesOwnedByCity,

    curBuildingsInCity,
    curMarketsInCity,
    bestLayoutReturn,
    tempLayouts,
    buildingLevelsCurrent,
  };
  vector<vector<TileState>> expectedMap = state.map;

  // Directly testing the base case
  int result = backtrackPlacements(state, 1, false);


  EXPECT_EQ(expectedMap, state.bestLayoutReturn);
  EXPECT_EQ(1, result);
}

TEST(backtrackPlacements, Tiny1Building0Market_1Deep_PlaceMarket) {
  /*
  Creates a very simple map with 1 city 1 buildings 0 markets
    |____
    | ∧ |
    | 0⊞|
    |  *|
  Expected result (only one possible, market level 1):  
    |____
    | ∧ |
    | 0⊞|
    | ⊕*|
  */
  static vector<vector<TileState>> map;
  static vector<Coord> cityCenters;
  static vector<vector<Coord>> tilesOwnedByCity;

  map = {
    {T(0, EMPTY), T(0, OBSTACLE), T(0, EMPTY)},
    {T(0, EMPTY), T(0, CITY), T(0, USED_RESOURCE)},
    {T(0, EMPTY), T(0, EMPTY), T(0, BUILDING)},
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
    Coord{-1, -1},
  };

  vector<int> buildingLevelsCurrent = {0};
  vector<vector<TileState>> bestLayoutReturn = map;
  vector<vector<vector<TileState>>> tempLayouts = {map, map};

  BacktrackState state = BacktrackState{
    map,
    cityCenters,
    tilesOwnedByCity,

    curBuildingsInCity,
    curMarketsInCity,
    bestLayoutReturn,
    tempLayouts,
    buildingLevelsCurrent,
  };

  vector<vector<TileState>> expectedMap = state.map;
  expectedMap[2][1] = T(0, MARKET); // Should place market here

  //Testing the case where all the building recursion is done
  int result = backtrackPlacements(state, 0, false);


  EXPECT_EQ(expectedMap, state.bestLayoutReturn);
  EXPECT_EQ(1, result);
}

TEST(backtrackPlacements, Tiny0Building0Market_2Deep_PlaceMarket) {
  /*
  Creates a very simple map with 1 city 0 buildings 0 markets
    |____
    | ∧ |
    | 0⊞|
    |  ■|
  Expected result (one of two possible, market level 2):  
    |____
    | ∧ |
    | 0⊞|
    |⊕*■| 
  */

  static vector<vector<TileState>> map;
  static vector<Coord> cityCenters;
  static vector<vector<Coord>> tilesOwnedByCity;

  map = {
    {T(0, EMPTY), T(0, OBSTACLE), T(0, EMPTY)},
    {T(0, EMPTY), T(0, CITY), T(0, USED_RESOURCE)},
    {T(0, EMPTY), T(0, EMPTY), T(0, RESOURCE)},
  };

  cityCenters = {
    Coord{1, 1},
  };

  tilesOwnedByCity.clear();
  tilesOwnedByCity.push_back({
    Coord{0, 0}, Coord{0, 1}, Coord{0, 2},
    Coord{1, 0}, Coord{1, 1}, Coord{1, 2},
    Coord{2, 0}, Coord{2, 1}, Coord{2, 2},
  });

  vector<Coord> curBuildingsInCity = {
    Coord{-1, -1},
  };

  vector<Coord> curMarketsInCity = {
    Coord{-1, -1},
  };

  vector<vector<TileState>> bestLayoutReturn = map;
  vector<vector<vector<TileState>>> tempLayouts = {map, map};

  vector<int> buildingLevelsCurrent = {0};

  BacktrackState state = BacktrackState{
    map,
    cityCenters,
    tilesOwnedByCity,

    curBuildingsInCity,
    curMarketsInCity,
    bestLayoutReturn,
    tempLayouts,
    buildingLevelsCurrent,
  };
  vector<vector<TileState>> expectedMap = state.map;
  expectedMap[2][1] = T(0, BUILDING); // Should place building here in both scenarios

  vector<vector<TileState>> expectedMap1 = expectedMap, expectedMap2 = expectedMap;
  expectedMap1[2][0] = T(0, MARKET); // Choice 1: place market here
  expectedMap2[1][0] = T(0, MARKET); // Choice 2: place market here



  // Testing full recursion through both cases
  int result = backtrackPlacements(state, 0, true);


  EXPECT_EQ(2, result);
  EXPECT_TRUE(expectedMap1 == state.bestLayoutReturn || expectedMap2 == state.bestLayoutReturn);
}


TEST(backtrackPlacements, 2CitiesMaxMarkets) {
  vector<vector<TileState>> map = {
    {T(0, EMPTY), T(0, RESOURCE), T(0, RESOURCE), T(1, RESOURCE), T(1, RESOURCE), T(1, EMPTY)},
    {T(0, EMPTY), T(0, CITY), T(0, RESOURCE), T(1, RESOURCE), T(1, CITY), T(1, EMPTY)},
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

  vector<Coord> curBuildingsInCity(2, Coord{-1, -1});
  vector<Coord> curMarketsInCity(2, Coord{-1, -1});
  vector<vector<TileState>> bestLayoutReturn = map;
  vector<vector<vector<TileState>>> tempLayouts(4, map);
  vector<int> buildingLevelsCurrent = {0, 0};

  BacktrackState state{
    map,
    cityCenters,
    tilesOwnedByCity,

    curBuildingsInCity,
    curMarketsInCity,
    bestLayoutReturn,
    tempLayouts,
    buildingLevelsCurrent,
  };

  int result = backtrackPlacements(state, 0, true);


  EXPECT_EQ(16, result);

  vector<vector<TileState>> res = state.bestLayoutReturn;

  // Expect 2 bulidings between the cities
  EXPECT_EQ(BUILDING, res[1][2].type);
  EXPECT_EQ(BUILDING, res[1][3].type);

  // Expect 2 markets, each city's market has 2 possible spots
  EXPECT_TRUE((res[0][2].type == MARKET) != (res[2][2].type == MARKET));
  EXPECT_TRUE((res[0][3].type == MARKET) != (res[2][3].type == MARKET));

}

