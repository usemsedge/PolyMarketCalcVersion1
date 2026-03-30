#include "marketcalc_test_common.h"

#include <gtest/gtest.h>


TEST(canPlaceBuilding, RealState) {
  BacktrackState state = makeRealState();

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