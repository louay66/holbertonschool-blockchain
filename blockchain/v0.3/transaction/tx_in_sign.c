#include "transaction.h"

/**
 * tx_in_sign-signs a transaction input, given the transaction id it is from
 * @in:points to the transaction input structure to sign
 * @tx_id: contains the ID (hash) of the transaction the transaction input to
 * sign is stored in
 * @sender:contains the private key of the receiver of the coins contained in
 * the transaction output referenced by the transaction input
 * @all_unspent:is the list of all unspent transaction outputs to date
 * Return: return a pointer to the resulting signature structure upon success,
 * or NULL upon failure
 */

sig_t *tx_in_sign(tx_in_t *in, uint8_t const tx_id[SHA256_DIGEST_LENGTH],
						EC_KEY const *sender, llist_t *all_unspent)
{
	uint8_t pub_of_sender[EC_PUB_LEN];
	unspent_tx_out_t *unspent;
	int i;

	for (i = 0; i < llist_size(all_unspent); i++)
	{
		unspent = llist_get_node_at(all_unspent, i);
		if (memcmp(in->tx_out_hash, unspent->out.hash, 32) == 0)
			break;
	}
	if (i == llist_size(all_unspent))
		return (NULL);

	ec_to_pub(sender, pub_of_sender);
	if (memcmp(pub_of_sender, unspent->out.pub, EC_PUB_LEN) != 0)
		return (NULL);
	ec_sign(sender, tx_id, SHA256_DIGEST_LENGTH, &in->sig);

	return (&in->sig);
}
