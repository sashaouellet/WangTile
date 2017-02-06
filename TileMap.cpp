/**
 * Represents the spread of all the tiles that will be outputted to the final bitmap file
 *
 * Tiles are stored in a vector of vectors, to represent the grid layout of the Tiles.
 *
 * @author Sasha Ouellet - spaouellet@me.com
 * @version 1.0 - 02/05/17
 */

#include <cstdlib>
#include <random>
#include <chrono>
#include "TileMap.h"

/**
 * Default constructor for the TileMap
 *
 * @param width The width, in number of tiles
 * @param height The height, in number of tiles
 */
TileMap::TileMap(vector<Tile> tileSet, unsigned int width, unsigned int height)
{
    m_tileSet = tileSet;
    m_width = width;
    m_height = height;

    m_tiles.resize(height);

    m_generator = std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());
    m_distribution = std::uniform_int_distribution<int>(0, m_tileSet.size() - 1);
}

/**
 * Runs through the generation process of the entire tile map, populating the vectors for each row of the grid
 * according to the specifications for Wang Tile tiling process
 */
void TileMap::generate()
{
    // As of now, starting with index 2 is the only known tile that will successfully tile the whole thing
    for (int i = 0 ; i < m_height ; i++)
    {
        cout << "i: " << i << endl;
        vector<Tile> row = m_tiles[i];

        for (int j = 0 ; j < m_width ; j++)
        {
           cout << "\tj: " << j << endl;
            // Special case for first tile
            if (i == 0 && j == 0)
            {
                Tile t = getRandom();
                row.push_back(t);
            }
            // Special case for first row, don't need to check for row above
            else if (i == 0)
            {
                Tile t = getRandom();

                // Pick while the codes for the E and W sides don't match
                while (!t.hasCodeAtSide(row[j - 1].getCodeAtSide(Tile::EAST), Tile::WEST))
                {
                    t = getRandom();
                    cout << "[EAST / WEST]   Need: " << row[j - 1].getCodeAtSide(Tile::EAST) << ", Have: " << t.getCodeAtSide(Tile::WEST) << endl;
                }

                row.push_back(t);
            }
            // Normal case, need to check row above for code matching as well
            else
            {
                Tile t = getRandom();

                // Pick while the codes for the E and W sides don't match, AND while the codes for N and S don't match

                // Special case for first of row, don't need to check E/W
                if (j == 0)
                {
                    while (!t.hasCodeAtSide(m_tiles[i - 1][j].getCodeAtSide(Tile::SOUTH), Tile::NORTH))
                    {
                        t = getRandom();
                        cout << "[NORTH / SOUTH] Need: " << m_tiles[i - 1][j].getCodeAtSide(Tile::SOUTH) << ", Have: " << t.getCodeAtSide(Tile::NORTH) << endl;
                    }
                }
                // Normal case
                else
                {
                    int tries = 0;
                    while ((!t.hasCodeAtSide(row[j - 1].getCodeAtSide(Tile::EAST), Tile::WEST) || !t.hasCodeAtSide(m_tiles[i - 1][j].getCodeAtSide(Tile::SOUTH), Tile::NORTH)) && tries < 10)
                    {
                        t = getRandom();
                        cout << "[NORTH / SOUTH] Need: " << m_tiles[i - 1][j].getCodeAtSide(Tile::SOUTH) << ", Have: " << t.getCodeAtSide(Tile::NORTH) << endl;
                        cout << "[EAST / WEST]   Need: " << row[j - 1].getCodeAtSide(Tile::EAST) << ", Have: " << t.getCodeAtSide(Tile::WEST) << endl;
//                        tries++;
                    }
                }

                row.push_back(t);
            }
        }
        m_tiles[i] = row;
    }
}

/**
 * Gets a random tile from the tile set
 * @return The randomly selected tile
 */
Tile TileMap::getRandom()
{
    int r = m_distribution(m_generator);

//    return m_tileSet[rand() % m_tileSet.size()];
    return m_tileSet[r];
}

/**
 * Prints the tile map configuration
 */
void TileMap::print()
{
    for (int i = 0 ; i < m_height ; i++)
    {
        cout << "ROW " << i << endl << "-----------------" << endl;

        for (int j = 0 ; j < m_width ; j++)
        {
            cout << "\t";
            m_tiles[i][j].print();
            cout << endl;
        }
    }
}