int Size(TreeNode* t)
{
	if (t == NULL) return 0;
	// radacina este primul nod

	return (1 + Size(t->l) + Size(t->r));	
	/*int count = 1;
	count += Size(t->l);
	count += Size(t->r);
	return count;*/
}

int Size_interns(TreeNode* t) {

	if((t == NULL) || ((t->l == NULL) && (t->r == NULL))) return 0;
	return(1 + Size_interns(t->l) + Size_interns(t->r));

}

int Size_externs(TreeNode* t) {

	if (t == NULL) return 0;
	if ((t->l == NULL) && (t->r == NULL)) return 1;

	return (Size_externs(t->l) + Size_externs(t->r));

}

int Size_preferred(TreeNode* t) {

	int count = 0;
	if (t == NULL) return 0;
	if (t->elem >= 11) count++; 
	//else return 0;

	return(count + Size_preferred(t->l) + Size_preferred(t->r));

}
// la laborator similar cu size-ul, in rest e la fel
// ca ce ai facut