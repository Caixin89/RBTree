#ifndef __RBTREE_H
#define __RBTREE_H

#include <cstddef>
#include <ostream>
#include <string>

using namespace std;

enum COLOUR_TYPE { RED, BLACK, BLACK_BLACK };
enum SIDE { LEFT = true, RIGHT = false };

template <typename T, typename U>
class RBTree {
	class Node {
	private:
		T key;
		U val;
		COLOUR_TYPE colour;
		Node *left, *right;
	public:
		Node(T u, U v, COLOUR_TYPE x, Node *y, Node *z) : key(u), val(v), colour(x), left(y), right(z) {}
		Node(COLOUR_TYPE x, Node *y, Node *z) : colour(x), left(y), right(z) {}
		U getVal() const { return val; }
		T getKey() const { return key; }
		COLOUR_TYPE getColour() const { return colour; }
		void setKey(T x) { key = x; }
		void setVal(U x) { val = x; }
		void setColour(COLOUR_TYPE x) { colour = x; }
		Node* getChild(SIDE x) const { return x == LEFT ? left : right; }
		void setChild(SIDE x, Node *y) {
			if (x == LEFT)
				left = y;
			else
				right = y;
		}
	};

private:
	unsigned int size;
	Node *TNIL, *root;

	Node* insert(Node *x, Node *y, bool &result) {
		if (x == TNIL)
			return y;

		if (y->getKey() == x->getKey()) {
			x->setVal(y->getVal());
			result = false;
			return x;
		}

		SIDE dir, dir2;
		Node *dir_node, *dir2_node;
		dir = y->getKey() < x->getKey() ? LEFT : RIGHT;
		dir_node = insert(x->getChild(dir), y, result);
		x->setChild(dir, dir_node);
		dir2 = y->getKey() > dir_node->getKey() ? RIGHT : LEFT;
		dir2_node = x->getChild(dir2);

		//Rebalancing
		if (dir_node->getColour() == RED && dir2_node->getColour() == RED)  {
			if (x->getChild(SIDE(!bool(dir)))->getColour() == RED) {
				//Case 1
				x->getChild(LEFT)->setColour(BLACK);
				x->getChild(RIGHT)->setColour(BLACK);
				x->setColour(RED);
			}
			else if (dir == dir2) {
				if (dir == RIGHT) {
					//Case 2 normal
					dir_node->setColour(BLACK);
					x->setColour(RED);
					x = rotateLeft(x);
				}
				else {
					//Case 2 inverted
					dir_node->setColour(BLACK);
					x->setColour(RED);
					x = rotateRight(x);
				}
			}
			else {
				if (dir == RIGHT) {
					//Case 3 normal
					dir2_node->setColour(BLACK);
					x->setColour(RED);
					x->setChild(RIGHT, rotateRight(dir_node));
					x = rotateLeft(x);
				}
				else {
					//Case 3 inverted
					dir2_node->setColour(BLACK);
					x->setColour(RED);
					x->setChild(LEFT, rotateLeft(dir_node));
					x = rotateRight(x);
				}
			}
		}

		return x;
	}

	Node* remove(Node *x, T y, Node* found, bool &result) {
		if (x == TNIL) {
			result = false;
			return TNIL;
		}

		if (x->getKey() == y) {
			found = x;

			if (x->getChild(LEFT) == TNIL && x->getChild(RIGHT) == TNIL) {
				TNIL->setColour(COLOUR_TYPE(int(TNIL->getColour()) + int(x->getColour())));
				delete x;
				x = NULL;
				return TNIL;
			}

			if (x->getChild(LEFT) != TNIL && x->getChild(RIGHT) == TNIL) {
				Node *tmp = x->getChild(LEFT);
				tmp->setColour(COLOUR_TYPE(int(tmp->getColour()) + int(x->getColour())));
				delete x;
				x = NULL;
				return tmp;
			}

			if (x->getChild(RIGHT) != TNIL && x->getChild(LEFT) == TNIL) {
				Node *tmp = x->getChild(RIGHT);
				tmp->setColour(COLOUR_TYPE(int(tmp->getColour()) + int(x->getColour())));
				delete x;
				x = NULL;
				return tmp;
			}
		}
		else if (found != NULL && x->getChild(RIGHT) == TNIL) {
			found->setKey(x->getKey());
			found->setVal(x->getVal());
			Node *tmp = x->getChild(LEFT);
			tmp->setColour(COLOUR_TYPE(int(tmp->getColour()) + int(x->getColour())));
			delete x;
			x = NULL;
			return tmp;
		}

		SIDE dir = x->getKey() >= y ? LEFT : RIGHT;
		x->setChild(dir, remove(x->getChild(dir), y, found, result));
		SIDE opp_dir = SIDE(!bool(dir));
		Node *c = x->getChild(dir), *s = x->getChild(opp_dir);

		if (c->getColour() != BLACK_BLACK)
			return x;

		Node *tmp = NULL;

		//Case 1
		if (s->getColour() == RED) {
			if (dir == LEFT) {
				x->setColour(RED);
				s->setColour(BLACK);
				tmp = rotateLeft(x);
			}
			else {
				x->setColour(RED);
				s->setColour(BLACK);
				tmp = rotateRight(x);
			}

			s = x->getChild(opp_dir);
		}

		//Case 2
		if (s->getChild(LEFT)->getColour() != RED && s->getChild(RIGHT)->getColour() != RED && s->getColour() != RED) {
			s->setColour(RED);
			c->setColour(BLACK);
			x->setColour(COLOUR_TYPE(int(x->getColour()) + 1));
		}
		else if (s->getChild(opp_dir)->getColour() != RED && s->getColour() != RED && s->getChild(dir)->getColour() == RED) {
			//Case 3 normal
			if (dir == LEFT) {
				s->getChild(dir)->setColour(x->getColour());
				x->setColour(BLACK);
				c->setColour(BLACK);
				x->setChild(RIGHT, rotateRight(s));

				if (tmp != NULL)
					tmp->setChild(dir, rotateLeft(x));
				else
					x = rotateLeft(x);
			}
			//Case 3 inverted
			else {
				s->getChild(dir)->setColour(x->getColour());
				x->setColour(BLACK);
				c->setColour(BLACK);
				x->setChild(LEFT, rotateLeft(s));

				if (tmp != NULL)
					tmp->setChild(dir, rotateRight(x));
				else
					x = rotateRight(x);
			}
		}
		//Case 4
		else if (s->getColour() != RED && s->getChild(opp_dir)->getColour() == RED) {
			if (dir == LEFT) {
				s->setColour(x->getColour());
				x->setColour(BLACK);
				c->setColour(BLACK);
				s->setColour(RED);
				s->getChild(opp_dir)->setColour(RED);

				if (tmp != NULL)
					tmp->setChild(dir, rotateLeft(x));
				else
					x = rotateLeft(x);
			}
			else {
				s->setColour(x->getColour());
				x->setColour(BLACK);
				c->setColour(BLACK);
				s->setColour(RED);
				s->getChild(opp_dir)->setColour(RED);

				if (tmp != NULL)
					tmp->setChild(dir, rotateRight(x));
				else
					x = rotateRight(x);
			}
		}

		if (tmp != NULL)
			return tmp;

		return x;

	}

	//Rotates left about subtree rooted x and returns the node that takes on x's position
	Node* rotateLeft(Node *x) {
		Node *tmp = x->getChild(RIGHT);
		x->setChild(RIGHT, tmp->getChild(LEFT));
		tmp->setChild(LEFT, x);
		return tmp;
	}

	//Rotates right about subtree rooted x and returns the node that takes on x's position
	Node* rotateRight(Node *x) {
		Node *tmp = x->getChild(LEFT);
		x->setChild(LEFT, tmp->getChild(RIGHT));
		tmp->setChild(RIGHT, x);
		return tmp;
	}


	Node* search(T x) const {
		Node *curr = root;

		while (curr != TNIL && curr->getKey() != x) {
			if (x > curr->getKey())
				curr = curr->getChild(RIGHT);
			else
				curr = curr->getChild(LEFT);
		}

		return curr;
	}

	void recurrPrint(ostream &stream, Node *x, string tabspace)
	{
		if (x != TNIL)
		{
			recurrPrint(stream, x->getChild(RIGHT), tabspace + "\t");
			stream << endl << tabspace << '(' << x->getKey() << ", " << x->getVal() << ", " << (x->getColour() == RED ? "r)" : "b)") << endl;
			recurrPrint(stream, x->getChild(LEFT), tabspace + "\t");
		}
	}

public:
	RBTree() : size(0) {
		root = TNIL = new Node(BLACK, NULL, NULL);
	}

	//Returns true if no such key exists, else replace current value with new value and returns false
	bool insert(T x, U y) {
		bool result = true;
		root = insert(root, new Node(x, y, RED, TNIL, TNIL), result);
		root->setColour(BLACK);
		if (result) ++size;
		return result;
	}

	//Returns true if such key exists, else returns false
	bool remove(T x) {
		bool result = true;
		root = remove(root, x, NULL, result);
		root->setColour(BLACK);
		if (result) --size;
		return result;
	}

	bool minimum(T &x, U &y) const {
		if (size == 0)
			return false;

		Node *curr = root;

		while (curr->getChild(LEFT) != TNIL) {
			curr = curr->getChild(LEFT);
		}

		x = curr->getKey();
		y = curr->getVal();
		return true;
	}

	bool maximum(T &x, U &y) const {
		if (size == 0)
			return false;

		Node *curr = root;

		while (curr->getChild(RIGHT) != TNIL) {
			curr = curr->getChild(RIGHT);
		}

		x = curr->getKey();
		y = curr->getVal();
		return true;
	}

	unsigned int getSize() const { return size; }

	bool search(T x, U &y) const {
		Node *node = search(x);

		if (node == TNIL)
			return false;

		y = node->getVal();
		return true;
	}

	void print(ostream& stream)
	{
		recurrPrint(stream, root, "");
	}
};

#endif
