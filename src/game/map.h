
#ifndef __MAP_H__
#define __MAP_H__

typedef struct
{
	unsigned char* data;
	int width;
	int height;
} map_t;

unsigned char get_tilemap_tile( map_t* map, int x, int y );
unsigned char* get_tilemap_data_ptr( map_t* map, int x, int y );

#endif
