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

	uint8_t pub[EC_PUB_LEN];
	tx_out_t *tx_out;
	tx_in_t *tx_in;
	transaction_t *tx;

	if (!receiver || !block_index || !ec_to_pub(receiver, pub))
		return (NULL);

	tx = calloc(1, sizeof(transaction_t));
	tx_in = calloc(1, sizeof(tx_in_t));

	if (!tx || !tx_in)
	{
		return (free(tx), free(tx_in), NULL);
	}
	memset(tx_in, 0, sizeof(*tx_in));
	memcpy(tx_in->tx_out_hash, &block_index, 4);
	tx->inputs = llist_create(MT_SUPPORT_FALSE);
	tx->outputs = llist_create(MT_SUPPORT_FALSE);
	tx_out = tx_out_create(COINBASE_AMOUNT, pub);
	llist_add_node(tx->inputs, tx_in, ADD_NODE_REAR);
	llist_add_node(tx->outputs, tx_out, ADD_NODE_REAR);
	if (!transaction_hash(tx, tx->id))
	{
		return (free(tx), free(tx_in), free(tx_out),
				  llist_destroy(tx->inputs, 0, NULL),
				  llist_destroy(tx->outputs, 0, NULL), NULL);
	}
	return (tx);
}
