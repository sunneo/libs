void even_extra_left_steal(bpt_node* left, bpt_node* right, void** divider)
{
	long i, j;


	for (i = BPT_ORDER - 1; i > 0; i--) {
		right->keys[i] = right->keys[i - 1];
		right->ptrs[i] = right->ptrs[i - 1];
	}
	left->nkeys--;
	right->nkeys++;
	if (left->is_leaf) {
		right->keys[0] = left->keys[left->nkeys];
		right->ptrs[0] = left->ptrs[left->nkeys];
		*divider = right->keys[0];
	} else {
		right->keys[0] = *divider;
		right->ptrs[0] = left->ptrs[left->nkeys + 1];
		*divider = left->keys[left->nkeys];
		((bpt_node*) right->ptrs[0])->parent = right;
	}
}



void even_extra_right_steal(bpt_node* left, bpt_node* right, void** divider)
{
	long i, j;

	
	if (left->is_leaf) {
		left->keys[left->nkeys] = right->keys[0];
		left->ptrs[left->nkeys] = right->ptrs[0];
		*divider = right->keys[1];
	} else {
		left->keys[left->nkeys] = *divider;
		left->ptrs[left->nkeys + 1] = right->ptrs[0];
		*divider = right->keys[0];
		((bpt_node*) right->ptrs[0])->parent = left;
	}

	left->nkeys++;
	right->nkeys--;

	for (i = 0; i < BPT_ORDER - 1; i++) {
		right->keys[i] = right->keys[i + 1];
		right->ptrs[i] = right->ptrs[i + 1];
	}
}
