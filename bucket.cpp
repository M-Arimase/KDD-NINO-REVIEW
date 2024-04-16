#include "bucket.hpp"
#include <iostream>
#include <algorithm>
#include <boost/dynamic_bitset.hpp>
#include "nmmintrin.h"
#include "immintrin.h"


Bucket::Bucket(bool use_flag /* = true*/) : use_flag(use_flag)
{
    flag = new std::bitset<1>(0);
}

Bucket::~Bucket()
{
    delete this->flag;
    this->counter.resize(0);
}

bool Bucket::IsEmptyBucket()
{
    if (this->fp == NULL)
    {
        return true;
    }
    return false;
}

void Bucket::ClearBucket()
{
    this->fp = NULL;
    this->flag->reset();
    this->counter.reset();
}

void Bucket::SetFP(std::bitset<L1_FP> &fp)
{
    this->fp = fp;
}

bool Bucket::FPEqual(std::bitset<L1_FP> &fp)
{
    if (this->fp == NULL)
    {
        return false;
    }
    return this->fp == fp;
}

std::bitset<L1_FP> Bucket::GetFP()
{

    return fp;
}

u_long Bucket::GetFPNumber()
{
    if (this->fp == NULL){
        return 0;
    }
    u_long a = this->fp.to_ulong();
    return this->fp.to_ulong();
}

void Bucket::SetFlag()
{
    if (this->use_flag)
    {
        this->flag->set();
    }
}

void Bucket::ResetFlag()
{
    this->flag->reset();
}

int Bucket::GetFlag()
{
    return (int)this->flag->to_ulong();
}

void Bucket::PrintFlag()
{
    std::cout << this->flag[0] << std::endl;
}

void Bucket::SetCounterSize(int size, int val)
{
    this->counter = boost::dynamic_bitset<>(size, val);
}

void Bucket::SetCounterVal(boost::dynamic_bitset<> &counter)
{
    int small_bit_len;
    if (this->counter.size() < counter.size())
    {
        small_bit_len = this->counter.size();
    }
    else
    {
        small_bit_len = counter.size();
    }
    for (int i = 0; i < small_bit_len; i++)
    {
        if (counter[i] != this->counter[i])
        {
            this->counter[i] = counter[i];
        }
    }
}

void Bucket::ResizeCounter(int size)
{
    this->counter.resize(size);
}

int Bucket::GetCounterSize()
{
    return this->counter.size();
}

void Bucket::Increment()
{
    // O(n)
    for (int i = 0; i < this->counter.size(); i++)
    {
        this->counter.flip(i);
        if (this->counter[i] == 1)
        {
            break;
        }
    }
}

void Bucket::Decrement()
{
    // O(n)
    int till = -1;
    for (int i = 0; i < this->counter.size(); i++)
    {
        if (this->counter[i] == 1)
        {
            till = i;
            break;
        }
    }

    if (till == -1)
    {
        return;
    }

    for (int i = 0; i <= till; i++)
    {
        this->counter.flip(i);
    }
}

bool Bucket::IsFull()
{
    if (this->counter.all())
    {
        return true;
    }
    return false;
}

void Bucket::PrintCounter()
{
    std::cout << "bucket.counter" << this->counter << std::endl;
}

boost::dynamic_bitset<> Bucket::GetCounter()
{
    return this->counter;
}

void Bucket::PrintBucket()
{
    std::cout << "FP: " << this->fp << "counter: " << this->counter << std::endl;
}






L2Bucket::L2Bucket(bool use_flag /* = true*/) : use_flag(use_flag)
{
    flag = new std::bitset<1>(0);
}

L2Bucket::~L2Bucket()
{
    delete this->flag;
    this->counter.resize(0);
}

bool L2Bucket::IsEmptyBucket()
{
    if (this->fp == NULL)
    {
        return true;
    }
    return false;
}

void L2Bucket::ClearBucket()
{
    this->fp = NULL;
    this->flag->reset();
    this->counter.reset();
}

void L2Bucket::SetFP(std::bitset<L2_FP> &fp)
{
    this->fp = fp;
}

bool L2Bucket::FPEqual(std::bitset<L2_FP> &fp)
{
    if (this->fp == NULL)
    {
        return false;
    }
    return this->fp == fp;
}

std::bitset<L2_FP> L2Bucket::GetFP()
{

    return fp;
}

u_long L2Bucket::GetFPNumber()
{
    if (this->fp == NULL){
        return 0;
    }
    u_long a = this->fp.to_ulong();
    return this->fp.to_ulong();
}

void L2Bucket::SetFlag()
{
    if (this->use_flag)
    {
        this->flag->set();
    }
}

void L2Bucket::ResetFlag()
{
    this->flag->reset();
}

int L2Bucket::GetFlag()
{
    return (int)this->flag->to_ulong();
}

void L2Bucket::PrintFlag()
{
    std::cout << this->flag[0] << std::endl;
}

void L2Bucket::SetCounterSize(int size, int val)
{
    this->counter = boost::dynamic_bitset<>(size, val);
}

void L2Bucket::SetCounterVal(boost::dynamic_bitset<> &counter)
{
    int small_bit_len;
    if (this->counter.size() < counter.size())
    {
        small_bit_len = this->counter.size();
    }
    else
    {
        small_bit_len = counter.size();
    }
    for (int i = 0; i < small_bit_len; i++)
    {
        if (counter[i] != this->counter[i])
        {
            this->counter[i] = counter[i];
        }
    }
}

void L2Bucket::ResizeCounter(int size)
{
    this->counter.resize(size);
}

int L2Bucket::GetCounterSize()
{
    return this->counter.size();
}

void L2Bucket::Increment()
{
    for (int i = 0; i < this->counter.size(); i++)
    {
        this->counter.flip(i);
        if (this->counter[i] == 1)
        {
            break;
        }
    }
}

void L2Bucket::Decrement()
{
    int till = -1;
    for (int i = 0; i < this->counter.size(); i++)
    {
        if (this->counter[i] == 1)
        {
            till = i;
            break;
        }
    }

    if (till == -1)
    {
        return;
    }

    for (int i = 0; i <= till; i++)
    {
        this->counter.flip(i);
    }
}

bool L2Bucket::IsFull()
{
    if (this->counter.all())
    {
        return true;
    }
    return false;
}

void L2Bucket::PrintCounter()
{
    std::cout << "bucket.counter" << this->counter << std::endl;
}

boost::dynamic_bitset<> L2Bucket::GetCounter()
{
    return this->counter;
}

void L2Bucket::PrintBucket()
{
    std::cout << "FP: " << this->fp << "counter: " << this->counter << std::endl;
}


L2Cell::L2Cell(bool use_flag /* = true*/) : use_flag(use_flag)
{
    this->bucket_num.set();
    int *counter_sizes = GetCounterSizes();
    for (int i = 0; i < 4; i++)
    {
        L2Bucket *bucket = new L2Bucket(use_flag);
        bucket->SetCounterSize(counter_sizes[i], 0);
        buckets.push_back(bucket);
    }
}

std::bitset<L2_FP> L2Cell::GetFP(int idx)
{
    return this->buckets[idx]->GetFP();
}

u_long L2Cell::GetFPNumber(int idx)
{
    return this->buckets[idx]->GetFPNumber();
}

int L2Cell::InCell(std::bitset<L2_FP> &fp)
{
    int bucket_num = GetBucketNum() - 1;
    for (int i = 0; i <= bucket_num; i++)
    {
        if (this->buckets[i]->FPEqual(fp))
        {
            return i;
        }
    }
    return -1;
}

int L2Cell::InCellSimd(std::bitset<L2_FP> &fp){
    __m256i vfp = _mm256_set_epi64x((uint64_t )(buckets[3]->GetFPNumber()), (uint64_t )(buckets[2]->GetFPNumber()), (uint64_t )(buckets[1]->GetFPNumber()), (uint64_t )(buckets[0]->GetFPNumber()));
    __m256i comfp = _mm256_set1_epi64x((uint64_t )fp.to_ulong());
    union {__m256i vflag; uint64_t flag[4];};
    vflag = _mm256_cmpeq_epi64 (vfp, comfp);
    for (int i = 0; i <= GetBucketNum() - 1; i++){
        if (flag[i] != 0){
            return i;
        }
    }
    return -1;
}

int L2Cell::GetBucketNum()
{
    return bucket_num[0] + bucket_num[1] * 2 + 1;
}

int L2Cell::GetFlag(int idx)
{
    return buckets[idx]->GetFlag();
}

void L2Cell::SetFlag(int idx)
{
    buckets[idx]->SetFlag();
}

void L2Cell::ResetFlags()
{
    for (auto bucket : buckets)
    {
        bucket->ResetFlag();
    }
}

bool L2Cell::HasEmptyBucket()
{
    for (auto bucket : buckets)
    {
        if (bucket->IsEmptyBucket())
        {
            return true;
        }
    }
    return false;
}

int L2Cell::AddNewElement(std::bitset<L2_FP> fp, boost::dynamic_bitset<> counter)
{
    int pos = -1;
    for (auto bucket : buckets)
    {
        pos++;
        if (bucket->IsEmptyBucket())
        {
            bucket->SetFP(fp);
            bucket->SetCounterVal(counter);
            break;
        }
    }
    return pos;
}

void L2Cell::ReplaceElement(int idx, std::bitset<L2_FP> fp, boost::dynamic_bitset<> counter)
{
    buckets[idx]->SetFP(fp);
    buckets[idx]->SetCounterVal(counter);
}

void L2Cell::MeltSmallestBucket()
{
    this->DecrementBucketNum();
    int *counter_sizes = this->GetCounterSizes();
    int bucket_num = GetBucketNum() - 1;
    for (int i = 0; i <= bucket_num; i++)
    {
        int target_size = counter_sizes[i];
        this->buckets[i]->ResizeCounter(target_size);
    }
    L2Bucket *bucket_to_delete = buckets.back();
    buckets.pop_back();
    delete bucket_to_delete;
}

bool L2Cell::Increment(int idx)
{
    if (buckets[idx]->IsFull())
    {
        if (GetBucketNum() - 1 == idx)
        {
            return false;
        }
        this->MeltSmallestBucket();
    }
    buckets[idx]->Increment();

    for (int i = idx; i > 0; i--)
    {
        if (buckets[i]->GetCounter() > buckets[i - 1]->GetCounter())
        {
            int lc_size = buckets[i - 1]->GetCounterSize();
            int sc_size = buckets[i]->GetCounterSize();
            std::swap(buckets[i - 1], buckets[i]);
            buckets[i - 1]->ResizeCounter(lc_size);
            buckets[i]->ResizeCounter(sc_size);
        }
        else
        {
            break;
        }
    }

    return true;
}

boost::dynamic_bitset<> L2Cell::GetCounter(int idx)
{
    return buckets[idx]->GetCounter();
}

int L2Cell::DecrementSmallestBucket()
{
    int idx = GetBucketNum() - 1;
    buckets[idx]->Decrement();
    return (int)buckets[idx]->GetCounter().to_ulong();
}

void L2Cell::DecrementBucketNum()
{
    int till = -1;
    for (int i = 0; i < this->bucket_num.size(); i++)
    {
        if (this->bucket_num[i] == 1)
        {
            till = i;
            break;
        }
    }

    if (till == -1)
    {
        return;
    }

    for (int i = 0; i <= till; i++)
    {
        this->bucket_num.flip(i);
    }
}

int *L2Cell::GetCounterSizes()
{
    int total_bits = L2_SIZE - 4 * L2_FP - 2;
    int t_size = total_bits * 3 / 12;
    int s_size = total_bits * 3 / 12;
    int m_size = total_bits * 3 / 12;
    int l_size = total_bits * 3 / 12;
    switch (GetBucketNum() - 1)
    {
    case 3:
        return new int[4]{l_size, m_size, s_size, t_size};
    case 2:
        s_size += (t_size + L2_FP) / 3;
        m_size += (t_size + L2_FP) / 3;
        l_size += (t_size + L2_FP) - (t_size + L2_FP) / 3 * 2;
        return new int[3]{l_size, m_size, s_size};
    case 1:
        m_size += (t_size + s_size + 2 * L2_FP) / 2;
        l_size += (t_size + s_size + 2 * L2_FP) - (t_size + s_size + 2 * L2_FP) / 2;
        return new int[2]{l_size, m_size};
    case 0:
        l_size += t_size + s_size + m_size + 3 * L2_FP;
        return new int[1]{l_size};
    }
}

void L2Cell::PrintCell()
{

    for (auto bucket : buckets)
    {
        if (bucket->IsEmptyBucket())
        {
            std::cout << "empty" << std::endl;
        }
        else
        {
            bucket->PrintBucket();
        }
    }
}
