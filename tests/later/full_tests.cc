#include "marketcalc_test_common.h"

#include <gtest/gtest.h>

/*
More complex tests for backtrack testing
Basically testing everything



2 city best layouts with/without BG

3 city best layouts with/without BG



*/

void prettyPrint(const vector<vector<TileState>>& map) {
  for (const auto& row : map) {
    for (const auto& tile : row) {
      char c;
      switch (tile.type) {
        case EMPTY: c = '.'; break;
        case CITY: c = 'C'; break;
        case OBSTACLE: c = 'X'; break;
        case RESOURCE: c = 'R'; break;
        case BUILDING: c = 'B'; break;
        case MARKET: c = 'M'; break;
        case USED_RESOURCE: c = 'R'; break;
        default: c = '?';
      }
      std::cout << c << " ";
    }
    std::cout << std::endl;
  }
}


TEST(full, TwoCitiesMaxMarkets) {
  vector<vector<int>> map = {
    {EMPTY, RESOURCE, RESOURCE, RESOURCE, RESOURCE, EMPTY},
    {EMPTY, CITY, RESOURCE, RESOURCE, CITY, EMPTY},
    {EMPTY, RESOURCE, RESOURCE, RESOURCE, RESOURCE, EMPTY},
  };
  vector<Coord> cityCenters = {Coord{1, 1}, Coord{1, 4}};
  vector<int> actionOrder = {0, 1};

  auto start = std::chrono::high_resolution_clock::now();
  BacktrackResult result = findBestMarketLayout(map, cityCenters, actionOrder);
  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "Time taken (2 cities): " << duration.count() << " ms" << std::endl;
  EXPECT_EQ(result.bestMarketTotal, 16);
  // Expect 2 bulidings between the cities
  EXPECT_EQ(result.bestLayout[1][2].type, BUILDING);
  EXPECT_EQ(result.bestLayout[1][3].type, BUILDING);

  // Exactly one of each of these should be a market, as only 1 building per city
  EXPECT_TRUE((result.bestLayout[0][2].type == MARKET) != (result.bestLayout[2][2].type == MARKET));
  EXPECT_TRUE((result.bestLayout[0][3].type == MARKET) != (result.bestLayout[2][3].type == MARKET));
}

TEST(full, Scenario1_BeatsHuman) {
  // See image examples/Scenario1.png for reference
  vector<vector<int>> map = {
    {RESOURCE, OBSTACLE, RESOURCE, RESOURCE, RESOURCE, RESOURCE, RESOURCE},
    {RESOURCE, CITY, RESOURCE, EMPTY, RESOURCE, CITY, OBSTACLE},
    {RESOURCE, EMPTY, RESOURCE, EMPTY, RESOURCE, OBSTACLE, OBSTACLE},
    {EMPTY, OBSTACLE, EMPTY, RESOURCE, EMPTY, OBSTACLE, OBSTACLE},
    {OBSTACLE, RESOURCE, CITY, RESOURCE, RESOURCE, RESOURCE, RESOURCE},
    {EMPTY, RESOURCE, RESOURCE, RESOURCE, RESOURCE, CITY, EMPTY},
    {OBSTACLE, EMPTY, EMPTY, OBSTACLE, RESOURCE, RESOURCE, EMPTY}
  };
  vector<Coord> cityCenters = {Coord{1, 1}, Coord{1, 5}, Coord{4, 2} , Coord{5, 5}};
  // Only one city was border growthed
  vector<int> actionOrder = {0, 1, 2, 3, 1};

  // I could find a layout with 8 + 8 + 6 + 4. Can it do better?
  auto start = std::chrono::high_resolution_clock::now();
  BacktrackResult result = findBestMarketLayout(map, cityCenters, actionOrder);
  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  EXPECT_GT(result.bestMarketTotal, 26);
  std::cout << "Best market total: " << result.bestMarketTotal << std::endl;
  std::cout << "Time taken (4 cities): " << duration.count() << " ms" << std::endl;
  prettyPrint(result.bestLayout);
}