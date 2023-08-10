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
	uint8_t pub_of_reciver[EC_PUB_LEN];
	unspent_tx_out_t *unspent;
	unsigned int i;

	if (!ec_to_pub(sender, pub_of_sender))
		return (NULL);

	i = 0;
	while (1)
	{
		unspent = llist_get_node_at(all_unspent, i);
		if (memcmp(unspent->out.hash, in->tx_out_hash, SHA256_DIGEST_LENGTH) == 0)
		{
			memcpy(pub_of_reciver, unspent->out.pub, EC_PUB_LEN);
			break;
		}
		i++;
		if (!unspent)
		{
			return (NULL);
		}
	}
	if (memcmp(pub_of_sender, pub_of_reciver, EC_PUB_LEN) != 0)
		return (NULL);
	if (!ec_sign(sender, tx_id, SHA256_DIGEST_LENGTH, &in->sig))
		return (NULL);
	return (&in->sig);
}
