#include "transaction.h"

/**
 * coinbase_create-creates a coinbase transaction
 * @receiver:contains the public key of the miner,
 * who will receive the coinbase coins
 * @block_index: is the index of the Block the coinbase transaction
 * will belong to
 * Return: pointer to the created transaction upon success,or NULL upon failure
 */
transaction_t *coinbase_create(EC_KEY const *receiver, uint32_t block_index)
{
	transaction_t *tx;
	tx_in_t *tx_in;
	tx_out_t *tx_out;
	uint8_t pub_key[EC_PUB_LEN];

	tx = malloc(sizeof(transaction_t));
	if (tx == NULL)
		return (NULL);
	tx_in = malloc(sizeof(tx_in_t));
	if (tx_in == NULL)
	{
		free(tx);
		return (NULL);
	}

	memset(tx_in, 0, sizeof(*tx_in));
	memcpy(tx_in->tx_out_hash, &block_index, 4);

	ec_to_pub(receiver, pub_key);
	tx_out = tx_out_create(COINBASE_AMOUNT, pub_key);

	tx->inputs = llist_create(MT_SUPPORT_FALSE);
	tx->outputs = llist_create(MT_SUPPORT_FALSE);

	llist_add_node(tx->inputs, tx_in, ADD_NODE_REAR);
	llist_add_node(tx->outputs, tx_out, ADD_NODE_REAR);

	transaction_hash(tx, out->id);

	return (out);
}
