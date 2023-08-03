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
	llist_t *inputs, *outputs;
	tx_out_t *tx_out;
	tx_in_t *tx_in;
	transaction_t *tx;

	if (!receiver || !block_index || !ec_to_pub(receiver, pub))
		return (NULL);

	tx = calloc(1, sizeof(transaction_t));
	tx_in = calloc(1, sizeof(tx_in_t));
	tx_out = tx_out_create(COINBASE_AMOUNT, pub);
	inputs = llist_create(MT_SUPPORT_FALSE);
	outputs = llist_create(MT_SUPPORT_FALSE);

	if (!tx || !tx_in || !tx_out || !inputs || !outputs ||
		 llist_add_node(inputs, tx_in, ADD_NODE_REAR) ||
		 llist_add_node(outputs, tx_out, ADD_NODE_REAR))
	{
		return (free(tx), free(tx_in), free(tx_out),
				  llist_destroy(inputs, 0, NULL),
				  llist_destroy(outputs, 0, NULL), NULL);
	}
	memcpy(tx_in->tx_out_hash, &block_index, 4);
	tx->inputs = inputs;
	tx->outputs = outputs;
	if (!transaction_hash(tx, tx->id))
	{
		return (free(tx), free(tx_in), free(tx_out),
				  llist_destroy(inputs, 0, NULL),
				  llist_destroy(outputs, 0, NULL), NULL);
	}
	return (tx);
}
