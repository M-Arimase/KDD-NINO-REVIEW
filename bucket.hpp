#include <bitset>
#include <vector>
#include "datatypes.hpp"
#include <boost/dynamic_bitset.hpp>

class Bucket
{

private:
    std::bitset<L1_FP> fp;
    bool use_flag;
    std::bitset<1> *flag;
    boost::dynamic_bitset<> counter;

public:
    Bucket(bool use_flag = true);
    ~Bucket();
    // bucket op
    bool IsEmptyBucket();
    void ClearBucket();

    // fp op
    void SetFP(std::bitset<L1_FP> &fp);
    bool FPEqual(std::bitset<L1_FP> &fp);
    std::bitset<L1_FP> GetFP();
    u_long GetFPNumber();

    // flag ops
    void SetFlag();
    void ResetFlag();
    int GetFlag();
    void PrintFlag();

    // counter ops
    void SetCounterSize(int size, int val);
    void SetCounterVal(boost::dynamic_bitset<> &counter);
    void ResizeCounter(int size);
    int GetCounterSize();
    void Increment();
    void Decrement();
    bool IsFull();
    boost::dynamic_bitset<> GetCounter();
    void PrintCounter();
    void PrintBucket();
};


class L2Bucket
{

private:
    std::bitset<L2_FP> fp;
    bool use_flag;
    std::bitset<1> *flag;
    boost::dynamic_bitset<> counter;

public:
    L2Bucket(bool use_flag = true);
    ~L2Bucket();
    // bucket op
    bool IsEmptyBucket();
    void ClearBucket();

    // fp op
    void SetFP(std::bitset<L2_FP> &fp);
    bool FPEqual(std::bitset<L2_FP> &fp);
    std::bitset<L2_FP> GetFP();
    u_long GetFPNumber();

    // flag ops
    void SetFlag();
    void ResetFlag();
    int GetFlag();
    void PrintFlag();

    // counter ops
    void SetCounterSize(int size, int val);
    void SetCounterVal(boost::dynamic_bitset<> &counter);
    void ResizeCounter(int size);
    int GetCounterSize();
    void Increment();
    void Decrement();
    bool IsFull();
    boost::dynamic_bitset<> GetCounter();
    void PrintCounter();
    void PrintBucket();
};

class L2Cell
{
private:
    bool use_flag;
    std::vector<L2Bucket *> buckets;
    std::bitset<2> bucket_num;

public:
    L2Cell(bool use_flag = true);
    int InCell(std::bitset<L2_FP> &fp);
    int InCellSimd(std::bitset<L2_FP> &fp);
    std::bitset<L2_FP> GetFP(int idx);
    u_long GetFPNumber(int idx);
    int GetBucketNum();
    int GetFlag(int idx);
    void SetFlag(int idx);
    void ResetFlags();
    bool HasEmptyBucket();
    int AddNewElement(std::bitset<L2_FP> fp, boost::dynamic_bitset<> counter);
    void ReplaceElement(int idx, std::bitset<L2_FP> fp, boost::dynamic_bitset<> counter);
    void DecrementBucketNum();
    bool Increment(int idx);
    boost::dynamic_bitset<> GetCounter(int idx);
    int DecrementSmallestBucket();
    void MeltSmallestBucket();
    int *GetCounterSizes();
    void PrintCell();
};