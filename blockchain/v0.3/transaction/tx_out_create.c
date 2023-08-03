#include "transaction.h"

/**
 * tx_out_create-create transection output
 * @amount: is the amount of the transaction
 * @pub: is the public key of the transaction receiver
 * Return: if successful return a pointer to the created transaction
 * output upon success, or NULL upon failure
 */
tx_out_t *tx_out_create(uint32_t amount, uint8_t const pub[EC_PUB_LEN])
{
	tx_out_t *tx_out;

	if (!amount || !pub)
		return (NULL);
	tx_out = calloc(1, sizeof(tx_out_t));
	if (!tx_out)
		return (NULL);
	tx_out->amount = amount;
	memcpy(tx_out->pub, pub, EC_PUB_LEN);

	if (!sha256((int8_t const *)tx_out,
					sizeof(tx_out->amount) + sizeof(tx_out->pub), tx_out->hash))
	{
		free(tx_out);
		return (NULL);
	}
	return (tx_out);
}
