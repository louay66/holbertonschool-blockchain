#include "transaction.h"

/**
 * coinbase_is_valid-checks whether a coinbase transaction is valid
 * @coinbase: points to the coinbase transaction to verify
 * @block_index: is the index of the Block the coinbase transaction
 * will belong to
 * Return:  return 1 if the coinbase transaction is valid, and 0 otherwise
 */

int coinbase_is_valid(transaction_t const *coinbase, uint32_t block_index)
{
	uint8_t tx_hash[SHA256_DIGEST_LENGTH];
	uint8_t zero[SHA256_DIGEST_LENGTH];
	tx_in_t *tx_in;
	tx_out_t *tx_out;
	size_t i;
	uint8_t *sig;

	if (!coinbase || !block_index)
		return (0);
	transaction_hash(coinbase, tx_hash);
	tx_out = llist_get_node_at(coinbase->outputs, 0);
	tx_in = llist_get_node_at(coinbase->inputs, 0);
	memset(zero, 0, SHA256_DIGEST_LENGTH);
	if (memcmp(tx_hash, coinbase->id, SHA256_DIGEST_LENGTH) != 0)
		return (0);
	if (llist_size(coinbase->inputs) != 1 || llist_size(coinbase->outputs) != 1)
		return (0);
	if (tx_out->amount != COINBASE_AMOUNT)
		return (0);
	if (memcmp(tx_in->tx_out_hash, &block_index, 4))
		return (0);

	if (
		 memcmp(zero, tx_in->tx_id, SHA256_DIGEST_LENGTH) ||
		 memcmp(zero, tx_in->block_hash, SHA256_DIGEST_LENGTH))
		return (0);
	sig = (uint8_t *)&tx_in->sig;
	for (i = 0; i < sizeof(sig); i++)
	{
		if (sig[i] != 0)
			return (0);
	}

	return (1);
}
