#include "marketcalc_test_common.h"

#include <gtest/gtest.h>

TEST(computeOwnership, AllCitiesGrow) {
  vector<vector<TileState>> map = makeNoSpecialMap();
  vector<Coord> cityCenters = makeNoSpecialCityCenters();
  vector<int> actionOrder = {0, 1, 2, 0, 1, 2};

  vector<vector<TileState>> expectedResult = {
    {T(0, EMPTY), T(0, EMPTY), T(0, EMPTY), T(0, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(0, EMPTY), T(0, CITY), T(0, EMPTY), T(0, EMPTY), T(1, EMPTY), T(1, EMPTY)},
    {T(0, EMPTY), T(0, EMPTY), T(0, EMPTY), T(1, EMPTY), T(1, EMPTY), T(1, EMPTY)},
    {T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(1, EMPTY), T(1, CITY), T(1, EMPTY)},
    {T(2, EMPTY), T(2, CITY), T(2, EMPTY), T(1, EMPTY), T(1, EMPTY), T(1, EMPTY)},
    {T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(1, EMPTY), T(1, EMPTY), T(1, EMPTY)},
    {T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
  };

  computeOwnership(map, cityCenters, actionOrder);
  EXPECT_EQ(map, expectedResult);
}

TEST(computeOwnership, AllCitiesGrowAlternateOrder) {
  vector<vector<TileState>> map = makeNoSpecialMap();
  vector<Coord> cityCenters = makeNoSpecialCityCenters();
  vector<int> actionOrder = {0, 1, 2, 2, 1, 0};

  vector<vector<TileState>> expectedResult = {
    {T(0, EMPTY), T(0, EMPTY), T(0, EMPTY), T(0, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(0, EMPTY), T(0, CITY), T(0, EMPTY), T(1, EMPTY), T(1, EMPTY), T(1, EMPTY)},
    {T(0, EMPTY), T(0, EMPTY), T(0, EMPTY), T(1, EMPTY), T(1, EMPTY), T(1, EMPTY)},
    {T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(1, EMPTY), T(1, CITY), T(1, EMPTY)},
    {T(2, EMPTY), T(2, CITY), T(2, EMPTY), T(1, EMPTY), T(1, EMPTY), T(1, EMPTY)},
    {T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(1, EMPTY), T(1, EMPTY)},
    {T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
  };

  computeOwnership(map, cityCenters, actionOrder);
  EXPECT_EQ(map, expectedResult);
}

TEST(computeOwnership, NotAllCitiesGrow) {
  vector<vector<TileState>> map = makeNoSpecialMap();
  vector<Coord> cityCenters = makeNoSpecialCityCenters();
  vector<int> actionOrder = {0, 1, 2, 2, 1};

  vector<vector<TileState>> expectedResult = {
    {T(0, EMPTY), T(0, EMPTY), T(0, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(0, EMPTY), T(0, CITY), T(0, EMPTY), T(1, EMPTY), T(1, EMPTY), T(1, EMPTY)},
    {T(0, EMPTY), T(0, EMPTY), T(0, EMPTY), T(1, EMPTY), T(1, EMPTY), T(1, EMPTY)},
    {T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(1, EMPTY), T(1, CITY), T(1, EMPTY)},
    {T(2, EMPTY), T(2, CITY), T(2, EMPTY), T(1, EMPTY), T(1, EMPTY), T(1, EMPTY)},
    {T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(1, EMPTY), T(1, EMPTY)},
    {T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
  };

  computeOwnership(map, cityCenters, actionOrder);
  EXPECT_EQ(map, expectedResult);
}

TEST(computeOwnership, GrowBeforeCapture) {
  vector<vector<TileState>> map = makeNoSpecialMap();
  vector<Coord> cityCenters = makeNoSpecialCityCenters();
  vector<int> actionOrder = {2, 2, 1, 0};

  vector<vector<TileState>> expectedResult = {
    {T(0, EMPTY), T(0, EMPTY), T(0, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(0, EMPTY), T(0, CITY), T(0, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(1, EMPTY), T(1, EMPTY)},
    {T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(1, CITY), T(1, EMPTY)},
    {T(2, EMPTY), T(2, CITY), T(2, EMPTY), T(2, EMPTY), T(1, EMPTY), T(1, EMPTY)},
    {T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
  };

  computeOwnership(map, cityCenters, actionOrder);
  EXPECT_EQ(map, expectedResult);
}

TEST(computeOwnership, ObstaclesBlockCapture) {
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
  vector<Coord> cityCenters = makeNoSpecialCityCenters();
  vector<int> actionOrder = {2, 1, 0};

  vector<vector<TileState>> expectedResult = {
    {T(0, EMPTY), T(0, EMPTY), T(0, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(0, EMPTY), T(0, CITY), T(-1, OBSTACLE), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(0, EMPTY), T(-1, OBSTACLE), T(-1, OBSTACLE), T(1, EMPTY), T(1, EMPTY), T(1, EMPTY)},
    {T(2, EMPTY), T(-1, OBSTACLE), T(-1, OBSTACLE), T(1, EMPTY), T(1, CITY), T(1, EMPTY)},
    {T(2, EMPTY), T(2, CITY), T(-1, OBSTACLE), T(1, EMPTY), T(1, EMPTY), T(1, EMPTY)},
    {T(2, EMPTY), T(-1, OBSTACLE), T(-1, OBSTACLE), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
  };

  computeOwnership(map, cityCenters, actionOrder);
  EXPECT_EQ(map, expectedResult);
}

TEST(computeOwnership, FullScenario) {
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
  vector<Coord> cityCenters = makeNoSpecialCityCenters();
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

TEST(computeOwnership, UnclaimedVillage) {
  vector<vector<TileState>> map = {
    {T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, CITY), T(-1, EMPTY), T(-1, EMPTY), T(-1, CITY)},
    {T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
  };
  // Only one city is claimed, then it grows
  vector<Coord> cityCenters = {Coord{1, 0}};
  vector<int> actionOrder = {0, 0};

  vector<vector<TileState>> expectedResult = {
    {T(0, EMPTY), T(0, EMPTY), T(0, EMPTY), T(-1, EMPTY)},
    {T(0, CITY), T(0, EMPTY), T(0, EMPTY), T(-1, CITY)},
    {T(0, EMPTY), T(0, EMPTY), T(0, EMPTY), T(-1, EMPTY)},
  };
  computeOwnership(map, cityCenters, actionOrder);
  EXPECT_EQ(map, expectedResult);
}