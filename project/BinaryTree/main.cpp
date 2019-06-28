#include "BinaryTree.h"

int main(int argc, char* argv[]) {
  BinaryTree<int> b;
  char ch;
  int element, parent;
  char is_left;
  std::cout << "Enter operations to be performed on a binary tree.\nThe number 0 is a visual placeholder so don't insert 0s.\nInsert requires a number, a parent number, and whether or not it will be a left child.\nSearch returns whether or not the number is in the tree.\nRemove replaces the element with the last element in the tree (unless it is the only element).\nWhen printing out the tree I did not make it look like an isosceles triangle because if the user entered a very large number it would throw off the shape.\nSo the tree does not look like what you might usually think of as a binary tree.\n\n";
  while (true) {
    std::cout << "Type i to insert an element, s to search whether an element is in the tree, r to remove an element, p to print in level order, or something else to quit: ";
    std::cin >> ch;
    std::cout << std::endl;
    if (ch == 'i' || ch == 's' || ch == 'r') {
      std::cout << "Enter a nonzero number: ";
      std::cin >> element;
      std::cout << std::endl;
      if (ch == 'i') {
	std::cout << "Enter its parent (0 if it will be the root): ";
	std::cin >> parent;
	std::cout << std::endl;
	std::cout << "Is it a left child? (y or n) (Enter either if root): ";
	std::cin >> is_left;
	std::cout << std::endl;
	if (is_left == 'y') {
	  b.insert(element, parent);
	}
	else {
	  b.insert(element, parent, false);
	}
	std::cout << std::endl;
      }
      else if (ch == 's') {
	std::cout << (b.search(element) == nullptr ? "Not in tree\n" : "In tree\n");
	std::cout << std::endl;
      }
      else if (ch == 'r') {
	b.remove(element);
	std::cout << std::endl;
      }
    }
    else if (ch == 'p') {
      b.print_level_order();
      std::cout << std::endl;
    }
    else {
      break;
    }
  }
  return 0;
}
