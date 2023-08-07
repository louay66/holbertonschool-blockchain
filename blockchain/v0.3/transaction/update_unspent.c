#include "transaction.h"

/**
 * search_inspent - find unspent matching tx output
 * @node:  all_unspent list
 * @arg: tx output hash
 * Return: 0 if continue else 1
 */
int search_inspent(llist_node_t node, void *arg)
{
	unspent_tx_out_t *unspent;

	unspent = node;
	if (!memcmp(unspent->out.hash, arg, SHA256_DIGEST_LENGTH))
		return (1);
	return (0);
}
/**
 * update_unspent-update unspent outputs list
 * @transactions: is the list of validated transactions
 * @block_hash: Hash of the validated Block that
 * contains the transaction list transactions
 * @all_unspent:is the current list of unspent transaction outputs
 * Return: create and return a new list of unspent transaction outputs
 */

llist_t *update_unspent(llist_t *transactions,
		uint8_t block_hash[SHA256_DIGEST_LENGTH], llist_t *all_unspent)
{
	transaction_t *tx;
	unspent_tx_out_t *unspent;
	tx_in_t *in_tx;
	tx_out_t *out_tx;
	int i, j;

	for (j = 0; j < llist_size(transactions); j++)
	{
		tx = llist_get_node_at(transactions, j);
		for (i = 0; i < llist_size(tx->inputs); i++)
		{
			in_tx = llist_get_node_at(tx->inputs, i);

			llist_remove_node(all_unspent, search_inspent, in_tx->tx_out_hash, 1, free);
		}

		for (i = 0; i < llist_size(tx->outputs); i++)
		{
			out_tx = llist_get_node_at(tx->outputs, i);

			unspent = unspent_tx_out_create(block_hash, tx->id, out_tx);
			if (!unspent)
				exit(1);
			if (llist_add_node(all_unspent, unspent, ADD_NODE_REAR))
				exit(1);
		}
	}
	return (all_unspent);
}
