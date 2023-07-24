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

	if (!block || !hash_buf)
		return (NULL);
	SHA256_Init(&HASH256);
	SHA256_Update(&HASH256, block, block->data.len + sizeof(block->info));
	SHA256_Final(hash_buf, &HASH256);
	return (hash_buf);
}
