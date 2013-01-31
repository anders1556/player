
#include "mysplit.h"
#include "ProgertyDlg.h"
#include "globalStuffs.h"



void MoveToNewRect(MYTREE *parent)
{
	for (MYTREE *cur=parent->getFirstSibling();cur;cur=cur->next)
	{
		if(cur->hasChild())
		{
			MoveToNewRect(cur->child);
		}
		else if (IsWindow(cur->data.hWnd))
		{
			RECT r=cur->getRect();
			MoveWindow(cur->data.hWnd,r.left,r.top,r.right-r.left,r.bottom-r.top,FALSE);
			ShowWindow(cur->data.hWnd,SW_SHOW);
			InvalidateRect(cur->data.hWnd,0,1);
		}
	}
}





MYTREE * MYTree_RemoveFromRoot(MYTREE *tree)
{
	if(tree->isroot())
		return tree;

	MYTREE *parent=tree->parent;

	//�����ǰֻ��һ���ֵܵĻ�
	//ɾ������,���ֵ��滻����
	/*
	if (tree->prev==NULL && tree->next!=NULL && tree->next->next==NULL)
	{
		tree->replaceParentWithBrother(tree->next);
		tree->parent->child=NULL;
	}
	else if (tree->next==NULL && tree->prev!=NULL && tree->prev->prev==NULL)
	{
		tree->replaceParentWithBrother(tree->prev);
		tree->parent->child=NULL;
	}
	else 
	*/
		
	if(tree->prev ==NULL)//��һ��?
	{
		tree->parent->child=tree->next;

		if(tree->next)
			tree->next->prev=NULL;
		delete tree;
	}
	else
	{
		tree->prev->next=tree->next;

		if(tree->next)
			tree->next->prev=tree->prev;
		delete tree;
	}


	return parent;
}



