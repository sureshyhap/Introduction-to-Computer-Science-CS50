#include <cmath>
#include <iostream>

template <typename T>
BinaryTree<T>::BinaryTree() : depth(-1) {
}

template <typename T>
BinaryTree<T>::BinaryTree(const BinaryTree& bt) {
  const int size = bt.tree.size();
  for (int i = 0; i < size; ++i) {
    Node* p = new Node(*(bt.tree[i]));
    tree.push_back(p);
  }
  depth = bt.depth;
}

template <typename T>
BinaryTree<T>::~BinaryTree() {
}

template <typename T>
typename BinaryTree<T>::BinaryTree& BinaryTree<T>::operator=(const BinaryTree& bt) {
  tree.clear();
  const int size = bt.tree.size();
  for (int i = 0; i < size; ++i) {
    Node* p = new Node(*(bt.tree[i]));
    tree.push_back(p);
  }
  depth = bt.depth;
}

template <typename T>
T& BinaryTree<T>::access(int level, int position) {
  try {
    int index = pow(2, level) - 1 + position;
    if (index >= tree.size() || tree[index] == nullptr) {
      throw "Not a valid position in tree. Either out of range or the node position is empty. "
"Returning root's data because must return something. Disregard return value.";
    }
    return tree[index]->data;
  }
  catch (const char* msg) {
    std::cerr << msg << std::endl;
  }
  if (!tree.empty()) {
    return tree[0]->data;
  }
}

template <typename T>
typename BinaryTree<T>::Node* BinaryTree<T>::search(const T& element) {
  int index = search_index(element);
  return index == -1 ? nullptr : tree[index];
}

template <typename T>
bool BinaryTree<T>::insert(const T& element, const T& parent_element, bool is_left) {
  if (tree.empty()) {
    Node* new_elem = new Node(element);
    tree.push_back(new_elem);
    ++depth;
    return true;
  }
  int index = search_index(parent_element);
  Node* par = index == -1 ? nullptr : tree[index];
  // Parent element is not in the tree
  if (par == nullptr) {
    return false;
  }
  // Parent element already has 2 children
  if (par->left_child != nullptr && par->right_child != nullptr) {
    return false;
  }
  // Already a child in its spot
  if (is_left && par->left_child != nullptr || !is_left && par->right_child != nullptr) {
    return false;
  }
  Node* new_elem = new Node(element, par);
  if (is_left) {
    par->left_child = new_elem;
  }
  else {
    par->right_child = new_elem;
  }
  int size = tree.size();
  // If the index of the parent corresponds to the deepest level
  if (index >= pow(2, depth) - 1 && index < size) {
    tree.resize(size + pow(2, ++depth), nullptr);
  }
  if (is_left) {
    // Element at 2 * index + 1 is left child of parent at index
    tree[2 * index + 1] = new_elem;
  }
  else {
    // Element at 2 * index + 2 is right child of parent at index
    tree[2 * index + 2] = new_elem;
  }
  return true;
}

template <typename T>
bool BinaryTree<T>::remove(const T& element) {
  if (tree.empty()) {
    return false;
  }
  int index = search_index(element);
  Node* elem = index == -1 ? nullptr : tree[index];
  if (elem == nullptr) {
    return false;
  }
  typename std::vector<Node*>::iterator it = tree.end();
  --it;
  for (; *it == nullptr; --it) {
    ;
  }
  elem->data = (*it)->data;
  Node* temp = *it;
  if ((*it)->parent != nullptr) {
    if (temp == (*it)->parent->left_child) {
      (*it)->parent->left_child = nullptr;
    }
    else if (temp == (*it)->parent->right_child) {
      (*it)->parent->right_child = nullptr;
    }
  }
  delete temp;
  *it = nullptr;  
  bool all_nulls_at_depth = true;
  int size = tree.size();
  for (int check_all_nulls_at_depth = pow(2, depth) - 1; check_all_nulls_at_depth < size; ++check_all_nulls_at_depth) {
    if (tree[check_all_nulls_at_depth] != nullptr) {
      all_nulls_at_depth = false;
      break;
    }
  }
  if (all_nulls_at_depth == true) {
    tree.resize(pow(2, depth--) - 1);
  }
  return true;
}

template <typename T>
void BinaryTree<T>::print_level_order() {
  int size = tree.size();
  for (int i = 0, pow2 = 1; i < size; ++i) {
    if (tree[i]) {
      std::cout << tree[i]->data << " ";
    }
    else {
      std::cout << T() << " ";
    }
    // Line break here
    if (i + 2 == int(pow(2, pow2))) {
      std::cout << std::endl;
      ++pow2;
    }
  }
}

template <typename T>
void BinaryTree<T>::print_pre_order() {
  if (!tree.empty()) {
    preorder(tree.front());
  }
}

template <typename T>
void BinaryTree<T>::print_post_order() {
  postorder(tree.front());
}

template <typename T>
void BinaryTree<T>::print_in_order() {
  inorder(tree.front());
}

template <typename T>
BinaryTree<T>::Node::Node(const T& data, Node* p, Node* l, Node* r) {
  this->data = data;
  parent = p;
  left_child = l;
  right_child = r;
}

template <typename T>
int BinaryTree<T>::search_index(const T& element) {
  const int size = tree.size();
  for (int i = 0; i < size; ++i) {
    if (tree[i] == nullptr) {
      continue;
    }
    if (tree[i]->data == element) {
      return i;
    }
  }
  // If not found, return -1
  return -1;
}

template <typename T>
void BinaryTree<T>::preorder(Node* subtree_root) {
  if (subtree_root == nullptr) {
    return;
  }
  std::cout << subtree_root->data << " ";
  preorder(subtree_root->left_child);
  preorder(subtree_root->right_child);
}

template <typename T>
void BinaryTree<T>::postorder(Node* subtree_root) {
  if (subtree_root == nullptr) {
    return;
  }
  postorder(subtree_root->left_child);
  postorder(subtree_root->right_child);
  std::cout << subtree_root->data << " ";
}

template <typename T>
void BinaryTree<T>::inorder(Node* subtree_root) {
  if (subtree_root == nullptr) {
    return;
  }
  inorder(subtree_root->left_child);
  std::cout << subtree_root->data << " ";
  inorder(subtree_root->right_child);
}
