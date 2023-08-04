#include "blockchain.h"
#include <stdbool.h>

/**
 * blockchain_difficulty- compute the difficulty of next block
 * @blockchain: points to the blockchain
 * Return: difficulty of next block
 */
uint32_t blockchain_difficulty(blockchain_t const *blockchain)
{
	block_t *last_block, *adjusted_block;
	uint64_t time_expected, actual_time;

	if (!blockchain)
		return (0);
	last_block = llist_get_tail(blockchain->chain);
	if (!last_block)
		return (0);
	if (last_block->info.index == 0 ||
		 (last_block->info.index % DIFFICULTY_ADJUSTMENT_INTERVAL != 0))
		return (last_block->info.difficulty);
	adjusted_block = llist_get_node_at(blockchain->chain,
												  last_block->info.index + 1 - DIFFICULTY_ADJUSTMENT_INTERVAL);
	time_expected = DIFFICULTY_ADJUSTMENT_INTERVAL * BLOCK_GENERATION_INTERVAL;
	actual_time = last_block->info.timestamp - adjusted_block->info.timestamp;

	if (actual_time * 2 < time_expected)
		return (adjusted_block->info.difficulty + 1);
	else if (actual_time > time_expected * 2)
		return (adjusted_block->info.difficulty - 1);

	return (adjusted_block->info.difficulty);
}
