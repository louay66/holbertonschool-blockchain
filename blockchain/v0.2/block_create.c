#include "blockchain.h"
/**
 * block_create-creates a new block depeden of prev block
 * @prev: the pointer to prev block
 * @data: the pointer to data of new block
 * @data_len: length of data
 * Return:block_t if successful otherwise rturn NULL
 */
block_t *block_create(block_t const *prev,
		int8_t const *data, uint32_t data_len)
{
	block_t *new_block;
	time_t seconds;

	new_block = calloc(1, sizeof(*new_block));
	if (!new_block || !prev || !data || data_len == 0)

	{
		free(new_block);
		return (NULL);
	}

	memcpy(&(new_block->info.prev_hash), prev->hash, SHA256_DIGEST_LENGTH);
	if (data_len >= BLOCKCHAIN_DATA_MAX)
	{
		memcpy(&(new_block->data.buffer), data, BLOCKCHAIN_DATA_MAX);
		new_block->data.len = BLOCKCHAIN_DATA_MAX;
	}
	else
	{
		memcpy(&(new_block->data.buffer), data, data_len);
		new_block->data.len = data_len;
	}

	time(&seconds);
	new_block->info.index = prev->info.index + 1;
	new_block->info.timestamp = (uint64_t)seconds;

	memset(&(new_block->hash), 0, SHA256_DIGEST_LENGTH);
	return (new_block);
}
