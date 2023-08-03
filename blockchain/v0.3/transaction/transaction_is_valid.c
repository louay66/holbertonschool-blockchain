#include "transaction.h"

/**
 * check_input-check input is refer to unspent
 * @inputs:pointer of inputs list
 * @all_unspent:is the list of all unspent transaction outputs to date
 * @tx: :pointer of transection  list
 * Return: if successful must return input amount otherwise return 0
 */
unsigned int check_input(llist_t *inputs, llist_t *all_unspent,
								 transaction_t const *tx)
{
	int i, j;
	int is_refer = 0;
	int in_balance = 0;
	tx_in_t *input;
	unspent_tx_out_t *unspent;
	EC_KEY *key;

	for (i = 0; i < llist_size(inputs); i++)
	{
		input = llist_get_node_at(inputs, i);
		for (j = 0; j < llist_size(all_unspent); j++)
		{
			unspent = llist_get_node_at(all_unspent, j);
			if (!memcmp(input->tx_out_hash, unspent->out.hash, SHA256_DIGEST_LENGTH))
			{
				is_refer++;
				in_balance += unspent->out.amount;

				key = ec_from_pub(unspent->out.pub);
				if (!key || !ec_verify(key, tx->id, SHA256_DIGEST_LENGTH, &input->sig))
				{
					EC_KEY_free(key);
					return (0);
				}
				EC_KEY_free(key);
			}
		}
	}
	if (is_refer != llist_size(inputs))
	{
		in_balance = 0;
	}
	return (in_balance);
}

/**
 * check_output-compute output amount
 * @outputs:pointer of outputs list
 * Return: output amount
 */
unsigned int check_output(llist_t *outputs)
{
	tx_out_t *output;
	unsigned int out_balance = 0;
	int i;

	for (i = 0; i < llist_size(outputs); i++)
	{
		output = llist_get_node_at(outputs, i);
		out_balance += output->amount;
	}
	return (out_balance);
}
/**
 * transaction_is_valid-validate transaction
 * @transaction:points to the transaction
 * @all_unspent:is the list of all unspent transaction outputs to date
 * Return: if successful must return 1
 * or 0 upon failure
 */
int transaction_is_valid(transaction_t const *transaction,
								 llist_t *all_unspent)
{

	uint8_t hash_buf[SHA256_DIGEST_LENGTH];
	unsigned int in_balance, out_balance;

	out_balance = 0;
	if (!transaction || !all_unspent)
		return (0);
	if (!transaction_hash(transaction, hash_buf))
		return (0);
	if (memcmp(hash_buf, transaction->id, SHA256_DIGEST_LENGTH) != 0)
		return (0);
	in_balance = check_input(transaction->inputs, all_unspent, transaction);
	if (!in_balance)
		return (0);
	out_balance = check_output(transaction->outputs);
	if (in_balance != out_balance || !in_balance)
		return (0);
	return (1);
}
