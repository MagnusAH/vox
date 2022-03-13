#pragma once

#include <stdint.h>



struct VoxBlock
{
	uint16_t id;
	uint16_t ext;
	/*
	16 bits block id => 65536 block types
	2 bits opaqueness => 0 = opaque, 1 = transparent, 2 = dual, 3 =
	2 bits block_type => 0 = standard, 1 = liquid, 2 = dynamic, 3 =
	4 bits custom data => varies by block type
	4 bits protection level => 0 - 15
	3 bits rotation => +/- x, +/- y, +/- z
	1 bit bigdata =>  (look for additional block data in table)
	*/
	// OR
	/* (think im going with this one)
	16 bits block id => 65536 block types
	2 bits render type => 0 = full (cube), 1 = partial (other shape), 2 = frame (doesn't have actual texture, but instead), 3 = partial frame
	2 bits physics type => 0 = static, 1 = liquid, 2 = dynamic
	4 bits custom data => varies by block type
	3 bits rotation => +/- x, +/- y, +/- z
	1 bit bigdata => look for additional block data in table
	4 bits reserved
	*/
};

// vertex type A (8 bytes, supports coords sized at 1/32 of a block) ( for close chunks )
/*
	16 bits texture id => 65536 textures (256 MiB at 32x32, 4k atlas, 16x bound)
	3 bits normal data => +/- x, +/- y, +/- z
	13 bits reserved
	32 bits position (y = position >> 20, z = (position >> 10) & 0x3ff, x = position & 0x3ff), (position = x | (z << 10) | (y << 20))
*/

// vertex type B (4 bytes, supports coords full size of block, axis aligned normals) ( for far chunks )
/*
	16 bits texture id => 65536 textures (256 MiB at 32x32, 4k atlas, 16x bound)
	{
		4 bits normal data => cxyz (c = data >> 3, x = (data >> 2) & 1, y = (data >> 1) & 1, z = data & 1), (vec3 normal = vec3(2 * c * (x - 0.5), 2 * x * (y - 0.5), 2 * y * (z - 0.5)) )
		or
		3 bits normal, lookup table, 0 -> +z, 1 -> -z, 2 -> +x, 3 -> -x, 4 -> +y, 5 -> -y, 6 -> absolute void of darkness (this seems like the better option)
	}
	12 bits position (y = position >> 8, z = (position >> 4) & 0xf, x = position & 0xf), (position = x | (z << 4) | (y << 8))
*/


/*
Minimum Specs -> OpenGL 4.5
			  -> VRAM 512 MiB
			  -> RAM 4 GiB

Recommended Specs -> OpenGL 4.5
				  -> VRAM 4 GiB
				  -> RAM 16 GiB
*/

/*
block type has full + opaque data for each side
1 bit full flag + 1 bit opaque flag  x6 = 12 bits
*/

/*
big data list, contains additional data about block for gameplay (server side, mainly used for functional block) -> single hashmap

little data list, contains additional data about block for rendering (client side, mainly used for decorative blocks) -> hashmap for every column, ~3127 for 32 render distance
*/

// old prototype
/*
#define VOX_BLOCK_FLAG_DATA 15		// custom data based on block type
#define VOX_BLOCK_FLAG_FULL 16		// block has full size faces on each side
#define VOX_BLOCK_FLAG_OPAQUE 32	// block is not transparent
#define VOX_BLOCK_FLAG_LUMINOUS	64	// block emmits light
#define VOX_BLOCK_FLAG_XROT 128		// x direction (+x, -x)
#define VOX_BLOCK_FLAG_YROT 256		// y direction (+y, -y)
#define VOX_BLOCK_FLAG_ZROT 512		// z direction (+z ,-z)
#define VOX_BLOCK_FLAG_TYPE 3072	// 0 = standard, 1 = liquid, 2 = dynamic, 3 = reserved
// #define VOX_BLOCK_FLAG_STATIC 1024	// isn't affected by gravity
// #define VOX_BLOCK_FLAG_LIQUID 2048	// block is a liquid
// #define VOX_BLOCK_FLAG_ 4096
// #define VOX_BLOCK_FLAG_ 8192
// #define VOX_BLOCK_FLAG_ 16384
#define VOX_BLOCK_FLAG_EXTRA 32768	// block has extra data

struct VoxBlock
{
	uint16_t id; 	// 65536 block types
	uint16_t flags; // 4 bits for misc data
};
*/