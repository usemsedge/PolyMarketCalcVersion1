#include "marketcalc_test_common.h"

#include <gtest/gtest.h>

/*
More complex tests for backtrack testing
Basically testing everything



2 city best layouts with/without BG

3 city best layouts with/without BG



*/

TEST(full, TwoCitiesMaxMarkets) {
  vector<vector<int>> map = {
    {EMPTY, RESOURCE, RESOURCE, RESOURCE, RESOURCE, EMPTY},
    {EMPTY, CITY, RESOURCE, RESOURCE, CITY, EMPTY},
    {EMPTY, RESOURCE, RESOURCE, RESOURCE, RESOURCE, EMPTY},
  };
  vector<Coord> cityCenters = {Coord{1, 1}, Coord{1, 4}};
  vector<int> actionOrder = {0, 1};

  BacktrackResult result = findBestMarketLayout(map, cityCenters, actionOrder);
  EXPECT_EQ(result.bestMarketTotal, 16);
  // Expect 2 bulidings between the cities
  EXPECT_EQ(result.bestLayout[1][2].type, BUILDING);
  EXPECT_EQ(result.bestLayout[1][3].type, BUILDING);

  // Exactly one of each of these should be a market, as only 1 building per city
  EXPECT_TRUE((result.bestLayout[0][2].type == MARKET) != (result.bestLayout[2][2].type == MARKET));
  EXPECT_TRUE((result.bestLayout[0][3].type == MARKET) != (result.bestLayout[2][3].type == MARKET));
}