/**
 * Represents the spread of all the tiles that will be outputted to the final bitmap file
 *
 * Tiles are stored in a vector of vectors, to represent the grid layout of the Tiles.
 *
 * @author Sasha Ouellet - spaouellet@me.com
 * @version 1.0 - 02/05/17
 * @version 1.1 - 02/19/17 - Allowing construction via pre-developed vector array of Tiles
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
TileMap::TileMap(vector<Tile>& tileSet, unsigned int width, unsigned int height)
{
    m_tileSet = tileSet;
    m_width = width;
    m_height = height;
    m_generator = std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());
    m_distribution = std::uniform_int_distribution<int>(0, m_tileSet.size() - 1);
}

/**
 * Constructs the tile map from the already generated tile set, in its grid format
 * @param tiles
 */
TileMap::TileMap(vector<vector<Tile>> &tiles, unsigned int width, unsigned int height)
{
    m_tiles = tiles;
    m_tileSet = tiles[0];
    m_width = width;
    m_height = height;
}

/**
 * Get the pixel width of the entire TileMap
 * @return The pixel width of the map
 */
int TileMap::getPixelWidth()
{
    return m_tileSet[0].getImage().getWidth() * m_width;
}

/**
 * Get the pixel height of the entire TileMap
 * @return The pixel height of the map
 */
int TileMap::getPixelHeight()
{
    return m_tileSet[0].getImage().getHeight() * m_height;
}

/**
 * Runs through the generation process of the entire tile map, populating the vectors for each row of the grid
 * according to the specifications for Wang Tile tiling process
 *
 * Specifications: Begin with any first tile. Next, select a tile to be placed to the right. This tile's West side must
 * have a matching code to the East side of the tile to its left. Repeat for the remainder of the first row.
 *
 * The next row must follow the same specifications, in addition to the North side codes matching the South side codes
 * of their neighbor directly above.
 */
void TileMap::generate()
{
    // As of now, starting with index 2 is the only known tile that will successfully tile the whole thing
    for (int i = 0 ; i < m_height ; i++)
    {
        cout << "i: " << i << endl;
        vector<Tile> row;

        for (int j = 0 ; j < m_width ; j++)
        {
           cout << "\tj: " << j << endl;
            // Special case for first tile
            if (i == 0 && j == 0)
            {
                row.push_back(getRandom());
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
//                        tries++; // Fail safe to avoid endless cycle if location not tile-able
                    }
                }

                row.push_back(t);
            }
        }

        m_tiles.push_back(row);
    }
}

/**
 * Gets a random tile from the tile set
 * @return The randomly selected tile
 */
Tile TileMap::getRandom()
{
    return m_tileSet[m_distribution(m_generator)];
}

/**
 * Prints the tile map configuration
 */
void TileMap::print()
{
    for (int i = 0 ; i < m_height ; i++)
    {
        cout << "ROW " << i << ", size: " << m_tiles[i].size() << endl << "-----------------" << endl;

        for (int j = 0 ; j < m_width ; j++)
        {
            cout << "\t";
            m_tiles[i][j].print();
            cout << endl;
        }
    }
}

/**
 * Aggregates all the tiles in the 2-dimensional vector structure to put all the Tile pixel data into the main
 * pixel data array that will be written as the output file.
 *
 * @return The final output pixel data array of all the tile's pixel data combined
 */
unsigned char* TileMap::makeArray()
{
    int size = 3 * getPixelWidth() * getPixelHeight();
    unsigned char *data = new unsigned char[size];

    for (int i = 0 ; i < m_height ; i++)
    {
        for (int j = 0 ; j < m_width ; j++)
        {
            placeTile(m_tiles[i][j], j, i, data);
        }
    }

	return data;
}

/**
 * Given a specific tile and its location in the 2-dimensional vector, populates the given main data array with the
 * Tile's pixel data. The function determines the initial "offset" where writing of the data will begin, and from there
 * the Tile's location in x-space determines how far after the offset the pixel data starts.
 *
 * @param tile The tile to populate the main data array with
 * @param x The x location of the tile in the 2-dimensional vector
 * @param y The y location of the tile in the 2-dimensional vector
 * @param data The main data pixel array of all the tiles
 */
void TileMap::placeTile(Tile &tile, int x, int y, unsigned char *data)
{
    y = m_height - 1 - y;
    BMPFile& image = tile.getImage();
    const unsigned char *imagePixels = image.getPixels();
    int tileWidth = image.getWidth();
    int size = tileWidth * image.getHeight() * 3;
    int offset = y * m_width * size;

    for (int i = 0; i < size; i++)
    {
        int row = i / (tileWidth * 3);
        int col = i % (tileWidth * 3);
        data[offset + (tileWidth * 3 * (x + (m_width * row))) + col] = imagePixels[i];
    }
}