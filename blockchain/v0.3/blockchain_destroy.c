#include "blockchain.h"
/**
 * blockchain_destroy-destroy a blockchai
 * @blockchain: pointer of blockchain to free
 * Return: void
 */
void blockchain_destroy(blockchain_t *blockchain)
{
	if (!blockchain)
		return;
	llist_destroy(blockchain->chain, 1, (void (*)(llist_node_t))free);
	llist_destroy(blockchain->unspent, 1, free);
	free(blockchain);
}
