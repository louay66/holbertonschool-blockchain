#include "transaction.h"

/**
 * select_sender_unspent - Selects unspent transaction outputs for the sender
 * @all_unspent: A list of all unspent transaction outputs.
 * @visitor: The visitor structure containing sender information.
 *
 * This function searches for unspent transaction outputs owned by the sender
 * and adds them to the visitor's sender_unspent list.
 */
void select_sender_unspent(llist_t *all_unspent, visitor_t *visitor)
{
	int i;
	unspent_tx_out_t *unspent;

	for (i = 0; i < llist_size(all_unspent); i++)
	{
		unspent = llist_get_node_at(all_unspent, i);
		if (!memcmp(unspent->out.pub, visitor->sender_pub, EC_PUB_LEN))
		{
			if (llist_add_node(visitor->sender_unspent, unspent, ADD_NODE_REAR))
				exit(1);
			visitor->total_amount += unspent->out.amount;
		}
	}
}
/**
 * build_tx - Builds a transaction using the provided inputs and outputs.
 * @sender: The private key of the transaction sender.
 * @visitor: The visitor structure containing sender information.
 * @all_unspent: A list of all unspent transaction outputs to date.
 * @sender_pub: The public key of the transaction sender.
 * @receiver_pub: The public key of the transaction receiver.
 * @tx: The transaction structure to be populated.
 * Return: A pointer to the created transaction upon success,
 * or NULL upon failure.
 */
transaction_t *build_tx(EC_KEY const *sender,
		visitor_t *visitor,
		llist_t *all_unspent, uint8_t *sender_pub,
		uint8_t *receiver_pub,
		transaction_t *tx)
{
	int i;
	tx_out_t *to_sender, *to_reciver;
	tx_in_t *txi, *in;
	unspent_tx_out_t *utx;

	tx->inputs = llist_create(MT_SUPPORT_FALSE);
	tx->outputs = llist_create(MT_SUPPORT_FALSE);
	to_reciver = tx_out_create(visitor->amount, receiver_pub);
	to_sender = visitor->total_amount > visitor->amount
		? tx_out_create(visitor->total_amount - visitor->amount, sender_pub) : 0;
	if (!tx->inputs || !tx->outputs || !to_reciver ||
			(visitor->total_amount > visitor->amount && !to_sender))
	{
		return (llist_destroy(tx->inputs, 1, free),
				llist_destroy(tx->outputs, 1, free), free(tx), NULL);
	}
	for (i = 0; i < llist_size(visitor->sender_unspent); i++)
	{
		utx = llist_get_node_at(visitor->sender_unspent, i);
		txi = tx_in_create(utx);
		if (!txi)
			exit(1);
		if (llist_add_node(tx->inputs, txi, ADD_NODE_REAR))
			exit(1);
	}
	if (llist_add_node(tx->outputs, to_reciver, ADD_NODE_REAR) ||
			(to_sender && llist_add_node(tx->outputs, to_sender, ADD_NODE_REAR)))
		exit(1);
	if (!transaction_hash(tx, tx->id))
		exit(1);
	for (i = 0; i < llist_size(tx->inputs); i++)
	{
		in = llist_get_node_at(tx->inputs, i);

		if (!tx_in_sign(in, tx->id, sender, all_unspent))
			exit(1);
	}
	llist_destroy(visitor->sender_unspent, 0, NULL);
	printf("2\n");
	return (tx);
}
/**
 * transaction_create-creates a transaction
 * @sender:contains the private key of the transaction sender
 * @receiver:contains the public key of the transaction receiver
 * @amount: is the amount to send
 * @all_unspent:is the list of all unspent transaction outputs to date
 * Return:return a pointer to the created transaction upon success,
 * or NULL upon failure
 */
transaction_t *transaction_create(EC_KEY const *sender, EC_KEY const *receiver,
		uint32_t amount, llist_t *all_unspent)
{
	uint8_t sender_pub[EC_PUB_LEN], receiver_pub[EC_PUB_LEN];
	visitor_t visitor = {0};
	transaction_t *tx;

	if (!sender || !receiver || !amount || !all_unspent)
		return (NULL);

	if (!ec_to_pub(sender, sender_pub) || !ec_to_pub(receiver, receiver_pub))
		return (NULL);

	visitor.amount = amount;
	visitor.sender_pub = sender_pub;
	visitor.sender_unspent = llist_create(MT_SUPPORT_FALSE);
	if (!visitor.sender_unspent)
		return (NULL);
	select_sender_unspent(all_unspent, &visitor);
	if (visitor.total_amount < amount)
	{
		llist_destroy(visitor.sender_unspent, 0, NULL);
		return (NULL);
	}
	tx = calloc(1, sizeof(*tx));
	if (!tx)
		return (NULL);
	return (build_tx(sender, &visitor, all_unspent, sender_pub,
				receiver_pub, tx));
}
