#include "blockchain.h"

/**
 * block_is_valid-valid block with next block prev_hash
 * @block: points to the Block to check
 * @prev_block: points to the prev block
 * Return: if successful return 0 otherwise return 1
 */
int block_is_valid(block_t const *block, block_t const *prev_block)
{
	uint8_t buff_hash[SHA256_DIGEST_LENGTH] = {0};
	block_t const _genesis = {
		{
			0 /* index */,
			0,			  /* difficulty */
			1537578000, /* timestamp */
			0,			  /* nonce */
			{0}			  /* prev_hash */
		},
		{
			/* data */
			"Holberton School", /* buffer */
			16						 /* len */
		},
		"\xc5\x2c\x26\xc8\xb5\x46\x16\x39\x63\x5d\x8e\xdf\x2a\x97\xd4\x8d"
			"\x0c\x8e\x00\x09\xc8\x17\xf2\xb1\xd3\xd7\xff\x2f\x04\x51\x58\x03"};

	if (!block || (block->info.index != 0 && !prev_block))
		return (1);
	if (block->info.index == 0)
		return (memcmp(block, &_genesis, sizeof(_genesis)));
	if (block->info.index != prev_block->info.index + 1)
		return (1);
	if (!block_hash(prev_block, buff_hash) ||
			memcmp(buff_hash, prev_block->hash, SHA256_DIGEST_LENGTH) != 0)
		return (1);
	if (!block_hash(block, buff_hash) ||
			memcmp(buff_hash, block->hash, SHA256_DIGEST_LENGTH))
		return (1);
	if (memcmp(block->info.prev_hash, prev_block->hash, SHA256_DIGEST_LENGTH))
		return (1);
	if (block->data.len > BLOCKCHAIN_DATA_MAX)
		return (1);
	if (prev_block->data.len > BLOCKCHAIN_DATA_MAX)
		return (1);
	if (!hash_matches_difficulty(block->hash, block->info.difficulty))
		return (1);
	return (0);
}
