#ifndef MARKETCALC_H_
#define MARKETCALC_H_

#include <iostream>
#include <vector>
#include <unordered_map>
#include <functional>
#include <utility>
#include <unordered_set>

using std::pair;
using std::vector;
using std::unordered_map;
using std::unordered_set;

#define MAX_BUILDING_LEVEL 8
#define MAX_MARKET_LEVEL 8

// Tile type constants
constexpr int EMPTY = 0;
constexpr int CITY = 1;
constexpr int OBSTACLE = 2;
constexpr int RESOURCE = 3;
constexpr int BUILDING = 4;
constexpr int MARKET = 5;
constexpr int USED_RESOURCE = 6;

struct TileState {
  int owner; // city ID that owns this tile, or -1 if unowned
  int type; // tile type (EMPTY, CITY, OBSTACLE, RESOURCE, BUILDING, MARKET)

  bool operator==(const TileState& other) const {
    return owner == other.owner && type == other.type;
  }

  bool operator!=(const TileState& other) const {
    return !(*this == other);
  }
};

struct Coord {
  int row;
  int col;

  bool operator==(const Coord& other) const {
    return row == other.row && col == other.col;
  }

  bool operator!=(const Coord& other) const {
    return !(*this == other);
  }
};



namespace std {
  template <>
  struct hash<Coord> {
    size_t operator()(const Coord& c) const {
      return hash<long long>()(((long long)c.row << 32) | (c.col & 0xFFFFFFFF));
    }
  };
}

// State structure
struct BacktrackState {
    // EVERYTHING MUST BE CONSISTENT WITH EACH OTHER

    // Ownership and tile map
    const vector<vector<TileState>>& map; 

    // cityCenters[i] corresponds to city ID i
    const vector<Coord>& cityCenters; 

    // tilesOwnedByCity[cityId] gives list of coordinates
    const unordered_map<int, vector<Coord>>& tilesOwnedByCity;

    // Requires: quick access to buildings/markets per city for at most 1 checks
    //           quick access to buildings/markets per tile for adjacency checks
    // Map from city ID to building/market, key not present if not placed
    // Set of all building/market coordinates for quick lookup
    unordered_map<int, Coord> curBuildingsInCity;
    unordered_set<Coord> curBuildingsSet; 
    unordered_map<int, Coord> curMarketsInCity;
    unordered_set<Coord> curMarketsSet;
};

// Result structure
struct BacktrackResult {
    int bestMarketTotal;
    vector<vector<TileState>> bestLayout;

    bool operator<(const BacktrackResult& other) const {
        return bestMarketTotal < other.bestMarketTotal;
    }
};


// Direction offsets for 8-neighbor adjacency
constexpr int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
constexpr int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};
constexpr int BASE_TILE_COUNT = 8;

// Direction offsets for extra 5x5 tiles
constexpr int dx5[] = {-2, -1, 0, 1, 2,
                      -2,            2,
                      -2,            2,
                      -2,            2,
                      -2, -1, 0, 1, 2};
constexpr int dy5[] = {-2, -2, -2, -2, -2,
                      -1,              -1,
                       0,               0,
                       1,               1,
                       2,  2,  2,  2,  2};
constexpr int EXPANDED_TILE_COUNT = 16;

/*
Compute ownership map based on city centers, capture order, and growth order.
Rules:
  - The map is a grid of tiles containing city centers (row, col).
      Cities are guarenteed to be at least 2 tiles away from each other both orthogonally and diagonally.
      This prevents cities from owning other city centers.
  - Two actions may be done: city capture and border growth.
  - Each city always owns its own tile. 
  - When captured, each city tries to claim each tile in the 3x3 area centered on itself,
    unless a tile is already claimed by another city (tilestate.owner != -1) or is an obstacle.
  - When border growthed, each city tries to claim each tile in the 5x5 area centered on itself,
    unless a tile is already claimed by another city (tilestate.owner != -1) or is an obstacle.
  - Consequently, if multiple cities are competing for ownership of the same tile,
    the first city which captures/grows into that tile will claim it.

NOTE: for simplicity, mountains are treated as obstacles.
      Forge markets cannot be calculated here, different logic.

Arguments:
map: 2D grid of TileStates
      tilestate.type can be EMPTY, CITY, OBSTACLE, RESOURCE, BUILDING, MARKET
      tilestate.owner should all be unowned (-1) at the start
cityCenters: list of (row, col) coordinates for city centers which are claimed
      cityCenters[i] corresponds to city ID i
actionOrder: order in which each city is captured and border growths  
      The first occurence of a city ID captures the city
      The second occurence of a city ID border growths
      All city IDs in cityCenters must appear in the map (but not necessarily the other way around).
      All city IDs in actionOrder must be in cityCenters.
      All city IDs in cityCenters must appear at least once in actionOrder, but at most twice.
    
Modifies:
Input map - marks each tile's owner field with the city ID that owns it, or -1 if unowned

*/
void computeOwnership(vector<vector<TileState>>& map, 
                      const vector<Coord>& cityCenters,
                      const vector<int>& actionOrder);

/*
Checks if a market is allowed to be placed on a tile, based off a BacktrackState
which contains ownership, current building placements and market placements

Rules:
- Market must be placed within city-owned tiles
- A city may only own up to 1 market
- Markets must be 8-direction adjacent to at least 1 building tile
    NOTE: We do not care about the case where we place on the last resource 
          used by another building, even though that is technically invalid
          in-game, as we will never want to do that in an optimal solution. 
          This simplifies our backtracking logic significantly.
- Can only be placed on empty or UNUSED resource tile
- Cannot be placed on a tile already occupied by another building or market

Arguments:
state: current backtracking state
coord: coordinates of the tile to check

Returns: 
true if we can place a market, false otherwise
*/
bool canPlaceMarket(const BacktrackState& state, Coord coord);

/*
Checks if a building is allowed to be placed on a tile, based off a BacktrackState
which contains ownership, current building placements and market placements

Rules:
- Building must be placed within city-owned tiles
- A city may only own up to 1 building
- Building must be 8-direction adjacent to at least 1 resource tile
    which does not have a building or market on it already
    NOTE: We do not care about the case where we place on the last resource 
          used by another building, even though that is technically invalid
          in-game, as we will never want to do that in an optimal solution. 
          This simplifies our backtracking logic significantly.
- Can only be placed on empty or UNUSED resource tile
- Cannot be placed on a tile already occupied by another building or market

Arguments:
state: current backtracking state
coord: coordinates of the tile to check

Returns: 
true if we can place a building, false otherwise
*/
bool canPlaceBuilding(const BacktrackState& state, Coord coord);

/*
Given an existing state, recursively place buildings and markets
in all possible configurations, then return the best one found.
Only places buildings and markets, does not modify border growths or captures.
NOTE: "Best" is typically defined as highest market total
  However, changing the comparison operator in BacktrackState allows us
  to break ties or have a different criteria

Try placing a building at cityIdx (or no place), then recurse to cityIdx + 1
  till it hits the last city.
Then try placing a market (or no place) at cityIdx, then recurse to cityIdx + 1
  till it hits the last city.
At the end, calculate market total and update best layout if needed.

Args:
state: current backtracking state
      which should have consistent ownership, building placements, and market placements
      and has a built in comparison operator
cityIdx: which city we are currently trying to place a building/market for, corresponds to city ID
placingBuilding: true if placing building, false if placing market

Returns: backtrackResult with the best market total found and the corresponding layout
*/
BacktrackResult backtrackPlacements(BacktrackState& state, int cityIdx, bool placingBuilding);

/*
Given a state, calculate total market level.
Rules:
- Building level is determined by adjacency to UNCOVERED resource tiles, up to MAX_BUILDING_LEVEL
- Market level is determined by sum of adjaicent building levels (up to MAX_MARKET_LEVEL)
- Total market level is the sum of all individual market levels.
*/
int calculateMarketTotal(const BacktrackState& state);


/*
Given a map and a capture/growth order, find the best market layout.
Throw errors if invalid input is given.
  (e.g. cities too close, too many buildings/markets placed, invalid city IDs, 
  cityCenters does not match with map, etc.)

Args:
map: 2D grid of ints representing tile types (EMPTY, CITY, OBSTACLE, RESOURCE)
cityCenters: list of (row, col) coordinates for city centers which are claimed
      cityCenters[i] corresponds to city ID i
vector<int> actionOrder: order in which each city is captured and border growths 
      (SAME DEF AS COMPUTEOWNERSHIP)
      The first occurence of a city ID captures the city
      The second occurence of a city ID border growths
      All city IDs in cityCenters must appear in the map (but not necessarily the other way around).
      All city IDs in actionOrder must be in cityCenters.
      All city IDs in cityCenters must appear at least once in actionOrder, but at most twice.

Return:
BacktrackResult containing the best market total found and the corresponding layout
*/
BacktrackResult findBestMarketLayout(vector<vector<int>>& map, 
                                    const vector<Coord>& cityCenters,
                                    const vector<int>& actionOrder);


#endif // MARKETCALC_H_