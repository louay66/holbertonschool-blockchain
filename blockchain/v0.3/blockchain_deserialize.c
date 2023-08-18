#include "blockchain.h"
#include "provided/endianness.h"

#define MAGIC_HBLK "HBLK"
#define VERSION_HBLK "0.1"

llist_t *deserialize_unspent(FILE *file, int size);
llist_t *deserialize_blocks(FILE *file, int size);
llist_t *deserialize_tx(FILE *file, int size);
llist_t *deserialize_tx_in(FILE *file, int size);
llist_t *deserialize_tx_out(FILE *file, int size);

/**
 * blockchain_deserialize-deserialize a  file format to bloackchain
 * @path: path of file to get from it file  of blockchain serializted
 * Return: if successful return hash otherwise return NULL
 */
blockchain_t *blockchain_deserialize(char const *path)
{

	blockchain_t *blockchain;
	FILE *file;
	uint8_t endianess;
	int size, nb_unspent;
	char buff[20];

	if (!path)
		return (NULL);

	file = fopen(path, "rb");
	if (!file)
		return (NULL);
	fread(&buff, 7, 1, file);
	if (memcmp(buff, "HBLK0.1", 7) != 0)
	{
		fclose(file);
		return (NULL);
	}

	fread(&endianess, 1, 1, file);
	fread(&size, 4, 1, file);
	fread(&nb_unspent, 4, 1, file);
	blockchain = calloc(1, sizeof(blockchain_t));
	blockchain->chain = deserialize_blocks(file, size);
	if (!blockchain->chain)
	{
		fclose(file);
		return (NULL);
	}
	blockchain->unspent = deserialize_unspent(file, nb_unspent);
	if (!blockchain->unspent)
	{
		fclose(file);
		return (NULL);
	}
	fclose(file);

	return (blockchain);
}
/**
 * deserialize_unspent-deserialize a  file format to unspent list
 * @file: file to deserialize blocks from it
 * @size: size of  blocks in blockchain
 * Return: if successful return hash otherwise return NULL
 */
llist_t *deserialize_unspent(FILE *file, int size)
{
	llist_t *list_unspent;
	unspent_tx_out_t *unspent;
	int i;

	list_unspent = llist_create(MT_SUPPORT_FALSE);
	if (!list_unspent)
		return (NULL);
	for (i = 0; i < size; i++)
	{
		unspent = calloc(1, sizeof(unspent_tx_out_t));
		if (!unspent)
			return (NULL);
		fread(&unspent->block_hash, SHA256_DIGEST_LENGTH, 1, file);
		fread(&unspent->tx_id, SHA256_DIGEST_LENGTH, 1, file);

		fread(&unspent->out.amount, 4, 1, file);
		fread(&unspent->out.pub, 65, 1, file);
		fread(&unspent->out.hash, SHA256_DIGEST_LENGTH, 1, file);
		llist_add_node(list_unspent, unspent, ADD_NODE_REAR);
	}
	return (list_unspent);
}
/**
 * deserialize_blocks-deserialize a  file format to blocks
 * @file: file to deserialize blocks from it
 * @size: size of  blocks in blockchain
 * Return: if successful return hash otherwise return NULL
 */
llist_t *deserialize_blocks(FILE *file, int size)
{
	llist_t *chain;
	block_t *block;
	int i, nb_tx;

	chain = llist_create(MT_SUPPORT_FALSE);
	if (!chain)
		return (NULL);
	for (i = 0; i < size; i++)
	{
		block = calloc(1, sizeof(block_t));
		fread(&block->info, sizeof(block->info), 1, file);
		fread(&block->data.len, 4, 1, file);
		fread(&block->data.buffer, block->data.len, 1, file);
		fread(&block->hash, SHA256_DIGEST_LENGTH, 1, file);
		fread(&nb_tx, 4, 1, file);
		llist_add_node(block->transactions, deserialize_tx(file, nb_tx),
							ADD_NODE_REAR);
		llist_add_node(chain, block, ADD_NODE_REAR);
	}
	return (chain);
}
/**
 * deserialize_tx-deserialize a  file format to transection of block
 * @file: file to deserialize blocks from it
 * @size: size of  blocks in blockchain
 * Return: if successful return hash otherwise return NULL
 */
llist_t *deserialize_tx(FILE *file, int size)
{

	llist_t *block_tx;
	transaction_t *tx;
	tx_out_t *out;
	tx_in_t *input;
	int j, i, nb_in, nb_out;

	block_tx = llist_create(MT_SUPPORT_FALSE);
	if (size == -1)
	{
		return (NULL);
	}
	for (i = 0; i < size; i++)
	{
		tx = calloc(1, sizeof(transaction_t));
		if (!tx)
			return (NULL);
		fread(tx, SHA256_DIGEST_LENGTH, 1, file);
		fread(&nb_in, 4, 1, file);
		fread(&nb_out, 4, 1, file);
		for (j = 0; j < nb_in; j++)
		{
			input = calloc(1, sizeof(tx_in_t));
			fread(input, 169, 1, file);
			llist_add_node(tx->inputs, input, ADD_NODE_REAR);
		}
		for (j = 0; j < nb_out; j++)
		{
			out = calloc(1, sizeof(tx_out_t));
			fread(out, 101, 1, file);

			llist_add_node(tx->outputs, out, ADD_NODE_REAR);
		}
		llist_add_node(block_tx, tx, ADD_NODE_REAR);
	}
	return (block_tx);
}
