//
// Created by Hila Cohen on 20/09/2022.
//

#ifndef UNTITLED88_HASHMAP_H
#define UNTITLED88_HASHMAP_H


#include <iostream>
#include <vector>
#include <cassert>

#define MAX_LOAD_FACTOR 0.75
#define MIN_LOAD_FACTOR 0.25
#define MULT_CONST 2

/**
 * this header contains an implementation of hashmap including a
 * const iterator . the hashmap implemented as follows:
 * #_map- is a pointer to pointers "buckets"
 *   of elem as each element is a pair of key and
 *   value (template).
 *   each "bucket" is a linked list of elements
 *   if empty- points to nullptr
 * #size_buckets_- contains the sizes of each "bucket"
 * #size_- size of the hashmap (how many elements are in)
 * #capacity_-number of "buckets"
 *
 *
 * #const iterator-iterate over the elements using operators it++
 * and ++it also != and == operators * and ->
 *
 * */



template<typename KeyT,typename ValueT>
struct elem{
    std::pair<KeyT, ValueT> data;
    struct elem* next=nullptr;
};


template<typename KeyT,typename ValueT>
class HashMap{

public:


    class const_iterator
    {
    public:
        typedef const_iterator self_type;
        typedef elem<KeyT,ValueT> value_type;
        typedef std::pair<KeyT,ValueT>& reference;
        typedef elem<KeyT,ValueT>* pointer;
        typedef int difference_type;
        typedef std::forward_iterator_tag iterator_category;

        const_iterator(pointer* ptr_map,pointer ptr_elem,size_t
        bucket,size_t* size_buckets,size_t index_elem,size_t
        capacity) :
                ptr_map_(ptr_map),bucket_(bucket),index_elem_(index_elem),
                size_buckets_(new size_t[capacity]),ptr_elem_(ptr_elem),
                capacity_(capacity)
        {
            for (int i = 0; i < capacity; ++i)
            {
                size_buckets_[i]=size_buckets[i];
            }

        }
        /** it++ **/
        self_type operator++(int)
        {
            assert(bucket_<capacity_);
            ///next in bucket
            if(index_elem_<size_buckets_[bucket_]-1){
                self_type i = *this;
                ptr_elem_=ptr_map_[bucket_]->next;
                index_elem_+=1;
                return i;
            }
                ///next bucket
            else{

                self_type i = *this;
                bucket_+=1;
                while(size_buckets_[bucket_]==0){
                    bucket_+=1;
                }
                ptr_elem_=ptr_map_[bucket_];
                index_elem_=0;
                return i;

            }

        }
        /** ++it **/
        self_type operator++()
        {
            assert(bucket_<capacity_);
            ///next in bucket
            if(index_elem_<size_buckets_[bucket_]-1){
                ptr_elem_=ptr_map_[bucket_]->next;
                index_elem_+=1;
                return *this;
            }
                ///next bucket
            else{
                bucket_+=1;
                while(size_buckets_[bucket_]==0){
                    bucket_+=1;

                }
                ptr_elem_=ptr_map_[bucket_];
                index_elem_=0;
                return *this;

            }
        }
        const reference operator*()
        {
            return ptr_elem_->data;
        }
        const pointer operator->()
        {
            return &ptr_elem_->data;
        }
        /** does the key and value of elem equal **/
        bool operator==(const self_type& rhs)
        {
            return ptr_elem_->data.first==rhs.ptr_elem_->data.first
                   && rhs.ptr_elem_->data.second==ptr_elem_->data.second;
        }
        bool operator!=(const self_type& rhs)
        {
            return !(ptr_elem_->data.first==rhs.ptr_elem_->data.first
                     &&rhs.ptr_elem_->data.second==ptr_elem_->data.second);
        }
    private:
        pointer* ptr_map_;
        pointer ptr_elem_;
        size_t index_elem_;
        size_t bucket_;
        size_t* size_buckets_;
        size_t capacity_;
        friend class HashMap<KeyT,ValueT>;
    };

    /** constructor- new (empty) hashmap with capacity of 16 and
     * size 0 */
    HashMap():_capacity(16),_size(0),
    _map(new elem<KeyT,ValueT>*[_capacity]),
    size_buckets_(new size_t[_capacity]){
        for (int i = 0; i < _capacity ;++i)
        {
            (_map)[i]= nullptr;
            size_buckets_[i]=0;

        }

    }
    /** constructor- new hashmap containing all elements of
     * Keyt:Valuet respectively */
    HashMap(std::vector<KeyT> Keyt, std::vector<ValueT> Valuet)
    :_capacity(16),_size(0),_map(new elem<KeyT,ValueT>*[_capacity]),
            size_buckets_(new size_t[_capacity])
    {
        for (int i = 0; i < _capacity ;++i)
        {
            (_map)[i]= nullptr;
            size_buckets_[i]=0;

        }
        bool key_exist=false;
        for (int i = 0; i <Keyt.size() ; ++i)
        {
            ///bucket is empty
            int hash_idx= hashFunc(Keyt[i]);
            if(!(_map)[hash_idx]){
                (_map)[hash_idx]=new elem<KeyT,ValueT>();
                (_map)[hash_idx]->next= nullptr;
                (_map)[hash_idx]->data=std::make_pair(Keyt[i],
                                                      Valuet[i]);

                _size+=1;
            }
            else{
                elem<KeyT,ValueT>* entry=(_map)[hash_idx];
                ///key already exist-change
                while(entry->next){
                    if(entry->data.first==Keyt[i]){
                        entry->data.second=Valuet[i];
                        key_exist=true;
                        entry=entry->next;
                    }

                }
                ///insert to bucket
                if(!key_exist){
                    entry->next->data=std::make_pair(Keyt[i],Valuet[i]);
                    entry->next->next= nullptr;
                    entry=entry->next;
                    _size+=1;
                }


            }
        }

    }
    /**  copy constructor */
    HashMap( const HashMap  &hm):_capacity(hm._capacity),_size(hm
    ._size),
    _map(new elem<KeyT,ValueT>*[hm._capacity]),size_buckets_(new size_t[_capacity]){

        for (int i = 0; i < _capacity; ++i)
        {
            _map[i]= nullptr;
            size_buckets_[i]=0;

        }
        for (int i = 0; i <hm._capacity ; ++i)
        {
            elem<KeyT,ValueT>* entry=(hm._map)[i];
            elem<KeyT,ValueT>* new_entry;
            while(entry){
                if(!_map[i]){
                    _map[i]=new elem<KeyT,ValueT>();
                    new_entry=_map[i];

                }
                else{
                    new_entry->next=new elem<KeyT,ValueT>();

                }
                new_entry->data=std::make_pair(entry->data.first,
                                               entry->data.second);

                entry=entry->next;
            }

        }
    }

    /** destructor*/
    ~HashMap(){
        size_buckets_=new size_t[_capacity];
        for (int i = 0; i <_capacity; ++i)
        {
            elem<KeyT,ValueT>* entry=(_map)[i];
            while(entry){
                elem<KeyT,ValueT>* tmp_entry=entry->next;
                delete entry;
                entry=tmp_entry;

            }
            (_map)[i]= nullptr;
            size_buckets_[i]=0;

        }
        delete[] _map;
    }
    /**hash function */
    size_t hashFunc(KeyT key){
        size_t key_hash = std::hash<KeyT>()(key);
        size_t hashRes= key_hash & (_capacity -1);
        std::cout<<hashRes<<" hash result"<<std::endl;
        return hashRes;
    }
    /** is this key in hashmap*/
    bool contains_key(KeyT key){
         int hash_index= hashFunc(key);
         elem<KeyT,ValueT>* entry=(_map)[hash_index];
         while(entry){
             if(entry->data.first==key){
                 return true;
             }
             entry=entry->next;
         }
        return false;
    }
    /** return value of the given key if not inside map throw
     * exception*/
    ValueT& at(KeyT key){
        if(!contains_key(key)){
            throw std::invalid_argument("invalid_key");
        }
        int hash_index= hashFunc(key);
        elem<KeyT,ValueT>* entry=_map[hash_index];
        while(entry){
            if(entry->data.first==key){
                return entry->data.second;
            }
            entry=entry->next;
        }
    }
    /** erase elem with given key . if not in map return false*/
    bool erase(KeyT key){
        ///not in map
        if(!contains_key(key)){
            return false;
        }
        ///erase
        int hash_index= hashFunc(key);
        elem<KeyT,ValueT>* prev= nullptr;
        elem<KeyT,ValueT>* entry=_map[hash_index];
        while(entry){
            if(entry->data.first==key){
                ///remove first element
                if(prev== nullptr){
                    delete entry;
                    _map[hash_index]= nullptr;
                    _size-=1;
                    resize();
                    return true;
                }
                prev->next=entry->next;
                delete entry;
                _size-=1;
                resize();
                return true;
            }
            prev=entry;
            entry=entry->next;
        }


    }


    int size(){
        return _size;
    }

    void set_size(int new_size){
        _size=new_size;
    }

    void set_capacity(int new_capacity){
        _capacity=new_capacity;
    }

    int capacity(){
        return _capacity;
    }

    bool empty(){
        return _size==0;
    }

    /** is this element in map*/
    bool in_bucket(elem<KeyT,ValueT>* element){
        if(!element  || element==NULL){
            return false;
        }
        int hash_index= hashFunc(element->data.first);
        elem<KeyT,ValueT>* entry=(_map)[hash_index];
        while(entry){
            ///keys are equal
            if(entry->data.first==element->data.first){
                return true;
            }
        }
        return false;

    }

    /** return number of elements in bucket of given key if not in
     * map throw an exception*/
    int bucket_size(KeyT key){
        if(!contains_key(key)){
            throw std::invalid_argument("invalid_key");
        }
        int hash_index= hashFunc(key);

        return size_buckets_[hash_index];
    }

    /** return index bucket of given key*/
    int bucket_index(KeyT key){
        if(!contains_key(key)){
            throw std::invalid_argument("invalid_key");
        }
        return hashFunc(key);
    }


    /** are hashmap equal to given hashmap*/
    bool equal(HashMap const &hm){
        if(hm._capacity!=_capacity ||
           hm._size!=_size ){
            return false;
        }
        for (int i = 0; i <hm._capacity ; ++i)
        {
            elem<KeyT,ValueT>* entry=(((hm._map))[i]);
            elem<KeyT,ValueT>* our_entry=((_map)[i]);
            while(entry){
                if(!in_bucket(our_entry)){
                    return false;
                }
                entry=entry->next;

            }

        }
        return true;

    }


    /** return size of hashmap divided by it's capacity*/
    float get_load_factor(){
        float lfactor=(float)_size/_capacity;
        return lfactor;
    }

    /**clear all elements from map-buckets will be empty and size e*/
    void clear(){
        for (int i = 0; i <_capacity; ++i)
        {
            elem<KeyT,ValueT>* entry=(_map)[i];
            while(entry){
                elem<KeyT,ValueT>* tmp_entry=entry->next;
                delete entry;
                entry=tmp_entry;

            }
            (_map)[i]= nullptr;
            size_buckets_[i]=0;


        }
        _size=0;
    }

    /** insert elements to newly sized map */
    void resize_helper(){
        elem<KeyT,ValueT>** new_map=new elem<KeyT,ValueT>*[_capacity];
        size_t tmp_size=_size;
        size_t* tmp_size_buckets=new size_t[_capacity];

        for (int i = 0; i <_capacity ; ++i){

            new_map[i]= nullptr;
            tmp_size_buckets[i]=0;

        }

        for (int i = 0; i <_capacity ; ++i)
        {

            elem<KeyT,ValueT>* entry=((_map)[i]);
            while(entry){
                int hash_index= hashFunc(entry->data.first);
                elem<KeyT,ValueT>* new_entry;
                if(!(new_map)[hash_index]){
                    (new_map)[hash_index]=new elem<KeyT,ValueT>();
                    new_entry=(new_map)[hash_index];
                    new_entry->data=std::make_pair(entry->data.first,
                                                   entry->data.second);
                    tmp_size_buckets[hash_index]+=1;


                }
                else{
                    new_entry= (new_map)[hash_index];

                    while(new_entry && new_entry->next){
                        new_entry=new_entry->next;
                    }
                    new_entry->next=new elem<KeyT,ValueT>();
                    new_entry->next->data=std::make_pair(entry->data.first,
                                                   entry->data.second);
                    tmp_size_buckets[hash_index]+=1;

                }

                entry=entry->next;

            }


        }
        ///clear all map and init new_map as new map
        clear();
        //delete _map;
        _size=tmp_size;
        for (int i = 0; i <_capacity ; ++i)
        {
            size_buckets_[i]=tmp_size_buckets[i];
        }
        delete tmp_size_buckets;

        ///assign new_map
        _map=new_map;


    }
    /** if after inserting load factor is grater than
     * MAX_LOAD_FACTOR  capacity will be divided by 2 .
     * if less than MIN_LOADER_FACTOR- capacity will be multiplied by
     * 2 */
    void resize(){
        float lf=get_load_factor();
        if(lf>MAX_LOAD_FACTOR){
            _capacity*=MULT_CONST;
            delete size_buckets_;
            size_buckets_=new size_t[_capacity];
            resize_helper();

        }
        else if(lf<MIN_LOAD_FACTOR){
            _capacity/=MULT_CONST;
            delete size_buckets_;
            size_buckets_=new size_t[_capacity];
            resize_helper();
        }
    }

    bool insert(KeyT key, ValueT value)
    {
        int idx = hashFunc(key);
        ///already in map
        if(contains_key(key)){
            elem<KeyT,ValueT>* entry=_map[idx];
            while(entry){
                if(entry->data.first==key){
                    entry->data.second=value;
                    return false;
                }
            }
        }
        ///added successfully
        elem<KeyT,ValueT>* entry;
        ///add as first element in bucket
        if(!(_map)[idx]){
            (_map)[idx]=new elem<KeyT,ValueT>();
            (_map)[idx]->data=std::make_pair(key,value);
            (_map)[idx]->next= nullptr;
            size_buckets_[idx]+=1;

        }
        ///else
        else{
            entry=(_map)[idx];
            while(entry){
                if(entry->next){
                    entry=entry->next;

                }
                else{
                    entry->next=new elem<KeyT,ValueT>();
                    entry->next->data=std::make_pair(key,value);
                    entry->next->next= nullptr;
                    size_buckets_[idx]+=1;
                    break;
                }
            }

        }

        _size+=1;
        resize();
        return true;
    }

    /**return first iterator of map*/
    const_iterator begin() const
    {
        int i=0;
        while (i<_capacity && size_buckets_[i]==0){
            i+=1;
        }
        return const_iterator(_map,_map[i],i,size_buckets_,0,
                              _capacity);

    }

    /**return last iterator of map*/
    const_iterator end() const
    {
        ///i-last bucket with element
        int i=_capacity-1;
        while (i>=0 && size_buckets_[i]==0){
            i-=1;
        }
        return const_iterator(_map,_map[i],i,size_buckets_,0,
                              _capacity);

    }

    /** assign given map to our map*/
    HashMap<KeyT,ValueT>& operator=(const HashMap<KeyT,ValueT>& hm){
        _map=new elem<KeyT,ValueT>*[hm._capacity];
        _capacity=hm._capacity;
        size_buckets_=new size_t[_capacity];
        _size=hm._size;
        for (int i = 0; i <hm._capacity ; ++i){
            _map[i]= nullptr;
            size_buckets_[i]=hm.size_buckets_[i];
        }

        for (int i = 0; i <hm._capacity ; ++i)
        {

            elem<KeyT,ValueT>* entry=(hm._map)[i];
            elem<KeyT,ValueT>* new_entry=(_map)[i];
            while(entry){
                if(!(_map)[i]){
                    (_map)[i]=new elem<KeyT,ValueT>();
                    (_map)[i]->data=std::make_pair(entry->data.first,
                                                   entry->data.second);
                    (_map)[i]->next= nullptr;

                }
                else{
                    new_entry=new elem<KeyT,ValueT>();
                    new_entry->data=std::make_pair(entry->data.first,
                                                   entry->data.second);
                    new_entry->next= nullptr;
                    entry=entry->next;
                    new_entry=new_entry->next;
                }

            }
        }
        return *this;

    }

    /** return value of given key*/
    ValueT& operator[](const KeyT& key){
        if(!contains_key(key)){
            insert(key,ValueT());
            return at(key);
        }
        return at(key);
    }

    /** return is given hashmap equal to our*/
    bool operator==(const HashMap<KeyT,ValueT>& hm){
        bool res=equal(hm);
        return res;
    }

    bool operator!=(const HashMap<KeyT,ValueT>& hm){
        return !equal(hm);
    }











protected:
    elem<KeyT,ValueT>** _map;
    int _size=0;
    int _capacity=16;
    size_t* size_buckets_;
};















#endif //UNTITLED88_HASHMAP_H
