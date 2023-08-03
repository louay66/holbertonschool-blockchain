#include "transaction.h"

/**
 * transaction_hash- computes the ID (hash) of a transaction
 * @transaction:points to the transaction to compute the hash of
 * @hash_buf: is a buffer in which to store the computed hash
 * Return: if successful must return a pointer to hash_buf
 * upon success, or NULL upon failure
 */

uint8_t *transaction_hash(transaction_t const *transaction,
								  uint8_t hash_buf[SHA256_DIGEST_LENGTH])
{
	SHA256_CTX HASH256;
	llist_node_t *input, *output;
	unsigned int i;

	SHA256_Init(&HASH256);

	i = 0;
	while (1)
	{
		input = llist_get_node_at(transaction->inputs, i);
		if (input)
		{
			SHA256_Update(&HASH256, ((tx_in_t *)input)->block_hash, 32);
			SHA256_Update(&HASH256, ((tx_in_t *)input)->tx_out_hash, 32);
			SHA256_Update(&HASH256, ((tx_in_t *)input)->tx_id, 32);
		}
		output = llist_get_node_at(transaction->outputs, i);
		if (output)
		{
			SHA256_Update(&HASH256, ((tx_out_t *)output)->hash, 32);
		}
		if (!input && !output)
		{
			break;
		}
		i++;
	}
	SHA256_Final(hash_buf, &HASH256);
	free(output);
	free(input);
	return (hash_buf);
}
