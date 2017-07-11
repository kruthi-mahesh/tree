#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <limits.h>
 
#define m 5
#define min (m/2 + m%2)
struct n{
	int key[m-1];
	struct n* ch[m];
	struct n* par;
	int leaf,ct;
};
typedef struct n* node;

struct temp
{
	int key[m];
	struct n* ch[m+1];
	int ct;
};
typedef struct temp* t_ptr;

void display(node root){

	int i;
	printf("\nTree elements are\n");
	if(root == NULL)
		return;
	node cur = root;
	while(cur->leaf != 1)
		cur=cur->ch[0];
	while(cur!=NULL){
		for(i=0; i<cur->ct;i++)
			printf("%d   	",cur->key[i]);
		cur = cur->ch[m-1];
	}
}
node getnode(){
	int i;
	node nn = (node) malloc(sizeof(struct n));
	if(nn == NULL){
		printf("Insufficient memory");
		exit(0);
	}
	nn->leaf = 1;
	nn->ct = 1;
	for(i=0; i<m; i++)
		nn->ch[i] = NULL;
	nn->par = NULL;
	return nn;
}

node find(node root,int data){
	int i = 0;
	node cur = root;
	while(cur->leaf != 1){
		i=0;
		while(i < cur->ct && data >= root->key[i])
			i++;
		cur = cur->ch[i];
	}
	return cur;
}

void insert_inLeaf(node ln,int data){
	int j,i = 0;
	while(i < ln->ct && data >= ln->key[i])
		i++;
	for(j =  ln->ct; j > i ; j--)
		ln->key[j] = ln->key[j-1];
	ln->key[i] = data;
	ln->ct++;
}
void insert_inTemp(t_ptr ln,int data){
	int j,i = 0;
	while(i < ln->ct && data >= ln->key[i])
		i++;
	for(j =  ln->ct; j > i ; j--)
		ln->key[j] = ln->key[j-1];
	ln->key[i] = data;
	ln->ct++;

}
void insertAfter(node P,node N,int K,node N_){
	int i=0;
	while(P->ch[i] != N)
		i++;
	int j = P->ct;
	while(j>i){
		P->key[j] = P->key[j-1];
		P->ch[j+1] = P->ch[j];
		j--;
	}
	P->key[i] = K;
	P->ch[i+1] = N_;
	P->ct++;
	N_->par = P;
}

void insertAfter_temp(t_ptr P,node N,int K,node N_){
	int i=0;
	while(P->ch[i] != N)
		i++;
	int j = P->ct;
	while(j>i){
		P->key[j] = P->key[j-1];
		P->ch[j+1] = P->ch[j];
		j--;
	}
	P->key[i] = K;
	P->ch[i+1] = N_;
	P->ct++;

}
node insert_inParent(node N,int K,node N_,node root){
	int j,k;
	if(N == root){
		node R = getnode();
		R->ch[0] = N;
		R->ch[1] = N_;
		R->key[0] = K;
		R->ct = 1;
		R->leaf = -1;
		R->par = NULL;
		N->par = R;
		N_->par = R;
		return R;
	}
	node P = N->par;
	if(P->ct < m-1){
		insertAfter(P,N,K,N_);
		return root;
	}
	else{
		t_ptr T = (t_ptr) malloc(sizeof(struct temp));
		for(j=0;j<P->ct;j++){
			T->key[j] = P->key[j];
			T->ch[j] = P->ch[j];
		}
		T->ch[j] = P->ch[j];
		T->ct = P->ct;
		insertAfter_temp(T,N,K,N_);
		node newP = getnode();
		newP->leaf = -1;
		newP->par = P->par;
		for(j=0;j<m/2;j++){
			P->key[j] = T->key[j];
			P->ch[j] = T->ch[j];
		}
		P->ch[j] = T->ch[j];
		P->ct = j;
		int newKey = T->key[j];
		j++;
		for(k=0;j<T->ct;j++,k++){
			newP->key[k] = T->key[j];
			newP->ch[k] = T->ch[j];
			newP->ch[k]->par = newP;
		}
		newP->ch[k] = T->ch[j];
		newP->ch[k]->par = newP;
		newP->ct = k;
		root = insert_inParent(P,newKey,newP,root);
		return root;
	}
}

node insert(node root,int data){
	node nn,ln,cur = root;
	int pos,i,j,newKey;
	if(root == NULL){
		nn = getnode();
		nn->key[0] = data;
		return nn;
	}
	else{
		ln  =  find(root,data);
		if(ln -> ct < m-1){
			insert_inLeaf(ln,data);
			return root;
		}
		else{
			//leaf is filled,so split it
			nn = getnode();
			t_ptr t = (t_ptr) malloc(sizeof(struct temp));
			nn->par = ln->par;
			for(i=0;i< (ln->ct);i++)
				t->key[i] = ln->key[i];
			t->ct = ln->ct;
			insert_inTemp(t,data);
			nn->ch[m-1] = ln->ch[m-1];
			ln->ch[m-1] = nn;
			for(i=0;i<min;i++)
				ln->key[i] = t->key[i];
			ln->ct = i;
			newKey = t->key[i];
		
			for(j=0; i<(t->ct);i++,j++)
				nn->key[j] = t->key[i];
			nn->ct = j;
			root = insert_inParent(ln,newKey,nn,root);
			return root;
		}
	}
}

void search(node root,int val){
	node cur = root;
	int i=0;
	while(cur->leaf != 1){
		i = 0;
		while(i<cur->ct && val >= cur->key[i])
			i++;
		cur = cur->ch[i];
	}
	for(i=0; i<cur->ct;i++){
		if(val == cur->key[i]){
			printf("Value %d is found ",val);
			return;
		}
	}
	printf("Value %d is not found",val);
}	

int isBplus_util(node root,int MIN,int MAX){
	int res=1,i=0,ct;
	if(root ==  NULL)
		return 1;
	ct = root->ct;
	if(root->key[0] < MIN || root->key[ct-1] >= MAX)
		return 0;
	res = isBplus_util(root->ch[0],MIN,root->key[i]);
	for(i = 1;i<ct;i++)
		res = res && isBplus_util(root->ch[i],root->key[i-1],root->key[i]);

	if(root->leaf != 1)
		res = res && isBplus_util(root->ch[ct],root->key[ct-1],MAX);
	return res;
}		
				
int isBplus(node root){
	return isBplus_util(root,INT_MIN,INT_MAX);
}

void main(){
	node root = NULL;
	int i,ch,item,num;
	while(1){
		printf("\n0.Display\n1.Insert\n2.search\n3.Verify\n4.exit\nEnter choice:");
		scanf("%d",&ch);
		switch(ch){
			case 0:
				display(root);
				break;
			case 1: {
				printf("Enter no of items\n");
				scanf("%d",&num);
				for(i=0;i<num;i++){
					//printf("Enter data\n");
					//scanf("%d",&item);
					root = insert(root,i);
				}
				break;
			}
			case 2:{
				printf("Enter data to be searched\n");
				scanf("%d",&item);
				search(root,item);
				break;
			}
			case 3:{
				if(isBplus(root) == 1)
					printf("Tree is correct");
				else
					printf("Tree is not correct");
				break;
			}
			case 4:
			exit(0);


		}
	}
}