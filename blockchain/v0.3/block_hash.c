#include "blockchain.h"

/**
 * block_hash-computes the block hash
 * @block: pointer of block to compute the hash
 * @hash_buf: pointer to hold the hash block
 * Return: if successful return hash otherwise return NULL
 */

uint8_t *block_hash(block_t const *block,
						  uint8_t hash_buf[SHA256_DIGEST_LENGTH])
{
	SHA256_CTX HASH256;
	int len_tx, size_memory, size_block, i;
	transaction_t *tx;
	int8_t *hash_block;

	if (!block || !hash_buf)
		return (NULL);
	SHA256_Init(&HASH256);
	len_tx = 0;
	if (llist_size(block->transactions) > 0)
		len_tx = llist_size(block->transactions);
	size_block = sizeof(block->info) + block->data.len;
	size_memory = size_block + len_tx * SHA256_DIGEST_LENGTH;
	hash_block = calloc(1, size_memory);
	if (!hash_block)
		return (NULL);
	memcpy(hash_block, block, size_block);
	for (i = 0; i < llist_size(block->transactions); i++)
	{
		tx = llist_get_node_at(block->transactions, i);
		memcpy(hash_block + size_block, tx->id, SHA256_DIGEST_LENGTH);
		size_block += SHA256_DIGEST_LENGTH;
	}

	SHA256_Update(&HASH256, hash_block, size_memory);
	SHA256_Final(hash_buf, &HASH256);
	free(hash_block);
	return (hash_buf);
}
