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
	transaction_t *out;
	tx_in_t *i_token;
	tx_out_t *o_token;
	uint8_t key_out[EC_PUB_LEN];

	out = malloc(sizeof(transaction_t));
	if (out == NULL)
		return (NULL);
	i_token = malloc(sizeof(tx_in_t));
	if (i_token == NULL)
	{
		free(out);
		return (NULL);
	}

	memset(i_token, 0, sizeof(*i_token));
	memcpy(i_token->tx_out_hash, &block_index, 4);

	ec_to_pub(receiver, key_out);
	o_token = tx_out_create(COINBASE_AMOUNT, key_out);

	out->inputs = llist_create(MT_SUPPORT_FALSE);
	out->outputs = llist_create(MT_SUPPORT_FALSE);

	llist_add_node(out->inputs, i_token, ADD_NODE_REAR);
	llist_add_node(out->outputs, o_token, ADD_NODE_REAR);

	transaction_hash(out, out->id);

	return (out);
}
