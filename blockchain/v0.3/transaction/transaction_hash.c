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
	tx_in_t *input;
	tx_out_t *output;
	unsigned int i, in_len, out_len, cursor, size;
	int8_t *space_hash;

	in_len = llist_size(transaction->inputs);
	out_len = llist_size(transaction->outputs);
	size = in_len * 32 * 3 + out_len * 32;
	space_hash = malloc(size);
	if (!space_hash)
		return (NULL);
	cursor = 0;
	i = 0;
	while (1)
	{
		input = llist_get_node_at(transaction->inputs, i);
		if (input)
		{
			memcpy(space_hash + cursor, input, 32 * 3);
			cursor += 32 * 3;
		}
		output = llist_get_node_at(transaction->outputs, i);
		if (output)
		{
			memcpy(space_hash + cursor, output->hash, 32);
			cursor += 32;
		}
		if (!input && !output)
		{
			break;
		}
		i++;
	}
	sha256(space_hash, size, hash_buf);
	free(output);
	free(input);
	free(space_hash);
	return (hash_buf);
}
