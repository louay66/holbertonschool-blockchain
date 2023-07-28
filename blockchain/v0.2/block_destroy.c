#include "blockchain.h"
/**
 * block_destroy-free a block
 *@block: pointer of block to free
 * Return: void
 */
void block_destroy(block_t *block)
{
	free(block);
}
