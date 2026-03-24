#include "pair_hash.h"
#include "marketcalc.h"

#include <gtest/gtest.h>

#include <iostream>

using std::vector;
using std::pair;
/*
// Tile type constants
constexpr int EMPTY = 0;
constexpr int CITY = 1;
constexpr int OBSTACLE = 2;
constexpr int RESOURCE = 3;
constexpr int BUILDING = 4;
constexpr int MARKET = 5;
*/

// Returns a tilestate with given owner and type
constexpr TileState T(int owner, int type) {
    return TileState{owner, type};
}

TEST(MarketCalcTest, computeOwnership_AllCitiesGrow) {

  // 6x8 grid
  vector<vector<TileState>> map = {
    {T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, CITY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, CITY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, CITY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
  };
  vector<Coord> cityCenters = {
    Coord{1, 1},
    Coord{3, 4},
    Coord{4, 1},
  };
  // Claim all three cities, then grow all three cities
  vector<int> actionOrder = {0, 1, 2, 0, 1, 2};

  vector<vector<TileState>> expectedResult = {
    {T(0, EMPTY), T(0, EMPTY), T(0, EMPTY), T(0, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(0, EMPTY), T(0, CITY), T(0, EMPTY), T(0, EMPTY), T(1, EMPTY), T(1, EMPTY)},
    {T(0, EMPTY), T(0, EMPTY), T(0, EMPTY), T(1, EMPTY), T(1, EMPTY), T(1, EMPTY)},
    {T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(1, EMPTY), T(1, CITY), T(1, EMPTY)},
    {T(2, EMPTY), T(2, CITY), T(2, EMPTY), T(1, EMPTY), T(1, EMPTY), T(1, EMPTY)},
    {T(2, EMPTY), T(2,EMPTY), T(2,EMPTY), T(1,EMPTY), T(1,EMPTY), T(1,EMPTY)},
    {T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
  };
  computeOwnership(map, cityCenters, actionOrder);
  EXPECT_EQ(map, expectedResult);
}

TEST(MarketCalcTest, computeOwnership_AllCitiesGrowAlternateOrder) {

  // 6x8 grid
  vector<vector<TileState>> map = {
    {T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, CITY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, CITY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, CITY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
  };
  vector<Coord> cityCenters = {
    Coord{1, 1},
    Coord{3, 4},
    Coord{4, 1},
  };
  // Claim all three cities, then grow all three cities
  vector<int> actionOrder = {0, 1, 2, 2, 1, 0};

  vector<vector<TileState>> expectedResult = {
    {T(0, EMPTY), T(0, EMPTY), T(0, EMPTY), T(0, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(0, EMPTY), T(0, CITY), T(0, EMPTY), T(1, EMPTY), T(1, EMPTY), T(1, EMPTY)},
    {T(0, EMPTY), T(0, EMPTY), T(0, EMPTY), T(1, EMPTY), T(1, EMPTY), T(1, EMPTY)},
    {T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(1, EMPTY), T(1, CITY), T(1, EMPTY)},
    {T(2, EMPTY), T(2, CITY), T(2, EMPTY), T(1, EMPTY), T(1, EMPTY), T(1, EMPTY)},
    {T(2, EMPTY), T(2,EMPTY), T(2,EMPTY), T(2,EMPTY), T(1,EMPTY), T(1,EMPTY)},
    {T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
  };
  computeOwnership(map, cityCenters, actionOrder);
  EXPECT_EQ(map, expectedResult);
}

TEST(MarketCalcTest, computeOwnership_NotAllCitiesGrow) {

  // 6x8 grid
  vector<vector<TileState>> map = {
    {T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, CITY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, CITY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, CITY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
  };
  vector<Coord> cityCenters = {
    Coord{1, 1},
    Coord{3, 4},
    Coord{4, 1},
  };
  // Claim all three cities, then grow only cities 2 and 1
  vector<int> actionOrder = {0, 1, 2, 2, 1};

  vector<vector<TileState>> expectedResult = { // KEY DIFF: THIS TILE IS NOT CLAIMED
    {T(0, EMPTY), T(0, EMPTY), T(0, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(0, EMPTY), T(0, CITY), T(0, EMPTY), T(1, EMPTY), T(1, EMPTY), T(1, EMPTY)},
    {T(0, EMPTY), T(0, EMPTY), T(0, EMPTY), T(1, EMPTY), T(1, EMPTY), T(1, EMPTY)},
    {T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(1, EMPTY), T(1, CITY), T(1, EMPTY)},
    {T(2, EMPTY), T(2, CITY), T(2, EMPTY), T(1, EMPTY), T(1, EMPTY), T(1, EMPTY)},
    {T(2, EMPTY), T(2,EMPTY), T(2,EMPTY), T(2,EMPTY), T(1,EMPTY), T(1,EMPTY)},
    {T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
  };
  computeOwnership(map, cityCenters, actionOrder);
  EXPECT_EQ(map, expectedResult);
}

TEST(MarketCalcTest, computeOwnership_GrowBeforeCapture) {

  // 6x8 grid
  vector<vector<TileState>> map = {
    {T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, CITY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, CITY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, CITY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
  };
  vector<Coord> cityCenters = {
    Coord{1, 1},
    Coord{3, 4},
    Coord{4, 1},
  };

  // Claim and grow city 2 first, then claim 1 and 0
  vector<int> actionOrder = {2, 2, 1, 0};

  vector<vector<TileState>> expectedResult = { 
    {T(0, EMPTY), T(0, EMPTY), T(0, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(0, EMPTY), T(0, CITY), T(0, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(1, EMPTY), T(1, EMPTY)},
    {T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(1, CITY), T(1, EMPTY)},
    {T(2, EMPTY), T(2, CITY), T(2, EMPTY), T(2, EMPTY), T(1, EMPTY), T(1, EMPTY)},
    {T(2, EMPTY), T(2,EMPTY), T(2,EMPTY), T(2,EMPTY), T(-1,EMPTY), T(-1, EMPTY)},
    {T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
  };
  computeOwnership(map, cityCenters, actionOrder);
  EXPECT_EQ(map, expectedResult);
}

TEST(MarketCalcTest, computeOwnership_ObstaclesBlockCapture) {

  // 6x8 grid
  // Cols 1 and 2 contain obstacles
  vector<vector<TileState>> map = {
    {T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, CITY), T(-1, OBSTACLE), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, OBSTACLE), T(-1, OBSTACLE), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, OBSTACLE), T(-1, OBSTACLE), T(-1, EMPTY), T(-1, CITY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, CITY), T(-1, OBSTACLE), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, OBSTACLE), T(-1, OBSTACLE), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
  };
  vector<Coord> cityCenters = {
    Coord{1, 1},
    Coord{3, 4},
    Coord{4, 1},
  };

  // Claim all three cities, don't grow
  // Check that obstacles are not owned
  vector<int> actionOrder = {2, 1, 0};

  vector<vector<TileState>> expectedResult = { 
    {T(0, EMPTY), T(0, EMPTY),     T(0, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(0, EMPTY), T(0, CITY),      T(-1, OBSTACLE), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(0, EMPTY), T(-1, OBSTACLE), T(-1, OBSTACLE), T(1, EMPTY), T(1, EMPTY), T(1, EMPTY)},
    {T(2, EMPTY), T(-1, OBSTACLE), T(-1, OBSTACLE), T(1, EMPTY), T(1, CITY), T(1, EMPTY)},
    {T(2, EMPTY), T(2, CITY),      T(-1, OBSTACLE), T(1, EMPTY), T(1, EMPTY), T(1, EMPTY)},
    {T(2, EMPTY), T(-1, OBSTACLE), T(-1, OBSTACLE), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
  };
  computeOwnership(map, cityCenters, actionOrder);
  EXPECT_EQ(map, expectedResult);
}

TEST(MarketCalcTest, computeOwnership_FullScenario) {

  // 6x8 grid
  // A mix of all tiles
  vector<vector<TileState>> map = {
    {T(-1, EMPTY), T(-1, OBSTACLE), T(-1, RESOURCE), T(-1, EMPTY), T(-1, OBSTACLE), T(-1, RESOURCE)},
    {T(-1, RESOURCE), T(-1, CITY), T(-1, EMPTY), T(-1, OBSTACLE), T(-1, RESOURCE), T(-1, EMPTY)},
    {T(-1, OBSTACLE), T(-1, RESOURCE), T(-1, EMPTY), T(-1, RESOURCE), T(-1, EMPTY), T(-1, OBSTACLE)},
    {T(-1, EMPTY), T(-1, OBSTACLE), T(-1, RESOURCE), T(-1, EMPTY), T(-1, CITY), T(-1, OBSTACLE)},
    {T(-1, RESOURCE), T(-1, CITY), T(-1, OBSTACLE), T(-1, EMPTY), T(-1, RESOURCE), T(-1, EMPTY)},
    {T(-1, OBSTACLE), T(-1, EMPTY), T(-1, RESOURCE), T(-1, OBSTACLE), T(-1, EMPTY), T(-1, RESOURCE)},
    {T(-1, EMPTY), T(-1, RESOURCE), T(-1, OBSTACLE), T(-1, RESOURCE), T(-1, OBSTACLE), T(-1, EMPTY)},
    {T(-1, RESOURCE), T(-1, OBSTACLE), T(-1, EMPTY), T(-1, OBSTACLE), T(-1, RESOURCE), T(-1, OBSTACLE)},
  };
  vector<Coord> cityCenters = {
    Coord{1, 1},
    Coord{3, 4},
    Coord{4, 1},
  };

  // Claim all three cities, then grow 1
  // Check that obstacles are not owned
  vector<int> actionOrder = {2, 1, 0, 1};

  vector<vector<TileState>> expectedResult = { 
    {T(0, EMPTY), T(-1, OBSTACLE), T(0, RESOURCE), T(-1, EMPTY), T(-1, OBSTACLE), T(-1, RESOURCE)},
    {T(0, RESOURCE), T(0, CITY), T(0, EMPTY), T(-1, OBSTACLE), T(1, RESOURCE), T(1, EMPTY)},
    {T(-1, OBSTACLE), T(0, RESOURCE), T(0, EMPTY), T(1, RESOURCE), T(1, EMPTY), T(-1, OBSTACLE)},
    {T(2, EMPTY), T(-1, OBSTACLE), T(2, RESOURCE), T(1, EMPTY), T(1, CITY), T(-1, OBSTACLE)},
    {T(2, RESOURCE), T(2, CITY), T(-1, OBSTACLE), T(1, EMPTY), T(1, RESOURCE), T(1, EMPTY)},
    {T(-1, OBSTACLE), T(2, EMPTY), T(2, RESOURCE), T(-1, OBSTACLE), T(1, EMPTY), T(1, RESOURCE)},
    {T(-1, EMPTY), T(-1, RESOURCE), T(-1, OBSTACLE), T(-1, RESOURCE), T(-1, OBSTACLE), T(-1, EMPTY)},
    {T(-1, RESOURCE), T(-1, OBSTACLE), T(-1, EMPTY), T(-1, OBSTACLE), T(-1, RESOURCE), T(-1, OBSTACLE)},
  };
  computeOwnership(map, cityCenters, actionOrder);
  EXPECT_EQ(map, expectedResult);
}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
