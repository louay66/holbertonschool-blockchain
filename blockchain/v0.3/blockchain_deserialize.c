#include "blockchain.h"
#include "provided/endianness.h"

#define MAGIC_HBLK "HBLK"
#define VERSION_HBLK "0.1"
#define CHECK_ENDIAN(x) (endianess ? SWAPENDIAN(x) : (void)0)

llist_t *deserialize_unspent(FILE *file, int size, uint8_t endianess);
llist_t *deserialize_blocks(FILE *file, int size, uint8_t endianess);
llist_t *deserialize_tx(FILE *file, int size, uint8_t endianess);
llist_t *deserialize_tx_in(FILE *file, int size);
llist_t *deserialize_tx_out(FILE *file, int size, uint8_t endianess);

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

	file = fopen(path, "r");
	if (!file)
		return (NULL);
	fread(&buff, 7, 1, file);
	if (memcmp(buff, "HBLK0.1", 7) != 0)
	{
		fclose(file);
		return (NULL);
	}
	fread(&endianess, 1, 1, file);
	endianess = endianess != _get_endianness();
	fread(&size, 4, 1, file);
	CHECK_ENDIAN(size);
	fread(&nb_unspent, 4, 1, file);
	CHECK_ENDIAN(nb_unspent);
	blockchain = calloc(1, sizeof(*blockchain));
	blockchain->chain = deserialize_blocks(file, size, endianess);

	if (!blockchain->chain)
	{
		blockchain_destroy(blockchain);
		fclose(file);
		return (NULL);
	}
	blockchain->unspent = deserialize_unspent(file, nb_unspent, endianess);
	if (!blockchain->unspent)
	{
		blockchain_destroy(blockchain);
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
 * @endianess: if 1 is big endian need to swap byte  else lettle endien
 * Return: if successful return hash otherwise return NULL
 */
llist_t *deserialize_unspent(FILE *file, int size, uint8_t endianess)
{
	llist_t *list_unspent;
	unspent_tx_out_t *unspent;
	int i;

	list_unspent = llist_create(MT_SUPPORT_TRUE);
	if (!list_unspent)
		return (NULL);
	for (i = 0; i < size; i++)
	{
		unspent = calloc(1, sizeof(*unspent));
		if (!unspent)
			return (NULL);
		if (fread(&unspent->block_hash, SHA256_DIGEST_LENGTH, 1, file) != 1)
		{
			free(unspent);
			llist_destroy(list_unspent, 1, NULL);
			return (NULL);
		}
		if (fread(&unspent->tx_id, SHA256_DIGEST_LENGTH, 1, file) != 1)
		{
			free(unspent);
			llist_destroy(list_unspent, 1, NULL);
			return (NULL);
		}
		if (fread(&unspent->out.amount, 4, 1, file) != 1)
		{
			free(unspent);
			llist_destroy(list_unspent, 1, NULL);
			return (NULL);
		}
		CHECK_ENDIAN(unspent->out.amount);
		if (fread(&unspent->out.pub, 65, 1, file) != 1)
		{
			free(unspent);
			llist_destroy(list_unspent, 1, NULL);
			return (NULL);
		}
		if (fread(&unspent->out.hash, SHA256_DIGEST_LENGTH, 1, file) != 1)
		{
			free(unspent);
			llist_destroy(list_unspent, 1, NULL);
			return (NULL);
		}
		if (llist_add_node(list_unspent, unspent, ADD_NODE_REAR))
		{
			free(unspent);
			llist_destroy(list_unspent, 1, NULL);
			return (NULL);
		}
	}
	return (list_unspent);
}
/**
 * deserialize_blocks-deserialize a  file format to blocks
 * @file: file to deserialize blocks from it
 * @size: size of  blocks in blockchain
 * @endianess: if 1 is big endian need to swap byte  else lettle endien
 * Return: if successful return hash otherwise return NULL
 */
llist_t *deserialize_blocks(FILE *file, int size, uint8_t endianess)
{
	llist_t *chain;
	block_t *block;
	int i, nb_tx;

	chain = llist_create(MT_SUPPORT_TRUE);
	if (!chain)
		return (NULL);

	for (i = 0; i < size; i++)
	{
		block = calloc(1, sizeof(*block));
		if (fread(&block->info, sizeof(block->info), 1, file) != 1)
		{
			block_destroy(block);
			llist_destroy(chain, 1, NULL);
			return (NULL);
		}
		CHECK_ENDIAN(block->info.index);
		CHECK_ENDIAN(block->info.difficulty);
		CHECK_ENDIAN(block->info.timestamp);
		CHECK_ENDIAN(block->info.nonce);
		if (fread(&block->data.len, 4, 1, file) != 1)
		{
			block_destroy(block);
			llist_destroy(chain, 1, NULL);
			return (NULL);
		}
		CHECK_ENDIAN(block->data.len);

		if (fread(&block->data.buffer, block->data.len, 1, file) != 1)
		{
			block_destroy(block);
			llist_destroy(chain, 1, NULL);
			return (NULL);
		}

		if (fread(&block->hash, SHA256_DIGEST_LENGTH, 1, file) != 1)
		{
			block_destroy(block);
			llist_destroy(chain, 1, NULL);
			return (NULL);
		}
		fread(&nb_tx, 4, 1, file);
		block->transactions = deserialize_tx(file, nb_tx, endianess);
		if (llist_add_node(chain, block, ADD_NODE_REAR))
		{
			block_destroy(block);
			llist_destroy(chain, 1, NULL);
			return (NULL);
		}
	}
	return (chain);
}
/**
 * deserialize_tx-deserialize a  file format to transection of block
 * @file: file to deserialize blocks from it
 * @size: size of  blocks in blockchain
 * @endianess: if 1 is big endian need to swap byte  else lettle endien
 * Return: if successful return hash otherwise return NULL
 */
llist_t *deserialize_tx(FILE *file, int size, uint8_t endianess)
{

	llist_t *block_tx;
	transaction_t *tx;
	int i, nb_in, nb_out;

	block_tx = llist_create(MT_SUPPORT_FALSE);
	if (!block_tx)
		return (NULL);
	for (i = 0; i < size; i++)
	{
		tx = calloc(1, sizeof(*tx));
		if (!tx)
			return (NULL);
		if (fread(&tx->id, SHA256_DIGEST_LENGTH, 1, file) != 1)
		{
			transaction_destroy(tx);
			llist_destroy(block_tx, 1, NULL);
			return (NULL);
		}
		fread(&nb_in, 4, 1, file);
		fread(&nb_out, 4, 1, file);
		tx->inputs = deserialize_tx_in(file, nb_in);
		if (!tx->inputs)
		{
			transaction_destroy(tx);
			llist_destroy(block_tx, 1, NULL);
			return (NULL);
		}
		tx->outputs = deserialize_tx_out(file, nb_out, endianess);
		if (!tx->outputs)
		{
			transaction_destroy(tx);
			llist_destroy(block_tx, 1, NULL);
			return (NULL);
		}
		if (llist_add_node(block_tx, tx, ADD_NODE_REAR))
		{
			transaction_destroy(tx);
			llist_destroy(block_tx, 1, NULL);
			return (NULL);
		}
	}
	return (block_tx);
}
/**
 * deserialize_tx_in-deserialize a  file format to input of transection
 * @file: file to deserialize blocks from it
 * @size: size of  blocks in blockchain
 * Return: if successful return hash otherwise return NULL
 */
llist_t *deserialize_tx_in(FILE *file, int size)
{
	llist_t *inputs;
	tx_in_t *input;
	int i;

	inputs = llist_create(MT_SUPPORT_FALSE);
	if (!inputs)
		return (NULL);
	for (i = 0; i < size; i++)
	{
		input = calloc(1, sizeof(*input));
		if (!input)
			return (NULL);
		if (fread(&input->block_hash, SHA256_DIGEST_LENGTH, 1, file) != 1)
		{
			free(input);
			llist_destroy(inputs, 1, NULL);
			return (NULL);
		}
		if (fread(&input->tx_id, SHA256_DIGEST_LENGTH, 1, file) != 1)
		{
			free(input);
			llist_destroy(inputs, 1, NULL);
			return (NULL);
		}
		if (fread(&input->tx_out_hash, SHA256_DIGEST_LENGTH, 1, file) != 1)
		{
			free(input);
			llist_destroy(inputs, 1, NULL);
			return (NULL);
		}
		if (fread(&input->sig.sig, 72, 1, file) != 1)
		{
			free(input);
			llist_destroy(inputs, 1, NULL);
			return (NULL);
		}
		if (fread(&input->sig.len, 1, 1, file) != 1)
		{
			free(input);
			llist_destroy(inputs, 1, NULL);
			return (NULL);
		}
		if (llist_add_node(inputs, input, ADD_NODE_REAR))
		{
			free(input);
			llist_destroy(inputs, 1, NULL);
			return (NULL);
		}
	}
	return (inputs);
}

/**
 * deserialize_tx_out-deserialize a  file format to output of transection
 * @file: file to deserialize blocks from it
 * @size: size of  blocks in blockchain
 * @endianess: if 1 is big endian need to swap byte  else lettle endien
 * Return: if successful return hash otherwise return NULL
 */
llist_t *deserialize_tx_out(FILE *file, int size, uint8_t endianess)
{
	llist_t *outputs;
	tx_out_t *out;
	int i;

	outputs = llist_create(MT_SUPPORT_FALSE);
	if (!outputs)
		return (NULL);
	for (i = 0; i < size; i++)
	{
		out = calloc(1, sizeof(*out));
		if (!out)
			return (NULL);
		if (fread(&out->amount, 4, 1, file) != 1)
		{
			free(out);
			llist_destroy(outputs, 1, NULL);
			return (NULL);
		}
		CHECK_ENDIAN(out->amount);
		if (fread(&out->pub, 65, 1, file) != 1)
		{
			free(out);
			llist_destroy(outputs, 1, NULL);
			return (NULL);
		}
		if (fread(&out->hash, SHA256_DIGEST_LENGTH, 1, file) != 1)
		{
			free(out);
			llist_destroy(outputs, 1, NULL);
			return (NULL);
		}
		if (llist_add_node(outputs, out, ADD_NODE_REAR))
		{
			free(out);
			llist_destroy(outputs, 1, NULL);
			return (NULL);
		}
	}
	return (outputs);
}
