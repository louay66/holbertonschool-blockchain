#include "blockchain.h"

block_t const genesis = {
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
	NULL, /* transection is NULL*/
	"\xc5\x2c\x26\xc8\xb5\x46\x16\x39\x63\x5d\x8e\xdf\x2a\x97\xd4\x8d"
		"\x0c\x8e\x00\x09\xc8\x17\xf2\xb1\xd3\xd7\xff\x2f\x04\x51\x58\x03"};

/**
 * block_is_valid-valid block with next block prev_hash
 * @block: points to the Block to check
 * @prev_block: points to the prev block
 * @all_unspent: points to the unspent list
 * Return: if successful return 0 otherwise return 1
 */
int block_is_valid(block_t const *block, block_t const *prev_block,
		llist_t *all_unspent)
{
	uint8_t buff_hash[SHA256_DIGEST_LENGTH] = {0};
	transaction_t *tx;
	int i;

	if (!block || (block->info.index != 0 && !prev_block))
		return (1);
	if (block->info.index == 0)
		return (memcmp(block, &genesis, sizeof(genesis)));
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
	if (llist_size(block->transactions) < 1)
		return (1);
	for (i = 0; i < llist_size(block->transactions); i++)
	{
		tx = llist_get_node_at(block->transactions, i);
		if (i == 0)
		{
			if (!coinbase_is_valid(tx, block->info.index))
				return (1);
		}
		else if (!transaction_is_valid(tx, all_unspent))
			return (1);
	}

	return (0);
}
