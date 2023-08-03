#include "transaction.h"

/**
 * unspent_tx_out_create-create unspent transaction
 * @block_hash:contains the hash of the Block where the referenced transaction
 * output is located
 * @tx_id:  contains the hash of a transaction in the Block block_hash, where
 * the referenced transaction output is located
 * @out: points to the referenced transaction output
 * Return: if successful return a pointer to created unspent transaction
 * output upon success, or NULL upon failure
 */
unspent_tx_out_t *unspent_tx_out_create(
		uint8_t block_hash[SHA256_DIGEST_LENGTH],
		uint8_t tx_id[SHA256_DIGEST_LENGTH], tx_out_t const *out)
{

	unspent_tx_out_t *unspent_tx;

	if (!block_hash || !tx_id || !out)
		return (NULL);

	unspent_tx = calloc(1, sizeof(unspent_tx_out_t));
	if (!unspent_tx)
		return (NULL);

	memcpy(unspent_tx->block_hash, block_hash, SHA256_DIGEST_LENGTH);
	memcpy(unspent_tx->tx_id, tx_id, SHA256_DIGEST_LENGTH);
	memcpy(&unspent_tx->out, out, sizeof(unspent_tx->out));

	return (unspent_tx);
}
