#ifndef MESketch_H
#define MESketch_H
#include "bucket.hpp"
#include "datatypes.hpp"
#include <algorithm>
#include <bitset>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <utility>
#include <vector>
extern "C" {
#include "hash.h"
#include "util.h"
}

class MESketch {
  val_tp sum;
  Bucket **l1_table;
  int l1_depth;
  int l1_width;
  unsigned long *l1_hash, *l1_scale, *l1_hardner;

  L2Cell **l2_table;
  int l2_depth;
  int l2_width;
  unsigned long *l2_hash, *l2_scale, *l2_hardner;

  int lgn;
  bool use_flag;

  bool quick_start;

public:
  MESketch(int l1_depth, int l1_width, int l2_depth, int l2_width, int lgn,
           bool use_flag = true);

  ~MESketch();

  fp_pair Key2FP(const unsigned char *key, int key_len);

  bool MoveAndIncrement(Bucket *src, L2Cell *dst, l2_fp_tp l2_fp);

  void Update(unsigned char *key, val_tp value);

  void Query(val_tp thresh, myvector &results);

  val_tp QueryL2(unsigned char *key);

  val_tp QueryL1(unsigned char *key);

  void PrintBucketStatus();

  void NewWindow();
};

class ElasticSketch {
  val_tp sum;

  std::bitset<ES_FP> *l2_fprint;
  uint32_t *l2_counter;
  uint32_t *l2_vote;

  int l2_depth;
  int l2_width;
  unsigned long l2_hardner;

  int lgn;

public:
  ElasticSketch(int l2_depth, int l2_width, int lgn);

  ~ElasticSketch();

  std::bitset<ES_FP> *Key2FP(const unsigned char *key, int key_len);

  void Update(unsigned char *key, val_tp value);

  void Query(val_tp thresh, myvector &results);

  val_tp QueryL2(unsigned char *key);
};

#endif
