#include "map.h"

unsigned char get_tilemap_tile( map_t* map, int x, int y )
{
	if( x >= 0 && x < map->width && y >= 0 && y < map->height )
	{
		int index = map->width * y + x;
		return map->data[index];
	}
	return 0;
}
