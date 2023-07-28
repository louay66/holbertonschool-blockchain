#include "blockchain.h"
#include <stdbool.h>

/**
 * block_mine-mine a new block
 * @block: points to the Block to check
 * Return: Void
 */

void block_mine(block_t *block)
{

	bool is_mined = true;
	uint64_t nonce = 0;

	while (is_mined)
	{
		block->info.nonce = nonce;
		block_hash(block, block->hash);
		if (hash_matches_difficulty(block->hash, block->info.difficulty))
		{
			is_mined = false;
		}
		nonce++;
	}
}
