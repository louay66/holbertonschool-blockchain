#include "blockchain.h"
/**
 * block_destroy-free a block
 *@block: pointer of block to free
 * Return: void
 */
void block_destroy(block_t *block)
{
	llist_destroy(block->transactions, 1, (node_dtor_t)transaction_destroy);
	free(block);
}
