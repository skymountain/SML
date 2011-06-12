#ifndef _SML_RED_BLACK_TREE_HPP
#define _SML_RED_BLACK_TREE_HPP

#include <functional>
#include <iterator>
#include <memory>
#include <utility>
#include <cstddef>
#include "sml/ext/memory.hpp"
#include "sml/ext/tuple.hpp"
#include "sml/utility/noncopyable.hpp"

namespace sml {

template<
  class Key,
  class T,
  class Lesser    = std::less<Key>,
  class Allocator = std::allocator< std::pair<const Key, T> >
>
class red_black_tree {
private:
  class node;
  template <class _V, class _N> class node_iterator;

  typedef sml::ext::shared_ptr<node>       node_type;
  typedef sml::ext::shared_ptr<const node> const_node_type;

public:
  typedef Key                     key_type;
  typedef T                       mapped_type;
  typedef std::pair<const Key, T> value_type;
  typedef Lesser                  key_compare;

  typedef Allocator                           allocator_type;
  typedef typename Allocator::reference       reference;
  typedef typename Allocator::const_reference const_reference;
  typedef typename Allocator::pointer         pointer;
  typedef typename Allocator::const_pointer   const_pointer;

  typedef std::size_t    size_type;
  typedef std::ptrdiff_t difference_type;

  typedef node_iterator<value_type, node_type>  iterator;
  typedef node_iterator<const value_type, const_node_type> const_iterator;
  typedef std::reverse_iterator<iterator>       reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

private:
  typedef sml::ext::weak_ptr<node>                          parent_node_type;
  typedef sml::ext::tuple<node_type, node_type, node_type*> find_result_type;

  typedef
    typename allocator_type::template rebind<node>::other   node_allocator_type;
  typedef typename node_allocator_type::pointer             node_ptr;

  class node :
    public sml::ext::enable_shared_from_this<node> {

  private:
    class node_deleter {
    public:
      node_deleter(node_allocator_type& allocator) :
        allocator_(allocator) {
      }

      void operator()(const node_ptr np) {
        this->allocator_.destroy(np);
        this->allocator_.deallocate(np, 1);
      }

    private:
      node_allocator_type& allocator_;
    };

  public:
    // copy constructor is public for Allocator::construct
    node(const node& r) :
      red_(r.red_),
      value_(r.value_),
      root_(r.root_),
      parent_(r.parent_),
      left_(r.left_),
      right_(r.right_) {
    }

    value_type& value() { return this->value_; }
    const value_type& value() const { return this->value_; }

    const key_type& key() const { return this->value().first; }

    mapped_type& mapped() { return this->value().second; }
    const mapped_type& mapped() const { return this->value().second; }

    node_type parent() { return this->parent_.lock(); }
    const_node_type parent() const { return this->parent_.lock(); }

    node_type left() { return this->left_; }
    const_node_type left() const { return this->left_; }

    node_type right() { return this->right_; }
    const_node_type right() const { return this->right_; }

    node_type next() {
      return utility::next(this->shared_from_this());
    }

    const_node_type next() const {
      return utility::next(this->shared_from_this());
    }

    node_type prior() {
      return utility::prior(this->shared_from_this());
    }

    const_node_type prior() const {
      return utility::prior(this->shared_from_this());
    }

    node_type left_most_descendant() {
      return utility::left_most_descendant(this->shared_from_this());
    }

    const_node_type left_most_descendant() const {
      return utility::left_most_descendant(this->shared_from_this());
    }

    node_type right_most_descendant() {
      return utility::right_most_descendant(this->shared_from_this());
    }

    const_node_type right_most_descendant() const {
      return utility::right_most_descendant(this->shared_from_this());
    }

    void rednize()       { this->red_ = true;  }
    void blacknize()     { this->red_ = false; }

    bool is_root() const { return this->parnet_; }
    bool red()     const { return this->red_; }
    bool black()   const { return !this->red(); }

    node_type right_brother() { return this->parent_.lock()->right_; }
    node_type left_brother()  { return this->parent_.lock()->left_;  }

    void right_rotate() {
      const node_type me         = this->shared_from_this();
      const node_type parent     = this->parent_.lock();
      const node_type left       = this->left_;
      const node_type left_right = left->right_;

      me->parent_   = left;
      me->left_     = left_right;

      left->parent_ = parent;
      left->right_  = me;

      if (left_right) {
        left_right->parent_ = me;
      }

      this->parent_reset(me, parent, left);
    }

    void left_rotate() {
      const node_type me         = this->shared_from_this();
      const node_type parent     = this->parent_.lock();
      const node_type right      = this->right_;
      const node_type right_left = right->left_;

      me->parent_    = right;
      me->right_     = right_left;

      right->parent_ = parent;
      right->left_   = me;

      if (right_left) {
        right_left->parent_ = me;
      }

      this->parent_reset(me, parent, right);
    }

  private:
    friend class sml::red_black_tree<Key, T, Lesser, Allocator>;

    node(
      const bool        red,
      const node_type&  parent,
      const value_type& value,
      node_type&        root
    ) :
      red_(red),
      value_(value),
      root_(root),
      parent_(parent),
      left_(),
      right_() {
    }

    void parent_reset(
      const node_type& me,
      const node_type& parent,
      const node_type& node
    ) {
      if (!parent) this->root_ = node;
      else if (parent->left_ == me) parent->left_ = node;
      else parent->right_ = node;
    }

    bool       red_;
    value_type value_;
    node_type& root_;
    node_type  left_;
    node_type  right_;
    parent_node_type parent_;

  public:
    static node_type create_root(
      node_allocator_type& allocator,
      const value_type&    value,
      node_type&           root
    ) {
      node_ptr np = allocator.allocate(1);
      allocator.construct(np, node(false, node_type(), value, root));
      return node_type(np, node_deleter(allocator));
    }

    static node_type create_node(
      node_allocator_type& allocator,
      const node_type&     parent,
      const value_type&    value,
      node_type&           root
    ) {
      node_ptr np = allocator.allocate(1);
      allocator.construct(np, node(true, parent, value, root));
      return node_type(np, node_deleter(allocator));
    }

    static node_type clone_node(
      node_allocator_type& allocator,
      const node_type&     src,
      node_type&           root,
      key_compare&         lesser
    ) {
      if (!src) return node_type();

      node_ptr np = allocator.allocate(1);
      allocator.construct(np, node(src->red_, node_type(), src->value(), root));

      node_type dst   = node_type(np, node_deleter(allocator));
      node_type left  = clone_node(allocator, src->left_,  root, lesser);
      node_type right = clone_node(allocator, src->right_, root, lesser);

      dst->left_  = left;
      dst->right_ = right;

      if (left)  left->parent_  = dst;
      if (right) right->parent_ = dst;

      return dst;
    }
  };

  // NOTE that value_type is V in node_iterator
  template<class V, class N>
  class node_iterator :
    public std::iterator<std::bidirectional_iterator_tag, V> {

  public:
    typedef V value_type;

  private:
    typedef N node_type;

  public:
    node_iterator() :
      right_most_(),
      node_() {
    }

    node_iterator(const node_iterator& r) :
      right_most_(r.right_most_),
      node_(r.node_) {
    }

    value_type& operator*()  const { return this->node_->value(); }
    value_type* operator->() const { return &(node_->value()); }

    node_iterator& operator=(const node_iterator& r) {
      this->right_most_ = r.right_most_;
      this->node_       = r.node_;
      return *this;
    }

    node_iterator& operator++() {
      this->node_ = this->node_->next();
      return *this;
    }

    // don't return const values to conform with standard iterators such as GCC
    node_iterator operator++(int) {
      node_iterator ret = *this;
      this->operator++();
      return ret;
    }

    node_iterator& operator--() {
      this->node_ = this->node_ ? this->node_->prior() : this->right_most_;
      return *this;
    }

    // don't return const values for same reason as operator ++
    node_iterator operator--(int) {
      node_iterator ret = *this;
      this->operator--();
      return ret;
    }

    bool operator==(const node_iterator& r) const {
      return this->node_ == r.node_;
    }

    bool operator!=(const node_iterator& r) const {
      return !(*this == r);
    }

  private:
    friend class sml::red_black_tree<Key, T, Lesser, Allocator>;

    explicit node_iterator(const red_black_tree& rb_tree) :
      right_most_(rb_tree.right_most_),
      node_() {
    }

    node_iterator(
      const red_black_tree& rb_tree,
      const node_type& node
    ) :
      right_most_(rb_tree.right_most_),
      node_(node) {
    }

    node_type right_most_;
    node_type node_;
  };

  struct utility {
    template<class N>
    static sml::ext::shared_ptr<N> left_most_descendant(
      sml::ext::shared_ptr<N> node
    ) {
      while (node->left()) node = node->left();
      return node;
    }

    template<class N>
    static sml::ext::shared_ptr<N> right_most_descendant(
      sml::ext::shared_ptr<N> node
    ) {
      while (node->right()) node = node->right();
      return node;
    }

    template<class N>
    static sml::ext::shared_ptr<N> next(sml::ext::shared_ptr<N> node) {
      typedef sml::ext::shared_ptr<N> node_type;

      if (node->right()) {
        return node->right()->left_most_descendant();
      }
      else {
        node_type parent = node->parent();
        if (parent) {
          while (parent && parent->right() == node) {
            node   = parent;
            parent = parent->parent();
          }
          return parent;
        }
      }

      return node_type();
    }

    template<class N>
    static sml::ext::shared_ptr<N> prior(sml::ext::shared_ptr<N> node) {
      typedef sml::ext::shared_ptr<N> node_type;

      if (node->left()) {
        return node->left()->right_most_descendant();
      }
      else {
        node_type parent = node->parent();
        if (parent) {
          while (parent && parent->left() == node) {
            node  = parent;
            parent = parent->parent();
          }
          return parent;
        }
      }

      return node_type();
    }

    template<class N>
    static sml::ext::shared_ptr<N> lower_bound(
      sml::ext::shared_ptr<N> node,
      const key_type&         key,
      key_compare             lesser
    ) {
      sml::ext::shared_ptr<N> lower;
      while (node != NULL) {
        if (lesser(node->key(), key)) {
          node = node->right();
        }
        else {
          lower = node;
          node  = node->left();
        }
      }
      return lower;
    }

    template<class N>
    static sml::ext::shared_ptr<N> upper_bound(
      sml::ext::shared_ptr<N> node,
      const key_type&         key,
      key_compare             lesser
    ) {
      sml::ext::shared_ptr<N> upper;
      while (node != NULL) {
        if (lesser(key, node->key())) {
          upper = node;
          node = node->left();
        }
        else {
          node = node->right();
        }
      }
      return upper;
    }

    template<class N>
    static std::pair<
      sml::ext::shared_ptr<N>,
      sml::ext::shared_ptr<N>
    > equal_range(
      sml::ext::shared_ptr<N> node,
      const key_type&         key,
      key_compare             lesser
    ) {
      typedef sml::ext::shared_ptr<N> node_type;

      const node_type lower = lower_bound(node, key, lesser);
      const node_type upper = !lower || lesser(key, lower->key()) ?
        lower : lower->next();

      return std::make_pair(lower, upper);
    }

    template<class N>
    static sml::ext::shared_ptr<N> find(
      sml::ext::shared_ptr<N> node,
      const key_type&         key,
      key_compare             lesser
    ) {
      typedef sml::ext::shared_ptr<N> node_type;

      const node_type lower = lower_bound(node, key, lesser);

      return !lower || lesser(key, lower->key()) ?
        node_type() : lower;
    }
  };

public:
  class value_compare :
    public std::binary_function<value_type, value_type, bool> {

  public:
    bool operator()(const value_type& x, const value_type& y) const {
      return this->lesser_(x.first, y.first);
    }

    bool operator()(const value_type& x, const value_type& y) {
      return this->lesser_(x.first, y.first);
    }

  private:
    friend class red_black_tree;

    value_compare(const key_compare& lesser) : lesser_(lesser) {}

    key_compare lesser_;
  };

public:
  explicit red_black_tree(
    const key_compare&    lesser    = key_compare(),
    const allocator_type& allocator = allocator_type()
  ) :
    root_(),
    left_most_(),
    right_most_(),
    size_(0),
    lesser_(lesser),
    allocator_(allocator) {
  }

  template<class InputIterator>
  red_black_tree(
    InputIterator         first,
    InputIterator         last,
    const key_compare&    lesser    = key_compare(),
    const allocator_type& allocator = allocator_type()
  ) :
    root_(),
    left_most_(),
    right_most_(),
    size_(0),
    lesser_(lesser),
    allocator_(allocator) {

    this->insert_seq(first, last);
  }

  red_black_tree(const red_black_tree& r) :
    root_(),
    left_most_(),
    right_most_(),
    size_(r.size_),
    lesser_(r.lesser_),
    allocator_(r.allocator_) {

    this->copy_node(r);
  }

  iterator begin() { return iterator(*this, this->left_most_); }
  const_iterator begin() const {
    return const_iterator(*this, this->left_most_);
  }

  iterator end() { return iterator(*this); }
  const_iterator end() const {
    return const_iterator(*this);
  }

  reverse_iterator rbegin() { return reverse_iterator(this->end()); }
  const_reverse_iterator rbegin() const {
    return const_reverse_iterator(this->end());
  }

  reverse_iterator rend() { return reverse_iterator(this->begin()); }
  const_reverse_iterator rend() const {
    return const_reverse_iterator(this->begin());
  }

  bool      empty()    const { return !this->root_; }
  size_type size()     const { return this->size_; }
  size_type max_size() const { return this->allocator_.max_size(); }

  key_compare    key_comp()      const { return this->lesser_; }
  value_compare  value_comp()    const { return value_compare(this->lesser_); }
  allocator_type get_allocator() const { return this->allocator_; }

  std::pair<iterator, bool> insert(const value_type& value) {
    return this->_insert(value, this->find_inserted_pos(value.first));
  }

  iterator insert(iterator pos, const value_type& value) {
    return this->_insert(value, pos).first;
  }

  template<class InputIterator>
  void insert(InputIterator first, InputIterator last) {
    this->insert_seq(first, last);
  }

  size_type erase(const key_type& key) {
    return this->_erase(this->find(key));
  }

  void erase(iterator pos) {
    this->_erase(pos);
  }

  void erase(iterator first, iterator last) {
    this->erase_seq(first, last);
  }

  size_type count(const key_type& key) const {
    return this->find(key) == this->end() ? 0 : 1;
  }

  iterator find(const key_type& key) {
    return iterator(*this, utility::find(this->root(), key, this->lesser_));
  }

  const_iterator find(const key_type& key) const {
    return const_iterator(
      *this,
      utility::find(this->root(), key, this->lesser_)
    );
  }

  iterator lower_bound(const key_type& key) {
    return iterator(
      *this,
      utility::lower_bound(this->root(), key, this->lesser_)
    );
  }

  const_iterator lower_bound(const key_type& key) const {
    return const_iterator(
      *this,
      utility::lower_bound(this->root(), key, this->lesser_)
    );
  }

  iterator upper_bound(const key_type& key) {
    return iterator(
      *this,
      utility::upper_bound(this->root(), key, this->lesser_)
    );
  }

  const_iterator upper_bound(const key_type& key) const {
    return const_iterator(
      *this,
      utility::upper_bound(this->root(), key, this->lesser_)
    );
  }

  std::pair<iterator, iterator> equal_range(const key_type& key) {
    const std::pair<node_type, node_type> range =
      utility::equal_range(this->root(), key, this->lesser_);

    return std::make_pair(
      iterator(*this, range.first),
      iterator(*this, range.second)
    );
  }

  std::pair<const_iterator, const_iterator> equal_range(
    const key_type& key
  ) const {
    const std::pair<const_node_type, const_node_type> range =
      utility::equal_range(this->root(), key, this->lesser_);

    return std::make_pair(
      const_iterator(*this, range.first),
      const_iterator(*this, range.second)
    );
  }

  void clear() {
    this->root_.reset();
    this->left_most_.reset();
    this->right_most_.reset();
    this->size_ = 0;
  }

  void swap(red_black_tree& r) {
    using std::swap;

    this->root_.swap(r.root_);
    this->left_most_.swap(r.left_most_);
    this->right_most_.swap(r.right_most_);

    swap(this->size_,      r.size_);
    swap(this->lesser_,    r.lesser_);
    swap(this->allocator_, r.allocator_);
  }

  red_black_tree& operator=(const red_black_tree& r) {
    if (this != &r) {
      this->clear();

      this->lesser_ = r.lesser_;
      this->size_   = r.size_;
      this->copy_node(r);
    }

    return *this;
  }

  mapped_type& operator[](const key_type& key) {
    const find_result_type res = this->find_inserted_pos(key);
    const node_type target = sml::ext::get<1>(res);
    return target ?
      target->mapped() :
      this->_insert(make_pair(key, mapped_type()), res).first->second;
  }

private:
  node_type root() { return this->root_; }
  const_node_type root() const { return this->root_; }

  find_result_type find_inserted_pos(const key_type& target) {
    node_type node = this->root();

    if (!node) {
      return sml::ext::make_tuple(
        node_type(),
        node_type(),
        static_cast<node_type*>(NULL)
      );
    }

    node_type parent    = node->parent_.lock();
    node_type* inserted = NULL;

    while (node != NULL) {
      const key_type& key = node->key();
      parent = node;

      if (this->lesser_(target, key)) {
        inserted = &node->left_;
        node = node->left_;
      }
      else if (this->lesser_(key, target)) {
        inserted = &node->right_;
        node = node->right_;
      }
      else {
        return sml::ext::make_tuple(
          node->parent_.lock(),
          node,
          static_cast<node_type*>(NULL)
        );
      }
    }

    return sml::ext::make_tuple(parent, node, inserted);
  }
  
  template<class InputIterator>
  void insert_seq(InputIterator first, InputIterator last) {
    for (; first != last; ++first) {
      this->insert_to_right_most_if_can(*first);
    }
  }

  std::pair<iterator, bool> insert_to_right_most_if_can(
    const value_type& value
  ) {
    if (
      !this->empty() &&
      this->lesser_(this->right_most_->key(), value.first)
    ) {
      return this->_insert(
        value,
        this->right_most_,
        node_type(),
        &this->right_most_->right_
      );
    }
    else {
      return this->insert(value);
    }
  }

  std::pair<iterator, bool> _insert(const value_type& value, iterator pos) {
    if (this->empty()) {
      return this->insert(value);
    }

    const node_type node = pos.node_;
    if (!node) {
      return this->insert_to_right_most_if_can(value);
    }
    else if (this->lesser_(value.first, node->key())) {
      if (node == this->left_most_) {
        return this->_insert(
          value,
          this->left_most_,
          node_type(),
          &this->left_most_->left_
        );
      }
      else {
        iterator prior_pos = pos; --prior_pos;
        const node_type prior = prior_pos.node_;
        if (this->lesser_(prior->key(), value.first)) {
          if (!prior->right_) {
            return this->_insert(value, prior, node_type(), &prior->right_);
          }
          else {
            return this->_insert(value, node, node_type(), &node->left_);
          }
        }
      }
    }

    return this->insert(value);
  }

  std::pair<iterator, bool> _insert(
    const value_type&       value,
    const find_result_type& res
  ) {
    return this->_insert(
      value,
      sml::ext::get<0>(res),
      sml::ext::get<1>(res),
      sml::ext::get<2>(res)
    );
  }

  std::pair<iterator, bool> _insert(
    const value_type& value,
    const node_type   parent,
    const node_type   target,
    node_type* const  inserted
  ) {
    if (target) {
      target->mapped() = value.second;
      return std::make_pair(iterator(*this, target), false);;
    }

    node_type iter_node;

    if (!parent) {
      iter_node = this->root_ = this->left_most_ = this->right_most_ =
        node::create_root(this->allocator_, value, this->root_);
    }
    else {
      iter_node = *inserted =
        node::create_node(this->allocator_, parent, value, this->root_);

      if (this->lesser_(value.first, this->left_most_->key())) {
        this->left_most_ = iter_node;
      }
      else if (this->lesser_(this->right_most_->key(), value.first)) {
        this->right_most_ = iter_node;
      }

      this->insert_fixup(iter_node);
    }

    ++this->size_;
    return std::make_pair(iterator(*this, iter_node), true);
  }

  void insert_fixup(node_type node) {
    for (node_type parent = node->parent_.lock();
         parent && parent->red();
         parent = node->parent_.lock()) {

      node = (parent->left_brother() == parent) ?
        insert_fixup_left_uncle(node) :
        insert_fixup_right_uncle(node);
    }

    this->root_->blacknize();
  }

  node_type insert_fixup_left_uncle(node_type node) {
    node_type parent      = node->parent_.lock();
    node_type grandparent = parent->parent_.lock();
    const node_type right_uncle = parent->right_brother();

    if (right_uncle && right_uncle->red()) {
      parent->blacknize();
      right_uncle->blacknize();
      grandparent->rednize();
      node = grandparent;
    }
    else {
      if (node == node->right_brother()) {
        node = parent;
        node->left_rotate();

        parent = node->parent_.lock();
        grandparent = parent->parent_.lock();
      }
      parent->blacknize();
      grandparent->rednize();
      grandparent->right_rotate();
    }

    return node;
  }

  node_type insert_fixup_right_uncle(node_type node) {
    node_type parent      = node->parent_.lock();
    node_type grandparent = parent->parent_.lock();
    const node_type left_uncle = parent->left_brother();

    if (left_uncle && left_uncle->red()) {
      parent->blacknize();
      left_uncle->blacknize();
      grandparent->rednize();
      node = grandparent;
    }
    else {
      if (node == node->left_brother()) {
        node = parent;
        node->right_rotate();

        parent = node->parent_.lock();
        grandparent = parent->parent_.lock();
      }
      parent->blacknize();
      grandparent->rednize();
      grandparent->left_rotate();
    }

    return node;
  }

  void erase_seq(iterator first, iterator last) {
    if (first == this->begin() && last == this->end()) {
      this->clear();
    }
    else {
      while (first != last) {
        this->erase(first++);
      }
    }
  }

  size_type _erase(iterator pos) {
    const node_type z = pos.node_;
    if (!z) return 0;

    const node_type y = !z->left_ || !z->right_ ? z : z->next();
    const node_type x = y->left_ ? y->left_ : y->right_;
    node_type y_parent = y->parent_.lock();
    
    if (x) x->parent_ = y_parent;

    if (!y_parent) {
      this->root_ = x;
    }
    else if (y == y->left_brother()) {
      y_parent->left_ = x;
    }
    else {
      y_parent->right_ = x;
    }

    const bool y_black = y->black();

    if (y != z) {
      y->parent_ = z->parent_;
      y->left_   = z->left_;
      y->right_  = z->right_;
      y->red_    = z->red_;
      y_parent   = y->parent_.lock();

      if (!y_parent) {
        this->root_ = y;
      }
      else if (z == z->left_brother()) {
        y_parent->left_ = y;
      }
      else {
        y_parent->right_ = y;
      }

      if (y->left_)  y->left_->parent_  = y;
      if (y->right_) y->right_->parent_ = y;
    }

    if (y_black) this->erase_fixup(x);

    if (this->left_most_  == z) this->left_most_  = z->next();
    if (this->right_most_ == z) this->right_most_ = z->prior();

    --this->size_;
    return 1;
  }

  void erase_fixup(node_type x) {
    while (x && !x->parent_.expired() && x->black()) {
      x = x == x->left_brother() ?
        this->erase_fixup_left_brother(x) :
        this->erase_fixup_right_brother(x);
    }
    if (x) x->blacknize();
  }

  node_type erase_fixup_left_brother(const node_type x) {
    node_type parent = x->parent_.lock();
    node_type w = parent->right_;

    if (w->red()) {
      w->blacknize();
      parent->rednize();
      parent->left_rotate();

      parent = x->parent_.lock();
      w = parent->right_;
    }

    node_type right_w       = w->right_;
    const node_type left_w  = w->left_;
    const bool wleft_black  = !left_w  || left_w->black();
    const bool wright_black = !right_w || right_w->black();

    if (wleft_black && wright_black) {
      w->rednize();
      return parent;
    }

    if (wright_black) {
      if (left_w) left_w->blacknize();
      w->rednize();
      w->right_rotate();
      w = parent->right_;
      right_w = w->right_;
    }

    w->red_ = parent->red_;
    parent->blacknize();
    if (right_w) right_w->blacknize();
    parent->left_rotate();

    return this->root_;
  }

  node_type erase_fixup_right_brother(const node_type x) {
    node_type parent = x->parent_.lock();
    node_type w = parent->left_;

    if (w->red()) {
      w->blacknize();
      parent->rednize();
      parent->right_rotate();

      parent = x->parent_.lock();
      w = parent->left_;
    }

    node_type left_w        = w->left_;
    const node_type right_w = w->right_;
    const bool wleft_black  = !left_w  || left_w->black();
    const bool wright_black = !right_w || right_w->black();

    if (wleft_black && wright_black) {
      w->rednize();
      return parent;
    }

    if (wleft_black) {
      if (right_w) right_w->blacknize();
      w->rednize();
      w->left_rotate();
      w = parent->left_;
      left_w = w->left_;
    }

    w->red_ = parent->red_;
    parent->blacknize();
    if (left_w) left_w->blacknize();
    parent->right_rotate();

    return this->root_;
  }

  void copy_node(const red_black_tree& r) {
    this->root_ =
      node::clone_node(this->allocator_, r.root_, this->root_, this->lesser_);

    if (this->root_) {
      this->left_most_  = this->root_->left_most_descendant();
      this->right_most_ = this->root_->right_most_descendant();
    }
  }

  node_type      root_;
  node_type      left_most_;
  node_type      right_most_;

  size_type      size_;
  key_compare    lesser_;
  node_allocator_type allocator_;
};

template<class Key, class T, class Lesser, class Allocator>
void swap(
  sml::red_black_tree<Key, T, Lesser, Allocator>& l,
  sml::red_black_tree<Key, T, Lesser, Allocator>& r
) {
  l.swap(r);
}

} // namespace sml

#endif
