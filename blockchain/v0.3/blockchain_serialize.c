#include "blockchain.h"
#include "provided/endianness.h"

#define MAGIC_HBLK "HBLK"
#define VERSION_HBLK "0.3"
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
	int size_block, size_unspent;
	FILE *fptr;
	uint8_t endianess;

	if (!blockchain || !blockchain->chain || !path)
		return (-1);

	size_block = llist_size(blockchain->chain);
	size_unspent = llist_size(blockchain->unspent);
	endianess = _get_endianness();
	fptr = fopen(path, "wb");
	if (!fptr)
		return (-1);
	fwrite(MAGIC_HBLK, 1, 4, fptr);
	fwrite(VERSION_HBLK, 1, 3, fptr);
	fwrite(&endianess, 1, 1, fptr);
	fwrite(&size_block, 1, 4, fptr);
	fwrite(&size_unspent, 1, 4, fptr);

	block_serialise(blockchain->chain, fptr, size_block);
	unspent_serialise(blockchain->unspent, fptr, size_unspent);
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

	for (i = 0; i < size; i++)
	{
		block = llist_get_node_at(chain, i);
		fwrite(&block->info, 1, sizeof(block->info), fptr);

		fwrite(&block->data.len, 1, 4, fptr);
		fwrite(&block->data.buffer, 1,
				 block->data.len, fptr);
		fwrite(&block->hash, 1, SHA256_DIGEST_LENGTH, fptr);
		if (block->info.index == 0)
		{
			size_tx = -1;
		}
		else
		{
			size_tx = llist_size(block->transactions);
		}
		fwrite(&size_tx, 1, 4, fptr);
		tx_serialise(block->transactions, fptr);
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

	for (i = 0; i < size; i++)
	{
		unspent = llist_get_node_at(unspent_list, i);
		fwrite(unspent, 1, 165, fptr);
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
	transaction_t *tx;

	size_tx = llist_size(trx);

	for (i = 0; i < size_tx; i++)
	{
		tx = llist_get_node_at(trx, i);
		size_in = llist_size(tx->inputs);
		size_out = llist_size(tx->outputs);

		fwrite(&tx->id, 1, SHA256_DIGEST_LENGTH, fptr);
		fwrite(&size_in, 1, 4, fptr);
		fwrite(&size_out, 1, 4, fptr);
		tx_input_serialise(tx->inputs, fptr);
		tx_output_serialise(tx->outputs, fptr);
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

		fwrite(tx_in, 1, 169, fptr);
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
		fwrite(tx_out, 1, 101, fptr);
	}
	return (0);
}
