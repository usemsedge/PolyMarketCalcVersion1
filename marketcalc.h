#include "pair_hash.h"
#ifndef MARKETCALC_H_
#define MARKETCALC_H_

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


struct MarketResult {
    int totalMarketLevel;
    vector<vector<int>> layout;              // output layout
};

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
 Calculate the best market spots and corresponding building spots
 given a tile map and a border growth order

 Best is defined as:
  1. Maximize total market level across all cities
      - Markets and buildings may be placed on empty or resource tiles owned by the city
      - Each city can place at most 1 building and 1 market
      - Market level is determined by the sum of adjacent building levels
      - Building level is determined by the number of adjacent resource tiles (capped at 8)
      - Market level is capped at 8 per market

Arguments:
map: 2D grid of tile types (EMPTY, CITY, etc.)
growthOrder: order in which cities grow their borders (list of city IDs)
      Can be missing some city IDs, as some cities may not grow
      Cannot have city IDs that are not in cityCenters

Returns:
a MarketResult struct containing 
  - total market level of the best result
  - layout of the best result (2D grid where BUILDING and MARKET tiles are placed)

*/
vector<vector<int>> bestMarketLayoutGivenBorderGrowthOrder(const vector<vector<int>>& map, 
                                        const vector<int>& growthOrder);


// State structure
struct BacktrackState {
    // The map will not be modified
    // but we will use it for building and market checks
    const vector<vector<int>>& map; 

    // cityCenters[i] corresponds to city ID i
    const vector<pair<int,int>>& cityCenters; 

    // tilesOwnedByCity[cityId] gives list of coordinates
    const unordered_map<int, vector<pair<int,int>>>& tilesOwnedByCity;

    // facts about the map
    int height;
    int width;
    int numCities;

    // These get sets for fast lookup and modification
    vector<pair<int,int>> curBuildings; // current building placements for each city
    unordered_set<pair<int, int>> curBuildingsSet;
    vector<pair<int,int>> curMarkets; // current market placements for each city
    unordered_set<pair<int, int>> curMarketsSet;

    int marketTotal; // Current total market level

    vector<vector<int>> bestLayout; // Best layout found so far
    int bestMarketTotal; // Best total market level found so far
};


/*
Checks if a building is allowed to be placed on a tile
IGNORES 1 PER CITY LIMIT

Rules:
- Building must be adjacent to at least 1 uncovered resource tile
- Can only be placed on empty or resource tile
- Cannot be placed on a tile already occupied by another building or market

Arguments:
state: current backtracking state
x, y: coordinates of the tile to check

Returns:
true if we can place a building, false otherwise
*/
bool canPlaceBuilding(const BacktrackState& state, int x, int y);


/*
Checks if a market is allowed to be placed on a tile
IGNORES 1 PER CITY LIMIT

Rules:
- Market must be adjacent to at least 1 building
- Can only be placed on empty or resource tile
- Cannot be placed on a tile already occupied by another building or market

Arguments:
state: current backtracking state
x, y: coordinates of the tile to check

Returns: 
true if we can place a market, false otherwise
*/
bool canPlaceMarket(const BacktrackState& state, int x, int y);


/*
Calculate the total market level for a given backtracking state
Rules: 
- Market level = the sum of adjacent building levels (capped at MAX_MARKET_LEVEL)
- Building level = number of adjacent uncovered resource tiles (capped at MAX_BUILDING_LEVEL)

Arguments:
state: current backtracking state with building and market placements

Returns:
total market level based on current placements
*/
int calculateMarketTotal(const BacktrackState& state);






#endif // MARKETCALC_H_