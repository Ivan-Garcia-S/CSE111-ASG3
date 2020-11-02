// $Id: listmap.tcc,v 1.15 2019-10-30 12:44:53-07 - - $

#include "listmap.h"
#include "debug.h"


//
/////////////////////////////////////////////////////////////////
// Operations on listmap.
/////////////////////////////////////////////////////////////////
//

//
// listmap::~listmap()
//
template <typename key_t, typename mapped_t, class less_t>
listmap<key_t,mapped_t,less_t>::~listmap() {
   DEBUGF ('l', reinterpret_cast<const void*> (this));
}

//
// iterator listmap::insert (const value_type&)
//
template <typename key_t, typename mapped_t, class less_t>
typename listmap<key_t,mapped_t,less_t>::iterator
listmap<key_t,mapped_t,less_t>::insert (const value_type& pair) {
   DEBUGF ('l', &pair << "->" << pair);
   less_t less_;
   node* curr = this->anchor()->next;
   node* new_node = new node(nullptr,nullptr,pair);
   DEBUGF('c',curr << pair);
      while(less_(curr->value.first,new_node->value.first) && curr != anchor()){
         curr = curr->next;   
      }
      if(less_(new_node->value.first,curr->value.first)){
         curr->prev->next = new_node;
         new_node->prev = curr->prev;
         new_node->next = curr;
         curr->prev = new_node;
      }
      else if (!less_(new_node->value.first,curr->value.first) && !less_(curr->value.first,new_node->value.first)){
         curr->value.second = new_node->value.second;
      }
      else{
         curr->prev->next = new_node;
         new_node->prev = curr->prev;
         new_node->next = curr;
         curr->prev = new_node;
      }
   return new_node;
}

//
// listmap::find(const key_type&)
//
template <typename key_t, typename mapped_t, class less_t>
typename listmap<key_t,mapped_t,less_t>::iterator
listmap<key_t,mapped_t,less_t>::find (const key_type& that) {
   DEBUGF ('l', that);
   less_t less_;
   //anchor_.next
   node* curr = this->anchor()->next;
   while(curr != anchor() && (less_(that,curr->value.first) || less_(curr->value.first,that))){
      curr = curr->next;
   }
   if(!less_(that,curr->value.first) && !less_(curr->value.first,that)){
      return curr;
   }
   return anchor();
}

//
// iterator listmap::erase (iterator position)
//
template <typename key_t, typename mapped_t, class less_t>
typename listmap<key_t,mapped_t,less_t>::iterator
listmap<key_t,mapped_t,less_t>::erase (iterator position) {
   DEBUGF ('l', &*position);
   node* new_node;
   //node* curr = anchor()->next;
   //for(iterator curr = anchor()->next; curr != anchor(); curr++){
   for (listmap<key_t,mapped_t,less_t>::iterator itor = begin(); itor != end(); ++itor) {
      if(itor == position){
         new_node = itor.where->next;
         itor.where->prev->next = itor.where->next;
         itor.where->next->prev = itor.where->prev;
         delete(itor.where);
         itor.where = NULL;
      }
   }
   
   return new_node;
}


