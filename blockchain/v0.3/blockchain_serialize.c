#include "blockchain.h"
#include "provided/endianness.h"

#define MAGIC_HBLK "HBLK"
#define VERSION_HBLK "0.1"
int handel_error_file_IO(const void *ptr, size_t size, size_t nmemb,
								 FILE *stream);
int tx_input_serialise(llist_t *input, FILE *fptr);
int tx_output_serialise(llist_t *output, FILE *fptr);
int tx_serialise(llist_t *trx, FILE *fptr);
int block_serialise(llist_t *chain, FILE *fptr, int size);
int unspent_serialise(llist_t *unspent_list, FILE *fptr, int size);

/**
 * blockchain_serialize-serialize a bloackchain to file format
 * @blockchain: pointer of bloockchain
 * @path: path of file to going save into it serializtion
 * Return: if successful return 0 otherwise return -1
 */

int blockchain_serialize(blockchain_t const *blockchain, char const *path)
{
	int size_block, size_unspent, i;
	FILE *fptr;
	uint8_t endianess;
	unspent_tx_out_t *unspent;

	if (!blockchain || !blockchain->chain || !path)
		return (-1);
	size_block = llist_size(blockchain->chain);
	size_unspent = llist_size(blockchain->unspent);
	endianess = _get_endianness();
	fptr = fopen(path, "wb");
	if (!fptr)
		return (-1);
	if (handel_error_file_IO(MAGIC_HBLK, 1, strlen(MAGIC_HBLK), fptr) == -1)
		return (-1);
	if (handel_error_file_IO(VERSION_HBLK, 1, strlen(VERSION_HBLK), fptr) == -1)
		return (-1);
	if (handel_error_file_IO(&endianess, 1, 1, fptr) == -1)
		return (-1);
	if (handel_error_file_IO(&size_block, 1, 4, fptr) == -1)
		return (-1);
	if (handel_error_file_IO(&size_unspent, 1, 4, fptr) == -1)
		return (-1);
	if (block_serialise(blockchain->chain, fptr, size_block))
		return (-1);
	if (unspent_serialise(blockchain->unspent, fptr, size_unspent))
		return (-1);
	fclose(fptr);
	return (0);
}

/**
 * block_serialise-serialize a block to file format
 * @chain: pointer of llist
 * @fptr: pointer to file
 * @size: size of blocks
 * Return: if successful return 0 otherwise return -1
 */
int block_serialise(llist_t *chain, FILE *fptr, int size)
{
	int i, size_tx;
	block_t *block;

	for (i == 0; i < size; i++)
	{
		block = llist_get_node_at(chain, i);
		size_tx = llist_size(block->transactions);
		if (handel_error_file_IO(&block->info, sizeof(block->info), 1, fptr) == -1)
			return (-1);
		if (handel_error_file_IO(&block->data.len, 4, 1, fptr) == -1)
			return (-1);
		if (handel_error_file_IO(&block->data.buffer,
										 block->data.len + 1, 1, fptr) == -1)
			return (-1);
		if (handel_error_file_IO(&block->hash, SHA256_DIGEST_LENGTH, 1, fptr) == -1)
			return (-1);

		if (handel_error_file_IO(&size_tx, 4, 1, fptr) == -1)
			return (-1);
		if (tx_serialise(block->transactions, fptr) == -1)
			return (-1);
	}
	return (0);
}

/**
 * unspent_serialise-serialize a unspent list to file format
 * @unspent_list: pointer of llist
 * @fptr: pointer to file
 * @size: size of unspent list
 * Return: if successful return 0 otherwise return -1
 */
int unspent_serialise(llist_t *unspent_list, FILE *fptr, int size)
{
	int i;
	unspent_tx_out_t *unspent;

	for (i == 0; i < size; i++)
	{
		unspent = llist_get_node_at(unspent_list, i);
		if (handel_error_file_IO(&unspent->block_hash, SHA256_DIGEST_LENGTH,
										 1, fptr) == -1)
			return (-1);
		if (handel_error_file_IO(&unspent->tx_id, SHA256_DIGEST_LENGTH,
										 1, fptr) == -1)
			return (-1);
		if (handel_error_file_IO(&unspent->out.amount, 4, 1, fptr) == -1)
			return (-1);
		if (handel_error_file_IO(&unspent->out.pub, 65, 1, fptr) == -1)
			return (-1);
		if (handel_error_file_IO(&unspent->out.hash, SHA256_DIGEST_LENGTH,
										 1, fptr) == -1)
			return (-1);
	}
	return (0);
}

/**
 * tx_serialise-serialize a transection to file format
 * @trx: pointer of llist
 * @fptr: pointer to file
 * Return: if successful return 0 otherwise return -1
 */
int tx_serialise(llist_t *trx, FILE *fptr)
{
	int size_tx, size_in, size_out, i;
	transaction_t tx;
	block_t *block;

	size_tx = llist_size(trx);

	for (i == 0; i < size_tx; i++)
	{
		tx = llist_get_node_at(trx, i);
		size_in = llist_size(tx.inputs);
		size_out = llist_size(tx.outputs);

		if (handel_error_file_IO(&tx.id, SHA256_DIGEST_LENGTH, 1, fptr) == -1)
			return (-1);
		if (handel_error_file_IO(&size_in, 4, 1, fptr) == -1)
			return (-1);
		if (handel_error_file_IO(&size_out, 4, 1, fptr) == -1)
			return (-1);
		if (tx_input_serialise(tx.inputs, fptr) == -1)
			return (-1);
		if (tx_output_serialise(tx.outputs, fptr) == -1)
			return (-1);
	}
	return (0);
}
/**
 * tx_input_serialise-serialize a input of tx to file format
 * @input: pointer of llist
 * @fptr: pointer to file
 * Return: if successful return 0 otherwise return -1
 */
int tx_input_serialise(llist_t *input, FILE *fptr)
{
	int size_in, i;
	tx_in_t *tx_in;

	size_in = llist_size(input);
	for (i = 0; i < size_in; i++)
	{
		tx_in = llist_get_node_at(input, i);

		if (handel_error_file_IO(&tx_in->block_hash, SHA256_DIGEST_LENGTH,
										 1, fptr) == -1)
			return (-1);
		if (handel_error_file_IO(&tx_in->tx_id, SHA256_DIGEST_LENGTH, 1, fptr) == -1)
			return (-1);
		if (handel_error_file_IO(&tx_in->tx_out_hash, SHA256_DIGEST_LENGTH,
										 1, fptr) == -1)
			return (-1);
		if (handel_error_file_IO(&tx_in->sig.sig, 72, 1, fptr) == -1)
			return (-1);
		if (handel_error_file_IO(&tx_in->sig.len, 1, 1, fptr) == -1)
			return (-1);
	}
	return (0);
}
/**
 * tx_output_serialise-serialize a output of tx to file format
 * @output: pointer of llist
 * @fptr: pointer to file
 * Return: if successful return 0 otherwise return -1
 */
int tx_output_serialise(llist_t *output, FILE *fptr)
{
	int size_out, i;
	tx_out_t *tx_out;

	size_out = llist_size(output);
	for (i = 0; i < size_out; i++)
	{
		tx_out = llist_get_node_at(output, i);
		if (handel_error_file_IO(&tx_out->amount, 4, 1, fptr) == -1)
			return (-1);
		if (handel_error_file_IO(&tx_out->pub, 65, 1, fptr) == -1)
			return (-1);
		if (handel_error_file_IO(&tx_out->hash, SHA256_DIGEST_LENGTH, 1, fptr) == -1)
			return (-1);
	}
	return (0);
}

/**
 * handel_error_file_IO-handell error of fwrite
 * @ptr: pointer to write
 * @size: ype of byte going to write
 * @nmemb: number of bytes
 * @stream: pointer to file
 * Return: if successful return 0 otherwise return -1
 */
int handel_error_file_IO(const void *ptr, size_t size, size_t nmemb,
								 FILE *stream)
{
	if (fwrite(ptr, size, nmemb, stream) != nmemb)
	{
		fclose(stream);
		return (-1);
	}
	return (0);
}
