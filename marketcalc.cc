#include "marketcalc.h"


#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <climits>
#include <cmath>
#include <functional>
#include <utility>
#include <unordered_set>


using std::vector;
using std::pair;
using std::unordered_map;
using std::unordered_set;



/*
Helper function
Checks if a coordinate is within the bounds of the map
*/
bool inBounds(const vector<vector<TileState>>& map, Coord coord) {
    int height = map.size();
    int width = map[0].size();
    return coord.row >= 0 && coord.row < height && coord.col >= 0 && coord.col < width;
}

/*
Defined in marketcalc.h
*/
void computeOwnership(vector<vector<TileState>>& map, 
                      const vector<Coord>& cityCenters,
                      const vector<int>& actionOrder) {
    // cityId -> whether captured
    unordered_map<int, int> timesCitySeen;

    // Simulate border growths in order to get proper ownership map
    for (int orderIdx = 0; orderIdx < (int)actionOrder.size(); orderIdx++) {
        int cityId = actionOrder[orderIdx];
        int cx = cityCenters[cityId].col;
        int cy = cityCenters[cityId].row;
        
        // City has not been captured yet
        // Capture it and compute 3x3
        if (timesCitySeen[cityId] == 0) {
            timesCitySeen[cityId] = 1;

          // Assign city tile itself
          map[cy][cx].owner = cityId;
          
          // Assign 3x3 ownership
          for (int i = 0; i < BASE_TILE_COUNT; i++) {
              int nx = cx + dx[i];
              int ny = cy + dy[i];
              Coord coord = {ny, nx};
              if (inBounds(map, coord) && map[ny][nx].type != OBSTACLE) {
                  // Only assign if unowned
                  // Do not overwrite previous border growths
                  if (map[ny][nx].owner == -1) {
                      map[ny][nx].owner = cityId;
                  }
              }
          }
        }

        else if (timesCitySeen[cityId] == 1) {
          timesCitySeen[cityId] = 2;
          // Assign 5x5 ownership
          for (int i = 0; i < EXPANDED_TILE_COUNT; i++) {
              int nx = cx + dx5[i];
              int ny = cy + dy5[i];
              Coord coord = {ny, nx};
              if (inBounds(map, coord) && map[ny][nx].type != OBSTACLE) {
                  // Only assign if unowned
                  // Do not overwrite previous border growths
                  if (map[ny][nx].owner == -1) {
                      map[ny][nx].owner = cityId;
                  }
              }
          }
        }
        
        else {
          throw std::invalid_argument("City ID " + std::to_string(cityId) + " appears more than twice in actionOrder");
        }
    }

}

/*
Defined in marketcalc.h
*/
bool canPlaceMarket(const BacktrackState& state, Coord coord) {
  int row = coord.row;
  int col = coord.col;
  
  // Can only place within city borders
  if (state.map[row][col].owner == -1) {
    return false;
  }

  // Can only place on empty or resource tile
  if (state.map[row][col].type != EMPTY && state.map[row][col].type != RESOURCE) {
    return false;
  }

  // Market exists in city already
  if (state.curMarketsInCity.find(state.map[row][col].owner) != state.curMarketsInCity.end()) {
    return false;
  }

  // Already occupied by something we just placed
  // SANITY CHECK, DONT NEED since we already checked the map
  if (state.curBuildingsSet.find(coord) != state.curBuildingsSet.end() || 
      state.curMarketsSet.find(coord) != state.curMarketsSet.end()) {
    return false;
  }

  // Check adjacency to at least one building tile
  for (int i = 0; i < BASE_TILE_COUNT; i++) {
    int ny = row + dx[i];
    int nx = col + dy[i];
    Coord adjacentCoord = {ny, nx};
    if (inBounds(state.map, adjacentCoord) &&
       state.curBuildingsSet.find(adjacentCoord) != state.curBuildingsSet.end()) {
      return true;
    }
  }
  // fix?
  return false;
}

/*
Defined in marketcalc.h
*/
bool canPlaceBuilding(const BacktrackState& state, Coord coord) {
  int row = coord.row;
  int col = coord.col;

  // Can only place within city borders
  if (state.map[row][col].owner == -1) {
    // std::cout << "Cannot place building at (" << row << ", " << col << ") because it is not within city borders." << std::endl;
    return false;
  }

  // Can only place on empty or resource tile
  if (state.map[row][col].type != EMPTY && state.map[row][col].type != RESOURCE) {
    // std::cout << "Cannot place building at (" << row << ", " << col << ") because it is not an empty or resource tile." << std::endl;
    return false;
  }

  // Building exists in city already
  if (state.curBuildingsInCity.find(state.map[row][col].owner) != state.curBuildingsInCity.end()) {
    // std::cout << state.curBuildingsInCity.at(state.map[row][col].owner).row << ", " << state.curBuildingsInCity.at(state.map[row][col].owner).col << std::endl;
    // std::cout << "Cannot place building at (" << row << ", " << col << ") because a building already exists in this city." << std::endl;
    return false;
  }

  // Already occupied by something we just placed
  // SANITY CHECK, DONT NEED since we already checked the map
  if (state.curBuildingsSet.find(coord) != state.curBuildingsSet.end() || 
      state.curMarketsSet.find(coord) != state.curMarketsSet.end()) {
    // std::cout << "Cannot place building at (" << row << ", " << col << ") because it is already occupied." << std::endl;
    return false;
  }

  // Check adjacency to at least one resource tile
  // which is not covered
  for (int i = 0; i < BASE_TILE_COUNT; i++) {
    int nx = col + dx[i];
    int ny = row + dy[i];
    Coord adjacentCoord = {ny, nx};
    if (inBounds(state.map, adjacentCoord) &&
       (state.map[ny][nx].type == RESOURCE || 
       state.map[ny][nx].type == USED_RESOURCE) && 
       state.curBuildingsSet.find(adjacentCoord) == state.curBuildingsSet.end() &&
       state.curMarketsSet.find(adjacentCoord) == state.curMarketsSet.end()) {

      return true;
    }
  }

  // std::cout << "Cannot place building at (" << row << ", " << col << ") because it is not adjacent to any uncovered resource tiles." << std::endl;
  // fix?
  return false;
}

/*
Defined in marketcalc.h
*/
BacktrackResult backtrackPlacements(BacktrackState& state, int cityIdx, bool placingBuilding) {
  // Base case (step 3): cityIdx is at the end and placingBuilding is false, calculate market total

  if (!placingBuilding && cityIdx == (int)state.cityCenters.size()) {
    vector<vector<TileState>> finalMap = state.map;
    for (const auto& building : state.curBuildingsSet) {
      finalMap[building.row][building.col].type = BUILDING;
    }
    for (const auto& market : state.curMarketsSet) {
      finalMap[market.row][market.col].type = MARKET;
    }
    std::cout << "Calculated market total: " << calculateMarketTotal(state) << std::endl;
    return BacktrackResult {
      calculateMarketTotal(state),
      finalMap,
    };
  }

  // Not base case: we may have bestResults passed to us from recursion
  BacktrackResult bestResult;

  // Recursive case 2 (step 2): PLACE A MARKET 
  // (placingBuilding is false, OR placingBuilding is true AND cityIdx is at the end)
  if (!placingBuilding || cityIdx == (int)state.cityCenters.size()) {
    // we just finished placing all the buildings so we hit the end
    // now we transition to placing markets
    if (placingBuilding && cityIdx == (int)state.cityCenters.size()) {
      cityIdx = 0;
      placingBuilding = false;
    }
    // Guarenteed choice: we may decide not to place a market
    bestResult = backtrackPlacements(state, cityIdx + 1, placingBuilding);
    
    // If no markets exist in this city, we may try placing markets in all valid tiles
    const auto& placeableTiles = state.tilesOwnedByCity.at(cityIdx);
    for (const auto& tile : placeableTiles) {
        if (canPlaceMarket(state, tile)) {
            // Place market
            state.curMarketsInCity[cityIdx] = tile;
            state.curMarketsSet.insert(tile);

            // Recurse to next city and placing market
            BacktrackResult result = backtrackPlacements(state, cityIdx + 1, placingBuilding);
            if (result.bestMarketTotal > bestResult.bestMarketTotal) {
              bestResult = result;
            }

            // Backtrack
            state.curMarketsInCity.erase(cityIdx);
            state.curMarketsSet.erase(tile);
        }
    }
  }

  // Recursive case 1 (step 1): place a building (placingBuilding is true and cityIdx is in bounds)
  else if (placingBuilding && cityIdx >= 0 && cityIdx < (int)state.cityCenters.size()) {
    const auto& placeableTiles = state.tilesOwnedByCity.at(cityIdx);


    // Guarenteed choice: we may decide not to place a building
    bestResult = backtrackPlacements(state, cityIdx + 1, placingBuilding);

    // Attempt to place a building in each tile
    // Update all nearby building and market levels
    // Backtrack from there
    for (const auto& tile : placeableTiles) {
        if (canPlaceBuilding(state, tile)) {
            // Place building
            state.curBuildingsInCity[cityIdx] = tile;
            state.curBuildingsSet.insert(tile);

            // Recurse to next city and placing market
            BacktrackResult result = backtrackPlacements(state, cityIdx + 1, placingBuilding);
            if (result.bestMarketTotal > bestResult.bestMarketTotal) {
                bestResult = result;
            }

            // Backtrack
            state.curBuildingsInCity.erase(cityIdx);
            state.curBuildingsSet.erase(tile);
        }
    }
  }

  else {
    throw std::invalid_argument("Invalid state in backtrackPlacements: placingBuilding is " + std::to_string(placingBuilding) + " and cityIdx is " + std::to_string(cityIdx));
  }

  std::cout << "Best market total for cityIdx " << cityIdx << " and placingBuilding " << placingBuilding << ": " << bestResult.bestMarketTotal << std::endl;
  if (cityIdx == 1 && placingBuilding && bestResult.bestMarketTotal == 16) {
    std::cout << "BREAKPOINT" << std::endl;
  }
  return bestResult;
}

/*
Defined in marketcalc.h
*/
int calculateMarketTotal(const BacktrackState& state) {
  // Calculate all building levels
  unordered_map<Coord, int> buildingLevels;
  for (const auto& buildingCoord : state.curBuildingsSet) {
    // Go 8 directionally and add up all the uncovered resource tiles
    int buildingLevel = 0;
    for (int i = 0; i < BASE_TILE_COUNT; i++) {
      int ny = buildingCoord.row + dx[i];
      int nx = buildingCoord.col + dy[i];
      Coord adjacentCoord = {ny, nx};
      if (inBounds(state.map, adjacentCoord) &&
          (state.map[ny][nx].type == RESOURCE || state.map[ny][nx].type == USED_RESOURCE) &&
          state.curBuildingsSet.find(adjacentCoord) == state.curBuildingsSet.end() &&
          state.curMarketsSet.find(adjacentCoord) == state.curMarketsSet.end()) {
        buildingLevel++;
      }
    }
    buildingLevels[buildingCoord] = std::min(buildingLevel, MAX_BUILDING_LEVEL);
  }

  int totalMarketLevel = 0;

  // Next, calculate all market levels
  for (const auto& marketCoord : state.curMarketsSet) {
    int marketLevel = 0;
    for (int i = 0; i < BASE_TILE_COUNT; i++) {
      int nx = marketCoord.row + dx[i];
      int ny = marketCoord.col + dy[i];
      Coord adjacentCoord = {nx, ny};
      if (inBounds(state.map, adjacentCoord) &&
          state.curBuildingsSet.find(adjacentCoord) != state.curBuildingsSet.end()) {
        marketLevel += buildingLevels[adjacentCoord];
      }
    }
    marketLevel = std::min(marketLevel, MAX_MARKET_LEVEL);
    totalMarketLevel += marketLevel;
  }

  return totalMarketLevel;
}

/*
Defined in marketcalc.h
*/
BacktrackResult findBestMarketLayout(vector<vector<int>>& map, 
                                    const vector<Coord>& cityCenters,
                                    const vector<int>& actionOrder) {
  // Create tileState map from raw tile map
  vector<vector<TileState>> tileMap(map.size(), vector<TileState>(map[0].size()));
  for (int i = 0; i < (int)map.size(); i++) {
    for (int j = 0; j < (int)map[0].size(); j++) {
      tileMap[i][j] = TileState{-1, map[i][j]};
    } 
  }

  // Get ownership
  computeOwnership(tileMap, cityCenters, actionOrder);

  return BacktrackResult{
    0,
    tileMap,
  };
}

/*
WASM TEST
*/
// Your real function using vectors
int sum(std::vector<int> vec) {
    int total = 0;
    for (int val : vec) {
        total += val;
    }
    return total;
}

// WASM-facing wrapper
extern "C" {
    int sum_wasm(int* data, int size) {
        std::vector<int> vec(data, data + size);  // construct vector from pointer
        return sum(vec);
    }
}